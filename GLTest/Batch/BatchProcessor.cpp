#include "BatchProcessor.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshNode.h"
#include "../Mesh/BoneNode.h"
#include "../Mesh/Triangle.h"
#include "../Mesh/Vertex.h"
#include "../Batch/VertexFormat.h"
#include "../Batch/Batch.h"
#include "../Batch/Appearance.h"
#include "../Utils/MathsUtils.h"

#include <wx\log.h>

namespace batch
{

#define DOT_THESHOLD         0.9999f        /* Closeness to a dot product of 1 at which two normals are considered the same */

BatchProcessor::BatchProcessor()
{
}


BatchProcessor::~BatchProcessor()
{
}

//TODO traingle strips
//https://developer.apple.com/library/ios/documentation/3ddrawing/conceptual/opengles_programmingguide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

// Create per vertex data in per material batches. Extra vertices are created to accommodate per triangle corner information.
void BatchProcessor::CreateBatches(
	mesh::MeshPtr &mesh,
	render::PerNodeBatchList &perNodeRenderBatches // Batch vector to fill in
	)
{
	// Each node needs a new batch (due to each node having a different matrix transform
	//TODO could insist on node transforms being zero and thus create fewer batches). Within each node each material needs a new batch.
	render::AppearanceTable& appearances = mesh->GetAppearanceTable();

	mesh::MeshNode* rootMeshNode = mesh->GetMeshNodeHierarchy();

	CreateBatchesInternal(mesh, rootMeshNode, appearances, perNodeRenderBatches);

	return;
}


void BatchProcessor::CreateBatchesInternal(
	mesh::MeshPtr &mesh,
	mesh::MeshNode* meshNode,
	render::AppearanceTable& appearances,
	render::PerNodeBatchList &perNodeRenderBatches // Batch vector to fill in
	)
{
	for(meshNode; meshNode != NULL; meshNode = meshNode->m_next)
	{
		render::BatchList renderBatches; // Each node needs a new set of batches (due to each node having a different matrix transform)
		renderBatches.resize(appearances.size());//Worst case scenario - should usually be less than this.

		mesh::MeshTriangleArray triangleArray = meshNode->GetTriangles();
		int numTriangles = meshNode->GetNumTriangles();
		if(numTriangles == 0)
		{
			continue;
		}
		mesh::MeshVertexArray vertexArray = meshNode->GetVertices();
		int numVertices = meshNode->GetNumVertices();
		if(numVertices == 0)
		{
			continue;
		}
		// Map containing the material id and a vector array containing the new index for vector[oldindex] in the array
		std::vector<int>previouslyAssignedVertexIndexMap(numVertices, -1);
		std::vector<std::vector<int>> perMaterialPreviouslyAssignedVertexIndexMap(appearances.size(), previouslyAssignedVertexIndexMap);

		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			int materialId = triangleArray[triangleIndex].GetMaterialId();
			assert(materialId != -1); // Every triangle must have a material
			std::vector<int> previouslyAssignedVertexIndexMap = perMaterialPreviouslyAssignedVertexIndexMap[materialId];

			if(!renderBatches[materialId]) // If a batch for this material does not already exist then create it
			{
				renderBatches[materialId] = render::BatchPtr(new render::Batch(/*render::ColourFormat*/));
				int numVertices = mesh->GetNumVerticesWithMaterialId(materialId);
				renderBatches[materialId]->AllocateVertices(numVertices);
				renderBatches[materialId]->AllocateIndices(numVertices);
				renderBatches[materialId]->SetAppearance(appearances[materialId]);
				glm::mat4 batchModelMatrix;
				utils::MathsUtils::ConvertFBXToGLMatrix(meshNode->GetGlobalTransform(), batchModelMatrix);
				renderBatches[materialId]->SetModelMatrix(batchModelMatrix);
			}

			// Assign the mesh information to batches. Reassigning the indices as the data is processed.
			for(int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
			{
				unsigned int testVertexIndex = triangleArray[triangleIndex].GetVertexIndex(triangleCornerIndex);

				// Compile the per vertex data from the mesh triangles

				// First create a per vertex vert - if a vert at this index has not been added previously then add it. 
				// If a vert at this index has already been added then only add this one if it differs from the previous one
				//TODO can bone indeces/weights be set by triangle corner vertex? If yes then these should also be split where they differ.
				render::TexturedSkinnedVertex testVertex;
				FbxVector4 position = vertexArray[testVertexIndex].GetPosition();
				testVertex.m_position = glm::vec3(static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2])); //TODO method called CreatePerVertexVertex
				testVertex.m_colour = triangleArray[triangleIndex].GetColour(triangleCornerIndex);
				testVertex.m_normal = glm::vec3(triangleArray[triangleIndex].GetNormal(triangleCornerIndex));
				testVertex.m_uv = triangleArray[triangleIndex].GetUV(triangleCornerIndex);
				for(int weightIndex = 0; weightIndex < MAX_INFLUENCES; weightIndex++)
				{
					testVertex.m_boneWeights[weightIndex] = vertexArray[testVertexIndex].GetBoneWeight(weightIndex);
					testVertex.m_boneIds[weightIndex] = vertexArray[testVertexIndex].GetBoneInfluenceId(weightIndex);
				}

				int previouslyCreatedIndex = previouslyAssignedVertexIndexMap[testVertexIndex];
				if(previouslyCreatedIndex == -1) // If this is the first time we have seen this index in this batch create a new vertex for the batch
				{
					AddDuplicateVertex(testVertexIndex, testVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
					continue;
				}

				render::TexturedSkinnedVertex previouslyCreatedVertex = renderBatches[materialId]->GetVertices()[previouslyCreatedIndex];
				
				// If we have seen this vertex before, but this triangle corner has a different colour, uv coord or normal 
				// then still create a new vertex for it (this will increase the vertex count in the mesh but will allow for
				// hard edges and texture seams which will otherwise have to be averaged for opengl).
				if(testVertex.m_colour != previouslyCreatedVertex.m_colour)
				{
					AddDuplicateVertex(testVertexIndex, testVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
				}
				else if(glm::dot(testVertex.m_normal, previouslyCreatedVertex.m_normal) < DOT_THESHOLD) // The two normals are not the same - add a duplicate vertex
				{
					AddDuplicateVertex(testVertexIndex, testVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
				}
				else if(testVertex.m_uv != previouslyCreatedVertex.m_uv) // dupicate verts for texture coordinates on texture seams
				{
					AddDuplicateVertex(testVertexIndex, testVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
				}
				else // Any identical vertex was already added - just add the index to refer to the existing vert when rendering
				{
					renderBatches[materialId]->AddIndex(previouslyCreatedIndex);
				}
			}
		}

		std::pair<unsigned int, render::BatchList> renderBatchWithMeshId(meshNode->GetId(), renderBatches);
		perNodeRenderBatches.insert(renderBatchWithMeshId);

		for(mesh::MeshNode *childNode = meshNode->m_firstChild; childNode != NULL; childNode = childNode->m_next)
		{
			CreateBatchesInternal(mesh, childNode, appearances, perNodeRenderBatches);
		}
	}
}

//TODO override equals?
void BatchProcessor::AddDuplicateVertex(
	const int oldVertexIndex,
	const render::TexturedSkinnedVertex &currentVertex,
	render::Batch &batch,
	std::vector<int> &perMaterialPreviouslyAssignedVertexIndexMap
	)
{
	render::TexturedSkinnedVertex vertex;
	vertex.m_position = currentVertex.m_position; //TODO override equals?
	vertex.m_colour = currentVertex.m_colour;
	vertex.m_uv = currentVertex.m_uv;
	vertex.m_normal += currentVertex.m_normal;
	if(vertex.m_normal.length() != 0) // Deal with the mesh which has it's normals deliberately set to zero(!)
	{
		vertex.m_normal = glm::normalize(vertex.m_normal); // Make sure these are normalised
	}
	for(int weightIndex = 0; weightIndex < MAX_INFLUENCES; weightIndex++)
	{
		vertex.m_boneWeights[weightIndex] = currentVertex.m_boneWeights[weightIndex];
		vertex.m_boneIds[weightIndex] = currentVertex.m_boneIds[weightIndex];
	}

	int newIndex = batch.GetNumVertices();
	perMaterialPreviouslyAssignedVertexIndexMap[oldVertexIndex] = newIndex; //TODO uh shorter name?
	batch.AddVertex(vertex);
	batch.AddIndex(newIndex);
}

void BatchProcessor::PrepareBatches(
	render::PerNodeBatchList &perNodeRenderBatches
	)
{
	render::PerNodeBatchList::const_iterator batchListIterator;
	for(batchListIterator = perNodeRenderBatches.begin(); batchListIterator != perNodeRenderBatches.end(); batchListIterator++)
	{
		render::BatchList renderBatches = batchListIterator->second;

		render::BatchList::const_iterator batchIterator;
		for(batchIterator = renderBatches.begin(); batchIterator != renderBatches.end(); batchIterator++)
		{
			if(*batchIterator != NULL) // This will be a case if an appearance exists but is not uses by this node //TODO get rid of this?
			{
				(*batchIterator)->Prepare();
			}
		}
	}
}

void BatchProcessor::SortBatches()
{

}

}

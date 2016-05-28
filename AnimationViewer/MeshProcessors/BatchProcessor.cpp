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

// This class converts the triangle mesh information loaded from the FBX file into a format that the renderer can use.
// A batch is a list of vertex and index information gathered from triangles which have the same material applied. 
// Batches are created for each material, as the renderer will switch to the correct shader for the material and then pass the vertex and index info for each batch to openGL.
// The class loops through each triangle (for each mesh node). It checks the material info and creates a new batch if it has not seen the material before.
// When the mesh info is loaded from the fbx file the vertex information is per triangle - but we need vertex strips for rendering.
// As such this class is cycling through each triangle, checking every adjoining vertex, and where the vertex is different from the vertices in the mesh next to it, it creates a duplicate vertex
// for the renderer to use. In this way the additional vertex information from the triangle mesh is preserved.

namespace render
{

#define DOT_THESHOLD         0.9999f        /* Closeness to a dot product of 1 at which two normals are considered the same */

BatchProcessor::BatchProcessor(
	mesh::MeshPtr &mesh
) : m_mesh(mesh)
{
}


BatchProcessor::~BatchProcessor()
{
}

// TODO triangle strips
//https://developer.apple.com/library/ios/documentation/3ddrawing/conceptual/opengles_programmingguide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

// Create per vertex data in per material batches. Extra vertices are created to accommodate per triangle corner information.
void BatchProcessor::CreateBatches(
	render::PerNodeBatchList &perNodeRenderBatches // Batch vector to fill in
	)
{
	// Each node needs a new batch (due to each node having a different matrix transform
	//TODO could insist on node transforms being zero and thus create fewer batches). Within each node each material needs a new batch.
	mesh::MeshNode* rootMeshNode = m_mesh->GetMeshNodeHierarchy();

	CreateBatchesInternal(rootMeshNode, perNodeRenderBatches);

	return;
}

// TODO can bone indeces/weights be set by triangle corner vertex? If yes then these should also be split where they differ.
// FIXME this can be improved as it is currently potentially wasteful - we only compare to the first added vertex at an index - so if there are 4 more verts at this index which are different from this one but the same as each other they will all be duplicated.
void BatchProcessor::CreateBatchesInternal(
	mesh::MeshNode* meshNode,
	render::PerNodeBatchList &perNodeRenderBatches // Batch vector to fill in
	)
{
	for(meshNode; meshNode != NULL; meshNode = meshNode->m_next)
	{
		render::AppearanceTable& appearances = meshNode->GetAppearanceTable();

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
		// TODO uh shorter name?
		std::vector<int>previouslyAssignedVertexIndexMap(numVertices, -1);
		std::vector<std::vector<int>> perMaterialPreviouslyAssignedVertexIndexMap(appearances.size(), previouslyAssignedVertexIndexMap);

		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			int materialId = triangleArray[triangleIndex].GetMaterialId();
			assert(materialId != -1); // Every triangle must have a material
			std::vector<int> previouslyAssignedVertexIndexMap = perMaterialPreviouslyAssignedVertexIndexMap[materialId];

			if(!renderBatches[materialId]) // If a batch for this material does not already exist then create it
			{
				AddNewBatch(renderBatches, meshNode, appearances, materialId);
			}

			// Assign the mesh information to batches. Reassigning the indices as the data is processed to account for duplicated vertices being added.
			for(int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
			{
				unsigned int testVertexIndex = triangleArray[triangleIndex].GetVertexIndex(triangleCornerIndex);

				// Here we create the per vertex data from the mesh triangles

				// First create a vertex which contains the vertex data from the various mesh data arrays.
				render::TexturedSkinnedVertex testVertex;
				CreateRenderVertex(testVertex, vertexArray, testVertexIndex, triangleArray, triangleIndex, triangleCornerIndex);

				int previouslyCreatedIndex = previouslyAssignedVertexIndexMap[testVertexIndex]; // FIXME this should loop to compare all vertices - see comment by function declaration

				// If a vertex at this index has not been added previously then we can just add the vertex to the correct render batch and store the index in the map
				if(previouslyCreatedIndex == -1)
				{
					AddDuplicateVertex(testVertexIndex, testVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
					continue;
				}

				// Otherwise get the previous vertex to compare with the new one
				render::TexturedSkinnedVertex previouslyCreatedVertex = renderBatches[materialId]->GetVertices()[previouslyCreatedIndex];
				
				// If we have seen this vertex before, but this triangle corner has a different colour, uv coord or normal 
				// then duplicate the vertex to preserve the extra info (this will increase the vertex count in the mesh but will allow for
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
				else // An identical vertex was already added so don't add this one - just add the index and we refer to the existing vertex when rendering.
				{
					renderBatches[materialId]->AddIndex(previouslyCreatedIndex);
				}
			}
		}

		std::pair<unsigned int, render::BatchList> renderBatchWithMeshId(meshNode->GetId(), renderBatches);
		perNodeRenderBatches.insert(renderBatchWithMeshId);

		for(mesh::MeshNode *childNode = meshNode->m_firstChild; childNode != NULL; childNode = childNode->m_next)
		{
			CreateBatchesInternal(childNode, perNodeRenderBatches);
		}
	}
}

void BatchProcessor::AddNewBatch(
	render::BatchList &renderBatches,
	mesh::MeshNode* meshNode,
	render::AppearanceTable& appearances,
	const int materialId
)
{
	renderBatches[materialId] = render::BatchPtr(new render::Batch(/*render::ColourFormat*/));
	int numVertices = meshNode->GetNumVerticesWithMaterialId(materialId);
	renderBatches[materialId]->AllocateVertices(numVertices);
	renderBatches[materialId]->AllocateIndices(numVertices);
	renderBatches[materialId]->SetAppearance(appearances[materialId]);
	glm::mat4 batchModelMatrix;
	utils::MathsUtils::ConvertFBXToGLMatrix(meshNode->GetGlobalTransform(), batchModelMatrix);
	renderBatches[materialId]->SetModelMatrix(batchModelMatrix);
}

void BatchProcessor::CreateRenderVertex(
	render::TexturedSkinnedVertex &vertex,		// Vertex to store info
	const mesh::MeshVertexArray &vertexArray,	// Vertex array containing the vertices
	const int vertexIndex,						// The index of a vertex to copy
	const mesh::MeshTriangleArray &triangleArray, // The traingle array containing all the triangles
	const int triangleIndex,					// The index of the triangle we need to copy a vert from
	const int triangleCornerIndex				// The specific vertex in the triangle that we need to copy
)
{
	// Copy the vertex data from the correct triangle. Positions are stored per vertex in fbx so get that as well.
	// TODO override equals or put this in a copy method (see add duplicate vertex which is doing the same thing here)
	FbxVector4 position = vertexArray[vertexIndex].GetPosition();
	vertex.m_position = glm::vec3(static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]));
	vertex.m_colour = triangleArray[triangleIndex].GetColour(triangleCornerIndex);
	vertex.m_normal = glm::vec3(triangleArray[triangleIndex].GetNormal(triangleCornerIndex));
	vertex.m_uv = triangleArray[triangleIndex].GetUV(triangleCornerIndex);

	// Copy the bone info
	for (int weightIndex = 0; weightIndex < MAX_INFLUENCES; weightIndex++)
	{
		vertex.m_boneWeights[weightIndex] = vertexArray[vertexIndex].GetBoneWeight(weightIndex);
		vertex.m_boneIds[weightIndex] = vertexArray[vertexIndex].GetBoneInfluenceId(weightIndex);
	}
}

void BatchProcessor::AddDuplicateVertex(
	const int oldVertexIndex,
	const render::TexturedSkinnedVertex &currentVertex,
	render::Batch &batch,
	std::vector<int> &perMaterialPreviouslyAssignedVertexIndexMap
	)
{
	render::TexturedSkinnedVertex vertex;
	vertex.m_position = currentVertex.m_position; //TODO override equals or put this in a copy method
	vertex.m_colour = currentVertex.m_colour;
	vertex.m_uv = currentVertex.m_uv;
	vertex.m_normal += currentVertex.m_normal;
	if(glm::length(vertex.m_normal) != 0) // Dont crash please on the bugged mesh which has it's normals set to zero(!)
	{
		vertex.m_normal = glm::normalize(vertex.m_normal); // Make sure these are normalised
	}
	for(int weightIndex = 0; weightIndex < MAX_INFLUENCES; weightIndex++)
	{
		vertex.m_boneWeights[weightIndex] = currentVertex.m_boneWeights[weightIndex];
		vertex.m_boneIds[weightIndex] = currentVertex.m_boneIds[weightIndex];
	}

	// Add the data to the batch
	int newIndex = batch.GetNumVertices();
	perMaterialPreviouslyAssignedVertexIndexMap[oldVertexIndex] = newIndex;
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
			if(*batchIterator != NULL) // This will be the case if an appearance exists but is not used by this node //TODO get rid of this?
			{
				(*batchIterator)->PrepareForRendering();
			}
		}
	}
}

void BatchProcessor::SortBatches()
{

}

}

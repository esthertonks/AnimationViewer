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
// When the mesh info is loaded from the fbx file the vertex information is per triangle - but we need indexed vertex lists for rendering.
// As such this class is cycling through each triangle, checking every adjoining vertex, and where the vertex is different from the vertices in the mesh at the same position (ie at adjoining triangles), it creates a duplicate vertex
// for the renderer to use. In this way the additional vertex information from the triangle mesh is preserved.

namespace render
{

#define DOT_THESHOLD         0.9999f        /* Closeness to a dot product of 1 at which two normals are considered the same */

BatchProcessor::BatchProcessor(
	const mesh::MeshPtr &mesh
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
	// Each node needs a new batch (due to each node having a different matrix transform)
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
				unsigned int duplicateVertexIndex = triangleArray[triangleIndex].GetVertexIndex(triangleCornerIndex);

				// Here we create the per vertex data from the mesh triangles

				// First create a vertex which contains the vertex data from the various mesh data arrays.
				FbxVector4 fbxPosition = vertexArray[duplicateVertexIndex].GetPosition();
				glm::vec3 position = glm::vec3(static_cast<float>(fbxPosition[0]), static_cast<float>(fbxPosition[1]), static_cast<float>(fbxPosition[2]));
				glm::vec3 colour = triangleArray[triangleIndex].GetColour(triangleCornerIndex);
				glm::vec3 normal = glm::vec3(triangleArray[triangleIndex].GetNormal(triangleCornerIndex));
				glm::vec2 uv = triangleArray[triangleIndex].GetUV(triangleCornerIndex);

				render::TexturedSkinnedVertex duplicateVertex(position, colour, normal, uv, vertexArray[duplicateVertexIndex].GetBoneWeights(), vertexArray[duplicateVertexIndex].GetBoneInfluences());

				int previouslyCreatedIndex = previouslyAssignedVertexIndexMap[duplicateVertexIndex]; // FIXME this should loop to compare all vertices - see comment by function declaration

				// If a vertex at this index has not been added previously then we can just add the vertex to the correct render batch and store the index in the map
				if(previouslyCreatedIndex == -1)
				{
					AddVertexToBatch(duplicateVertexIndex, duplicateVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
					continue;
				}

				// Otherwise get the previous vertex to compare with the new one
				render::TexturedSkinnedVertex previouslyCreatedVertex = renderBatches[materialId]->GetVertices()[previouslyCreatedIndex];
				
				// If we have seen this vertex before, but this triangle corner has a different colour, uv coord or normal 
				// then duplicate the vertex to preserve the extra info (this will increase the vertex count in the mesh but will allow for
				// hard edges and texture seams which will otherwise have to be averaged for opengl).
				if(duplicateVertex.m_colour != previouslyCreatedVertex.m_colour)
				{
					AddVertexToBatch(duplicateVertexIndex, duplicateVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
				}
				else if(glm::dot(duplicateVertex.m_normal, previouslyCreatedVertex.m_normal) < DOT_THESHOLD) // The two normals are not the same - add a duplicate vertex
				{
					AddVertexToBatch(duplicateVertexIndex, duplicateVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
				}
				else if(duplicateVertex.m_uv != previouslyCreatedVertex.m_uv) // dupicate verts for texture coordinates on texture seams
				{
					AddVertexToBatch(duplicateVertexIndex, duplicateVertex, *renderBatches[materialId], previouslyAssignedVertexIndexMap);
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
	renderBatches[materialId] = render::BatchPtr(new render::Batch());
	int numVertices = meshNode->GetNumVerticesWithMaterialId(materialId);
	renderBatches[materialId]->AllocateVertices(numVertices);
	renderBatches[materialId]->AllocateIndices(numVertices);
	renderBatches[materialId]->SetAppearance(appearances[materialId]);
	glm::mat4 batchModelMatrix;
	utils::MathsUtils::ConvertFBXToGLMatrix(meshNode->GetGlobalTransform(), batchModelMatrix);
	renderBatches[materialId]->SetModelMatrix(batchModelMatrix);
}

void BatchProcessor::AddVertexToBatch(
	const int oldVertexIndex,
	const render::TexturedSkinnedVertex &vertex,
	render::Batch &batch,
	std::vector<int> &perMaterialPreviouslyAssignedVertexIndexMap
	)
{
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

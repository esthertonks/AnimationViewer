#include "BatchProcessor.h"
#include "../ImportMesh/Mesh.h"
#include "../ImportMesh/Triangle.h"
#include "../ImportMesh/Vertex.h"
#include "../Batch/VertexFormat.h"
#include "../Batch/BatchList.h"
#include "../Batch/Batch.h"
#include "../Batch/Appearance.h"

#include <wx\log.h>

namespace batch
{

BatchProcessor::BatchProcessor()
{
}


BatchProcessor::~BatchProcessor()
{
}

//TODO traingle strips
//https://developer.apple.com/library/ios/documentation/3ddrawing/conceptual/opengles_programmingguide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

void BatchProcessor::CreateBatches(
	mesh::Mesh &importMesh,
	render::BatchList &renderBatches // Batch vector to fill in
	)
{
		// TODO for testing atm assume only one - rewrite for many shortly
		render::AppearanceTable& appearances = importMesh.GetAppearances();
		int numBatches = appearances.size();
		renderBatches.resize(renderBatches.size() + numBatches);

		// TODO need to split verts along uv seams first
		// TODO need to split verts along hard edge normals first
		// TODO need to split by vertex format first?

	// TODO For now just average everything, but this needs to create batches and split normals for textures, colours and normals.
	for(mesh::MeshNode* meshNode = importMesh.m_root; meshNode != NULL; meshNode = meshNode->m_next)
	{
		mesh::MeshTriangleArray triangleArray = meshNode->GetTriangles();
		int numTriangles = meshNode->GetNumTriangles();
		mesh::MeshVertexArray vertexArray = meshNode->GetVertices();
		int numVertices = meshNode->GetNumVertices();

				// Map containing the material id and a vector array containing the new index for vector[oldindex] in the array
		std::vector<int>oldToNewVertexIndexMap(numVertices, -1);
		std::vector<std::vector<int>> perMaterialOldToNewVertexIndexMap(numBatches, oldToNewVertexIndexMap);

		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			unsigned int materialId = triangleArray[triangleIndex].m_materialId;
			//std::vector<int> oldToNewVertexIndexMap = perMaterialOldToNewVertexIndexMap[materialId];

			if(!renderBatches[materialId]) // If a batch for this material does not already exist then create it
			{
				renderBatches[materialId] = render::BatchPtr(new render::Batch(render::VertexFormatType::ColourFormat));
				int numVertices = importMesh.GetNumVerticesWithMaterialId(materialId);
				renderBatches[materialId]->AllocateVertices(numVertices);
				renderBatches[materialId]->AllocateIndices(numVertices);
				renderBatches[materialId]->SetAppearance(appearances[materialId]);
			}

			// Assign the mesh information to batches. Reassigning the indices as the data is processed.
			for(int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
			{
				unsigned int currentVertexIndex = triangleArray[triangleIndex].m_vertexIndices[triangleCornerIndex];
				render::Vertex vertex;

				int newIndex = perMaterialOldToNewVertexIndexMap[materialId][currentVertexIndex];
				if(newIndex == -1) // If this is the first time we have seen this index in this batch create a new vertex for the batch
				{
					vertex.m_position = glm::vec3(vertexArray[currentVertexIndex].m_position);
					vertex.m_colour = triangleArray[triangleIndex].m_colours[triangleCornerIndex]; //TODO currently this is just getting overriden by the next
					vertex.m_normal += glm::vec3(triangleArray[triangleIndex].m_normals[triangleCornerIndex]); //TODO these need splitting
					vertex.m_normal = glm::normalize(vertex.m_normal); // TODO Not ideal as we're normalizing more than we need to - split into another loop?

					newIndex = renderBatches[materialId]->GetNumVertices();
					perMaterialOldToNewVertexIndexMap[materialId][currentVertexIndex] = newIndex;
					renderBatches[materialId]->AddVertex(vertex);
					renderBatches[materialId]->AddIndex(newIndex);
				}
				else // Otherwise get the existing vertex at this index and average the information with the new vertex information
				{
					vertex = renderBatches[materialId]->GetVertices()[newIndex];
					vertex.m_colour += triangleArray[triangleIndex].m_colours[triangleCornerIndex];// TODO This doesnt really work. These verts needs splitting first. They also need averaging properly.
					vertex.m_normal += glm::vec3(triangleArray[triangleIndex].m_normals[triangleCornerIndex]);
					vertex.m_normal = glm::normalize(vertex.m_normal); // TODO Not ideal as we're normalizing more than we need to - split into another loop?
					renderBatches[materialId]->AddIndex(newIndex);
				}
			}
		}
	}

	return;
}

void BatchProcessor::SortBatches()
{



}

}

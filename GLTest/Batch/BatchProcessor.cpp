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

render::BatchList *BatchProcessor::CreateBatches(
	mesh::Mesh &importMesh
	)
{
	render::BatchList* renderBatch = new render::BatchList();

	// TODO For now just average everything, but this needs to create batches and split normals for textures, colours and normals.
	for(mesh::MeshNode* meshNode = importMesh.m_root; meshNode != NULL; meshNode = meshNode->m_next)
	{
		boost::shared_array<mesh::Triangle>& triangleArray = meshNode->GetTriangles();
		int numTriangles = meshNode->GetNumTriangles();
		boost::shared_array<mesh::Vertex>& vertexArray = meshNode->GetVertices();
		int numVertices = meshNode->GetNumVertices();

		// TODO for testing atm assume only one - rewrite for many shortly
		mesh::AppearanceTable& appearances = meshNode->GetAppearances();
		int numBatches = appearances.size();
		std::vector<render::Batch> renderBatches;
		
		// For each triangle
		// Get the material id
		// Get the associated batch.
		// Add the verts and indeces for that triangle to the batch


		(appearances.begin()->second, render::VertexFormatType::ColourFormat)[numBatches];
		renderBatch->Add(*batch);
		batch->AllocateIndices(numTriangles * 3);
		render::IndexArrayPtr renderIndexArray = batch->GetIndices();	

		batch->AllocateVertices(numVertices); // Currently this is the same. TODO need to consider that when batching we will not know this count in advance.
		render::BatchVertexArrayPtr renderVertexArray = batch->GetVertices();

		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			for(int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
			{
				renderIndexArray[triangleIndex * 3 + triangleCornerIndex] = triangleArray[triangleIndex].m_vertexIndices[triangleCornerIndex];
			}
		}

		// TODO this will change but for now we'll just copy across the vertex position and the last colour
		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			for(int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
			{
				unsigned int vertexIndex = triangleArray[triangleIndex].m_vertexIndices[triangleCornerIndex];
				render::ColourVertexFormat &vertex = (render::ColourVertexFormat &) renderVertexArray[vertexIndex];

				vertex.m_colour = triangleArray[triangleIndex].m_colours[triangleCornerIndex]; //TODO currently this is just getting overriden by the next
				vertex.m_normal += glm::vec3(triangleArray[triangleIndex].m_normals[triangleCornerIndex]); //TODO these need splitting
				//wxLogDebug("NormalA %f, %f, %f\n", triangleArray[triangleIndex].m_normals[triangleCornerIndex].x, triangleArray[triangleIndex].m_normals[triangleCornerIndex].y, triangleArray[triangleIndex].m_normals[triangleCornerIndex].z);
			}
		}

		for(int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
		{
			render::ColourVertexFormat &vertex = (render::ColourVertexFormat &) renderVertexArray[vertexIndex];

			// Currently vec3 in rendering mesh
			vertex.m_position[0] = vertexArray[vertexIndex].m_position[0];
			vertex.m_position[1] = vertexArray[vertexIndex].m_position[1];
			vertex.m_position[2] = vertexArray[vertexIndex].m_position[2];

			vertex.m_normal = glm::normalize(vertex.m_normal);
			//wxLogDebug("NormalB %f, %f, %f\n", renderVertexArray[vertexIndex].m_normal.x, renderVertexArray[vertexIndex].m_normal.y, renderVertexArray[vertexIndex].m_normal.z);
		}
	}

	return renderBatch;
}

void BatchProcessor::SortBatches()
{



}

}

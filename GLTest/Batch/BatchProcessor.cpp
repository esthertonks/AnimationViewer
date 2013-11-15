#include "BatchProcessor.h"
#include "../ImportMesh/Mesh.h"
#include "../ImportMesh/Triangle.h"
#include "../ImportMesh/Vertex.h"
#include "../BatchMesh/RenderVertex.h"
#include "../BatchMesh/RenderMesh.h"
#include "../BatchMesh/RenderMeshNode.h"

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

mesh::RenderMesh *BatchProcessor::CreateRenderMesh(
	mesh::Mesh &importMesh
	)
{
	mesh::RenderMesh* renderMesh = new mesh::RenderMesh();

	// TODO For now just average everything, but this needs to create batches and split normals for textures, colours and normals.
	for(mesh::MeshNode* meshNode = importMesh.m_root; meshNode != NULL; meshNode = meshNode->m_next)
	{
		boost::shared_array<mesh::Triangle>& triangleArray = meshNode->GetTriangles();
		int numTriangles = meshNode->GetNumTriangles();
		boost::shared_array<mesh::Vertex>& vertexArray = meshNode->GetVertices();
		int numVertices = meshNode->GetNumVertices();

		mesh::RenderMeshNode *renderMeshNode = new mesh::RenderMeshNode();
		renderMesh->AddChildNode(*renderMeshNode);
		renderMeshNode->AllocateIndices(numTriangles * 3);
		boost::shared_array<unsigned short>& renderIndexArray = renderMeshNode->GetIndices();	

		renderMeshNode->AllocateVertices(numVertices); // Currently this is the same. TODO need to consider that when batching we will not know this count in advance.
		boost::shared_array<mesh::RenderVertex>& renderVertexArray = renderMeshNode->GetVertices();


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
				mesh::RenderVertex &vertex = renderVertexArray[vertexIndex];

				vertex.m_colour = triangleArray[triangleIndex].m_colours[triangleCornerIndex]; //TODO currently this is just getting overriden by the next
				vertex.m_normal = glm::vec3(triangleArray[triangleIndex].m_normals[triangleCornerIndex]); //TODO these need splitting
				//wxLogDebug("NormalA %f, %f, %f\n", triangleArray[triangleIndex].m_normals[triangleCornerIndex].x, triangleArray[triangleIndex].m_normals[triangleCornerIndex].y, triangleArray[triangleIndex].m_normals[triangleCornerIndex].z);
			}
		}


		for(int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
		{
			// Currently vec3 in rendering mesh
			renderVertexArray[vertexIndex].m_position[0] = vertexArray[vertexIndex].m_position[0];
			renderVertexArray[vertexIndex].m_position[1] = vertexArray[vertexIndex].m_position[1];
			renderVertexArray[vertexIndex].m_position[2] = vertexArray[vertexIndex].m_position[2];

			renderVertexArray[vertexIndex].m_normal = glm::normalize(renderVertexArray[vertexIndex].m_normal);
			wxLogDebug("NormalB %f, %f, %f\n", renderVertexArray[vertexIndex].m_normal.x, renderVertexArray[vertexIndex].m_normal.y, renderVertexArray[vertexIndex].m_normal.z);
		}
	}

	return renderMesh;
}

}

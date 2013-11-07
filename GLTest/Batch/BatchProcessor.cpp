#include "BatchProcessor.h"
#include "../ImportMesh/Mesh.h"
#include "../ImportMesh/Triangle.h"
#include "../ImportMesh/Vertex.h"
#include "../BatchMesh/RenderVertex.h"
#include "../BatchMesh/RenderMesh.h"
#include "../BatchMesh/RenderMeshNode.h"

namespace batch
{

BatchProcessor::BatchProcessor()
{
}


BatchProcessor::~BatchProcessor()
{
}

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
		boost::shared_array<unsigned int>& renderIndexArray = renderMeshNode->GetIndices();	

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

				vertex.m_colour = glm::vec3(255.0f, 255.0f, 255.0f);//triangleArray[triangleIndex].m_colours[triangleCornerIndex];
			}
		}


		for(int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
		{
			// Currently vec3 in rendering mesh
			renderVertexArray[vertexIndex].m_position[0] = vertexArray[vertexIndex].m_position[0];
			renderVertexArray[vertexIndex].m_position[1] = vertexArray[vertexIndex].m_position[1];
			renderVertexArray[vertexIndex].m_position[2] = vertexArray[vertexIndex].m_position[2];
		}
	}

	return renderMesh;
}

}

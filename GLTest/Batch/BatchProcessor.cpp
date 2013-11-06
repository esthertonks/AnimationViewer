#include "BatchProcessor.h"
#include "../ImportMesh/Mesh.h"
#include "../BatchMesh/RenderVertex.h"

namespace batch
{

BatchProcessor::BatchProcessor()
{
}


BatchProcessor::~BatchProcessor()
{
}

mesh::Mesh &BatchProcessor::ProcessImportMesh(
	mesh::Mesh &importMesh
	)
{
	// TODO For now just average everything, but this needs to create batches and split normals for textures, colours and normals.
	for(mesh::MeshNode* meshNode = importMesh.m_meshHierarchyRoot; meshNode != NULL; meshNode = meshNode->child)
	{
		boost::shared_array<mesh::Triangle>& triangleArray = meshNode->GetTriangles();
		int numTriangles = meshNode->GetNumTriangles();
		boost::shared_array<mesh::Vertex>& vertexArray = meshNode->GetVertices();
		int numVertices = meshNode->GetNumVertices();

		meshNode->AllocateRenderIndices(numTriangles * 3);
		boost::shared_array<unsigned int>& renderIndexArray = meshNode->GetRenderIndices();	

		meshNode->AllocateRenderVertices(numVertices); // Currently this is the same. TODO need to consider that when batching we will not know this count in advance.
		boost::shared_array<mesh::RenderVertex>& renderVertexArray = meshNode->GetRenderVertices();


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

				vertex.m_colour = triangleArray[triangleIndex].m_colours[triangleCornerIndex];
			}
		}


		for(int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
		{
			renderVertexArray[vertexIndex].m_position = vertexArray[vertexIndex].m_position;
		}
	}







	return importMesh;
}

}

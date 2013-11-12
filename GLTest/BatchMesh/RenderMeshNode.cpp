#include "RenderMeshNode.h"
#include "RenderVertex.h"

namespace mesh
{

RenderMeshNode::RenderMeshNode()
: m_numIndices(0),
m_numVertices(0)
{

}

void RenderMeshNode::AllocateVertices(
	const int numVertices
	)		
{
	m_vertexArray = boost::shared_array<RenderVertex>(new RenderVertex[numVertices]);
	m_numVertices = numVertices;
}

void RenderMeshNode::AllocateIndices(
	const int numIndices
	)		
{
	m_indexArray = boost::shared_array<unsigned short>(new unsigned short[numIndices]);
	m_numIndices = numIndices;
}
}
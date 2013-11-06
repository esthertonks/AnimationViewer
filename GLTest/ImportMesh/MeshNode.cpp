#include "MeshNode.h"

namespace mesh
{

MeshNode::MeshNode()
	: m_numTriangles(0),
	m_numVertices(0),
	m_numRenderIndices(0),
	m_numRenderVertices(0)
{


};

void MeshNode::AllocateVertices(
	const int numVertices
	)		
{
	m_vertexArray = boost::shared_array<Vertex>(new Vertex[numVertices]);
	m_numVertices = numVertices;
}

void MeshNode::AllocateTriangles(
	const int numTriangles
	)		
{
	m_triangleArray = boost::shared_array<Triangle>(new Triangle[numTriangles]);
	m_numTriangles = numTriangles;
}

void MeshNode::AllocateRenderVertices(
	const int numRenderVertices
	)		
{
	m_renderVertexArray = boost::shared_array<RenderVertex>(new RenderVertex[numRenderVertices]);
	m_numRenderVertices = numRenderVertices;
}

void MeshNode::AllocateRenderIndices(
	const int numRenderIndices
	)		
{
	m_renderIndexArray = boost::shared_array<unsigned int>(new unsigned int[numRenderIndices]);
	m_numRenderIndices = numRenderIndices;
}

MeshNode::~MeshNode()
{

};


}
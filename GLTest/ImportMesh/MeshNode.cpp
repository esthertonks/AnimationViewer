#include "MeshNode.h"
#include "Vertex.h"
#include "Triangle.h"

namespace mesh
{

MeshNode::MeshNode()
	: m_numTriangles(0),
	m_numVertices(0)
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

MeshNode::~MeshNode()
{

};


}
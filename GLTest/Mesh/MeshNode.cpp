#include "MeshNode.h"
#include "Vertex.h"
#include "Triangle.h"
#include "../Batch/Appearance.h"

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
	m_vertexArray = MeshVertexArray(new Vertex[numVertices]);
	m_numVertices = numVertices;
}

void MeshNode::AllocateTriangles(
	const int numTriangles
	)		
{
	m_triangleArray = MeshTriangleArray(new Triangle[numTriangles]);
	m_numTriangles = numTriangles;
}

MeshNode::~MeshNode()
{
};


}
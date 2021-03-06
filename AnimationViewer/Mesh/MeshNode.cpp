#include "MeshNode.h"
#include "Vertex.h"
#include "Triangle.h"
#include "../Batch/Appearance.h"

namespace mesh
{
	unsigned int MeshNode::m_counter = 0;

MeshNode::MeshNode()
	: m_numTriangles(0),
	m_numVertices(0),
	m_id(m_counter++) //TODO could do with something less error prone than an int id - name crc maybe?
{


};

void MeshNode::AllocateVertices(
	const int numVertices
	)		
{
	m_vertexArray = MeshVertexArrayPtr(new Vertex[numVertices]);
	m_numVertices = numVertices;
}

void MeshNode::AllocateTriangles(
	const int numTriangles
	)		
{
	m_triangleArray = MeshTriangleArrayPtr(new Triangle[numTriangles]);
	m_numTriangles = numTriangles;
}

MeshNode::~MeshNode()
{
};


}
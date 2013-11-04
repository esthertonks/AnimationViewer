#include "MeshNode.h"

namespace mesh
{

MeshNode::MeshNode()
{


};

void MeshNode::AllocateVertices(
	const int numVertices
	)		
{
	m_vertexArray = boost::shared_array<Vertex>(new Vertex[numVertices]);
}

void MeshNode::AllocateTriangles(
	const int numTriangles
	)		
{
	m_triangleArray = boost::shared_array<Triangle>(new Triangle[numTriangles]);
}

MeshNode::~MeshNode()
{

};


}
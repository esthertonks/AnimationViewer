#include "Batch.h"
#include "VertexFormat.h"

namespace render
{

Batch::Batch(
	AppearancePtr appearance,
	VertexFormatType vertexFormatType
	)
: 
m_appearance(appearance),
m_numIndices(0),
m_numVertices(0),
m_vertexFormatType(vertexFormatType)
{

}

void Batch::AllocateVertices(
	const int numVertices
	)		
{
	m_vertexArray.reserve(numVertices);
	m_numVertices = numVertices;
}

void Batch::AllocateIndices(
	const short numIndices
	)		
{
	m_indexArray.reserve(numIndices);
	m_numIndices = numIndices;
}

void Batch::AddVertex(
	const VertexFormat &vertex,
	const short vertexIndex
	)
{
	m_vertexArray.push_back(vertex);
	m_indexArray.push_back(vertexIndex);
}

}
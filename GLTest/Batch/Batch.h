#pragma once

#include <boost\shared_array.hpp>
#include "../Container/LinkedList .h"
#include "BatchFwdDecl.h"

namespace render
{

enum VertexFormatType;

class Batch : public container::LinkedListItem<Batch>
{

public:

	Batch(
		AppearancePtr appearance,
		VertexFormatType vertexFormatType
		);

	~Batch(){};

	int GetNumIndices()
	{
		return m_numIndices;
	}

	int GetNumVertices()
	{
		return m_numVertices;
	}

	BatchVertexArrayPtr GetVertices()
	{
		return m_vertexArray;
	}

	VertexFormatType Batch::GetVertexFormatType()
	{
		return m_vertexFormatType;
	}

	IndexArrayPtr GetIndices()
	{
		return m_indexArray;
	}

	void AddVertex(
		const VertexFormat &vertex,
		const short vertexIndex
		);

	void CorrectIndeces();

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateIndices(
										const short numIndices
										);

	void Prepare();

	void Render();

private:

	BatchVertexArrayPtr m_vertexArray;
	IndexArrayPtr m_indexArray;
	short m_numIndices;
	int m_numVertices;
	AppearancePtr m_appearance; // Several batches could have the same appearance
	VertexFormatType m_vertexFormatType;
};
}
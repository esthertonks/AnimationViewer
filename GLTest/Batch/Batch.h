#pragma once

#include <boost\shared_array.hpp>
#include "../Container/LinkedList .h"
#include "BatchFwdDecl.h"
#include "../Render/RenderFwdDecl.h"

namespace render
{

enum VertexFormatType;

class Batch : public container::LinkedListItem<Batch>
{

public:

	Batch(
		VertexFormatType vertexFormatType
		);

	~Batch();

	inline GLuint GetVertexArrayHandle()
	{
		return m_vertexArrayHandle;
	}

	int GetNumIndices()
	{
		return m_indexArray.size();
	}

	int GetNumVertices()
	{
		return m_vertexArray.size();
	}

	RenderVertexArrayPtr GetVertices()
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

	AppearancePtr GetAppearance()
	{
		return m_appearance;
	}

	void SetAppearance(
	AppearancePtr appearance
	)
	{
		m_appearance = appearance;
	};

	ShaderProgramType GetShaderProgramType();

	void AddVertex(
		const Vertex &vertex
		);

	void AddIndex(
		const short vertexIndex
		);

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateIndices(
										const short numIndices
										);

	void Prepare();

	void Batch::PrepareShaderParams(
		GLuint programId
		);


	void Render();

private:

	RenderVertexArrayPtr m_vertexArray;
	IndexArrayPtr m_indexArray;
	AppearancePtr m_appearance; // Several batches could have the same appearance
	VertexFormatType m_vertexFormatType;

	GLuint m_vertexArrayHandle;

	GLuint m_indexBufferHandle;
	GLuint m_positionBufferHandle;

	bool m_meshLoaded;
};
}
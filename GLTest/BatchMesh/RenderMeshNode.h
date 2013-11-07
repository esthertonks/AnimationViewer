#pragma once

#include <boost\shared_array.hpp>
#include "../Container/LinkedList .h"

namespace mesh
{

class RenderVertex;

class RenderMeshNode : public container::LinkedListItem<RenderMeshNode>
{

public:

	RenderMeshNode();
	~RenderMeshNode(){};

	int GetNumIndeces()
	{
		return m_numIndices;
	}

	int GetNumVerteces()
	{
		return m_numVertices;
	}

	boost::shared_array<RenderVertex> &GetVertices()
	{
		return m_vertexArray;
	}

	boost::shared_array<unsigned int> &GetIndices()
	{
		return m_indexArray;
	}

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateIndices(
										const int numIndices
										);

private:


	boost::shared_array<RenderVertex> m_vertexArray;
	boost::shared_array<unsigned int> m_indexArray;
	int m_numIndices;
	int m_numVertices;

};
}
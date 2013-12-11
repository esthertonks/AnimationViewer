#pragma once

#include <map>
#include <boost\shared_array.hpp>
#include "Node.h"

namespace import
{
	class FBXImport;
}

namespace mesh
{
	class ImportVertex;
	class Triangle;

	typedef boost::shared_array<ImportVertex> MeshVertexArray; //TODO any better container?
	typedef boost::shared_array<Triangle> MeshTriangleArray;

class MeshNode : public Node
{
public:

	MeshNode();
	~MeshNode();

	virtual NodeType GetType()
	{
		return MeshType;
	}

	int GetNumTriangles()
	{
		return m_numTriangles;
	}

	int GetNumVertices()
	{
		return m_numVertices;
	}

	MeshVertexArray GetVertices()
	{
		return m_vertexArray;
	}

	MeshTriangleArray GetTriangles()
	{
		return m_triangleArray;
	}

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateTriangles(
										const int numTriangles
										);

private:
	// uvsets?
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	MeshVertexArray m_vertexArray;
	MeshTriangleArray m_triangleArray;
	int m_numTriangles;
	int m_numVertices;
};


}
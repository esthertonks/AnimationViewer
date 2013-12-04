#pragma once

#include <glm\glm.hpp>
#include <string>
#include <map>
#include <boost\shared_array.hpp>
#include "../Container/LinkedList .h"
#include "../Import/FBXImport.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{
	class Vertex;
	class Triangle;

	typedef boost::shared_array<Vertex> MeshVertexArray; //TODO any better container?
	typedef boost::shared_array<Triangle> MeshTriangleArray;

class MeshNode : public container::LinkedListItem<MeshNode>
{
public:

	MeshNode();
	~MeshNode();

	void SetName(
		const std::string& name
		)
	{
		m_name = name;
	};

	const std::string&	GetName()
	{		
		return m_name;
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
	friend import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

	std::string m_name;

	//glm::mat4x4 m_localTransform;			//Transform from this node to the parent node
	//glm::mat4x4 m_globalTransform;		// Transform from this node to the global model transform

	// uvsets?

	MeshVertexArray m_vertexArray;
	MeshTriangleArray m_triangleArray;
	int m_numTriangles;
	int m_numVertices;
};


}
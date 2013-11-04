#pragma once

#include <glm\glm.hpp>
#include <string>
#include <boost\shared_array.hpp>
#include "Vertex.h"
#include "Triangle.h"

namespace mesh
{

class MeshNode
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

	boost::shared_array<Vertex> &GetVertices()
	{
		return m_vertexArray;
	}

	boost::shared_array<Triangle> &GetTriangles()
	{
		return m_triangleArray;
	}

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateTriangles(
										const int numTriangles
										);

	MeshNode *child;
	MeshNode *parent;

private:
	std::string m_name;

	glm::mat4x4 localTransform;			//Transform from this node to the parent node
	glm::mat4x4 globalTransform;		// Transform from this node to the world //TODO to the world or to the mesh?

	// uvsets?

	boost::shared_array<Vertex> m_vertexArray;
	boost::shared_array<Triangle> m_triangleArray;
};


}
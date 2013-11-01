#pragma once

#include <glm\glm.hpp>
#include <string>
#include "Vertex.h"
#include "Triangle.h"

namespace mesh
{

class MeshNode
{
public:

	void SetName(std::string& name)
	{
		m_name = name;
	}

	MeshNode *child;
	MeshNode *parent;

private:
	std::string m_name;

	glm::mat4x4 localTransform;			//Transform from this node to the parent node
	glm::mat4x4 globalTransform;		// Transform from this node to the world //TODO to the world or to the mesh?

	// uvsets?

	Vertex *m_vertexArray;
	Triangle *m_triangleArray;
};


}
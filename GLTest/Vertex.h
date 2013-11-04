#pragma once

#include <glm\glm.hpp>

namespace mesh
{

class Vertex
{
public:

	glm::vec4 m_position;
	glm::vec4 m_normal;//Vertex normal
	glm::vec2 m_uvs;
	glm::vec3 m_colour;
};

}
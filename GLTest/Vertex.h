#pragma once

#include <glm\glm.hpp>

namespace mesh
{

class Vertex
{
public:

	glm::vec4 m_position;

	glm::vec4 m_normal;
	glm::vec4 m_binormal;
	glm::vec4 m_tangent;

	glm::vec2 m_uv[3];
	glm::vec3 m_colour[3];
};

}
#pragma once

#include <glm\glm.hpp>

namespace render
{

enum VertexFormatType //TODO may not need this?
{
	ColourFormat,
	TextureFormat
};

struct Vertex
{
	glm::vec3 m_position;
	glm::vec2 m_uv[2];
	glm::vec3 m_colour;
	glm::vec3 m_normal;
};

}
#pragma once

#include <glm\glm.hpp>

namespace render
{

enum VertexFormatType
{
	ColourFormat,
	TextureFormat
};

class VertexFormat
{
	public:

	VertexFormat(){};
	virtual ~VertexFormat(){};
};

class ColourVertexFormat : public VertexFormat
{
	public:

	ColourVertexFormat(){};
	~ColourVertexFormat(){};

	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec3 m_normal;
};

class TextureVertexFormat : public VertexFormat
{
	public:

	TextureVertexFormat(){};
	~TextureVertexFormat(){};

	glm::vec3 m_position;
	glm::vec2 m_uv[2];
	glm::vec3 m_normal;
};

}
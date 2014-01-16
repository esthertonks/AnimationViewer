#pragma once

#include <glm\glm.hpp>

namespace render
{

//enum VertexFormatType //TODO may not need this?
//{
//	ColourFormat,
//	TextureFormat,
//	SkinnedTexturedFormat
//};

struct TexturedVertex
{
	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
};

struct TexturedSkinnedVertex
{
	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
	float m_boneWeights[4];
	float m_boneIds[4]; // float because gl required passing floats
	int m_numWeights;
};

// This vertex has a position and colour but no normal or uv (used for overlays)
struct ColourVertex
{
	glm::vec3 m_position;
	glm::vec3 m_colour;
};

}
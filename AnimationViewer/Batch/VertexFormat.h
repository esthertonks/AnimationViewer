#pragma once

// TODO move this file somewhere more general?

#include <glm\glm.hpp>
#include "../Mesh/MeshFwdDecl.h"

namespace render
{

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
	int m_boneIds[MAX_INFLUENCES];
	float m_boneWeights[MAX_INFLUENCES];

	TexturedSkinnedVertex(
		const glm::vec3 position, 
		const glm::vec3 colour,
		const glm::vec3 normal,
		const glm::vec2 uv,
		const float *boneWeights,
		const unsigned int *boneIds
	)
	{
		m_position = position;
		m_colour = colour;
		m_uv = uv;
		m_normal = normal;
		if (glm::length(m_normal) != 0) // Dont crash please on the bugged mesh which has it's normals set to zero(!)
		{
			m_normal = glm::normalize(m_normal); // Make sure these are normalised
		}
		for (int weightIndex = 0; weightIndex < MAX_INFLUENCES; weightIndex++)
		{
			m_boneWeights[weightIndex] = boneWeights[weightIndex];
			m_boneIds[weightIndex] = boneIds[weightIndex];
		}
	}
};


// This vertex has a position and colour but no normal or uv (used for overlays)
struct ColourVertex
{
	glm::vec3 m_position;
	glm::vec3 m_colour;
};

}
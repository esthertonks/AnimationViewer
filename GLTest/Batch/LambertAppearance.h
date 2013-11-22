#pragma once

#include "Appearance.h"

namespace render
{

class LambertAppearance : public Appearance
{

public:

	LambertAppearance(){};
	~LambertAppearance(){};

virtual MaterialType GetType()
{
	return MaterialType::Lambert;
}

inline void SetAmbient(
	const glm::vec3 &ambient // Ambient colour
	)
{
	m_ambient = ambient;
}

inline void SetDiffuse(
	const glm::vec3 &diffuse // Diffuse colour
	)
{
	m_diffuse = diffuse;
}

inline void SetEmissive(
	const glm::vec3 &emissive // Emissive colour
	)
{
	m_emissive = emissive;
}

inline void SetTransparency(
	double transparency
	)
{
	m_transparency = transparency;
}

inline const glm::vec3& GetAmbient() const
{
	return m_ambient;
}

inline const glm::vec3& GetDiffuse() const
{
	return m_diffuse;
}

inline const glm::vec3& GetEmissive() const
{
	return m_emissive;
} 

inline const double GetTransparency() const
{
	return m_transparency;
}

private:
	// Shader Inputs
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_emissive;

	double m_transparency;
};
}
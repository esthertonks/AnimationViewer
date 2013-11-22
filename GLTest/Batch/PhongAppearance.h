#pragma once

#include "Appearance.h"

namespace render
{

class PhongAppearance : public Appearance
{

public:

	PhongAppearance(){};
	~PhongAppearance(){};

virtual MaterialType GetType()
{
	return MaterialType::Phong;
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

inline void SetSpecular(
	 const glm::vec3 &specular // Specular colour
	)
{
	m_specular = specular;
}

inline void SetEmissive(
	 const glm::vec3 &emissive // Emissive colour
	)
{
	m_emissive = emissive;
}

inline void SetTransparency(
	 const double transparency
	)
{
	m_transparency = transparency;
}

inline void SetShininess(
	double shininess
	)
{
	m_shininess = shininess;
}

inline void SetReflectivity(
	double reflectivity
	)
{
	m_reflectivity = reflectivity;
}

inline const glm::vec3 &GetAmbient() const
{
	return m_ambient;
}

inline const glm::vec3 &GetDiffuse() const
{
	return m_diffuse;
}

inline const glm::vec3 &GetSpecular() const
{
	return m_specular;
}

inline const glm::vec3 &GetEmissive() const
{
	return m_emissive;
}

inline const double GetTransparency() const
{
	return m_transparency;
}

inline const double GetShininess() const
{
	return m_shininess;
}

inline const double GetReflectivity() const
{
	return m_reflectivity;
}

private:
	// Shader Inputs
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	glm::vec3 m_emissive;

	double m_transparency;
	double m_shininess;
	double m_reflectivity;
};
}
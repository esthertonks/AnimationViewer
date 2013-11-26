#pragma once

#include "Appearance.h"

namespace render
{

class PhongAppearance : public Appearance
{

public:

	PhongAppearance();
	~PhongAppearance(){};

virtual MaterialType GetType()
{
	return MaterialType::Phong;
}

inline void SetSpecular(
	 const glm::vec3 &specular // Specular colour
	)
{
	m_specular = specular;
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

inline const glm::vec3 &GetSpecular() const
{
	return m_specular;
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
	glm::vec3 m_specular;

	double m_shininess;
	double m_reflectivity;
};
}
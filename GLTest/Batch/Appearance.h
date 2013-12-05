#pragma once

#include "../Render/RenderFwdDecl.h"
#include <vector>
#include <string>
#include <glm\glm.hpp>

namespace render
{

class Appearance abstract
{

public:

	Appearance();
	virtual ~Appearance(){};

void SetDiffuseTexturePath(
	const std::string &texturePath
	)
{
	m_diffuseTexturePath = texturePath;
};

std::string &GetDiffuseTexturePath()
{
	return m_diffuseTexturePath;
}

virtual ShaderProgramType GetType() = 0;

virtual void ConvertToShaderParams(
	const GLuint programId
	) const = 0;

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

inline void SetDiffuseFactor(
	double diffuseFactor
	)
{
	m_diffuseFactor = diffuseFactor;
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

inline const double GetDiffuseFactor() const
{
	return m_diffuseFactor;
}

protected:

	// opengl states

	std::string m_diffuseTexturePath;

	ShaderProgramType m_type;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_emissive;

	double m_transparency;
	double m_diffuseFactor;

};
}
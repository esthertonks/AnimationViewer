#include "LambertAppearance.h"

namespace render
{

LambertAppearance::LambertAppearance()
	: Appearance(), 
	defaultMaterialAmbient(0.4f, 0.4f, 0.5f),
	defaultMaterialDiffuse(0.4f, 0.4f, 0.5f),
	defaultMaterialDiffuseFactor(0.5),
	defaultLightAmbient(0.6f, 0.6f, 0.6f),
	defaultLightDiffuse(1.0f, 1.0f, 1.0f)
{
}

void LambertAppearance::ConvertToShaderParams(
	GLuint programId
	) const
{
	// TODO need to be able to add more than one light
	GLint lightAmbientLocation = glGetUniformLocation(programId, "light.ambient");
	glUniform3f(lightAmbientLocation, defaultLightAmbient.r, defaultLightAmbient.g, defaultLightAmbient.b);

	GLint lightDiffuseLocation = glGetUniformLocation(programId, "light.diffuse");
	glUniform3f(lightDiffuseLocation, defaultLightDiffuse.r, defaultLightDiffuse.g, defaultLightDiffuse.b);

	GLint ambientLocation = glGetUniformLocation(programId, "material.ambient");

	float ambientr = glm::max(defaultMaterialAmbient.r, m_ambient.r);
	float ambientg = glm::max(defaultMaterialAmbient.g, m_ambient.g);
	float ambientb = glm::max(defaultMaterialAmbient.b, m_ambient.b);

	glUniform3f(ambientLocation, ambientr, ambientg, ambientb);

	float diffuser = glm::max(defaultMaterialDiffuse.r, m_diffuse.r);
	float diffuseg = glm::max(defaultMaterialDiffuse.g, m_diffuse.g);
	float diffuseb = glm::max(defaultMaterialDiffuse.b, m_diffuse.b);

	GLint diffuseLocation = glGetUniformLocation(programId, "material.diffuse");
	glUniform3f(diffuseLocation, diffuser, diffuseg, diffuseb);

	double diffuseFactor = glm::max(defaultMaterialDiffuseFactor, m_diffuseFactor); // 1.0 == full diffuse, 0.0 == no diffuse

	GLint diffuseFactorLocation = glGetUniformLocation(programId, "material.diffuseFactor");
	glUniform1f(diffuseFactorLocation, m_diffuseFactor);

	if(!m_diffuseTexturePath.empty())
	{
		int textureLocation = glGetUniformLocation(programId, "diffuseTexture");
		glUniform1i(textureLocation, 0);
	}
}

}
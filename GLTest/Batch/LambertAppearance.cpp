#include "LambertAppearance.h"

namespace render
{

LambertAppearance::LambertAppearance()
: Appearance()
{
}

void LambertAppearance::ConvertToShaderParams(
	GLuint programId
	) const
{
	// TODO need to be able to add more than one light
	GLint lightAmbientLocation = glGetUniformLocation(programId, "light.ambient");
	glUniform3f(lightAmbientLocation, 0.5f, 0.5f, 0.5f);

	GLint lightDiffuseLocation = glGetUniformLocation(programId, "light.diffuse");
	glUniform3f(lightDiffuseLocation, 0.9f, 0.9f, 0.9f);

	GLint ambientLocation = glGetUniformLocation(programId, "material.ambient");
	glUniform3f(ambientLocation, m_ambient.r, m_ambient.g, m_ambient.b);

	GLint diffuseLocation = glGetUniformLocation(programId, "material.diffuse");
	glUniform3f(diffuseLocation, m_diffuse.r, m_diffuse.g, m_diffuse.b);

	GLint diffuseFactorLocation = glGetUniformLocation(programId, "material.diffuseFactor");
	glUniform1f(diffuseFactorLocation, m_diffuseFactor);

	if(!m_diffuseTexturePath.empty())
	{
		int textureLocation = glGetUniformLocation(programId, "diffuseTexture");
		glUniform1i(textureLocation, 0);
	}
}

}
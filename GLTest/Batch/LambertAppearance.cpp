#include "LambertAppearance.h"

namespace render
{

LambertAppearance::LambertAppearance()
: Appearance()
{
}

void LambertAppearance::ConvertToShaderParams(
	GLuint programHandle
	) const
{
	GLint ambientLocation = glGetUniformLocation(programHandle, "material.ambient");
	glUniform3f(ambientLocation, m_ambient.r, m_ambient.g, m_ambient.b);

	GLint diffuseLocation = glGetUniformLocation(programHandle, "material.diffuse");
	glUniform3f(diffuseLocation, m_diffuse.r, m_diffuse.g, m_diffuse.b);

	GLint diffuseFactorLocation = glGetUniformLocation(programHandle, "material.diffuseFactor");
	glUniform1f(diffuseFactorLocation, m_diffuseFactor);

	if(!m_diffuseTexturePath.empty())
	{
		int textureLocation = glGetUniformLocation(programHandle, "diffuseTexture");
		glUniform1i(textureLocation, 0);
	}
}

}
#include "PhongAppearance.h"

namespace render
{

PhongAppearance::PhongAppearance()
: Appearance(),
m_shininess(0.0f),
m_reflectivity(0.0f)
{
}

void PhongAppearance::ConvertToShaderParams(
	GLuint programHandle
	) const
{
	GLint ambientLocation = glGetUniformLocation(programHandle, "material.ambient");
	glUniform3f(ambientLocation, m_ambient.r, m_ambient.g, m_ambient.b);

	GLint diffuseLocation = glGetUniformLocation(programHandle, "material.diffuse");
	glUniform3f(diffuseLocation, m_diffuse.r, m_diffuse.g, m_diffuse.b);

	GLint diffuseFactorLocation = glGetUniformLocation(programHandle, "material.diffuseFactor");
	glUniform1f(diffuseFactorLocation, m_diffuseFactor);

	GLint specularLocation = glGetUniformLocation(programHandle, "material.specular");
	glUniform3f(specularLocation, m_specular.r, m_specular.g, m_specular.b);

	GLint shininessLocation = glGetUniformLocation(programHandle, "material.shininess");
	glUniform1f(shininessLocation, m_shininess);
}

}
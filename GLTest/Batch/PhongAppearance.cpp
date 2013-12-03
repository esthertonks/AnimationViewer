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
	GLuint programId
	) const
{
	// TODO need to be able to add more than one light
	GLint lightAmbientLocation = glGetUniformLocation(programId, "light.ambient");
	glUniform3f(lightAmbientLocation, 0.1f, 0.1f, 0.1f);

	GLint lightDiffuseLocation = glGetUniformLocation(programId, "light.diffuse");
	glUniform3f(lightDiffuseLocation, 0.5f, 0.5f, 0.5f);

	GLint lightSpecularLocation = glGetUniformLocation(programId, "light.specular");
	glUniform3f(lightSpecularLocation, 0.9f, 0.9f, 0.9f);

	GLint ambientLocation = glGetUniformLocation(programId, "material.ambient");
	glUniform3f(ambientLocation, m_ambient.r, m_ambient.g, m_ambient.b);

	GLint diffuseLocation = glGetUniformLocation(programId, "material.diffuse");
	glUniform3f(diffuseLocation, m_diffuse.r, m_diffuse.g, m_diffuse.b);

	GLint diffuseFactorLocation = glGetUniformLocation(programId, "material.diffuseFactor");
	glUniform1f(diffuseFactorLocation, m_diffuseFactor);

	GLint specularLocation = glGetUniformLocation(programId, "material.specular");
	glUniform3f(specularLocation, m_specular.r, m_specular.g, m_specular.b);

	GLint shininessLocation = glGetUniformLocation(programId, "material.shininess");
	glUniform1f(shininessLocation, m_shininess);
}

}
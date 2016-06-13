#pragma once

#include "Appearance.h"

namespace render
{

class LambertAppearance : public Appearance
{

public:

	LambertAppearance();
	~LambertAppearance(){};

	virtual ShaderProgramType GetType()
	{
		return Lambert;
	}

	virtual void ConvertToShaderParams(
		const GLuint programId
		) const;

private:

	glm::vec3 defaultMaterialAmbient;
	glm::vec3 defaultMaterialDiffuse;
	double defaultMaterialDiffuseFactor;
	glm::vec3 defaultLightAmbient;
	glm::vec3 defaultLightDiffuse;
};
}
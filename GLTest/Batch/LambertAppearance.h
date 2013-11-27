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
	return ShaderProgramType::Lambert;
}

virtual void ConvertToShaderParams(
	const GLuint programHandle
	) const;

private:

	// Shader Inputs?
};
}
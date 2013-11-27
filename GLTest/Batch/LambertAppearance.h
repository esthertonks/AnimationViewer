#pragma once

#include "Appearance.h"

namespace render
{

class LambertAppearance : public Appearance
{

public:

	LambertAppearance();
	~LambertAppearance(){};

virtual MaterialType GetType()
{
	return MaterialType::Lambert;
}

virtual void ConvertToShaderParams(
	const GLuint programHandle
	) const;

private:

	// Shader Inputs?
};
}
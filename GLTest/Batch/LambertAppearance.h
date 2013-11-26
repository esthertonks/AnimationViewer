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

private:

	// Shader Inputs?
};
}
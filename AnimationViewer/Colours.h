#pragma once

#include "wx/wx.h"
#include <glm\glm.hpp>

struct Colour {
	float r;
	float g;
	float b;

	inline Colour(float red, float green, float blue) { r = red; g = green; b = blue; };
};


namespace theme
{
class Colours
{
public:

	const static Colour m_darkGrey;
	const static Colour m_lightGrey;
	const static Colour m_duskyBlue;
	const static Colour m_grey;
};

}


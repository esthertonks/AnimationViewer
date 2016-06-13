#pragma once

#include "wx/wx.h"
#include <glm\glm.hpp>

struct Colour {

	inline Colour(
		const float red,
		const float green, 
		const float blue
	) : r(red), g(green),  b(blue) {};

	inline const wxColour GetAsUIColour() const { 
		return wxColour(r * 255.0f, g * 255.0f, b * 255.0f);
	};

	inline const glm::vec3 GetAsRenderColour() const { 
		return glm::vec3(r, g, b);
	};

	const float r;
	const float g;
	const float b;
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


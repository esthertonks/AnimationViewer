#pragma once

#include "wx/wx.h"
#include <glm\glm.hpp>

namespace theme
{
class Colour
{
public:

	// TODO glm::vec3 - this is glm's idea of a colour!! typedef???
	const static glm::vec3 m_darkGrey;
	const static glm::vec3 m_lightGrey;
	const static glm::vec3 m_duskyBlue;
	const static glm::vec3 m_grey;
};

}


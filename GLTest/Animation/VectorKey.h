#pragma once

#include "Key.h"
#include <glm/glm.hpp>

/*
	Track base class
*/
namespace animation
{

class VectorKey : public Key
{
public :

VectorKey(
	glm::vec3 &value
	)
{
	m_value = value;
};

glm::vec3 m_value;
};

}
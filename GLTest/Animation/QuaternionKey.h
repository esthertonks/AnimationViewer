#pragma once

#include "Key.h"
#include <glm/gtx/quaternion.hpp> 

/*
	Track base class
*/
namespace animation
{

class QuaternionKey : public Key
{
public :

QuaternionKey(
	glm::quat &value
	)
{
	m_value = value;
}

glm::quat m_value;
};

}
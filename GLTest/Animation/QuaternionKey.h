#pragma once

#include "Key.h"
//#include <glm/gtx/quaternion.hpp> 

/*
	Track base class
*/
namespace animation
{

class QuaternionKey : public Key
{
public :

//QuaternionKey(
//	glm::quat &value
//	)
//{
//	m_value = value;
//}

//glm::quat m_value;

QuaternionKey(float x, float y, float z, float w, long time) : Key(time)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;

}

float m_x;
float m_y;
float m_z;
float m_w;

};

}
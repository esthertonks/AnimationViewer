#pragma once

#include "Key.h"

/*
	Track base class
*/
namespace animation
{

class VectorKey : public Key
{
public :

VectorKey(
	float x, float y, float z, long time) : Key(time)
{
	m_x = x;
	m_y = y;
	m_z = z;
};

float m_x;
float m_y;
float m_z;
};

}
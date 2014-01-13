#pragma once

#include "Key.h"

/*
	Track base class
*/
namespace animation
{

class QuaternionKey : public Key
{
public :

QuaternionKey(
double x, 
double y,
double z,
double w,
long time) 
: Key(time)
{
	m_quaternion[0] = x;
	m_quaternion[1] = y;
	m_quaternion[2] = z;
	m_quaternion[3] = w;
};

QuaternionKey(FbxQuaternion quaternion, long time) : Key(time)
{
	m_quaternion = quaternion;
}

FbxQuaternion m_quaternion;

};

}
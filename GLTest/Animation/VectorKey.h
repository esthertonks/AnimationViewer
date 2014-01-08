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
FbxVector4 vector, 
long time) 
: Key(time)
{
	m_vector = vector;
};

FbxVector4 m_vector;
};

}
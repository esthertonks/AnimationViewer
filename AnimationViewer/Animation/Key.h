#pragma once

#include "AnimationFwdDecl.h"

namespace animation
{

class Key
{
	public :

	Key(long time){
		m_time = time;
	};

	virtual ~Key(){};

	long m_time;
};

}
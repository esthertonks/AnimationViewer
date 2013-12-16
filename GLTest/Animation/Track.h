#pragma once

#include "AnimationFwdDecl.h"

/*
	Track base class
*/
namespace animation
{

class Track
{
public:

Track(
	const int numFrames
	);

void AddKey(
	const boost::shared_ptr<Key>key
	);

const boost::shared_ptr<Key> GetKey(
	int frame
	);

private:

	KeyArray m_keys;
};

}
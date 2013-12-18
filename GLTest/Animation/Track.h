#pragma once

#include "Key.h"
#include <vector>
#include "boost/shared_ptr.hpp"

//#include "AnimationFwdDecl.h"
typedef std::vector<boost::shared_ptr<animation::Key>> KeyArray;

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
	boost::shared_ptr<Key>key
	);

const boost::shared_ptr<Key> GetKey(
	int frame
	);

const KeyArray GetKeys() const
{
	return m_keys;
}

const int GetNumKeys() const
{
	return m_keys.size();

}

private:

	KeyArray m_keys;
};

}
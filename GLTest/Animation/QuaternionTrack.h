#pragma once

#include "Track.h"
#include "QuaternionKey.h"

namespace animation
{

class QuaternionTrack : public Track
{
public:

QuaternionTrack(
	const int totalNumKeys
	) : Track()
{
	m_keys.reserve(totalNumKeys);

}

void QuaternionTrack::AddKey(
	boost::shared_ptr<QuaternionKey> key
	)
{
	m_keys.push_back(key);
}

const boost::shared_ptr<QuaternionKey> QuaternionTrack::GetKey(
	int key
	)
{
	return m_keys[key];
}


private:
	QuaternionKeyArray m_keys;

};
}
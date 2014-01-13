#pragma once

#include "Track.h"
#include "VectorKey.h"

namespace animation
{

class VectorTrack : public Track
{
public:

VectorTrack(
	const int totalNumKeys
	) : Track()
{
	m_keys.reserve(totalNumKeys);

}

void VectorTrack::AddKey(
	boost::shared_ptr<VectorKey> key
	)
{
	m_keys.push_back(key);
}

const boost::shared_ptr<VectorKey> VectorTrack::GetKey(
	int key
	)
{
	return m_keys[key];
}


private:
	VectorKeyArray m_keys;

};

}
#include "Track.h"

namespace animation
{

Track::Track(
	const int numFrames
	)
{
	m_keys.reserve(numFrames);
}

void Track::AddKey(
	boost::shared_ptr<Key> key
	)
{
	m_keys.push_back(key);
}


const boost::shared_ptr<Key> Track::GetKey(
	int frame
	)
{
	return m_keys[frame];
}




}
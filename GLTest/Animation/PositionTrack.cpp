#include "PositionTrack.h"

namespace animation
{

PositionTrack::PositionTrack(
	const int numFrames
	)
{
	m_positionKeys.reserve(numFrames);
}

void PositionTrack::AddKey(
	const glm::vec3 &position
	)
{
	m_positionKeys.push_back(position);
}


const glm::vec3 &PositionTrack::GetKey(
	int frame
	)
{

	return m_positionKeys[frame];
}




}
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
	const int frame,
	const glm::vec3 &position
	)
{
	m_positionKeys[frame] = position;
}







}
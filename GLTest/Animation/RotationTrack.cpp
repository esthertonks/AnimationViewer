#include "RotationTrack.h"

namespace animation
{

RotationTrack::RotationTrack(
	const int numFrames
	)
{
	m_rotationKeys.reserve(numFrames);
}

void RotationTrack::AddKey(
	const int frame,
	const glm::quat &rotation
	)
{
	m_rotationKeys[frame] = rotation;
}







}
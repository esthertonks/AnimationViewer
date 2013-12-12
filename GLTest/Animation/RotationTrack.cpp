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
	const glm::quat &rotation
	)
{
	m_rotationKeys.push_back(rotation);
}

const glm::quat &RotationTrack::GetKey(
	int frame
	)
{

	return m_rotationKeys[frame];
}







}
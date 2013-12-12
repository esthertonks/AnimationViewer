#include "ScaleTrack.h"

namespace animation
{

ScaleTrack::ScaleTrack(
	const int numFrames
	)
{
	m_scaleKeys.reserve(numFrames);
}

void ScaleTrack::AddKey(
	const glm::vec3 &scale
	)
{
	m_scaleKeys.push_back(scale);
}

const glm::vec3 &ScaleTrack::GetKey(
	int frame
	)
{

	return m_scaleKeys[frame];
}

}
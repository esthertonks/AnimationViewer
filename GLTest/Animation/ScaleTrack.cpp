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
	const int frame,
	const glm::vec3 &scale
	)
{
	m_scaleKeys[frame] = scale;
}

}
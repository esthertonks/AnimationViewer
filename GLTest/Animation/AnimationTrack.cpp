#include "AnimationTrack.h"

#include "RotationTrack.h"
#include "ScaleTrack.h"
#include "PositionTrack.h"

namespace animation
{

AnimationTrack::AnimationTrack(
	const int numFrames
	)
: fps(0.0f),
numFrames(numFrames),
loop(false)
{
	m_rotationTrack = RotationTrackPtr(new RotationTrack(numFrames));
	m_scaleTrack = ScaleTrackPtr(new ScaleTrack(numFrames));
	m_positionTrack = PositionTrackPtr(new PositionTrack(numFrames));
}


}
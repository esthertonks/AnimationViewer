#pragma once

#include "RotationTrack.h"
#include "ScaleTrack .h"
#include "PositionTrack.h"

/*
	AnimationTrack class containing the rotate, scale and position tracks for each bone
*/
namespace animation
{

class AnimationTrack
{
public :

	RotationTrack m_rotationTrack;
	ScaleTrack m_scaleTrack;
	PositionTrack m_positionTrack;
};

}
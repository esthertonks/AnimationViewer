#pragma once

#include "AnimationFwdDecl.h"
/*
	PositionTrack class
*/
namespace animation
{

class PositionTrack
{
public :

PositionTrack(
	const int numFrames
	);

void AddKey(
	const int frame,
	const glm::vec3 &position
	);


private:

	Vector3Array m_positionKeys;
};

}
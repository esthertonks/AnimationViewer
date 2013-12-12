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
	const glm::vec3 &position
	);

const glm::vec3 &GetKey(
	int frame
	);


private:

	Vector3Array m_positionKeys;
};

}
#pragma once

#include "AnimationFwdDecl.h"
/*
	Rotation quaterion animation track class
*/
namespace animation
{

class RotationTrack
{
public :

RotationTrack(
	const int numFrames
	);

void AddKey(
	const int frame,
	const glm::quat &rotation
	);


private:

	QuaternionArray m_rotationKeys;
};

}
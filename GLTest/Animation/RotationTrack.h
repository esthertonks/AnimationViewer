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
	const glm::quat &rotation
	);

const glm::quat &GetKey(
	int frame
	);


private:

	QuaternionArray m_rotationKeys;
};

}
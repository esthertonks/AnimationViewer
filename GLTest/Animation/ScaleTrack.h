#pragma once

#include "AnimationFwdDecl.h"
/*
	ScaleTrack class
*/
namespace animation
{

class ScaleTrack
{
public :

ScaleTrack(
	const int numFrames
	);

void AddKey(
	const int frame,
	const glm::vec3 &scale
	);


private:

	Vector3Array m_scaleKeys;
};

}
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
	const glm::vec3 &scale
	);

const glm::vec3 &GetKey(
	int frame
	);

private:

	Vector3Array m_scaleKeys;
};

}
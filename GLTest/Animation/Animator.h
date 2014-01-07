#pragma once

#include <string>
#include <glm/glm.hpp>
//#include "AnimationFwdDecl.h"
#include "Track.h"
#include "VectorKey.h"
#include "QuaternionKey.h"
/*
	Information about an animation.
*/

namespace mesh
{
	class Node;
}

namespace animation
{
	class AnimationInfo;

class Animator
{
public :

	Animator();
	~Animator(){};

void StartAnimation(
	long globalStartTime,
	AnimationInfo *animationInfo
	);

void PauseAnimation();

void StopAnimation();

void PrepareBoneHierarcy(
	mesh::Node *node,
	const long globalTime
	);

void PrepareBoneHierarcy(
	int sample,
	mesh::Node* node,
	const glm::mat4x4 &parentGlobalScaleMatrix,
	const glm::mat4x4 &parentGlobalRotationMatrix
	);

private:
	void ClampTime();
//void BinarySearchKeys(
//	boost::shared_ptr<animation::Track> track,
//	const long timeToFind,
//	int startKeyIndex,
//	int endKeyIndex,
//	int &currentKeyIndex
//	);

boost::shared_ptr<VectorKey> InterpolatePosition(
	int sample,
	boost::shared_ptr<animation::Track> positionTrack
	);

boost::shared_ptr<QuaternionKey> InterpolateRotation(
	int sample,
	boost::shared_ptr<animation::Track> rotationTrack
	);

boost::shared_ptr<VectorKey> InterpolateScale(
	int sample,
	boost::shared_ptr<animation::Track> scaleTrack
	);

boost::shared_ptr<VectorKey> Lerp(
	const long time, 
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey
	);

boost::shared_ptr<QuaternionKey> Lerp(
	const long time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	);

boost::shared_ptr<QuaternionKey> Slerp(
	const long time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	);

	long m_globalStartTime;
	long m_localCurrentTime;
	long m_animStartTime;
	long m_animEndTime;
	AnimationInfo *m_animationInfo;
};

}
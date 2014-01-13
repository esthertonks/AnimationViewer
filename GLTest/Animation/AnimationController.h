#pragma once

#include <string>
#include <glm/glm.hpp>
//#include "AnimationFwdDecl.h"
#include "Track.h"
#include "VectorKey.h"
#include "QuaternionKey.h"
#include "../Mesh/MeshFwdDecl.h"
/*
	Animates any mesh::Mesh using the animation tracks contained within that mesh
*/

namespace animation
{

class AnimationController
{
public :

	AnimationController();

	~AnimationController(){};

void StartAnimation(
	long globalStartTime,
	long animStartTime,
	long animEndTime
	);

void PauseAnimation();

void StopAnimation();

void Update(
	mesh::MeshPtr m_mesh,
	const long globalTime,
	const bool isLooping
	);

private:
	void ClampTime(
		const bool isLooping
		);

	void PrepareBoneHierarcy(
	int sample,
	mesh::Node* node,
	const FbxAMatrix &parentGlobalScaleMatrix,
	const FbxAMatrix &parentGlobalRotationMatrix
	);

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
	const float time, 
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey
	);

boost::shared_ptr<QuaternionKey> Lerp(
	const float time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	);

boost::shared_ptr<QuaternionKey> Slerp(
	const float time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	);

	long m_globalStartTime;
	long m_localCurrentTime;
	long m_animStartTime;
	long m_animEndTime;
};

}
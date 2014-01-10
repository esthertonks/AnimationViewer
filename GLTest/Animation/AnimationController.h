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

/*
	Takes the current time and the time at the keyframe before and after this time.
	Returns a normalised time value between 0.0 and 1.0 where 0.0 is the time at the last
	keyframe and 1.0 is the time at the next keyframe.
*/
float NormalizeTime(
	const long currentTime, // The current local time
	const long lastKeyTime, // The time at the last keyframe
	const long nextKeyTime	 // The time at the next keyframe
	) const;

	long m_globalStartTime;
	long m_localCurrentTime;
	long m_animStartTime;
	long m_animEndTime;
};

}
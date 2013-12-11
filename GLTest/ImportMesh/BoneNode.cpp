#include "BoneNode.h"
#include "../Animation/AnimationTrack.h"

namespace mesh
{

BoneNode::BoneNode()
	: m_inheritScale(false),
	m_animationTrack(NULL)
{

};

// Allocate memory for an animation track. Currently only one animation track is supported.
void BoneNode::AllocateAnimationTrack(
	int numFrames
	)
{
	m_animationTrack = animation::AnimationTrackPtr(new animation::AnimationTrack(numFrames));
}

void BoneNode::SetLocalKeyTransform(
	int frameNum,
	const glm::mat4x4 &localTransform
	)
{
	//m_localTransform = localTransform;
	//Tsl::Maths::CVector3 position, scale;
	//Tsl::Maths::CQuaternion orientation;
	//localTransform.Decompose(&position, &scale, &orientation);

	m_animationTrack->GetPositionTrack().AddKey(frameNum, position.x, position.y, position.z, 0);
	m_animationTrack->GetScaleTrack().AddKey(frameNum, scale.x, scale.y, scale.z, 0);
	m_animationTrack->GetOrientationTrack().AddKey(frameNum, orientation.x, orientation.y, orientation.z, orientation.w, 0);
};

glm::mat4x4 &BoneNode::GetLocalKeyTransform(
	int key
	)
{
	return m_localTransform;
};


BoneNode::~BoneNode()
{
};


}
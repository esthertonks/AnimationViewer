#include "BoneNode.h"
#include "../Animation/AnimationTrack.h"
#include "../Animation/PositionTrack.h"
#include "../Animation/RotationTrack.h"
#include "../Animation/ScaleTrack.h"
#include "../Utils/MathsUtils.h"

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

	// As we are using uniform scaling atm can just get the rotation and translation from the transform.
	//glm::vec3 position;
	//glm::vec3 scale;
	//glm::quat rotation;

	//m_animationTrack->GetPositionTrack().AddKey(frameNum, position.x, position.y, position.z, 0);
	//m_animationTrack->GetScaleTrack().AddKey(frameNum, scale.x, scale.y, scale.z, 0);
	//m_animationTrack->GetOrientationTrack().AddKey(frameNum, orientation.x, orientation.y, orientation.z, orientation.w, 0);
};

void BoneNode::AddLocalKeyTransform(
	const glm::vec3 &position,
	const glm::quat &rotation,
	const glm::vec3 &scale
	)
{
	m_animationTrack->GetPositionTrack()->AddKey(position);
	m_animationTrack->GetScaleTrack()->AddKey(scale);
	m_animationTrack->GetRotationTrack()->AddKey(rotation);
};

void BoneNode::GetLocalKeyTransform(
	int key,
	glm::vec3& position,
	glm::quat& rotationQuat,
	glm::vec3& scale
	)
{
	position = m_animationTrack->GetPositionTrack()->GetKey(key);
	rotationQuat = m_animationTrack->GetRotationTrack()->GetKey(key);
	scale = m_animationTrack->GetScaleTrack()->GetKey(key);

	//utils::MathsUtils::TranslateRotateScale(position, rotationMatrix, scale, localTransform);

	return;
};


BoneNode::~BoneNode()
{
};


}
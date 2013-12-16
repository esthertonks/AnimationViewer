#include "BoneNode.h"
#include "../Animation/VectorTrack.h"
#include "../Animation/QuaternionTrack.h"
#include "../Utils/MathsUtils.h"
#include "../Animation/Key.h"

namespace mesh
{

BoneNode::BoneNode()
	: m_inheritScale(false),
	m_rotationTrack(NULL),
	m_positionTrack(NULL),
	m_scaleTrack(NULL)
{

};

// Allocate memory for an animation track. Currently only one animation track is supported.
void BoneNode::AllocateAnimationTracks(
	int numFrames
	)
{
	m_rotationTrack = animation::TrackPtr(new animation::Track(numFrames));
	m_scaleTrack = animation::TrackPtr(new animation::Track(numFrames));
	m_positionTrack = animation::TrackPtr(new animation::Track(numFrames));
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
	const long time,
	const boost::shared_ptr<animation::VectorKey> position,
	const boost::shared_ptr<animation::QuaternionKey> rotation,
	const boost::shared_ptr<animation::VectorKey> scale
	)
{
	m_positionTrack->AddKey(boost::static_pointer_cast<animation::Key>(position));
	m_scaleTrack->AddKey(boost::static_pointer_cast<animation::Key>(scale));
	m_rotationTrack->AddKey(boost::static_pointer_cast<animation::Key>(rotation));
};
//
//void BoneNode::GetLocalKeyTransform(
//	int key,
//	glm::vec3& position,
//	glm::quat& rotationQuat,
//	glm::vec3& scale
//	)
//{
//	position = m_positionTrack->GetKey(key);
//	rotationQuat = m_rotationTrack->GetKey(key);
//	scale = m_scaleTrack->GetKey(key);
//
//	//utils::MathsUtils::TranslateRotateScale(position, rotationMatrix, scale, localTransform);
//
//	return;
//};


BoneNode::~BoneNode()
{
};


}
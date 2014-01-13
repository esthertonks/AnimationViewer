#include "AnimationController.h"

#include "../Mesh/Node.h"
#include "../Mesh/BoneNode.h"
#include "../Animation/QuaternionKey.h"
#include "../Animation/VectorKey.h"
#include "../Animation/Track.h"
#include "../Utils/MathsUtils.h"
#include "../Mesh/AnimationInfo.h"
#include "../Mesh/Mesh.h"

namespace animation
{

AnimationController::AnimationController()
	: 
	m_globalStartTime(0),
	m_localCurrentTime(0),
	m_animStartTime(0),
	m_animEndTime(0)
{


}

void AnimationController::StartAnimation(
	long globalStartTime,
	long animStartTime,
	long animEndTime
	)
{
	m_globalStartTime = globalStartTime;

	// Calculate the start and end times for this animation
	m_animStartTime = animStartTime;
	m_animEndTime = animEndTime;
}

// TODO the bone list is currently controlling this = shouldn't be??!
void AnimationController::PauseAnimation()
{
	m_globalStartTime = 0;
}

// TODO the bone list is currently controlling this = shouldn't be??!
void AnimationController::StopAnimation()
{
	m_globalStartTime = 0;
	m_localCurrentTime = 0;//TODO use floats and seconds for times????
	m_animEndTime = 0;
	m_animStartTime = 0;
}

void AnimationController::ClampTime(
	const bool isLooping
	)
{
	if(isLooping)
	{
		long animLength = m_animEndTime - m_animStartTime;//TODO anim duration
		m_localCurrentTime = m_localCurrentTime % animLength;
	}
	else
	{
		m_localCurrentTime = utils::MathsUtils::Clamp(m_localCurrentTime, m_animStartTime, m_animEndTime);
	}
}

// Passing the scale and rotation matrices to avoid have to extract the rotation and scale from the parent global matrix which is non trivial (necessary to switch between scale inheritance types).
void AnimationController::Update(
	mesh::MeshPtr mesh,
	const long globalTime,
	const bool isLooping
	)
{
	mesh::Node *root = mesh->GetNodeHierarchy();

	float playSpeed = 1; // 2x. Slower = 1/2 times. reverse = -2x
	bool reverse = false;
	if(!reverse)
	{
		m_localCurrentTime = (globalTime - m_globalStartTime) * playSpeed;
	}
	else
	{
		m_localCurrentTime = m_animEndTime - (globalTime - m_globalStartTime) * playSpeed;
	}

	mesh::AnimationInfoPtr animationInfo = mesh->GetAnimationInfo();

	ClampTime(isLooping);

	int sample = animationInfo->ConvertMillisecondsToFrame(m_localCurrentTime);

	assert(sample <= animationInfo->GetNumFrameSamples()); // Number of frames = number of frame samples - 1

	const FbxAMatrix parentGlobalScaleMatrix;// These will be identity for the root node
	const FbxAMatrix parentGlobalRotationMatrix;

	PrepareBoneHierarcy(sample, root, parentGlobalScaleMatrix, parentGlobalRotationMatrix);
}

void AnimationController::PrepareBoneHierarcy(
	int sample,
	mesh::Node *node,
	const FbxAMatrix &parentGlobalScaleMatrix,
	const FbxAMatrix &parentGlobalRotationMatrix
	)
{
	for(node; node != NULL; node = node->m_next)
	{
		if(node->GetType() == mesh::BoneType)
		{
			mesh::BoneNode* boneNode = static_cast<mesh::BoneNode*>(node);

			boost::shared_ptr<VectorKey> localPositionKey = InterpolatePosition(sample, boneNode->GetPositionTrack());
			boost::shared_ptr<QuaternionKey> localRotationKey = InterpolateRotation(sample, boneNode->GetRotationTrack());
			boost::shared_ptr<VectorKey> localScaleKey = InterpolateScale(sample, boneNode->GetScaleTrack());

			FbxAMatrix localScaleMatrix;
			localScaleMatrix.SetS(localScaleKey->m_vector);
			FbxAMatrix localRotationMatrix;
			localRotationMatrix.SetQ(localRotationKey->m_quaternion);

			// Get the transform which needs setting
			FbxAMatrix& globalTransform = boneNode->GetGlobalTransform();

			if(boneNode->m_parent)
			{			
				const FbxAMatrix& parentTransform = boneNode->m_parent->GetGlobalTransform();

				FbxAMatrix globalRotationAndScale;

				// Bones exported from Max or Maya can inherit or not inherit scale on each node. In order to deal with this the rotation and scale must be calucated separately from the translation.

				if(boneNode->InheritsScale()) //FbxTransform::eInheritRrSs - translation * (parentRotate * localRotate * parentScale * local Scale)
				{
					globalRotationAndScale = parentGlobalRotationMatrix * localRotationMatrix * parentGlobalScaleMatrix * localScaleMatrix;
				}
				else //FbxTransform::eInheritRrs - translation * (parentRotate * localRotate * local Scale)
				{
					globalRotationAndScale = parentGlobalRotationMatrix * localRotationMatrix * localScaleMatrix;
				}

				// Transform the position by the whole parent transform so that the position gets rotated and scaled correctly.
				// Use a vector transform here as we don't want any other info from the global transform at this point.
				FbxVector4 globalPosition = parentTransform.MultT(localPositionKey->m_vector);

				FbxAMatrix globalPositionMatrix;
				globalPositionMatrix.SetT(globalPosition);
				globalTransform = globalPositionMatrix * globalRotationAndScale;

				if(node->m_firstChild)
				{
					// Record the global scale and rotation matrices for use by the child
					FbxAMatrix globalScaleMatrix = parentGlobalScaleMatrix * localScaleMatrix;

					// Record the global scale and rotation matrices for use by the child
					FbxAMatrix globalRotationMatrix = parentGlobalRotationMatrix * localRotationMatrix;

					PrepareBoneHierarcy(sample, node->m_firstChild, globalScaleMatrix, globalRotationMatrix); // Passing the parent scale and rotation avoids extracting them from the global transform which is non trivial
				}

			}
			else // No parent, so the global transform must be the same as the local transform
			{
				FbxAMatrix localTranslationMatrix;
				localTranslationMatrix.SetT(localPositionKey->m_vector);
				globalTransform = localTranslationMatrix * localRotationMatrix * localScaleMatrix;

				if(node->m_firstChild)
				{
					PrepareBoneHierarcy(sample, node->m_firstChild, localScaleMatrix, localRotationMatrix); // Passing the parent scale and rotation avoids extracting them from the global transform which is non trivial
				}

			}
		}
	}
}

boost::shared_ptr<VectorKey> AnimationController::InterpolatePosition(
	int sample,
	boost::shared_ptr<animation::Track> positionTrack
	)
{
	const boost::shared_ptr<VectorKey> lastPositionKey = boost::static_pointer_cast<VectorKey>(positionTrack->GetKey(sample));
	if(lastPositionKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	{
		return lastPositionKey;
	}
	else // Otherwise interpolate
	{
		const boost::shared_ptr<VectorKey> nextPositionKey = boost::static_pointer_cast<VectorKey>(positionTrack->GetKey(sample + 1));

		// Find the current time value as a 0 - 1 proporion between the two keys
		const float normalizedTime = utils::MathsUtils::NormalizeValue(m_localCurrentTime, lastPositionKey->m_time, nextPositionKey->m_time);
		return Lerp(normalizedTime, lastPositionKey, nextPositionKey);
	}
}

boost::shared_ptr<QuaternionKey> AnimationController::InterpolateRotation(
	int sample,
	boost::shared_ptr<animation::Track> rotationTrack
	)
{
	const boost::shared_ptr<QuaternionKey> lastRotationKey = boost::static_pointer_cast<QuaternionKey>(rotationTrack->GetKey(sample));
	if(lastRotationKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	{
		return lastRotationKey;
	}
	else
	{
		const boost::shared_ptr<QuaternionKey> nextRotationKey = boost::static_pointer_cast<QuaternionKey>(rotationTrack->GetKey(sample + 1));

		// Find the current time value as a 0 - 1 proporion between the two keys
		const float normalizedTime = utils::MathsUtils::NormalizeValue(m_localCurrentTime, lastRotationKey->m_time, nextRotationKey->m_time);
		return Slerp(normalizedTime, lastRotationKey, nextRotationKey);
	}
}

boost::shared_ptr<VectorKey> AnimationController::InterpolateScale(
	int sample,
	boost::shared_ptr<animation::Track> scaleTrack
	)
{
	const boost::shared_ptr<VectorKey> lastScaleKey = boost::static_pointer_cast<VectorKey>(scaleTrack->GetKey(sample));
	if(lastScaleKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	{
		return lastScaleKey;
	}
	else
	{
		const boost::shared_ptr<VectorKey> nextScaleKey = boost::static_pointer_cast<VectorKey>(scaleTrack->GetKey(sample + 1));
		// Find the current time value as a 0 - 1 proportion between the two keys
		const float normalizedTime = utils::MathsUtils::NormalizeValue(m_localCurrentTime, lastScaleKey->m_time, nextScaleKey->m_time);
		return Lerp(normalizedTime, lastScaleKey, nextScaleKey);
	}
}

boost::shared_ptr<VectorKey> AnimationController::Lerp(
	const float normalizedTime, // time value between 0 and 1 for interpolating betwen the keys
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey
	)
{
	// keyA + (keyB - keyA) * t
	FbxVector4 result = key->m_vector + (nextKey->m_vector - key->m_vector) * normalizedTime;
	return boost::shared_ptr<VectorKey>(new VectorKey(result, 0));
}

boost::shared_ptr<QuaternionKey> AnimationController::Lerp(
	const float normalizedTime, // time value between 0 and 1 for interpolating betwen the keys
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	)
{
	// keyA + (keyB - keyA) * t
	FbxQuaternion result = key->m_quaternion + (nextKey->m_quaternion - key->m_quaternion) * normalizedTime;
	return boost::shared_ptr<QuaternionKey>(new QuaternionKey(result, 0));
}

// Interpolate a quaternion rotation using SLERP
// TODO optimise ie http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
boost::shared_ptr<QuaternionKey> AnimationController::Slerp(
	const float normalizedTime, // time value between 0 and 1 for interpolating betwen the keys
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	)
{
	FbxQuaternion nextQuaternion = nextKey->m_quaternion; //Copy this incase we need to negate it

	// First calcualte the 4D dot product which gives cos theta
	double cosTheta = key->m_quaternion[0] * nextQuaternion[0] + key->m_quaternion[1] * nextQuaternion[1]
					+ key->m_quaternion[2] * nextQuaternion[2] + key->m_quaternion[3] * nextQuaternion[3];

	if(cosTheta == 1)
	{
		return key; // The two key are the same so just return.
	}

	if(cosTheta < 0) // q1 and q2 are more than 90 degrees apart so invert one of them to reduce spinning
	{
		cosTheta *= -1;
		nextQuaternion = -nextKey->m_quaternion;
	}

	if(cosTheta < 0.99999f) // If the keys are different
	{
		double theta = glm::acos(cosTheta);
		double firstKeyWeight = glm::sin((1 - normalizedTime) * theta) / glm::sin(theta);
		double nextKeyWeight = glm::sin(normalizedTime * theta) / glm::sin(theta);

		FbxQuaternion result = (key->m_quaternion * firstKeyWeight) + (nextQuaternion * nextKeyWeight);

		return boost::shared_ptr<QuaternionKey>(new QuaternionKey(result, 0));
	}

	return Lerp(normalizedTime, key, nextKey); // Keys are pretty much the same so just lerp
}

}
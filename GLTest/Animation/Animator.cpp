#include "Animator.h"

#include "../ImportMesh/Node.h"
#include "../ImportMesh/BoneNode.h"
#include "../Animation/QuaternionKey.h"
#include "../Animation/VectorKey.h"
#include "../Animation/Track.h"
#include "../Utils/MathsUtils.h"
#include "AnimationInfo.h"

#include <wx/log.h>

namespace animation
{

Animator::Animator()
	: m_globalStartTime(0),
	m_localCurrentTime(0),
	m_animStartTime(0),
	m_animEndTime(0),
	m_animationInfo(NULL)
{


}

void Animator::StartAnimation(
	long globalStartTime,
	AnimationInfo *animationInfo
	)
{
	m_globalStartTime = globalStartTime;
	m_animationInfo = animationInfo;

	// Calculate the start and end times for this animation
	m_animStartTime = animationInfo->ConvertFrameToMilliseconds(animationInfo->GetStartSample());
	m_animEndTime = animationInfo->ConvertFrameToMilliseconds(animationInfo->GetEndSample());
}

// TODO the bone list is currently controlling this = shouldn't be??!
void Animator::PauseAnimation()
{
	m_globalStartTime = 0;
	m_animationInfo = NULL;

}

// TODO the bone list is currently controlling this = shouldn't be??!
void Animator::StopAnimation()
{
	m_globalStartTime = 0;
	m_localCurrentTime = 0;
	m_animEndTime = 0;
	m_animStartTime = 0;
	m_animationInfo = NULL;
}

void Animator::ClampTime()
{
	if(m_animationInfo->IsLooping())
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
void Animator::PrepareBoneHierarcy(
	mesh::Node *node,
	const long globalTime
	)
{
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
	//playSpeed = 1 / playSpeed;
	//wxLogDebug("tick");
	//m_localCurrentTime = (globalTime - m_globalStartTime) * (1/m_animationInfo->GetFPS());

	//wxLogDebug("current time 1 %ld", m_localCurrentTime);
	ClampTime();
	//wxLogDebug("current time clamped %ld", m_localCurrentTime);
	int sample = m_animationInfo->ConvertMillisecondsToFrame(m_localCurrentTime);
	//wxLogDebug("sample: %ld\n", sample);
	assert(sample <= m_animationInfo->GetNumFrameSamples()); // Number of frames = number of frame samples - 1

	const FbxAMatrix parentGlobalScaleMatrix;// These will be identity for the root node
	const FbxAMatrix parentGlobalRotationMatrix;

	PrepareBoneHierarcy(sample, node, parentGlobalScaleMatrix, parentGlobalRotationMatrix);
	//wxLogDebug("tick2");
	//mesh::BoneNode* boneNode = static_cast<mesh::BoneNode*>(node);
	//FbxAMatrix& globalTransform = boneNode->GetGlobalTransform();
	//wxLogDebug("local posx: %f\n", static_cast<float>(globalTransform[3][0]));
	//wxLogDebug("local posx: %f\n", static_cast<float>(globalTransform[3][1]));
	//wxLogDebug("local posx: %f\n", static_cast<float>(globalTransform[3][2]));
}

void Animator::PrepareBoneHierarcy(
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
			//wxLogDebug("node: %s\n", boneNode->GetName().c_str());
			// Create local matrix in the order scale, rotate, translate to be stored in the global transform data (local transform data is only temporarily store
			//boneNode->GetLocalKeyTransform(frame, localPosition, localRotationKey, localScaleKey);
			boost::shared_ptr<VectorKey> localPositionKey = InterpolatePosition(sample, boneNode->GetPositionTrack());
			boost::shared_ptr<QuaternionKey> localRotationKey = InterpolateRotation(sample, boneNode->GetRotationTrack());
			boost::shared_ptr<VectorKey> localScaleKey = InterpolateScale(sample, boneNode->GetScaleTrack());

			//wxLogDebug("name: %s\n", boneNode->GetName().c_str());

			//wxLogDebug("local posx: %f\n", localPositionKey->m_vector[0]);
			//wxLogDebug("local posy: %f\n", localPositionKey->m_vector[1]);
			//wxLogDebug("local posz: %f\n", localPositionKey->m_vector[2]);

			//wxLogDebug("local rotx: %f\n", localRotationKey->m_quaternion[0]);
			//wxLogDebug("local roty: %f\n", localRotationKey->m_quaternion[1]);
			//wxLogDebug("local rotz: %f\n", localRotationKey->m_quaternion[2]);

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
				//wxLogDebug("global posx without local rotation: %f\n", globalPosition.x);
				//wxLogDebug("global posy without local rotation: %f\n", globalPosition.y);
				//wxLogDebug("global posz without local rotation: %f\n", globalPosition.z);

				FbxAMatrix globalPositionMatrix;
				globalPositionMatrix.SetT(globalPosition);
				globalTransform = globalPositionMatrix * globalRotationAndScale;

				//wxLogDebug("global transform00: %f\n", globalTransform[0][0]);
				//wxLogDebug("global transform01: %f\n", globalTransform[0][1]);
				//wxLogDebug("global transform02: %f\n", globalTransform[0][2]);
				//wxLogDebug("global transform03: %f\n", globalTransform[0][3]);

				//wxLogDebug("global transform10: %f\n", globalTransform[1][0]);
				//wxLogDebug("global transform11: %f\n", globalTransform[1][1]);
				//wxLogDebug("global transform12: %f\n", globalTransform[1][2]);
				//wxLogDebug("global transform13: %f\n", globalTransform[1][3]);

				//wxLogDebug("global transform20: %f\n", globalTransform[2][0]);
				//wxLogDebug("global transform21: %f\n", globalTransform[2][1]);
				//wxLogDebug("global transform22: %f\n", globalTransform[2][2]);
				//wxLogDebug("global transform23: %f\n", globalTransform[2][3]);

				//wxLogDebug("global transform30: %f\n", globalTransform[3][0]);
				//wxLogDebug("global transform31: %f\n", globalTransform[3][1]);
				//wxLogDebug("global transform32: %f\n", globalTransform[3][2]);
				//wxLogDebug("global transform33: %f\n", globalTransform[3][3]);

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

//void Animator::BinarySearchKeys(
//	boost::shared_ptr<animation::Track> track,
//	const long timeToFind,
//	int startKeyIndex,
//	int endKeyIndex,
//	int &currentKeyIndex
//	)
//{
//	int midKey =  startKeyIndex + (endKeyIndex - startKeyIndex) / 2;
//
//	if((track->GetKey(midKey)->m_time <= timeToFind))
//	{
//		currentKeyIndex = midKey;
//		return;
//	}
//
//	if(track->GetKey(midKey)->m_time > timeToFind)
//	{
//		endKeyIndex = midKey - 1;
//		BinarySearchKeys(track, timeToFind, startKeyIndex, endKeyIndex, currentKeyIndex);
//		return;
//	}
//
//	if(track->GetKey(midKey + 1)->m_time < timeToFind)
//	{
//		startKeyIndex = midKey + 1;
//		BinarySearchKeys(track, timeToFind, startKeyIndex, endKeyIndex, currentKeyIndex);
//		return;
//	}
//}

boost::shared_ptr<VectorKey> Animator::InterpolatePosition(
	int sample,
	boost::shared_ptr<animation::Track> positionTrack
	)
{
	const boost::shared_ptr<VectorKey> currentPositionKey = boost::static_pointer_cast<VectorKey>(positionTrack->GetKey(sample));
	if(currentPositionKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	{
		return currentPositionKey;
	}
	else // Otherwise interpolate
	{
		const boost::shared_ptr<VectorKey> nextPositionKey = boost::static_pointer_cast<VectorKey>(positionTrack->GetKey(sample + 1));

		// Find the current time value as a 0 - 1 proporion between the two keys
		float normalizedTime = (m_localCurrentTime - currentPositionKey->m_time) / (float)(nextPositionKey->m_time - currentPositionKey->m_time);
		assert(normalizedTime > -0.00001f && normalizedTime < 1.000001f);
		//wxLogDebug("pos time %f", normalizedTime);
		return Lerp(normalizedTime, currentPositionKey, nextPositionKey);
	}
}

boost::shared_ptr<QuaternionKey> Animator::InterpolateRotation(
	int sample,
	boost::shared_ptr<animation::Track> rotationTrack
	)
{
	const boost::shared_ptr<QuaternionKey> currentRotationKey = boost::static_pointer_cast<QuaternionKey>(rotationTrack->GetKey(sample));
	if(currentRotationKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	{
		return currentRotationKey;
	}
	else
	{
		const boost::shared_ptr<QuaternionKey> nextRotationKey = boost::static_pointer_cast<QuaternionKey>(rotationTrack->GetKey(sample + 1));

		// Find the current time value as a 0 - 1 proporion between the two keys
		float normalizedTime = (m_localCurrentTime - currentRotationKey->m_time) / (float)(nextRotationKey->m_time - currentRotationKey->m_time);
		assert(normalizedTime > -0.00001f && normalizedTime < 1.000001f);
		return Slerp(normalizedTime, currentRotationKey, nextRotationKey); //TODO put back commented out to rule out interpolation issues
	}
}

boost::shared_ptr<VectorKey> Animator::InterpolateScale(
	int sample,
	boost::shared_ptr<animation::Track> scaleTrack
	)
{
	const boost::shared_ptr<VectorKey> currentScaleKey = boost::static_pointer_cast<VectorKey>(scaleTrack->GetKey(sample));
	if(currentScaleKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	{
		return currentScaleKey;
	}
	else
	{
		const boost::shared_ptr<VectorKey> nextScaleKey = boost::static_pointer_cast<VectorKey>(scaleTrack->GetKey(sample + 1));
		// Find the current time value as a 0 - 1 proporion between the two keys
		float normalizedTime = (m_localCurrentTime - currentScaleKey->m_time) / (float)(nextScaleKey->m_time - currentScaleKey->m_time);
		assert(normalizedTime > -0.00001f && normalizedTime < 1.000001f);
		return Lerp(normalizedTime, currentScaleKey, nextScaleKey);
	}
}

boost::shared_ptr<VectorKey> Animator::Lerp(
	const float normalizedTime, // time value between 0 and 1 for interpolating betwen the keys
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey
	)
{
	// Next (LERP) interpolate keyA + (keyB - keyA) * t
	FbxVector4 result = key->m_vector + (nextKey->m_vector - key->m_vector) * normalizedTime;

	//wxLogDebug("position result: %f, %f, %f\n", result[0], result[1], result[2]);
	//wxLogDebug("next key: %f, %f, %f\n", nextKey->m_vector[0], nextKey->m_vector[1], nextKey->m_vector[2]);
	//wxLogDebug("key: %f, %f, %f\n", key->m_vector[0], key->m_vector[1], key->m_vector[2]);
	//wxLogDebug("prop time: %f\n", normalizedTime);

	return boost::shared_ptr<VectorKey>(new VectorKey(result, 0));
}

boost::shared_ptr<QuaternionKey> Animator::Lerp(
	const float normalizedTime, // time value between 0 and 1 for interpolating betwen the keys
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	)
{
	//assert((propertionalTime >= 0) && (propertionalTime <= 1));
	// Next (LERP) interpolate keyA + (keyB - keyA) * t
	FbxQuaternion result = key->m_quaternion + (nextKey->m_quaternion - key->m_quaternion) * normalizedTime;

	return boost::shared_ptr<QuaternionKey>(new QuaternionKey(result, 0));
}

// Interpolate a quaternion rotation using SLERP
// TODO optimise ie http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
boost::shared_ptr<QuaternionKey> Animator::Slerp(
	const float normalizedTime, // time value between 0 and 1 for interpolating betwen the keys
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	)
{
	FbxQuaternion nextQuaternion = nextKey->m_quaternion; //Copy this incase we need to negate it

	//assert((propertionalTime >= 0) && (propertionalTime <= 1));
	// First calcualte the 4D dot product which gives cos theta
	float cosTheta = key->m_quaternion[0] * nextQuaternion[0]
					+ key->m_quaternion[1] * nextQuaternion[1]
					+ key->m_quaternion[2] * nextQuaternion[2]
					+ key->m_quaternion[3] * nextQuaternion[3];

	if(cosTheta == 1)
	{
		return key; //No change
	}

	if(cosTheta < 0) // q1 and q2 are more than 90 degrees apart so invert one of them to reduce spinning
	{
		cosTheta *= -1;
		nextQuaternion = -nextKey->m_quaternion;
	}

	if(cosTheta < 0.99999f)
	{
	float theta = glm::acos(cosTheta);

	float firstKeyWeight = glm::sin((1 - normalizedTime) * theta) / glm::sin(theta);

	float nextKeyWeight = glm::sin(normalizedTime * theta) / glm::sin(theta);

	FbxQuaternion result = (key->m_quaternion * firstKeyWeight) + (nextQuaternion * nextKeyWeight);

	//wxLogDebug("rotation result: %f, %f, %f\n", x, y, z);
	//wxLogDebug("next key: %f, %f, %f, %f\n", nextKey->m_x, nextKey->m_y, nextKey->m_z, key->m_w);
	//wxLogDebug("key: %f, %f, %f, %f\n", key->m_x, key->m_y, key->m_z, key->m_w);
	//wxLogDebug("prop time: %f\n", propertionalTime);

		//wxLogDebug("result rotx: %f\n", result[0]);
		//wxLogDebug("result roty: %f\n", result[1]);
		//wxLogDebug("result rotz: %f\n", result[2]);

		return boost::shared_ptr<QuaternionKey>(new QuaternionKey(result, 0));
	}

	return Lerp(normalizedTime, key, nextKey);
}

}
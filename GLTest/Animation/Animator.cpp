#include "Animator.h"
#include <glm/gtx/quaternion.hpp> 

#include "../ImportMesh/Node.h"
#include "../ImportMesh/BoneNode.h"
#include <glm/gtc/matrix_transform.hpp>
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
	m_localCurrentTime = (globalTime - m_globalStartTime) * (1/m_animationInfo->GetFPS());

	ClampTime();

	int sample = m_animationInfo->ConvertMillisecondsToFrame(m_localCurrentTime);
	wxLogDebug("sample: %ld\n", sample);
	assert(sample <= m_animationInfo->GetNumFrameSamples()); // Number of frames = number of frame samples - 1

	const glm::mat4x4 parentGlobalScaleMatrix(1.0f);// These will be identity for the root node
	const glm::mat4x4 parentGlobalRotationMatrix(1.0f);

	PrepareBoneHierarcy(sample, node, parentGlobalScaleMatrix, parentGlobalRotationMatrix);
}

void Animator::PrepareBoneHierarcy(
	int sample,
	mesh::Node *node,
	const glm::mat4x4 &parentGlobalScaleMatrix,
	const glm::mat4x4 &parentGlobalRotationMatrix
	)
{
	for(node; node != NULL; node = node->m_next)
	{
		if(node->GetType() == mesh::BoneType)
		{
			mesh::BoneNode* boneNode = static_cast<mesh::BoneNode*>(node);

			// Create local matrix in the order scale, rotate, translate to be stored in the global transform data (local transform data is only temporarily store
			//boneNode->GetLocalKeyTransform(frame, localPosition, localRotationKey, localScaleKey);
			boost::shared_ptr<VectorKey> localPositionKey = InterpolatePosition(sample, boneNode->GetPositionTrack());
			boost::shared_ptr<QuaternionKey> localRotationKey = InterpolateRotation(sample, boneNode->GetRotationTrack());
			boost::shared_ptr<VectorKey> localScaleKey = InterpolateScale(sample, boneNode->GetScaleTrack());
			//wxLogDebug("name: %s\n", boneNode->GetName().c_str());

			//wxLogDebug("local posx: %f\n", localPositionKey->m_x);
			//wxLogDebug("local posy: %f\n", localPositionKey->m_y);
			//wxLogDebug("local posz: %f\n", localPositionKey->m_z);

			//wxLogDebug("local rotx: %f\n", localRotationKey->m_x);
			//wxLogDebug("local roty: %f\n", localRotationKey->m_y);
			//wxLogDebug("local rotz: %f\n", localRotationKey->m_z);

			glm::mat4x4 localScaleMatrix(glm::scale(glm::mat4(1.0), glm::vec3(localScaleKey->m_x, localScaleKey->m_y, localScaleKey->m_z)));
			glm::mat4x4 localRotationMatrix(glm::mat4_cast(glm::quat(localRotationKey->m_w, localRotationKey->m_x, localRotationKey->m_y, localRotationKey->m_z))); // The constructor takes w, x, y, z not x, y, z, w

			// Get the transform which needs setting
			glm::mat4x4& globalTransform = boneNode->GetGlobalTransform();

			if(boneNode->m_parent)
			{			
				const glm::mat4x4& parentTransform = boneNode->m_parent->GetGlobalTransform();

				glm::mat4x4 globalRotationAndScale;

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
				glm::vec4 globalPosition = parentTransform * glm::vec4(localPositionKey->m_x, localPositionKey->m_y, localPositionKey->m_z, 1.0f);
				//wxLogDebug("global posx without local rotation: %f\n", globalPosition.x);
				//wxLogDebug("global posy without local rotation: %f\n", globalPosition.y);
				//wxLogDebug("global posz without local rotation: %f\n", globalPosition.z);

				glm::mat4x4 globalPositionMatrix(glm::translate(glm::mat4(1.0), glm::vec3(globalPosition)));
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
					glm::mat4x4 globalScaleMatrix = parentGlobalScaleMatrix * localScaleMatrix;

					// Record the global scale and rotation matrices for use by the child
					glm::mat4x4 globalRotationMatrix = parentGlobalRotationMatrix * localRotationMatrix;

					PrepareBoneHierarcy(sample, node->m_firstChild, globalScaleMatrix, globalRotationMatrix); // Passing the parent scale and rotation avoids extracting them from the global transform which is non trivial
				}

			}
			else // No parent, so the global transform must be the same as the local transform
			{
				glm::mat4x4 localTranslationMatrix(glm::translate(glm::mat4(1.0), glm::vec3(localPositionKey->m_x, localPositionKey->m_y, localPositionKey->m_z)));
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

		return Lerp(m_localCurrentTime, currentPositionKey, nextPositionKey);
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

		return currentRotationKey;//Slerp(m_localCurrentTime, currentRotationKey, nextRotationKey); //TODO put back commented out to rule out interpolation issues
	}
}

boost::shared_ptr<VectorKey> Animator::InterpolateScale(
	int sample,
	boost::shared_ptr<animation::Track> scaleTrack
	)
{
	const boost::shared_ptr<VectorKey> currentScaleKey = boost::static_pointer_cast<VectorKey>(scaleTrack->GetKey(sample));
	//if(currentScaleKey->m_time == m_localCurrentTime) // First check if the time is exactly on the key
	//{
		return currentScaleKey;
	//}
	//else
	//{
	//	const boost::shared_ptr<VectorKey> nextScaleKey = boost::static_pointer_cast<VectorKey>(scaleTrack->GetKey(sample + 1));
	//	return Lerp(m_localCurrentTime, currentScaleKey, nextScaleKey);
	//}
}

boost::shared_ptr<VectorKey> Animator::Lerp(
	const long time, 
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey
	)
{
	// Most likely it isnt so find the 2 keys on either side of the time and find the proportional (0.0 - 1.0) time between the two
	float propertionalTime = (time - key->m_time) / (float)(nextKey->m_time - key->m_time);
	//assert((propertionalTime >= 0) && (propertionalTime <= 1));
	//wxLogDebug("time: %ld\n", time);
	//wxLogDebug("keytime: %ld\n", key->m_time);
	//wxLogDebug("next key time: %ld\n", nextKey->m_time);

	// Next (LERP) interpolate keyA + (keyB - keyA) * t
	float x = key->m_x + (nextKey->m_x - key->m_x) * propertionalTime;
	float y = key->m_y + (nextKey->m_y - key->m_y) * propertionalTime;
	float z = key->m_z + (nextKey->m_z - key->m_z) * propertionalTime;

	//wxLogDebug("position result: %f, %f, %f\n", x, y, z);
	//wxLogDebug("next key: %f, %f, %f\n", nextKey->m_x, nextKey->m_y, nextKey->m_z);
	//wxLogDebug("key: %f, %f, %f\n", key->m_x, key->m_y, key->m_z);
	//wxLogDebug("prop time: %f\n", propertionalTime);

	return boost::shared_ptr<VectorKey>(new VectorKey(x, y, z, 0));
}

boost::shared_ptr<QuaternionKey> Animator::Lerp(
	const long time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	)
{
	// Most likely it isnt so find the 2 keys on either side of the time and find the proportional (0.0 - 1.0) time between the two
	float propertionalTime = time - key->m_time / (float)nextKey->m_time - key->m_time;
	//assert((propertionalTime >= 0) && (propertionalTime <= 1));
	// Next (LERP) interpolate keyA + (keyB - keyA) * t
	float x = key->m_x + (nextKey->m_x - key->m_x) * propertionalTime;
	float y = key->m_y + (nextKey->m_y - key->m_y) * propertionalTime;
	float z = key->m_z + (nextKey->m_z - key->m_z) * propertionalTime;
	float w = key->m_w + (nextKey->m_w - key->m_w) * propertionalTime;

	return boost::shared_ptr<QuaternionKey>(new QuaternionKey(x, y, z, w, 0));
}

// Interpolate a quaternion rotation using SLERP
// TODO optimise
boost::shared_ptr<QuaternionKey> Animator::Slerp(
	const long time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey
	)
{
	float propertionalTime = time - key->m_time / (float)nextKey->m_time - key->m_time;
	//assert((propertionalTime >= 0) && (propertionalTime <= 1));
	// First calcualte the 4D dot product which gives cos theta
	float cosTheta = key->m_x * nextKey->m_x
					+ key->m_y * nextKey->m_y
					+ key->m_z * nextKey->m_z
					+ key->m_w * nextKey->m_w;

	if(cosTheta == 1)
	{
		return key; //No change
	}

	if(cosTheta < 0)
	{
		cosTheta *= -1;
		nextKey->m_x =  -nextKey->m_x;
		nextKey->m_y =  -nextKey->m_y;
		nextKey->m_z =  -nextKey->m_z;
		nextKey->m_w =  -nextKey->m_w;
	}

	float theta = glm::acos(cosTheta);

	float firstKeyWeight = glm::sin((1 - propertionalTime) * theta) / glm::sin(theta);

	float nextKeyWeight = glm::sin(propertionalTime * theta) / glm::sin(theta);

	float x = (key->m_x * firstKeyWeight) + (nextKey->m_x * nextKeyWeight);
	float y = (key->m_y * firstKeyWeight) + (nextKey->m_y * nextKeyWeight);
	float z = (key->m_z * firstKeyWeight) + (nextKey->m_z * nextKeyWeight);
	float w = (key->m_w * firstKeyWeight) + (nextKey->m_w * nextKeyWeight);

	//wxLogDebug("rotation result: %f, %f, %f\n", x, y, z);
	//wxLogDebug("next key: %f, %f, %f, %f\n", nextKey->m_x, nextKey->m_y, nextKey->m_z, key->m_w);
	//wxLogDebug("key: %f, %f, %f, %f\n", key->m_x, key->m_y, key->m_z, key->m_w);
	//wxLogDebug("prop time: %f\n", propertionalTime);

	return boost::shared_ptr<QuaternionKey>(new QuaternionKey(x, y, z, w, 0));
}

}
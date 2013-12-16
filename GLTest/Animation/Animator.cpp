#include "Animator.h"
#include "../ImportMesh/Node.h"
#include "../ImportMesh/BoneNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Animation/QuaternionKey.h"
#include "../Animation/VectorKey.h"

namespace animation
{

Animator::Animator()
{


}

// Passing the scale and rotation matrices to avoid have to extract the rotation and scale from the parent global matrix which is non trivial (necessary to switch between scale inheritance types).
void Animator::PrepareBoneHierarcy(
	mesh::Node *node,
	const int frame
	)
{
	const glm::mat4x4 parentGlobalScaleMatrix(1.0f);// These will be identity for the root node
	const glm::mat4x4 parentGlobalRotationMatrix(1.0f);

	PrepareBoneHierarcy(node, frame, parentGlobalScaleMatrix, parentGlobalRotationMatrix);
}

void Animator::PrepareBoneHierarcy(
	mesh::Node *node,
	const int frame,
	const glm::mat4x4 &parentGlobalScaleMatrix,
	const glm::mat4x4 &parentGlobalRotationMatrix
	)
{
	for(node; node != NULL; node = node->m_next)
	{
		if(node->GetType() == mesh::BoneType)
		{
			mesh::BoneNode* boneNode = static_cast<mesh::BoneNode*>(node);

			// Create loca matrix in the order scale, rotate, translate
			boost::shared_ptr<VectorKey> localPosition;
			boost::shared_ptr<QuaternionKey> localRotationQuat;
			boost::shared_ptr<VectorKey> localScale;

			//boneNode->GetLocalKeyTransform(frame, localPosition, localRotationQuat, localScale);
			InterpolateKeys(frame, localPosition, localRotationQuat, localScale, boneNode->GetPositionTrack(), boneNode->GetRotationTrack(), boneNode->GetScaleTrack());

			glm::mat4x4 localScaleMatrix(glm::scale(glm::mat4(1.0), localScale->m_value));
			glm::mat4x4 localRotationMatrix(glm::mat4_cast(localRotationQuat->m_value));

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
				glm::vec4 globalPosition = parentTransform * glm::vec4(localPosition->m_value, 1.0f);
				glm::mat4x4 globalPositionMatrix(glm::translate(glm::mat4(1.0), glm::vec3(globalPosition)));
				globalTransform = globalPositionMatrix * globalRotationAndScale;

				if(node->m_firstChild)
				{
					// Record the global scale and rotation matrices for use by the child
					glm::mat4x4 globalScaleMatrix = parentGlobalScaleMatrix * localScaleMatrix;

					// Record the global scale and rotation matrices for use by the child
					glm::mat4x4 globalRotationMatrix = parentGlobalRotationMatrix * localRotationMatrix;

					PrepareBoneHierarcy(node->m_firstChild, frame, globalScaleMatrix, globalRotationMatrix); // Passing the parent scale and rotation avoids extracting them from the global transform which is non trivial
				}

			}
			else // No parent, so the global transform must be the same as the local transform
			{
				glm::mat4x4 localTranslationMatrix(glm::translate(glm::mat4(1.0), glm::vec3(localPosition->m_value)));
				globalTransform = localTranslationMatrix * localRotationMatrix * localScaleMatrix;

				if(node->m_firstChild)
				{
					PrepareBoneHierarcy(node->m_firstChild, frame, localScaleMatrix, localRotationMatrix); // Passing the parent scale and rotation avoids extracting them from the global transform which is non trivial
				}

			}
		}
	}
}

void Animator::BinarySearchKeys(
	animation::TrackPtr track,
	const long timeToFind,
	boost::shared_ptr<Key> previousKey,
	boost::shared_ptr<Key> nextKey
	)
{


}

void Animator::InterpolateKeys(
	const long time,
	boost::shared_ptr<VectorKey> localPosition, 
	boost::shared_ptr<QuaternionKey> localRotationQuat, 
	boost::shared_ptr<VectorKey> localScale,
	animation::TrackPtr positionTrack,
	animation::TrackPtr rotationTrack,
	animation::TrackPtr scaleTrack
	)
{
	const boost::shared_ptr<VectorKey> previousPositionKey;
	const boost::shared_ptr<VectorKey> nextPositionKey;

	const boost::shared_ptr<QuaternionKey> previousRotationKey;
	const boost::shared_ptr<QuaternionKey> nextRotationKey;

	const boost::shared_ptr<VectorKey> previousScaleKey;
	const boost::shared_ptr<VectorKey> nextScaleKey;

	// If the key time is not exact then find the keys immediately before the time
	BinarySearchKeys(positionTrack, time, previousPositionKey, nextPositionKey);
	BinarySearchKeys(rotationTrack, time, previousRotationKey, nextRotationKey);
	BinarySearchKeys(scaleTrack, time, previousScaleKey, nextScaleKey);

	InterpolateVector(time, previousPositionKey, nextPositionKey, localPosition);
	InterpolateQuaternion(time, previousRotationKey, nextRotationKey, localRotationQuat);
	InterpolateVector(time, previousScaleKey, nextScaleKey, localScale);
}

void Animator::InterpolateVector(
	const long time, 
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey,
	boost::shared_ptr<VectorKey>result
	)
{
	// Check whether the time is exactly on a frame

	// Most likely it isnt so find the 2 keys on either side of the time and find the proportional (0.0 - 1.0) time between the two
	float propertionalTime = time - key->m_time / (float)nextKey->m_time - key->m_time;

	// Next (LERP) interpolate keyA + (keyB - keyA) * t
	result->m_value.x = key->m_value.x + (nextKey->m_value.x - key->m_value.x) * propertionalTime;
	result->m_value.y = key->m_value.y + (nextKey->m_value.y - key->m_value.y) * propertionalTime;
	result->m_value.z = key->m_value.z + (nextKey->m_value.z - key->m_value.z) * propertionalTime;
}

// Interpolate a quaternion rotation using SLERP
void Animator::InterpolateQuaternion(
	const long time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey,
	boost::shared_ptr<QuaternionKey> result
	)
{
	// First find the closest frame to this time

	// Next interpolate

}





}
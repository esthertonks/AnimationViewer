#include "Animator.h"
#include "../ImportMesh/Node.h"
#include "../ImportMesh/BoneNode.h"
#include <glm/gtc/matrix_transform.hpp>

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
			glm::vec3 localPosition;
			glm::quat localRotationQuat;
			glm::vec3 localScale;

			boneNode->GetLocalKeyTransform(frame, localPosition, localRotationQuat, localScale);

			glm::mat4x4 localScaleMatrix(glm::scale(glm::mat4(1.0), localScale));
			glm::mat4x4 localRotationMatrix(glm::mat4_cast(localRotationQuat));

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
				glm::vec4 globalPosition = parentTransform * glm::vec4(localPosition, 1.0f);
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
				glm::mat4x4 localTranslationMatrix(glm::translate(glm::mat4(1.0), glm::vec3(localPosition)));
				globalTransform = localTranslationMatrix * localRotationMatrix * localScaleMatrix;

				if(node->m_firstChild)
				{
					PrepareBoneHierarcy(node->m_firstChild, frame, localScaleMatrix, localRotationMatrix); // Passing the parent scale and rotation avoids extracting them from the global transform which is non trivial
				}

			}
		}
	}
}







}
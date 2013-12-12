#include "Animator.h"
#include "../ImportMesh/Node.h"
#include "../ImportMesh/BoneNode.h"

namespace animation
{

Animator::Animator()
{


}

void Animator::PrepareBoneHierarcy(
	mesh::Node *rootBoneNode,
	int frame
	)
{
	if(rootBoneNode->GetType() == mesh::BoneType)
	{
		
		mesh::BoneNode* boneNode = static_cast<mesh::BoneNode*>(rootBoneNode);
		glm::mat4x4 localTransform;
		boneNode->GetLocalKeyTransform(frame, localTransform);

		if(boneNode->m_parent)
		{	
			const glm::mat4x4& parentTransform = boneNode->m_parent->GetGlobalTransform();
			glm::mat4x4& globalTransform = localTransform * parentTransform;
			boneNode->SetGlobalTransform(globalTransform);
		}
		else
		{
			boneNode->SetGlobalTransform(localTransform);
		}
	}

	for(mesh::Node *childNode = rootBoneNode->m_firstChild; childNode != rootBoneNode->m_endChild; childNode++)
	{
		PrepareBoneHierarcy(childNode, frame);
	}
}







}
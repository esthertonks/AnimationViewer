#pragma once

#include "Node.h"
#include "../Animation/AnimationFwdDecl.h"

namespace animation
{
	class AnimationTrack;
}

namespace import
{
	class FBXImport;
}

namespace mesh
{

class BoneNode : public Node
{
public:

BoneNode();
~BoneNode();

virtual NodeType GetType()
{
	return BoneType;
}

void AllocateAnimationTrack(
	int numFrames
	);

void SetLocalKeyTransform(
	int key,
	const glm::mat4x4 &localTransform
	);

void AddLocalKeyTransform(
	const glm::vec3 &position,
	const glm::quat &rotation,
	const glm::vec3 &scale
	);

void GetLocalKeyTransform(
	int key,
	glm::vec3& position,
	glm::quat& rotationQuat,
	glm::vec3& scale
	);

void SetInheritScale(
	bool inheritScale // When true this node inherits scale from it's parent (FbxTransform::eInheritRSrs). When false scale is not inherited (eInheritRrs)
)
{
	m_inheritScale = inheritScale;
}

bool InheritsScale()
{
	return m_inheritScale;
}

private:
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	bool m_inheritScale; // When true this node inherits scale from it's parent (FbxTransform::eInheritRSrs). When false scale is not inherited (eInheritRrs)
	glm::mat4x4 m_localTransform;		//Transform from this node to the parent node //TEMP to debug - we will get this from the keys
	animation::AnimationTrackPtr m_animationTrack; // Local animation transforms per keyframe
};

}
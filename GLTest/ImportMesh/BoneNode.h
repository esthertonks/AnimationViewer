#pragma once

#include "Node.h"
#include "../Animation/AnimationFwdDecl.h"

namespace animation
{
	class AnimationTrack;
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

glm::mat4x4 &GetLocalKeyTransform(
	int key
	);


private:
	bool m_inheritScale;
	//glm::mat4x4 m_localTransform;		//Transform from this node to the parent node
	animation::AnimationTrackPtr m_animationTrack; // Local animation transforms per keyframe
};

}
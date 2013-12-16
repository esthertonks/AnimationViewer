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

void AllocateAnimationTracks(
	int numFrames
	);

const animation::TrackPtr GetPositionTrack()
{
	return m_positionTrack;
}

const animation::TrackPtr GetScaleTrack()
{
	return m_scaleTrack;
}

const animation::TrackPtr GetRotationTrack()
{
	return m_rotationTrack;
}

void SetLocalKeyTransform(
	int key,
	const glm::mat4x4 &localTransform
	);

void AddLocalKeyTransform(
	const long time,
	const boost::shared_ptr<animation::VectorKey> position,
	const boost::shared_ptr<animation::QuaternionKey> rotation,
	const boost::shared_ptr<animation::VectorKey> scale
	);

//void GetLocalKeyTransform(
//	int key,
//	glm::vec3& position,
//	glm::quat& rotationQuat,
//	glm::vec3& scale
//	);

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
	animation::TrackPtr m_rotationTrack;
	animation::TrackPtr m_scaleTrack;
	animation::TrackPtr m_positionTrack;
};

}
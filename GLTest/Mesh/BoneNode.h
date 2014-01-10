#pragma once

#include "Node.h"
#include "../Animation/Track.h"
#include "../Animation/VectorKey.h"
#include "../Animation/QuaternionKey.h"
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

const boost::shared_ptr<animation::Track> GetPositionTrack()
{
	return m_positionTrack;
}

const boost::shared_ptr<animation::Track> GetScaleTrack()
{
	return m_scaleTrack;
}

const boost::shared_ptr<animation::Track> GetRotationTrack()
{
	return m_rotationTrack;
}

//void SetLocalKeyTransform(
	//int key,
	//const FbxAMatrix localTransform
	//);

void AddLocalKeyTransform(
	boost::shared_ptr<animation::VectorKey> position,
	boost::shared_ptr<animation::QuaternionKey> rotation,
	boost::shared_ptr<animation::VectorKey> scale
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
	FbxAMatrix m_localTransform;		//Transform from this node to the parent node //TEMP to debug - we will get this from the keys
	boost::shared_ptr<animation::Track> m_rotationTrack;
	boost::shared_ptr<animation::Track> m_scaleTrack;
	boost::shared_ptr<animation::Track> m_positionTrack;
};

}
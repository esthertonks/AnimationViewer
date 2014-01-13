#pragma once

#include "Node.h"
#include "boost/shared_ptr.hpp"

namespace animation
{
	class VectorTrack;
	class QuaternionTrack;
	class VectorKey;
	class QuaternionKey;
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

const boost::shared_ptr<animation::VectorTrack> GetPositionTrack()
{
	return m_positionTrack;
}

const boost::shared_ptr<animation::VectorTrack> GetScaleTrack()
{
	return m_scaleTrack;
}

const boost::shared_ptr<animation::QuaternionTrack> GetRotationTrack()
{
	return m_rotationTrack;
}

//void SetLocalKeyTransform(
	//int key,
	//const FbxAMatrix localTransform
	//);

void AddPositionKey(
	const	boost::shared_ptr<animation::VectorKey> position
	);

void AddRotationKey(
	const boost::shared_ptr<animation::QuaternionKey> rotation
	);

void AddScaleKey(
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
	//FbxAMatrix m_localTransform;		//Transform from this node to the parent node //TEMP to debug - we will get this from the keys
	boost::shared_ptr<animation::QuaternionTrack> m_rotationTrack;
	boost::shared_ptr<animation::VectorTrack> m_scaleTrack;
	boost::shared_ptr<animation::VectorTrack> m_positionTrack;
};

}
#pragma once

#include "Node.h"
#include "boost/shared_ptr.hpp"
#include "../Animation/VectorKey.h"
#include "../Animation/QuaternionKey.h"

namespace animation
{
	class VectorTrack;
	class QuaternionTrack;
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

BoneNode(
	unsigned int id
	);

~BoneNode();

virtual NodeType GetType()
{
	return BoneType;
}

unsigned int GetId()
{
	return m_id;
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
	const animation::VectorKey &position
	);

void AddRotationKey(
	const animation::QuaternionKey &rotation
	);

void AddScaleKey(
	const animation::VectorKey &scale
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

void SetInverseReferenceMatrix(
	FbxAMatrix &inverseReferenceMatrix
)
{
	m_inverseReferenceMatrix = inverseReferenceMatrix;
}

const FbxAMatrix &GetInverseReferenceMatrix()
{
	return m_inverseReferenceMatrix;
}

private:
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	bool m_inheritScale; // When true this node inherits scale from it's parent (FbxTransform::eInheritRSrs). When false scale is not inherited (eInheritRrs)

	boost::shared_ptr<animation::QuaternionTrack> m_rotationTrack;
	boost::shared_ptr<animation::VectorTrack> m_scaleTrack;
	boost::shared_ptr<animation::VectorTrack> m_positionTrack;

	FbxAMatrix m_inverseReferenceMatrix;	// The inverse reference matrix for this bone - calculated on import

	unsigned int m_id;
};

}
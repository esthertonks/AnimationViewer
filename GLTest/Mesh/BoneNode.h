#pragma once

//#include "Node.h"
#include <fbxsdk.h>
#include "../Container/LinkedTree.h"
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

class BoneNode : public container::LinkedTreeItem<BoneNode>
{
public:

BoneNode();

~BoneNode();

//virtual NodeType GetType()
//{
//	return BoneType;
//}

void SetName(
	const std::string& name
	)
{
	m_name = name;
};

std::string&	GetName()
{		
	return m_name;
}

unsigned int GetId()
{
	return m_id;
}

static void ResetIdCounter()
{
	 m_counter = 0;
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

const FbxAMatrix &GetGlobalTransform() const
{
	return m_globalTransform;
};

FbxAMatrix &GetGlobalTransform()
{
	return m_globalTransform;
};

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
	static unsigned int m_counter;

	std::string m_name;

	FbxAMatrix m_globalTransform;		// Transform from this node to the global model transform. This is updated each tick during animation
};

}
#include "BoneNode.h"
#include "../Animation/QuaternionTrack.h"
#include "../Animation/VectorTrack.h"
#include "../Animation/VectorKey.h"
#include "../Animation/QuaternionKey.h"

namespace mesh
{

unsigned int BoneNode::m_counter = 0;

BoneNode::BoneNode()
	: m_inheritScale(false),
	m_rotationTrack(NULL),
	m_positionTrack(NULL),
	m_scaleTrack(NULL),
	m_id(m_counter++) //TODO could do with something less error prone than an int id - name crc maybe?
{
};

// Allocate memory for an animation track. Currently only one animation track is supported.
void BoneNode::AllocateAnimationTracks(
	int numFrames
	)
{
	m_rotationTrack = boost::shared_ptr<animation::QuaternionTrack>(new animation::QuaternionTrack(numFrames));
	m_scaleTrack = boost::shared_ptr<animation::VectorTrack>(new animation::VectorTrack(numFrames));
	m_positionTrack = boost::shared_ptr<animation::VectorTrack>(new animation::VectorTrack(numFrames));
}

void BoneNode::AddPositionKey(
	const animation::VectorKey &position
	)
{
	m_positionTrack->AddKey(position);
};

void BoneNode::AddRotationKey(
	const animation::QuaternionKey &rotation
	)
{
	m_rotationTrack->AddKey(rotation);
};

void BoneNode::AddScaleKey(
	const animation::VectorKey &scale
	)
{
	m_scaleTrack->AddKey(scale);
};

BoneNode::~BoneNode()
{
};


}
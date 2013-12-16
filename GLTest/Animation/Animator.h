#pragma once

#include <string>
#include <glm/glm.hpp>
#include "AnimationFwdDecl.h"
/*
	Information about an animation.
*/

namespace mesh
{
	class Node;
}

namespace animation
{

class Animator
{
public :

	Animator();
	~Animator(){};

void PrepareBoneHierarcy(
	mesh::Node *node,
	const int frame
	);

void PrepareBoneHierarcy(
	mesh::Node* node,
	const int frame,
	const glm::mat4x4 &parentGlobalScaleMatrix,
	const glm::mat4x4 &parentGlobalRotationMatrix
	);

private:

void BinarySearchKeys(
	animation::TrackPtr track,
	const long timeToFind,
	boost::shared_ptr<Key> previousKey,
	boost::shared_ptr<Key> nextKey
	);

void InterpolateKeys(
	const long time,
	boost::shared_ptr<VectorKey> localPosition, 
	boost::shared_ptr<QuaternionKey> localRotationQuat, 
	boost::shared_ptr<VectorKey> localScale,
	animation::TrackPtr positionTrack,
	animation::TrackPtr rotationTrack,
	animation::TrackPtr scaleTrack
	);

void InterpolateVector(
	const long time, 
	const boost::shared_ptr<VectorKey> key,
	const boost::shared_ptr<VectorKey> nextKey,
	boost::shared_ptr<VectorKey> result
	);

void InterpolateQuaternion(
	const long time, 
	const boost::shared_ptr<QuaternionKey> key,
	const boost::shared_ptr<QuaternionKey> nextKey,
	boost::shared_ptr<QuaternionKey> result
	);
};

}
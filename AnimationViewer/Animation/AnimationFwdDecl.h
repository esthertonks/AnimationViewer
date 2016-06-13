#pragma once

#include <fbxsdk.h>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace animation
{
	class AnimationController;
	class VectorKey;
	class QuaternionKey;

	typedef std::vector<animation::QuaternionKey> QuaternionKeyArray;
	typedef std::vector<animation::VectorKey> VectorKeyArray;

	typedef boost::shared_ptr<animation::AnimationController> AnimationControllerPtr;
}
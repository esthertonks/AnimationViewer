#pragma once

#include <boost\shared_ptr.hpp>
#include <fbxsdk.h>

namespace mesh
{
	class Mesh;
	class AnimationInfo;
	class Node;

	typedef boost::shared_ptr<Mesh> MeshPtr;
	typedef boost::shared_ptr<AnimationInfo> AnimationInfoPtr;
}
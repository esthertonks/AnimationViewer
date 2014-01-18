#pragma once

#include <boost\shared_ptr.hpp>
#include <fbxsdk.h>

namespace mesh
{
	class Mesh;
	class AnimationInfo;

	typedef boost::shared_ptr<Mesh> MeshPtr;
	typedef boost::shared_ptr<AnimationInfo> AnimationInfoPtr;

	#define MAX_BONES 128;
	#define MAX_INFLUENCES 4
}
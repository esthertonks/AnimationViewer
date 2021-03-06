#pragma once

#include <boost\shared_ptr.hpp>
#include <boost\shared_array.hpp>
#include <fbxsdk.h>

namespace mesh
{
	class Mesh;
	class AnimationInfo;

	class Vertex;
	class Triangle;

	class BoneNode;
	class MeshNode;

	typedef boost::shared_array<Vertex> MeshVertexArrayPtr;
	typedef boost::shared_array<Triangle> MeshTriangleArrayPtr;

	typedef boost::shared_ptr<Mesh> MeshPtr;
	typedef boost::shared_ptr<AnimationInfo> AnimationInfoPtr;

	#define MAX_BONES 128;
	#define MAX_INFLUENCES 4 // Note this number is currently hardcoded separately in the skinning shaders and will need changing if this changes
}
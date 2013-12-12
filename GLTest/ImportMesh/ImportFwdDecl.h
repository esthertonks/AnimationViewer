#pragma once

#include <boost\shared_ptr.hpp>
#include <fbxsdk.h>

namespace mesh
{
	class Mesh;

	typedef boost::shared_ptr<Mesh> MeshPtr;
}
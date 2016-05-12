#pragma once

#include "../Batch/BatchFwdDecl.h"

namespace mesh {
	class BoneNode;
}

namespace render{

class BoneListCreator
{
public:
	BoneListCreator();
	~BoneListCreator();

	void AddPositionToVertexList(
		mesh::BoneNode *boneHierarchyRoot,
		ColourVertexArrayPtr &vertexArray
	);
};

}


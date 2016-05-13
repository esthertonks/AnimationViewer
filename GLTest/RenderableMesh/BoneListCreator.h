#pragma once

#include "../Batch/BatchFwdDecl.h"

namespace mesh {
	class BoneNode;
}

namespace render{

class VertexListCreator
{
public:
	VertexListCreator();
	~VertexListCreator();

	void CreateVertexListFromBonePositions(
		mesh::BoneNode *boneHierarchyRoot,
		ColourVertexArrayPtr &vertexArray
	);
};

}


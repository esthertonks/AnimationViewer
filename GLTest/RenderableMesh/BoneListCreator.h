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

	int GetNumVertsInList();

	ColourVertexArrayPtr &GetVertexList();

	void CreateVertexListFromBonePositions(
		mesh::BoneNode *boneHierarchyRoot
	);

private:

	void CreateVertexListFromBonePositionsInternal(
		mesh::BoneNode *boneHierarchyRoot
	);

	ColourVertexArrayPtr m_vertexArray;
	int m_numVerts;
};

}


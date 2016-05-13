#pragma once

#include "../Batch/BatchFwdDecl.h"

namespace mesh {
	class BoneNode;
}

namespace render{

class BoneVertexListCreator
{
public:
	BoneVertexListCreator();
	~BoneVertexListCreator();

	int GetNumVertsInList();

	ColourVertexArray &GetVertexList();

	void CreateVertexListFromBonePositions(
		mesh::BoneNode *boneHierarchyRoot
	);

private:

	void CreateVertexListFromBonePositionsInternal(
		mesh::BoneNode *boneHierarchyRoot
	);

	ColourVertexArray m_vertexArray;
	int m_numVerts;
};

}


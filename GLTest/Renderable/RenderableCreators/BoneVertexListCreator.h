//#pragma once

#include "VertexListCreatorBase.h"

namespace mesh {
	class BoneNode;
}

namespace render{

class BoneVertexListCreator : public VertexListCreatorBase
{
public:
	BoneVertexListCreator();
	~BoneVertexListCreator();

	virtual int GetNumVertsInList();

	virtual ColourVertexArray &GetVertexList();

	virtual void CreateAnimatedVertexList(
		mesh::BoneNode *boneHierarchyRoot
	);

private:

	void UpdateVertexListFromBonePositions(
		mesh::BoneNode *boneNode
	);

	ColourVertexArray m_vertexArray;
	int m_numVerts;
};

}


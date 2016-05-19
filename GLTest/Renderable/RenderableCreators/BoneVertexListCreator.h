//#pragma once

#include "RenderableVertexListCreator.h"

namespace mesh {
	class BoneNode;
}

namespace render{

class BoneVertexListCreator : public RenderableVertexListCreator
{
public:
	BoneVertexListCreator();
	~BoneVertexListCreator();

	virtual int GetNumVertsInList();

	virtual ColourVertexArray &GetVertexList();

	//virtual void Create();

	virtual void Update(
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


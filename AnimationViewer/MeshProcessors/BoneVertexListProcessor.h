//#pragma once

#include "VertexListProcessor.h"

namespace mesh {
	class BoneNode;
}

namespace render{

class BoneVertexListProcessor : public VertexListProcessor
{
public:
	BoneVertexListProcessor(
		glm::vec3 colour
	);
	~BoneVertexListProcessor();

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
	glm::vec3 m_colour;
};

}


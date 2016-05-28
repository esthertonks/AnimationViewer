#pragma once

#include "../Renderable/RenderableFwdDecl.h"

namespace mesh
{
	class BoneNode;
}

namespace render
{

class VertexListProcessor abstract
{
public:
	VertexListProcessor();

	virtual int GetNumVertsInList() = 0;
	virtual ColourVertexArray &GetVertexList() = 0;
	virtual void CreateAnimatedVertexList(
		mesh::BoneNode *boneHierarchyRoot
	) = 0;

	virtual ~VertexListProcessor() {};
};

}


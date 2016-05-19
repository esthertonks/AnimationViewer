#pragma once

#include "RenderableComponentCreator.h"
#include "../RenderableFwdDecl.h"

namespace mesh
{
	class BoneNode;
}

namespace render
{

class RenderableVertexListCreator : public RenderableComponentCreator
{
public:
	RenderableVertexListCreator();

	virtual int GetNumVertsInList() = 0;
	virtual ColourVertexArray &GetVertexList() = 0;
	virtual void Update(
		mesh::BoneNode *boneHierarchyRoot
	) = 0;

	virtual ~RenderableVertexListCreator() {};
};

}


#pragma once

#include "../Renderable/RenderableFwdDecl.h"

namespace mesh {
	class BoneNode;
}

namespace render {

class SkinningMatrixProcessor
{
public:
	SkinningMatrixProcessor();
	~SkinningMatrixProcessor();

	BoneMatrixPalette &SkinningMatrixProcessor::GetBoneMatrixPalette();

	bool CreateBoneMatrix(
		mesh::BoneNode * boneHierarchyRoot
	);

private:

	bool CreateBoneMatrixInternal(
		mesh::BoneNode * node
	);

	BoneMatrixPalette m_matrixPalette;	//TODO 4x3 matrix could be used here to save space
	unsigned int m_boneIdCheck;
};

}


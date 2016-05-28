#pragma once

#include "../RenderableFwdDecl.h"

namespace mesh {
	class BoneNode;
}

namespace render {

class SkinningMatrixCreator
{
public:
	SkinningMatrixCreator();
	~SkinningMatrixCreator();

	BoneMatrixPalette &SkinningMatrixCreator::GetBoneMatrixPalette();

	bool CreateBoneMatrix(
		mesh::BoneNode * boneHierarchyRoot
	);

private:

	bool CreateBoneMatrixInternal(
		mesh::BoneNode * node
	);

	BoneMatrixPalette m_matrixPalette;//TODO 4x3
	unsigned int m_boneIdCheck;
};

}


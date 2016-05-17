#include "SkinningMatrixCreator.h"
#include "../Utils/MathsUtils.h"
#include "../Mesh/BoneNode.h"

namespace render {

	SkinningMatrixCreator::SkinningMatrixCreator()
		: m_boneIdCheck(0)
	{
	}

	BoneMatrixPalette &SkinningMatrixCreator::GetBoneMatrixPalette() {
		return m_matrixPalette;
	}

	bool SkinningMatrixCreator::CreateBoneMatrix(
		mesh::BoneNode *boneHierarchyRoot
	)
	{
		if (!boneHierarchyRoot)
		{
			return false;
		}
		m_matrixPalette.clear();
		m_boneIdCheck = 0;

		CreateBoneMatrixInternal(boneHierarchyRoot);

		return true;
	}

	bool SkinningMatrixCreator::CreateBoneMatrixInternal(
		mesh::BoneNode *node
	)
	{
		//TODO these MUST be in the sameorder they were added in import so the id's match up
		assert(node->GetId() == m_boneIdCheck);
		glm::mat4x4 bonePaletteMatrix;
		utils::MathsUtils::ConvertFBXToGLMatrix(node->GetGlobalTransform() * node->GetInverseReferenceMatrix(), bonePaletteMatrix);

		m_matrixPalette.push_back(bonePaletteMatrix);

		m_boneIdCheck++;

		for (mesh::BoneNode* childNode = node->m_firstChild; childNode != NULL; childNode = childNode->m_next)
		{
			CreateBoneMatrixInternal(childNode);
		}

		return true;
	}

	SkinningMatrixCreator::~SkinningMatrixCreator()
	{
	}

}

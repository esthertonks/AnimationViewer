#include "SkinningMatrixProcessor.h"
#include "../Utils/MathsUtils.h"
#include "../Mesh/BoneNode.h"

namespace render {

	SkinningMatrixProcessor::SkinningMatrixProcessor()
		: m_boneIdCheck(0)
	{
	}

	BoneMatrixPalette &SkinningMatrixProcessor::GetBoneMatrixPalette() {
		return m_matrixPalette;
	}

	bool SkinningMatrixProcessor::CreateBoneMatrix(
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

	bool SkinningMatrixProcessor::CreateBoneMatrixInternal(
		mesh::BoneNode *node
	)
	{
		// These MUST be in the sameorder they were added on import so the id's match up
		assert(node->GetId() == m_boneIdCheck++);
		glm::mat4x4 bonePaletteMatrix;
		utils::MathsUtils::ConvertFBXToGLMatrix(node->GetGlobalTransform() * node->GetInverseReferenceMatrix(), bonePaletteMatrix);

		m_matrixPalette.push_back(bonePaletteMatrix);

		for (mesh::BoneNode* childNode = node->m_firstChild; childNode != NULL; childNode = childNode->m_next)
		{
			CreateBoneMatrixInternal(childNode);
		}

		return true;
	}

	SkinningMatrixProcessor::~SkinningMatrixProcessor()
	{
	}

}

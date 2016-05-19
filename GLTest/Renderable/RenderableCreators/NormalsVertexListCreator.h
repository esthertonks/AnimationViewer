#pragma once

#include "../RenderableFwdDecl.h"
#include "../../Mesh/MeshFwdDecl.h"
#include "../../Batch/BatchFwdDecl.h"

namespace mesh {
	class BoneNode;
	class MeshNode;
}

namespace render {

	class SkinningMatrixCreator;

	class NormalsVertexListCreator
	{
	public:
		NormalsVertexListCreator(
			mesh::MeshPtr mesh
		);

		~NormalsVertexListCreator();

		int GetNumVertsInList();

		ColourVertexArray &GetVertexList();

		void CreateVertexListFromNormals(
			mesh::BoneNode *boneHierarchyRoot
		);

	private:

		void CreateVertexListFromNormalsInternal(
			mesh::MeshNode* meshNode
		);

		ColourVertexArray m_normalsVertexArray;
		int m_numVerts;
		mesh::MeshPtr m_mesh;
		render::SkinningMatrixCreatorPtr m_skinningMatrixCreator;
	};

}

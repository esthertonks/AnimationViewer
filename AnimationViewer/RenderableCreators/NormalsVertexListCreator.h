#pragma once

#include "../Mesh/MeshFwdDecl.h"
#include "../Batch/BatchFwdDecl.h"
#include "VertexListCreatorBase.h"

namespace mesh {
	class BoneNode;
	class MeshNode;
}

namespace render {

	class SkinningMatrixCreator;

	class NormalsVertexListCreator : public VertexListCreatorBase
	{
	public:
		NormalsVertexListCreator(
			mesh::MeshPtr mesh,
			glm::vec3 colour,
			float normalLength
		);

		~NormalsVertexListCreator();

		virtual int GetNumVertsInList();

		virtual ColourVertexArray &GetVertexList();

		void CreateAnimatedVertexList(
			mesh::BoneNode *boneHierarchyRoot
		);

	private:

		void CreateVertexListFromNormalsInternal(
			mesh::MeshNode* meshNode
		);

		ColourVertexArray m_normalsVertexArray;
		int m_numVerts;
		glm::vec3 m_colour;
		float m_normalLength;
		mesh::MeshPtr m_mesh;
		render::SkinningMatrixCreatorPtr m_skinningMatrixCreator;
	};

}

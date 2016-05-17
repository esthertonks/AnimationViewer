#pragma once

#include "../Batch/BatchFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"

namespace mesh {
	class BoneNode;
	class MeshNode;
}

namespace render {

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

		bool CreateBoneMatrix(
			mesh::BoneNode * boneHierarchyRoot
		);

		bool CreateBoneMatrixInternal(
			mesh::BoneNode * node
		);

		void CreateVertexListFromNormalsInternal(
			mesh::MeshNode* meshNode
		);

		ColourVertexArray m_vertexArray;
		int m_numVerts;
		mesh::MeshPtr m_mesh;
		std::vector<glm::mat4x4> m_matrixPalette;//TODO 4x3
	};

}

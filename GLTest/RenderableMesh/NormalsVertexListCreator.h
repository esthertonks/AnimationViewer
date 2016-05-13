#pragma once

#include "../Batch/BatchFwdDecl.h"

namespace mesh {
	class BoneNode;
}

namespace render {

	class NormalsVertexListCreator
	{
	public:
		NormalsVertexListCreator();
		~NormalsVertexListCreator();

		int GetNumVertsInList();

		ColourVertexArray &GetVertexList();

		void CreateVertexListFromNormals(
			mesh::BoneNode *boneHierarchyRoot
		);

	private:

		void CreateVertexListFromNormalsInternal(
			mesh::BoneNode *boneHierarchyRoot
		);

		ColourVertexArray m_vertexArray;
		int m_numVerts;
	};

}

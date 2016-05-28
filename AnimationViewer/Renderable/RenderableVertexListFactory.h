#pragma once

#include "RenderableFwdDecl.h"

namespace render
{
	class RenderableVertexListFactory
	{
	public:

		static RenderableVertexListPtr CreateBoneVertexList(
			const glm::vec3 colour,
			const int pointSize
		);

		static RenderableVertexListPtr CreateNormalsVertexList(
			mesh::MeshPtr mesh,
			const glm::vec3 colour,
			const float normalLength,
			const int pointSize
			);
	};


}


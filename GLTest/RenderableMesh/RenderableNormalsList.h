#pragma once

#include "Renderable.h"

namespace render
{
	class RenderableNormalsList : public Renderable
	{
	public:
		RenderableNormalsList();
		~RenderableNormalsList();

		virtual bool Create(
			mesh::MeshPtr mesh
		);

		virtual bool Update(
			mesh::BoneNode *boneHierarchyRoot
		);

		virtual void Render(
			ShaderManager &shaderManager,
			glm::mat4x4 &viewMatrix,
			glm::mat4x4 &projectionMatrix,
			glm::vec4 &lightPosition
		);

	private:
		void Prepare();

	};
}


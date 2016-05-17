#pragma once

#include "../Render/RenderFwdDecl.h"
#include "Renderable.h"
#include "../Batch/BatchFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"

namespace mesh
{
	class Mesh;
	class Node;
}

namespace animation
{
	class AnimationController;
	class AnimationInfo;
}

namespace render
{

	class NormalsVertexListCreator;
	enum VertexFormatType;

	class RenderableNormalsList : public Renderable
	{

	public:

		RenderableNormalsList(
			mesh::MeshPtr mesh
		);

		~RenderableNormalsList();

		virtual bool Create();

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
		void PrepareForRendering();

		GLuint m_vertexArrayHandle;
		GLuint m_positionBufferHandle;
		NormalsVertexListCreatorPtr m_normalsVertexListCreator;
		mesh::MeshPtr m_mesh;
	};
}
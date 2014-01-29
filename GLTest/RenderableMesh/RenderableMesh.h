#pragma once

#include "Renderable.h"
#include "../Batch/BatchFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"

namespace mesh
{
	class Mesh;
}

namespace Animation
{
	class AnimationInfo;
}

namespace render
{

enum VertexFormatType;

class RenderableMesh : public Renderable
{

public:

	RenderableMesh();

	~RenderableMesh();

	const render::PerNodeBatchList &GetBatches()
	{
		return m_perNodeRenderBatches;
	}

	bool Create(
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
	bool UpdateInternal(
		mesh::BoneNode *node
		);

	render::PerNodeBatchList m_perNodeRenderBatches; //TODO rendermesh now is aware of nodes - do we really want this?
	std::vector<glm::mat4x4> m_matrixPalette;//TODO 4x3
};
}
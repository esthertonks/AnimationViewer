#pragma once

#include "Renderable.h"
#include "RenderableFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"
#include "../Batch/BatchFwdDecl.h"

namespace render
{

class RenderableMesh : public Renderable
{

public:

	RenderableMesh(
		mesh::MeshPtr mesh
	);

	~RenderableMesh();

	const render::PerNodeBatchList &GetBatches()
	{
		return m_perNodeRenderBatches;
	}

	virtual bool ShouldCreate();

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
	render::PerNodeBatchList m_perNodeRenderBatches; //TODO rendermesh now is aware of nodes - do we really want this?
	BatchProcessorPtr m_meshBatchProcessor;
	SkinningMatrixProcessorPtr m_skinningMatrixProcessor;
};
}
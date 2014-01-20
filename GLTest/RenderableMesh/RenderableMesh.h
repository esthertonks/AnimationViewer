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

	virtual void Rotate(
		const float rotY,
		const float rotX
		);

	virtual void Render(
		ShaderManager &shaderManager,
		glm::mat4x4 &viewMatrix,
		glm::mat4x4 &projectionMatrix
		);

private:
	bool UpdateInternal(
		mesh::BoneNode *parent
		);

	render::PerNodeBatchList m_perNodeRenderBatches; //TODO rendermesh now is aware of nodes - do we really want this?
	std::vector<glm::mat4x4> m_matrixPalette;//TODO 4x3
};
}
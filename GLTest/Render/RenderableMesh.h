#pragma once

#include "Renderable.h"
#include "../Batch/BatchFwdDecl.h"

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

	const glm::mat4x4 &GetModelMatrix();

	const render::BatchList &GetBatches()
	{
		return m_renderBatches;
	}

	bool Create(
		mesh::MeshPtr &importMesh
		);

	virtual void Animate(
		long globalStartTime,
		animation::AnimationInfo *animationInfo
		);

	virtual void PauseAnimation();
	virtual void StopAnimation();

	virtual bool Update(
		long globalTime
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
	render::BatchList m_renderBatches;
};
}
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

	const glm::mat4x4 &GetModelMatrix();

	const render::BatchList &GetBatches()
	{
		return m_renderBatches;
	}

	bool Create(
		mesh::MeshPtr &mesh
		);

	virtual bool Update(
		mesh::Node *root
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
		mesh::Node *parent
		);

	render::BatchList m_renderBatches;
	std::vector<glm::mat4x4> m_matrixPalette;//TODO 4x3
};
}
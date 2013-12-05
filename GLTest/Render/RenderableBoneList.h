#pragma once

#include "../Render/RenderFwdDecl.h"
#include "Renderable.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{
	class Mesh;
}

namespace render
{

enum VertexFormatType;

class RenderableBoneList : public Renderable
{

public:

	RenderableBoneList();

	~RenderableBoneList();

	const glm::mat4x4 &GetModelMatrix();

	virtual bool Create(
		mesh::MeshPtr &importMesh
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
	void Prepare();

	RenderVertexArrayPtr m_vertexArray;
	GLuint m_vertexArrayHandle;
	GLuint m_positionBufferHandle;
};
}
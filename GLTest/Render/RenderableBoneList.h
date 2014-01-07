#pragma once

#include "../Render/RenderFwdDecl.h"
#include "Renderable.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{
	class Mesh;
	class Node;
}

namespace animation
{
	class Animator;
	class AnimationInfo;
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
	void AddPositionToVertexList(
	mesh::Node *node
	);

	void Prepare();

	ColourVertexArrayPtr m_vertexArray;
	GLuint m_vertexArrayHandle;
	GLuint m_positionBufferHandle;
	int m_numVerts;

	mesh::MeshPtr m_mesh;
	boost::shared_ptr<animation::Animator> m_animator;
};
}
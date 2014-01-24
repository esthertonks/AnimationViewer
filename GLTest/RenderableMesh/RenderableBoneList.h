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
	class AnimationController;
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

	virtual bool Create(
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
	void AddPositionToVertexList(
		mesh::BoneNode *boneHierarchyRoot
	);

	void Prepare();

	ColourVertexArrayPtr m_vertexArray;
	GLuint m_vertexArrayHandle;
	GLuint m_positionBufferHandle;
	int m_numVerts;
};
}
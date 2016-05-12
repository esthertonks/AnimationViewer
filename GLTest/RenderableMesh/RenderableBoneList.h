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
	void Prepare();

	ColourVertexArrayPtr m_vertexArray;
	GLuint m_vertexArrayHandle;
	GLuint m_positionBufferHandle;
	int m_numVerts;
};
}
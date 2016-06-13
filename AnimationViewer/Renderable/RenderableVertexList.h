#pragma once

#include "../Render/RenderFwdDecl.h"
#include "Renderable.h"
#include "RenderableFwdDecl.h"


namespace animation
{
	class AnimationController;
	class AnimationInfo;
}

namespace render
{

class RenderableVertexList : public Renderable
{

public:

	RenderableVertexList(
		VertexListProcessorPtr vertexListProcessor,
		int pointSize
	);

	~RenderableVertexList();

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
	VertexListProcessorPtr m_vertexListProcessor;
	int m_pointSize;
};
}
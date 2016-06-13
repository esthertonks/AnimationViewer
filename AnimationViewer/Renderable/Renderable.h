#pragma once

#include <glm\glm.hpp>

namespace mesh
{
	class BoneNode;
}

namespace render
{
	class ShaderManager;

class Renderable abstract
{
public:

	Renderable();

	virtual ~Renderable();

	virtual bool Create() = 0;

	virtual bool Update(
		mesh::BoneNode *boneHierarchyRoot
		) = 0;

	virtual void Render(
		ShaderManager &shaderManager,
		glm::mat4x4 &viewMatrix,
		glm::mat4x4 &projectionMatrix,
		glm::vec4 &lightPosition
		) = 0;

protected:

};
}
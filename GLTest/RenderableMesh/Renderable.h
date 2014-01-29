#pragma once

#include <glm\glm.hpp>
#include "../Mesh/MeshFwdDecl.h"

namespace mesh
{
	class BoneNode;
}

namespace animation
{
	class AnimationInfo;//TODO urgh get rid
}

namespace render
{
	class ShaderManager;

class Renderable abstract
{
public:

	Renderable();

	virtual ~Renderable();

	virtual bool Create(
		mesh::MeshPtr mesh
		) = 0;

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
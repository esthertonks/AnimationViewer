#pragma once

#include <glm\glm.hpp>
#include "../ImportMesh/ImportFwdDecl.h"

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
		mesh::MeshPtr &importMesh
		) = 0;

	virtual void Animate(
		long globalStartTime,
		animation::AnimationInfo *animationInfo
		) = 0;

	virtual void PauseAnimation() = 0;
	virtual void StopAnimation() = 0;

	virtual bool Update(
		long deltaTime
	) = 0;

	virtual void Rotate(
		const float rotY,
		const float rotX
		) = 0;

	virtual void Render(
		ShaderManager &shaderManager,
		glm::mat4x4 &viewMatrix,
		glm::mat4x4 &projectionMatrix
		) = 0;

protected:
	float m_rotAroundY;
	float m_rotAroundX;
	glm::mat4x4 m_modelMatrix; // Animatable meshes must be identity
};
}
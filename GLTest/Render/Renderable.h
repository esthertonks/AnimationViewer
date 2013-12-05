#pragma once

#include <glm\glm.hpp>
#include "../ImportMesh/ImportFwdDecl.h"

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
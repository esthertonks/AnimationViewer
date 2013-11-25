#pragma once

#include <glm\glm.hpp>
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{
	class Mesh;
}

namespace render
{

enum VertexFormatType;

class RenderEntity //TODO renerable?
{

public:

	RenderEntity();

	~RenderEntity();

	const glm::mat4x4 &GetModelMatrix();

	const render::BatchList &GetBatches()
	{
		return m_renderBatches;
	}

	bool Create(
		mesh::Mesh &importMesh
		);

	void Rotate(
		float rotX,
		float rotZ
		);

private:
	render::BatchList m_renderBatches;
	//glm::mat4x4 m_modelMatrix; // Must be identity
	float m_rotateX;
	float m_rotateZ;
};
}
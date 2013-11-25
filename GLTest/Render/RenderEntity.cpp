#include "RenderEntity.h"
#include "../Batch/BatchProcessor.h"
#include "../Batch/BatchList.h"
#include "../Batch/Batch.h"
#include "../ImportMesh/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

namespace render
{

RenderEntity::RenderEntity()
	: m_rotateX(0),
	m_rotateZ(0)
{
}

bool RenderEntity::Create(
	mesh::Mesh &importMesh
	)
{			
	batch::BatchProcessor meshBatchProcessor;
		
	meshBatchProcessor.CreateBatches(importMesh, m_renderBatches);
		
	meshBatchProcessor.PrepareBatches(m_renderBatches);

	return true;
}

void RenderEntity::Rotate(
	float rotX,
	float rotZ
	)
{
	m_rotateX = rotX;
	m_rotateZ = rotZ;
}

const glm::mat4x4 &RenderEntity::GetModelMatrix()
{
	glm::mat4x4 modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotateX, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, m_rotateZ, glm::vec3(0.0f, 0.0f, 1.0f));
	return modelMatrix;
}

RenderEntity::~RenderEntity()
{
	if(m_renderBatches.size() != 0)
	{
		m_renderBatches.clear();
		m_renderBatches.resize(0); //TODO does this need to be a pointer???
	}
}

}
#include "RenderEntity.h"
#include "../Batch/BatchProcessor.h"
#include "../Batch/BatchList.h"
#include "../Batch/Batch.h"
#include "../ImportMesh/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

namespace render
{

RenderEntity::RenderEntity()
	: m_rotAroundY(0),
	m_rotAroundX(0),
	m_modelMatrix(1.0f)
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
	float rotAroundY,
	float rotAroundX
	)
{
	m_rotAroundY += rotAroundY;
	m_rotAroundX += rotAroundX;

	m_modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotAroundY, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotAroundX, glm::vec3(1.0f, 0.0f, 0.0f));
}

const glm::mat4x4 &RenderEntity::GetModelMatrix()
{
	return m_modelMatrix;
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
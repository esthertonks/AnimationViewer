#include "RenderableMesh.h"
#include "ShaderManager.h"
#include "../Batch/BatchProcessor.h"
#include "../Batch/BatchList.h"
#include "../Batch/Batch.h"
#include "../ImportMesh/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

namespace render
{

RenderableMesh::RenderableMesh()
	: Renderable()
{
}

bool RenderableMesh::Create(
	mesh::MeshPtr &importMesh
	)
{			
	batch::BatchProcessor meshBatchProcessor;
		
	meshBatchProcessor.CreateBatches(importMesh, m_renderBatches);
		
	meshBatchProcessor.PrepareBatches(m_renderBatches);

	return true;
}

void RenderableMesh::Animate(
	long globalStartTime,
	animation::AnimationInfo *animationInfo
	)
{
	//m_animator = boost::shared_ptr<animation::Animator>(new animation::Animator());
	//m_animator = boost::shared_ptr<animation::Animator>(new animation::Animator());
	//m_animator->StartAnimation(0, fps);
}

void RenderableMesh::PauseAnimation()
{
	//m_animator->PauseAnimation();
	//delete m_animator;
}

void RenderableMesh::StopAnimation()
{
	//m_animator->StopAnimation();
	//delete m_animator;
}

bool  RenderableMesh::Update(
	long time
	)
{		
	//mesh::Node *root = m_mesh->GetNodeHierarchy();
	//m_animator->PrepareBoneHierarcy(root, time, fps);

	//AddPositionToVertexList(root);

	//Prepare();

	return true;
}

void RenderableMesh::Rotate(
	const float rotAroundY,
	const float rotAroundX
	)
{
	m_rotAroundY += rotAroundY;
	m_rotAroundX += rotAroundX;

	m_modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotAroundY, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotAroundX, glm::vec3(1.0f, 0.0f, 0.0f));
}

const glm::mat4x4 &RenderableMesh::GetModelMatrix()
{
	return m_modelMatrix;
}

void RenderableMesh::Render(
	ShaderManager &shaderManager,
	glm::mat4x4& viewMatrix,
	glm::mat4x4& projectionMatrix
	)
{

	render::BatchList::const_iterator batchIterator;

	for(batchIterator = m_renderBatches.begin(); batchIterator != m_renderBatches.end(); batchIterator++)
	{
		ShaderProgramType batchShaderProgramType = (*batchIterator)->GetShaderProgramType();
		if(batchShaderProgramType != shaderManager.GetCurrentProgramType())
		{
			shaderManager.SetProgramCurrent(batchShaderProgramType);
			int programId = shaderManager.GetProgramId(batchShaderProgramType);

			(*batchIterator)->PrepareShaderParams(programId);
		}

		if((*batchIterator)->GetShaderProgramType() != None)
		{
			int programId = shaderManager.GetCurrentProgramId();
			assert(programId != -1);

			GLint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
			GLint viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
			GLint projectionMatrixLocation = glGetUniformLocation(programId, "projectionMatrix"); //TODO only needs setting on resize
			GLint normalMatrixLocation = glGetUniformLocation(programId, "normalMatrix");
			if(modelMatrixLocation >= 0 && viewMatrixLocation >= 0 && projectionMatrixLocation >= 0)
			{
				glm::mat4x4 modelViewMatrix = viewMatrix * GetModelMatrix();
				glm::mat3x3 normalMatrix = glm::mat3x3(glm::vec3(modelViewMatrix[0]), glm::vec3(modelViewMatrix[1]), glm::vec3(modelViewMatrix[2]));

				//TODO this should not be here - light class please. Also the other light params currently in the appearances should be in the same place as this...
				GLint lightPositionLocation = glGetUniformLocation(programId, "light.position");
				glm::vec4 lightPositionMatrix = viewMatrix * glm::vec4(100.0f, 0.0f, 0.0f, 1.0f); //TODO this is the same position as the camera - could do with being enforced...
				glUniform4f(lightPositionLocation, lightPositionMatrix.x, lightPositionMatrix.y, lightPositionMatrix.z, lightPositionMatrix.w);

				glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
				glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
				glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
				glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
			}

				//wxLogDebug("%u\n", GetVertexArrayHandle());
			glBindVertexArray((*batchIterator)->GetVertexArrayHandle());

			//m_indexBufferHandle
			glDrawElements(GL_TRIANGLES, (*batchIterator)->GetNumIndices(), GL_UNSIGNED_SHORT, (GLvoid*)0);
		}
	}
}

RenderableMesh::~RenderableMesh()
{
	if(m_renderBatches.size() != 0)
	{
		m_renderBatches.clear();
		m_renderBatches.resize(0); //TODO does this need to be a pointer???
	}
}

}
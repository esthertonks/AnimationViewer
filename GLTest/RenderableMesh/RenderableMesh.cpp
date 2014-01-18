#include "RenderableMesh.h"
#include "../Render/ShaderManager.h"
#include "../Batch/BatchProcessor.h"
#include "../Batch/Batch.h"
#include "../Mesh/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Mesh/BoneNode.h"
#include "../Utils/MathsUtils.h"

namespace render
{

RenderableMesh::RenderableMesh()
	: Renderable()
{
}

bool RenderableMesh::Create(
	mesh::MeshPtr &mesh
	)
{	
	batch::BatchProcessor meshBatchProcessor;
		
	meshBatchProcessor.CreateBatches(mesh, m_renderBatches);
		
	meshBatchProcessor.PrepareBatches(m_renderBatches);

	return true;
}

static int boneIdCheck = 0;

bool RenderableMesh::Update(
	mesh::Node *root
	)
{		
	m_matrixPalette.clear();
	boneIdCheck = 0;
	UpdateInternal(root);

	return true;
}

bool RenderableMesh::UpdateInternal(
	mesh::Node *parent
	)
{		
	//TODO these MUST be in the sameorder they were added in import so the id's match up //TODO createPalette method in either animator or mesh class?
	if(parent->GetType() == mesh::NodeType::BoneType)
	{
		mesh::BoneNode *bone = static_cast<mesh::BoneNode*>(parent);
		assert(bone->GetId() == boneIdCheck);

		glm::mat4x4 bonePaletteMatrix;

		utils::MathsUtils::ConvertFBXToGLMatrix(bone->GetGlobalTransform() * bone->GetInverseReferenceMatrix(), bonePaletteMatrix);

		m_matrixPalette.push_back(bonePaletteMatrix);

		boneIdCheck++;
	}

	for(mesh::Node* childNode = parent->m_firstChild; childNode != NULL; childNode = childNode->m_next)
	{
		UpdateInternal(childNode);
	}

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

			GLint bonePaletteMatrixLocation = glGetUniformLocation(programId, "boneMatrixPalette");
			if(bonePaletteMatrixLocation >= 0 && m_matrixPalette.size() > 0)
			{
				glUniformMatrix4fv(bonePaletteMatrixLocation, m_matrixPalette.size(), GL_FALSE, &m_matrixPalette[0][0][0]);
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
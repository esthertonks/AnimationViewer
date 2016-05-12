#include "RenderableMesh.h"
#include "../Render/ShaderManager.h"
#include "../Batch/BatchProcessor.h"
#include "../Batch/Batch.h"
#include "../Mesh/BoneNode.h"
#include "../Utils/MathsUtils.h"

namespace render
{

RenderableMesh::RenderableMesh()
	: Renderable()
{
}

bool RenderableMesh::Create(
	mesh::MeshPtr mesh //TODO FIXME dont want to be passing this in here.
	)
{	
	batch::BatchProcessor meshBatchProcessor; // FIXME rename to batchCreator? Also normals list creator, bone list creator etc
		
	meshBatchProcessor.CreateBatches(mesh, m_perNodeRenderBatches);
		
	meshBatchProcessor.PrepareBatches(m_perNodeRenderBatches);

	return true;
}

static unsigned int boneIdCheck = 0;

bool RenderableMesh::Update(
	mesh::BoneNode *boneHierarchyRoot
	)
{	
	if(!boneHierarchyRoot)
	{
		return false;
	}
	m_matrixPalette.clear();
	boneIdCheck = 0;

	UpdateInternal(boneHierarchyRoot);

	return true;
}

bool RenderableMesh::UpdateInternal(
	mesh::BoneNode *node
	)
{		
	//TODO these MUST be in the sameorder they were added in import so the id's match up //TODO createPalette method in either animator or mesh class?
	assert(node->GetId() == boneIdCheck);
	glm::mat4x4 bonePaletteMatrix;
	utils::MathsUtils::ConvertFBXToGLMatrix(node->GetGlobalTransform() * node->GetInverseReferenceMatrix(), bonePaletteMatrix);

	m_matrixPalette.push_back(bonePaletteMatrix);

	boneIdCheck++;

	for(mesh::BoneNode* childNode = node->m_firstChild; childNode != NULL; childNode = childNode->m_next)
	{
		UpdateInternal(childNode);
	}

	return true;
}

void RenderableMesh::Render(
	ShaderManager &shaderManager,
	glm::mat4x4& viewMatrix,
	glm::mat4x4& projectionMatrix,
	glm::vec4 &lightPosition
	)
{
	render::PerNodeBatchList::const_iterator batchListIterator;

	for(batchListIterator = m_perNodeRenderBatches.begin(); batchListIterator != m_perNodeRenderBatches.end(); batchListIterator++)
	{
		BatchList renderBatches = batchListIterator->second;
		render::BatchList::const_iterator batchIterator;

		for(batchIterator = renderBatches.begin(); batchIterator != renderBatches.end(); batchIterator++)
		{
			if(*batchIterator == NULL) // This will be a case if an appearance exists but is not uses by this node //TODO get rid of this?
			{
				continue;
			}

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
					glm::mat4x4 modelViewMatrix = viewMatrix * (*batchIterator)->GetModelMatrix();
					glm::mat3x3 normalMatrix = glm::mat3x3(glm::vec3(modelViewMatrix[0]), glm::vec3(modelViewMatrix[1]), glm::vec3(modelViewMatrix[2]));

					//TODO this should not be here - light class please. Also the other light params currently in the appearances should be in the same place as this...
					GLint lightPositionLocation = glGetUniformLocation(programId, "light.position");
					//lightPosition = viewMatrix * lightPosition;
					glUniform4f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);

					glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &(*batchIterator)->GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
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
}

RenderableMesh::~RenderableMesh()
{
	if(m_perNodeRenderBatches.size() != 0)
	{
		m_perNodeRenderBatches.clear();
	}
}

}
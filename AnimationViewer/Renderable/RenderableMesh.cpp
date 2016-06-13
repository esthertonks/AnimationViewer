#include "RenderableMesh.h"
#include "../Render/ShaderManager.h"
#include "../MeshProcessors/BatchProcessor.h"
#include "../Batch/Batch.h"
#include "../Mesh/BoneNode.h"
#include "../Utils/MathsUtils.h"
#include "../MeshProcessors/SkinningMatrixProcessor.h"

namespace render
{

RenderableMesh::RenderableMesh(
	mesh::MeshPtr mesh
)
	: Renderable(),
	m_meshBatchProcessor(new BatchProcessor(mesh)),
	m_skinningMatrixProcessor(new SkinningMatrixProcessor())
{
}

bool RenderableMesh::Create()
{			
	m_meshBatchProcessor->CreateBatches(m_perNodeRenderBatches);
		
	m_meshBatchProcessor->PrepareBatches(m_perNodeRenderBatches);

	return true;
}

static unsigned int boneIdCheck = 0;

bool RenderableMesh::Update(
	mesh::BoneNode *boneHierarchyRoot
	)
{	
	m_skinningMatrixProcessor->CreateBoneMatrix(boneHierarchyRoot);

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
			if(*batchIterator == NULL) // This will be a case if an appearance exists but is not used by this node //TODO get rid of this?
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
					//lightPosition = viewMatrix * lightPosition; // Multiply by the view matrix to move the light with the mesh
					glUniform4f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);

					glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &(*batchIterator)->GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
					glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
					glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
					glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
				}

				BoneMatrixPalette boneMatrixPalette = m_skinningMatrixProcessor->GetBoneMatrixPalette();
				GLint bonePaletteMatrixLocation = glGetUniformLocation(programId, "boneMatrixPalette");
				if(bonePaletteMatrixLocation >= 0 && boneMatrixPalette.size() > 0)
				{
					glUniformMatrix4fv(bonePaletteMatrixLocation, boneMatrixPalette.size(), GL_FALSE, &boneMatrixPalette[0][0][0]);
				}

				glBindVertexArray((*batchIterator)->GetVertexArrayHandle());

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
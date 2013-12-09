#include "RenderableBoneList.h"

#include "../ImportMesh/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/ShaderManager.h"

namespace render
{

RenderableBoneList::RenderableBoneList()
	: Renderable()
{
}

bool RenderableBoneList::Create(
	mesh::MeshPtr &importMesh
	)
{		
	mesh::BoneNode *boneNode = importMesh->GetBoneNodeHierarchy();
	for(boneNode; boneNode != NULL; boneNode = boneNode->m_next)
	{
		ColourVertex vertex;
		glm::mat4x4 globalTransform = boneNode->GetGlobalTransform();
		vertex.m_position.x = globalTransform[3][0];
		vertex.m_position.y = globalTransform[3][1];
		vertex.m_position.z = globalTransform[3][2];
		vertex.m_colour = glm::vec3(1.0, 0.0, 0.0);

		m_vertexArray.push_back(vertex);
	}

	Prepare();

	return true;
}

void RenderableBoneList::Prepare()
{
	/////////////////// Create the VBO ////////////////////
	// Create and set-up the vertex array object
	glGenVertexArrays( 1, &m_vertexArrayHandle);
	glBindVertexArray(m_vertexArrayHandle);

	// Create and populate the buffer objects
	GLuint vboHandles[1];
	glGenBuffers(1, vboHandles);
	m_positionBufferHandle = vboHandles[0];

	glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(render::ColourVertex) * m_vertexArray.size(), &m_vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render::ColourVertex), (GLubyte *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render::ColourVertex), (GLubyte *)sizeof(glm::vec3));

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
}

void RenderableBoneList::Rotate(
	const float rotAroundY,
	const float rotAroundX
	)
{
	m_rotAroundY += rotAroundY;
	m_rotAroundX += rotAroundX;

	m_modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotAroundY, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotAroundX, glm::vec3(1.0f, 0.0f, 0.0f));
}

const glm::mat4x4 &RenderableBoneList::GetModelMatrix()
{
	return m_modelMatrix;
}

void RenderableBoneList::Render(
	ShaderManager &shaderManager,
	glm::mat4x4& viewMatrix,
	glm::mat4x4& projectionMatrix
	)
{
	int numVertices = m_vertexArray.size();
	if(numVertices == 0)
	{
		return;
	}

	if(shaderManager.GetCurrentProgramType() != Overlay)
	{
		shaderManager.SetProgramCurrent(Overlay);
	}

	int programId = shaderManager.GetProgramId(Overlay);
	if(programId != -1)
	{
		//TODO shader to draw linelists?
		//TODO well most of this is duplicated in all renderables!!!!!!!!!??
		GLint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
		GLint viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
		GLint projectionMatrixLocation = glGetUniformLocation(programId, "projectionMatrix"); //TODO only needs setting on resize
		if(modelMatrixLocation >= 0 && viewMatrixLocation >= 0 && projectionMatrixLocation >= 0)
		{
			glm::mat4x4 modelViewMatrix = viewMatrix * GetModelMatrix();
			glm::mat3x3 normalMatrix = glm::mat3x3(glm::vec3(modelViewMatrix[0]), glm::vec3(modelViewMatrix[1]), glm::vec3(modelViewMatrix[2]));

			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		}

		glBindVertexArray(m_vertexArrayHandle);

		glDrawArrays(GL_LINE_STRIP, 0, m_vertexArray.size());
	}
}

RenderableBoneList::~RenderableBoneList()
{

}

}
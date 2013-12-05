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
		Vertex vertex;
		glm::mat4x4 localTransform = boneNode->GetLocalKeyTransform(0);
		vertex.m_position.x = localTransform[3][0];
		vertex.m_position.y = localTransform[3][1];
		vertex.m_position.z = localTransform[3][2];

		m_vertexArray.push_back(vertex);
	}

	return true;
}

void RenderableBoneList::Prepare()
{
	/////////////////// Create the VBO ////////////////////
	// Create and set-up the vertex array object
	glGenVertexArrays( 1, &m_vertexArrayHandle);
	glBindVertexArray(m_vertexArrayHandle);

	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	m_positionBufferHandle = vboHandles[0];

	glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(render::Vertex) * m_vertexArray.size(), &m_vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)sizeof(glm::vec3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)(sizeof(glm::vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)(sizeof(glm::vec3) * 3));

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
	glEnableVertexAttribArray(2);  // Vertex normal
	glEnableVertexAttribArray(3);  // Texture coord
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

	//TODO shader to draw linelists?
	//TODO well most of this is duplicated in all renderables!!!!!!!!!??
	GLint modelMatrixLocation = glGetUniformLocation(shaderManager.GetCurrentProgramId(), "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(shaderManager.GetCurrentProgramId(), "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(shaderManager.GetCurrentProgramId(), "projectionMatrix"); //TODO only needs setting on resize
	GLint normalMatrixLocation = glGetUniformLocation(shaderManager.GetCurrentProgramId(), "normalMatrix");
	if(modelMatrixLocation >= 0 && viewMatrixLocation >= 0 && projectionMatrixLocation >= 0)
	{
		glm::mat4x4 modelViewMatrix = viewMatrix * GetModelMatrix();
		glm::mat3x3 normalMatrix = glm::mat3x3(glm::vec3(modelViewMatrix[0]), glm::vec3(modelViewMatrix[1]), glm::vec3(modelViewMatrix[2]));

		//TODO this should not be here - light class please. Also the other light params currently in the appearances should be in the same place as this...
		GLint lightPositionLocation = glGetUniformLocation(shaderManager.GetCurrentProgramId(), "light.position");
		glm::vec4 lightPositionMatrix = viewMatrix * glm::vec4(100.0f, 0.0f, 0.0f, 1.0f); //TODO this is the same position as the camera - could do with being enforced...
		glUniform4f(lightPositionLocation, lightPositionMatrix.x, lightPositionMatrix.y, lightPositionMatrix.z, lightPositionMatrix.w);

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	}

	glBindVertexArray(m_vertexArrayHandle);

	glDrawElements(GL_LINE_STRIP, m_vertexArray.size(), GL_UNSIGNED_SHORT, (GLvoid*)0);
}

RenderableBoneList::~RenderableBoneList()
{

}

}
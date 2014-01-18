#include "RenderableBoneList.h"

#include "../Mesh/BoneNode.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/ShaderManager.h"
#include <wx/log.h>

namespace render
{

RenderableBoneList::RenderableBoneList()
	: Renderable(),
	m_numVerts(0)
{
}

bool RenderableBoneList::Create(
	mesh::MeshPtr mesh // Ignored - this particular overlay doesnt need the mesh itself
	)
{		
	m_numVerts = 0;
	return true;
}

bool RenderableBoneList::Update(
	mesh::BoneNode *boneHierarchyRoot
	)
{
	m_vertexArray.clear();

	AddPositionToVertexList(boneHierarchyRoot);
	m_numVerts = m_vertexArray.size(); // Keep a record of the new verts so that the draw calls can use it
	Prepare(); // Prepare for rendering
	//wxLogDebug("tick3");
	return true;
}

static float col = 0.0f;
void RenderableBoneList::AddPositionToVertexList(
	mesh::BoneNode *boneNode
	)
{
	for(boneNode; boneNode != NULL; boneNode = boneNode->m_next)
	{
		if(boneNode->m_parent) // Dont bother with the root - it will be added as the parent of it's children
		{
			// Add the parent nodes position
			ColourVertex parentVertex;
			FbxAMatrix& parentGlobalTransform = boneNode->m_parent->GetGlobalTransform();
			parentVertex.m_position.x = static_cast<float>(parentGlobalTransform[3][0]);
			parentVertex.m_position.y = static_cast<float>(parentGlobalTransform[3][1]);
			parentVertex.m_position.z = static_cast<float>(parentGlobalTransform[3][2]);
			parentVertex.m_colour = glm::vec3(col, 0.0, 0.0);

			m_vertexArray.push_back(parentVertex);

			// Add this nodes position
			ColourVertex vertex;
			FbxAMatrix& globalTransform = boneNode->GetGlobalTransform();
			vertex.m_position.x = static_cast<float>(globalTransform[3][0]);
			vertex.m_position.y = static_cast<float>(globalTransform[3][1]);
			vertex.m_position.z = static_cast<float>(globalTransform[3][2]);
			vertex.m_colour = glm::vec3(col, 0.0, 0.0);
			col += 0.02f;

			m_vertexArray.push_back(vertex);
		}

		if(boneNode->m_firstChild)
		{
			AddPositionToVertexList(boneNode->m_firstChild); // If this node has any children then add their info two
		}
	}
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(render::ColourVertex) * m_numVerts, &m_vertexArray[0], GL_STATIC_DRAW);

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
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_DEPTH_TEST);

	if(m_numVerts == 0)
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
		//wxLogDebug("draw!");
		//wxLogDebug("pos x %f", m_vertexArray[0].m_position.x);
		//wxLogDebug("pos y %f", m_vertexArray[0].m_position.y);
		//wxLogDebug("pos z %f", m_vertexArray[0].m_position.z);
		//TODO shader to draw linelists?
		//TODO well most of this is duplicated in all renderables!!!!!!!!!??
		GLint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
		GLint viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
		GLint projectionMatrixLocation = glGetUniformLocation(programId, "projectionMatrix"); //TODO only needs setting on resize
		if(modelMatrixLocation >= 0 && viewMatrixLocation >= 0 && projectionMatrixLocation >= 0)
		{
			glm::mat4x4 modelViewMatrix = viewMatrix * GetModelMatrix();

			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		}

		GLint pointSizeLocation = glGetUniformLocation(programId, "pointSize");
		glUniform1f(pointSizeLocation, 10);

		glBindVertexArray(m_vertexArrayHandle);

		glDrawArrays(GL_LINES, 0, m_numVerts);
		glDrawArrays(GL_POINTS, 0, m_numVerts);
	}

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
}

RenderableBoneList::~RenderableBoneList()
{

}

}
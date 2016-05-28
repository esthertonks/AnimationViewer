#include "RenderableVertexList.h"

#include "../Mesh/BoneNode.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/ShaderManager.h"
#include "../RenderableCreators/VertexListCreatorBase.h"
#include <wx/log.h>

namespace render
{

RenderableVertexList::RenderableVertexList(
	VertexListCreatorBasePtr boneListCreator,
	int pointSize
)
	: Renderable(),
	m_vertexListCreator(boneListCreator),
	m_pointSize(pointSize)
{
}

bool RenderableVertexList::Create()
{		
	return true;
}

bool RenderableVertexList::Update(
	mesh::BoneNode *boneHierarchyRoot
	)
{
	if(!boneHierarchyRoot)
	{
		return false;
	}

	m_vertexListCreator->CreateAnimatedVertexList(boneHierarchyRoot);

	assert(m_vertexListCreator->GetNumVertsInList() != 0);

	PrepareForRendering();

	return true;
}

void RenderableVertexList::PrepareForRendering()
{
	// Create the VBO:

	// Create and set-up the vertex array object
	glGenVertexArrays( 1, &m_vertexArrayHandle);
	glBindVertexArray(m_vertexArrayHandle);

	// Create and populate the buffer objects
	GLuint vboHandles[1];
	glGenBuffers(1, vboHandles);
	m_positionBufferHandle = vboHandles[0];

	glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(render::ColourVertex) * m_vertexListCreator->GetNumVertsInList(), &m_vertexListCreator->GetVertexList()[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render::ColourVertex), (GLubyte *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render::ColourVertex), (GLubyte *)sizeof(glm::vec3));

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
}

void RenderableVertexList::Render(
	ShaderManager &shaderManager,
	glm::mat4x4& viewMatrix,
	glm::mat4x4& projectionMatrix,
	glm::vec4 &lightPosition
	)
{
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_DEPTH_TEST);

	if(m_vertexListCreator->GetNumVertsInList() == 0)
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
		GLint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
		GLint viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
		GLint projectionMatrixLocation = glGetUniformLocation(programId, "projectionMatrix"); //TODO only needs setting on resize
		if(modelMatrixLocation >= 0 && viewMatrixLocation >= 0 && projectionMatrixLocation >= 0)
		{
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);//&GetModelMatrix()[0][0]);//TODO pass fewer matrices through!!!
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		}

		GLint pointSizeLocation = glGetUniformLocation(programId, "pointSize");
		glUniform1f(pointSizeLocation, m_pointSize);

		glBindVertexArray(m_vertexArrayHandle);

		glDrawArrays(GL_LINES, 0, m_vertexListCreator->GetNumVertsInList());
		glDrawArrays(GL_POINTS, 0, m_vertexListCreator->GetNumVertsInList());
	}

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
}

RenderableVertexList::~RenderableVertexList()
{

}

}
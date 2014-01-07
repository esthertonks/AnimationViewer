#include "RenderableBoneList.h"

#include "../ImportMesh/Mesh.h"
#include "../ImportMesh/BoneNode.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/ShaderManager.h"

#include "../Animation/Animator.h"

namespace render
{

RenderableBoneList::RenderableBoneList()
	: Renderable(),
	m_animator(NULL),
	m_numVerts(0)
{
}

bool RenderableBoneList::Create(
	mesh::MeshPtr &importMesh
	)
{		
	m_mesh = importMesh;
	m_numVerts = 0;
	return true;
}

//TODO animator in main class and pass in the mesh!!! render mesh is something entirely different!!!!!
// TODO isnt this called setAnimation?
void RenderableBoneList::Animate(
	long globalStartTime,
	animation::AnimationInfo *animationInfo
	)
{
	m_animator = boost::shared_ptr<animation::Animator>(new animation::Animator());
	m_animator->StartAnimation(globalStartTime, animationInfo);
}

void RenderableBoneList::PauseAnimation()
{
	if(m_animator) // TODO stop using this to control whether it is animating. At least use a bool. NULL with shared_ptr???
	{
		m_animator->StopAnimation(); //TODO we are controlling whether the animation is playing here not the animator. Incorrent???!
		m_animator = NULL;
	}
}

void RenderableBoneList::StopAnimation()
{
	if(m_animator) // TODO stop using this to control whether it is animating. At least use a bool. NULL with shared_ptr???
	{
		Update(0);
		m_animator->StopAnimation();//TODO we are controlling whether the animation is playing here not the animator. Incorrent???!
		m_animator = NULL;
	}
}

bool RenderableBoneList::Update(
	long globalTime
	)
{
	m_vertexArray.clear();
	mesh::Node *root = m_mesh->GetNodeHierarchy();
	if(m_animator)
	{
		m_animator->PrepareBoneHierarcy(root, globalTime);
	}

	AddPositionToVertexList(root);
	m_numVerts = m_vertexArray.size(); // Keep a record of the new verts so that the draw calls can use it
	Prepare();

	return true;
}

static float col = 0.0f;
void RenderableBoneList::AddPositionToVertexList(
	mesh::Node *node
	)
{
	for(node; node != NULL; node = node->m_next)
	{
		if(node->GetType() != mesh::BoneType)
		{
			continue;
		}

		if(node->m_parent) // Dont bother with the root - it will be added as the parent of it's children
		{
			// Add the parent nodes position
			mesh::BoneNode *parentNode = static_cast<mesh::BoneNode*>(node->m_parent); //TODO static cast?
			ColourVertex parentVertex;
			glm::mat4x4& parentGlobalTransform = parentNode->GetGlobalTransform();
			parentVertex.m_position.x = parentGlobalTransform[3][0];
			parentVertex.m_position.y = parentGlobalTransform[3][1];
			parentVertex.m_position.z = parentGlobalTransform[3][2];
			parentVertex.m_colour = glm::vec3(col, 0.0, 0.0);

			m_vertexArray.push_back(parentVertex);

			// Add this nodes position
			mesh::BoneNode *boneNode = static_cast<mesh::BoneNode*>(node); //TODO static cast?
			if(boneNode->GetName() == "Bip01 L UpperArm")
			{
				int stop = 0;
			}
			ColourVertex vertex;
			glm::mat4x4& globalTransform = boneNode->GetGlobalTransform();
			vertex.m_position.x = globalTransform[3][0];
			vertex.m_position.y = globalTransform[3][1];
			vertex.m_position.z = globalTransform[3][2];
			vertex.m_colour = glm::vec3(col, 0.0, 0.0);
			col += 0.02f;

			m_vertexArray.push_back(vertex);
		}

		if(node->m_firstChild)
		{
			AddPositionToVertexList(node->m_firstChild); // If this node has any children then add their info two
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
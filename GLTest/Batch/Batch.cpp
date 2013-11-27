#include "Batch.h"
#include "VertexFormat.h"
#include "Appearance.h"
#include "SOIL.h"

namespace render
{

Batch::Batch(
	VertexFormatType vertexFormatType
	)
: 
m_appearance(NULL),
m_vertexFormatType(vertexFormatType)
{

}

void Batch::AllocateVertices(
	const int numVertices
	)		
{
	m_vertexArray.reserve(numVertices);
}

void Batch::AllocateIndices(
	const short numIndices
	)		
{
	m_indexArray.reserve(numIndices);
}

void Batch::AddVertex(
	const Vertex &vertex
	)
{
	m_vertexArray.push_back(vertex);
}

void Batch::AddIndex(
	const short vertexIndex
	)
{
	m_indexArray.push_back(vertexIndex);
}

void Batch::Prepare()
{
	//for each in batchlist
	//create and bind vbo - store id's in batchlist
	//load textures - store in batch list
	std::string texturePath = m_appearance->GetDiffuseTexturePath();
	if(!texturePath.empty())
	{
		int width;
		int height;
		unsigned char* texture = SOIL_load_image(texturePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

		glActiveTexture(GL_TEXTURE0);
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

		SOIL_free_image_data(texture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	/////////////////// Create the VBO ////////////////////
	// Create and set-up the vertex array object
	glGenVertexArrays( 1, &m_vertexArrayHandle);
	glBindVertexArray(m_vertexArrayHandle);

	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	m_positionBufferHandle = vboHandles[0];
	m_indexBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(render::Vertex) * GetNumVertices(), &m_vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)sizeof(glm::vec3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)(sizeof(glm::vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (GLubyte *)(sizeof(glm::vec3) * 3));

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
	glEnableVertexAttribArray(2);  // Vertex normal
	glEnableVertexAttribArray(3);  // Texture coord

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * GetNumIndices(), &m_indexArray[0], GL_STATIC_DRAW);

	m_meshLoaded = true;
}

void Batch::PrepareShaderParams(
	GLuint programHandle
	)
{
	m_appearance->ConvertToShaderParams(programHandle);
}

Batch::~Batch()
{
	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(0, &m_indexBufferHandle);

	// Release the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_positionBufferHandle);

	// Release the vertex array object.
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vertexArrayHandle);

}

}
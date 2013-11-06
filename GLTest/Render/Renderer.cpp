#include "Renderer.h"

#include <fstream>

#include <wx/log.h>

namespace Render
{

GLubyte Renderer::m_indices[] = {
    0, 1, 2, //Front face
    0, 2, 3,
    5, 0, 3,
    5, 3, 4, //Right face
    5, 6, 7, //Back face
    5, 7, 4,
    5, 6, 1, //Upper face
    5, 1, 0,
    1, 6, 7, //Left face
    1, 7, 2,
    7, 4, 3, //Bottom face
    7, 3, 2,
    };

Vert Renderer::verts[] = {
	glm::vec3(0.5f,  0.5f,  0.5f),  //V0
	glm::vec3(0.0f, 1.0f, 0.0f),    //C0

	glm::vec3(-0.5f,  0.5f,  0.5f), //V1
	glm::vec3(0.0f, 1.0f, 0.0f),    //C1

	glm::vec3(-0.5f, -0.5f,  0.5f), //V2
	glm::vec3(0.0f, 1.0f, 0.0f),    //C2

	glm::vec3(0.5f, -0.5f,  0.5f),  //V3
	glm::vec3(0.0f, 1.0f, 0.0f),    //C3

	glm::vec3(0.5f, -0.5f, -0.5f),  //V4
	glm::vec3(0.0f, 1.0f, 0.0f),    //C4

	glm::vec3(0.5f,  0.5f, -0.5f),  //V5
	glm::vec3(0.0f, 1.0f, 0.0f),    //C5

	glm::vec3(-0.5f,  0.5f, -0.5f), //V6
	glm::vec3(0.0f, 1.0f, 0.0f),    //C6

	glm::vec3(-0.5f, -0.5f, -0.5f), //V7
	glm::vec3(0.0f, 1.0f, 0.0f),    //C7
	};

Renderer::Renderer()
	: m_loaded(false)
{

}

bool Renderer::LoadShaders()
{
	GLuint vertexShaderId = LoadShader("Shaders/basic.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = LoadShader("Shaders/basic.frag", GL_FRAGMENT_SHADER);

	if(!CompileShader("basic.vert", vertexShaderId))
	{
		return false;
	}

	if(!CompileShader("basic.frag", fragmentShaderId))
	{
		return false;
	}

	shaders.push_back(vertexShaderId);
	shaders.push_back(fragmentShaderId);

	if(!LinkProgram(shaders))
	{
		return false;
	}

	glUseProgram(m_programHandle);

	OutputDebugShaderAttributeInfo();

	CreateVertexBuffers();

	m_loaded = true;

	return true;
}

/**
	@brief Loads a shader of the specified name and type
	@return GL_FALSE if failed or GL_TRUE if successfully loaded
*/
GLuint Renderer::LoadShader(
	const std::string &shaderName, // The name of the shader to load
	const GLenum shaderType	// The type of the shader ie GL_VERTEX_SHADER etc
	)
{
	const char *shaderTextBuffer = ReadShaderSourceFile(shaderName);
	if(!shaderTextBuffer)
	{
		return GL_FALSE;
	}

	// Create the shader object
	GLuint shaderId = glCreateShader(shaderType);
	if(shaderId)
	{
		// Pass the shader source to GL
		glShaderSource(shaderId, 1, &shaderTextBuffer, NULL);
	}
	else
	{
		wxLogDebug("Error creating vertex shader.\n");
	}

	delete[] shaderTextBuffer;

	return shaderId;
}

const char* Renderer::ReadShaderSourceFile(
	const std::string &shaderName
	)
{
	std::ifstream shaderStream(shaderName, std::ifstream::in);
	if(!shaderStream) {
		wxLogDebug("Error opening file: %s\n", shaderName);
		return NULL;
	}

	// TODO something better than a text buffer?
   char *shaderTextBuffer = new GLchar[10000];
	int i = 0;
	while(shaderStream.good()) 
	{
		int c = shaderStream.get();
		if(c > 0)
		{
			shaderTextBuffer[i++] = c;
		}
	}
	shaderStream.close();
	shaderTextBuffer[i++] = '\0';

	//wxLogDebug(buffer);
	return shaderTextBuffer;
}

/**
	@brief Compiles the shader with this shaderHandle and name
	@return GL_FALSE if failed or GL_TRUE if successfully loaded
*/
GLenum Renderer::CompileShader(
	const std::string &shaderName, // The name of the shader to load
	const GLuint shaderId //GLuint id for the shader source
	)
{
	// Compile the shader
	glCompileShader(shaderId);

	// Check whether the shader compiled successfully
	GLint compiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		wxLogDebug("Shader compilation failed for shader %s\n", shaderName);

		GLint errorTextLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorTextLength);

		std::string errorText(errorTextLength, ' ');

		GLsizei written;
		glGetShaderInfoLog(shaderId, errorTextLength, &written, &errorText[0]);

		wxLogDebug("Shader compilation errors: \n%s", errorText);
	}

	return compiled;
}

GLenum Renderer::LinkProgram(
	const std::vector<GLuint> &shaders // List of shaders needed for this shader program
	)
{
	// Create the program object
	m_programHandle = glCreateProgram();
	if(!m_programHandle)
	{
		wxLogDebug("Error creating program object.\n");
		return false;
    }

//    // Bind index 0 to the shader input variable "VertexPosition"
//    glBindAttribLocation(programHandle, 0, "VertexPosition");
//    // Bind index 1 to the shader input variable "VertexColor"
//    glBindAttribLocation(programHandle, 1, "VertexColor");

	// Attach the shaders to the program object
	std::vector<GLuint>::const_iterator iter;

	for(iter = shaders.begin(); iter != shaders.end(); iter++)
	{
		glAttachShader(m_programHandle, *iter);
	}

	// Link the program
	glLinkProgram(m_programHandle);

	//// Detach the vertex and fragment shaders from the program.
	//glDetachShader(m_programHandle, m_vertexShader);
	//glDetachShader(m_programHandle, m_fragmentShader);

	//// Delete the vertex and fragment shaders.
	//glDeleteShader(m_vertexShader);
	//glDeleteShader(m_fragmentShader);

	// Check for successful linking
	GLint linked;
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &linked);

	if(!linked)
	{
		wxLogDebug("Failed to link shader program\n" );

		GLint errorTextLength;
		glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &errorTextLength);

		std::string errorText(errorTextLength, ' ');

		GLsizei written;
		glGetShaderInfoLog(m_programHandle, errorTextLength, &written, &errorText[0]);

		wxLogDebug("Link shader errors: \n%s", errorText);
	}

	return linked;
}

void Renderer::OutputDebugShaderAttributeInfo()
{
	GLint maxlength, noofAttributes;
	glGetProgramiv(m_programHandle, GL_ACTIVE_ATTRIBUTES, &noofAttributes);
	glGetProgramiv(m_programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxlength);
 
	GLchar *name = (GLchar *)malloc(maxlength);
 
	GLint written, size, location;
	GLenum type;
	wxLogDebug("Index | Name\n");
	wxLogDebug("---------------------------------");
	for(int i = 0; i < noofAttributes; i++)
	{
		glGetActiveAttrib(m_programHandle, i, maxlength, &written, &size, &type, name);
 
		location = glGetAttribLocation(m_programHandle, name);
		wxLogDebug("Location %d name %s\n", location, name);
	}
 
	GLint noofUniforms;
	glGetProgramiv(m_programHandle, GL_ACTIVE_UNIFORMS, &noofUniforms);
	glGetProgramiv(m_programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlength);
 
	GLchar *uniformName = (GLchar *)malloc(maxlength);
 
	wxLogDebug("Location | Name\n");
	wxLogDebug("---------------------------------");
	for(int i = 0; i < noofUniforms; i++)
	{
		glGetActiveUniform(m_programHandle, i, maxlength, &written, &size, &type, uniformName);
 
		location = glGetUniformLocation(m_programHandle, uniformName);
		wxLogDebug("Location %d, uniformName %s\n", location, uniformName);
	}
}

void Renderer::CreateVertexBuffers()
{
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
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vert), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLubyte *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLubyte *)sizeof(glm::vec3));

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex color

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
}

Renderer::~Renderer()
{
	if(!m_loaded)
	{
		return;
	}

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

	std::vector<GLuint>::iterator iter;
	for(iter = shaders.begin(); iter != shaders.end(); iter++)
	{
		// Detach and delete any shaders
		glDetachShader(m_programHandle, *iter);
		glDeleteShader(*iter);
	}

	// Delete the shader program.
	glDeleteProgram(m_programHandle);
}
}

#include "ShaderManager.h"

#include <wx/log.h>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderable/RenderableMesh.h"
#include "../Batch/VertexFormat.h"
#include "../Batch/Batch.h"
#include "OrbitCamera.h"
#include "../Batch/Appearance.h"
#include "../Batch/PhongAppearance.h"
#include "../Batch/LambertAppearance.h"

namespace render
{
	// TODO redo this with a phong shader program and lambert shader program class which keep track of their own types etc. So one class per type in use.
	//Can thenget rid of m_shaderProgramByProgramType
	//TODO only really current need one set of shaders - discuss...
ShaderManager::ShaderManager()
	: m_currentProgramType(None)
{
	//TODO these could be specified in a text file
	//TODO the program id doesnt get setup until initialise - should all of this be done at once? Or could initialise setup the program id passed on this info here?
	Program phong;
	phong.m_programId = -1;
	phong.m_pathByShaderType.insert(std::pair<GLuint, std::string>(GL_VERTEX_SHADER, "Shaders/PerFragmentPhong.vert"));
	phong.m_pathByShaderType.insert(std::pair<GLuint, std::string>(GL_FRAGMENT_SHADER, "Shaders/PerFragmentPhong.frag"));

	m_shaderProgramByProgramType.insert(std::pair<ShaderProgramType, Program>(Phong, phong));

	Program lambert;
	lambert.m_programId = -1;//TODO umm constructor?!
	lambert.m_pathByShaderType.insert(std::pair<GLuint, std::string>(GL_VERTEX_SHADER, "Shaders/PerFragmentLambert.vert"));
	lambert.m_pathByShaderType.insert(std::pair<GLuint, std::string>(GL_FRAGMENT_SHADER, "Shaders/PerFragmentLambert.frag"));

	m_shaderProgramByProgramType.insert(std::pair<ShaderProgramType, Program>(Lambert, lambert));

	Program overlay;
	overlay.m_programId = -1;//TODO umm constructor?!
	overlay.m_pathByShaderType.insert(std::pair<GLuint, std::string>(GL_VERTEX_SHADER, "Shaders/Overlay.vert"));
	overlay.m_pathByShaderType.insert(std::pair<GLuint, std::string>(GL_FRAGMENT_SHADER, "Shaders/Overlay.frag"));

	m_shaderProgramByProgramType.insert(std::pair<ShaderProgramType, Program>(Overlay, overlay));
}

Program& ShaderManager::GetProgram(
	ShaderProgramType programType
	)
{
	return m_shaderProgramByProgramType.find(programType)->second;
}

int ShaderManager::GetProgramId(
	ShaderProgramType programType
	)
{
	Program& program = GetProgram(programType);
	if(program.m_programId == -1)
	{
		wxLogDebug("Program type %d not found, check the shader compilation.\n", programType);
	}
	return program.m_programId;
}

void ShaderManager::SetProgramCurrent(
	ShaderProgramType programType
	)
{
	int programId = GetProgramId(programType);

	glUseProgram(programId);

	m_currentProgramType = programType;

	//OutputDebugShaderAttributeInfo(programType);
}

/**
	Takes a map of all available shaders and the type of that shader.
	Stores a map of all loaded shaders and the openGL id assigned to that shader

*/
bool ShaderManager::InitialiseShaders()
{
	// Setup the phong shaders
	InitialiseShaders(Phong);
	LinkPrograms(Phong);

	// Setup the lambert shaders
	InitialiseShaders(Lambert);
	LinkPrograms(Lambert);

	// Setup the overlay shaders
	InitialiseShaders(Overlay);
	LinkPrograms(Overlay);

	return true;
}

bool ShaderManager::InitialiseShaders(
	ShaderProgramType programType
	)
{
	Program& program = GetProgram(programType);

	for(std::map<GLuint, std::string>::const_iterator shaderIterator = program.m_pathByShaderType.begin(); shaderIterator != program.m_pathByShaderType.end(); shaderIterator++)
	{
		// Pass the name and type to the shader loading
		GLuint shaderId = LoadShader(shaderIterator->second, shaderIterator->first);

		// Keep a record of the gl id for this shader

		program.m_idByShaderType.insert(std::pair<GLuint, GLuint>(shaderIterator->first, shaderId));
		// Compile the shader of this name and id
		if(!CompileShader(shaderIterator->second, shaderId))
		{
			return false;
		}
	}

	return true;
}

/**
	@brief Loads a shader of the specified name and type
	@return GL_FALSE if failed or GL_TRUE if successfully loaded
*/
GLuint ShaderManager::LoadShader(
	const std::string &shaderName, // The name of the shader to load
	const GLenum shaderType	// The type of the shader ie GL_VERTEX_SHADER etc
	)
{
	const char *shaderTextBuffer = ReadShaderSourceFile(shaderName);
	if(!shaderTextBuffer)
	{
		wxLogDebug("Error reading shader.\n");
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
		wxLogDebug("Error creating shader.\n");
	}

	delete[] shaderTextBuffer;

	return shaderId;
}

const char* ShaderManager::ReadShaderSourceFile(
	const std::string &shaderName
	)
{
	std::ifstream shaderStream(shaderName, std::ifstream::in);
	if(!shaderStream) {
		wxLogDebug("Error opening file: %s\n", shaderName.c_str());
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
GLenum ShaderManager::CompileShader(
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
		wxLogDebug("Shader compilation failed for shader %s\n", shaderName.c_str());

		GLint errorTextLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorTextLength);

		std::string errorText(errorTextLength, ' ');

		GLsizei written;
		glGetShaderInfoLog(shaderId, errorTextLength, &written, &errorText[0]);

		wxLogDebug("Shader compilation errors: \n%s", errorText.c_str());
	}

	return compiled;
}

GLenum ShaderManager::LinkPrograms(
	const ShaderProgramType programType // The type of this program ie Phong etc
	)
{
		
	Program& program = GetProgram(programType);

	// Create the program object
	int programId = glCreateProgram();
	if(programId == -1)
	{
		wxLogDebug("Error creating program object.\n");
		return false;
	}

	// Attach the shaders to the program object
	for(std::map<GLuint, GLuint>::const_iterator programIterator = program.m_idByShaderType.begin(); programIterator != program.m_idByShaderType.end(); programIterator++)
	{
		glAttachShader(programId, programIterator->second);
	}

	// Link the program
	glLinkProgram(programId);

	// Check for successful linking
	GLint success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if(!success)
	{
		wxLogDebug("Failed to link shader program\n" );

		GLint errorTextLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &errorTextLength);

		std::vector<char> errorText(errorTextLength);

		glGetShaderInfoLog(programId, errorTextLength, &errorTextLength, &errorText[0]);

		wxLogDebug("Shader linking errors are: \n%s", &errorText[0]);

		glDeleteShader(programId);

		return false;
	}

	program.m_programId = programId;

	return true;
}

void ShaderManager::OutputDebugShaderAttributeInfo(
	const ShaderProgramType programType
	)
{
	Program& program = GetProgram(programType);
	if(program.m_programId == -1)
	{
		wxLogDebug("Shader program not initialised\n");
		return;
	}

	GLint textlength, noofAttributes;
	glGetProgramiv(program.m_programId, GL_ACTIVE_ATTRIBUTES, &noofAttributes);
	glGetProgramiv(program.m_programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &textlength);
 
	std::string attributeName(textlength, ' ');

	GLint written, size, location;
	GLenum type;
	wxLogDebug("Index | Name\n");
	wxLogDebug("---------------------------------");
	for(int i = 0; i < noofAttributes; i++)
	{
		glGetActiveAttrib(program.m_programId, i, textlength, &written, &size, &type, &attributeName[0]);
 
		location = glGetAttribLocation(program.m_programId, &attributeName[0]);
		wxLogDebug("Location %d name %s\n", location, attributeName.c_str());
	}
 
	GLint noofUniforms;
	glGetProgramiv(program.m_programId, GL_ACTIVE_UNIFORMS, &noofUniforms);
	glGetProgramiv(program.m_programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &textlength);
 
	std::string uniformName(textlength, ' ');
 
	wxLogDebug("Location | Name\n");
	wxLogDebug("---------------------------------");
	for(int i = 0; i < noofUniforms; i++)
	{
		glGetActiveUniform(program.m_programId, i, textlength, &written, &size, &type, &uniformName[0]);
 
		location = glGetUniformLocation(program.m_programId, &uniformName[0]);
		wxLogDebug("Location %d, uniformName %s\n", location, uniformName.c_str());
	}
}

ShaderManager::~ShaderManager()
{
	for(std::map<ShaderProgramType, Program>::const_iterator programIterator = m_shaderProgramByProgramType.begin(); programIterator != m_shaderProgramByProgramType.end(); programIterator++)
	{
		// Detach and delete any shaders
		Program program = programIterator->second;
		for(std::map<GLuint, GLuint>::const_iterator shaderIterator = program.m_idByShaderType.begin(); shaderIterator != program.m_idByShaderType.end(); shaderIterator++)
		{
			GLuint shaderId = shaderIterator->second;
			glDetachShader(program.m_programId, shaderId);
			glDeleteShader(shaderId);
		}

		// Delete the shader program.
		if(program.m_programId != -1)
		{
			glDeleteProgram(program.m_programId);
		}
	}
}
}
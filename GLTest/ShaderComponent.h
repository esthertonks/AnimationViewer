#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace Render
{

struct Vert{
    glm::vec3 Position;
    glm::vec3 Colour;
};

class ShaderComponent
{

public:

	ShaderComponent();
	~ShaderComponent();

	inline GLuint GetProgramHandle()
	{
		return m_programHandle;
	}

	inline GLuint GetVertexArrayHandle()
	{
		return m_vertexArrayHandle;
	}

	bool Loaded()
	{
		return m_loaded;
	}

	bool LoadShaders();

private:
	GLuint LoadShader(
		const std::string &shaderName, // The name of the shader to load
		const GLenum shaderType	// The type of the shader ie GL_VERTEX_SHADER etc
	);

	const char* ReadShaderSourceFile(
		const std::string &shaderName
	);

	GLenum CompileShader(
		const std::string &shaderName, // The name of the shader to load
		const GLuint shaderId //GLuint id for the shader source
	);

	GLenum LinkProgram(
		const std::vector<GLuint> &shaders
		);

	void OutputDebugShaderAttributeInfo();

	void CreateVertexBuffers();

	GLint m_programHandle;
    GLuint m_vertexArrayHandle;
    static GLubyte m_indices[];
    static Vert verts[];

	GLuint m_indexBufferHandle;
	GLuint m_positionBufferHandle;

	std::vector<GLuint> shaders;

	bool m_loaded;
};
}

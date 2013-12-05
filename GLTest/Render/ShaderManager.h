#pragma once

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "RenderFwdDecl.h"

namespace render
{
	struct Program
	{
		int m_programId;
		std::map<GLuint, std::string> m_pathByShaderType; // The type of shader ie vertex, fragment, tessellation and the path to load the physical shader
		std::map<GLuint, GLuint> m_idByShaderType; // The type of this shader ie vertex, fragment, tessellation and the allocated gl id for this shader
	};

class ShaderManager
{
public:

	ShaderManager();

	~ShaderManager();

	void InitGL();

	inline const GLuint GetCurrentProgramId()
	{
		return m_shaderProgramByProgramType.find(m_currentProgramType)->second.m_programId;
	}

	bool InitialiseShaders();

	int ShaderManager::GetProgramId(
		ShaderProgramType programType
	);

	Program& ShaderManager::GetProgram(
		ShaderProgramType programType
	);

	void SetProgramCurrent(
		ShaderProgramType programType
		);

	ShaderProgramType GetCurrentProgramType()
	{
		return m_currentProgramType;
	}

private:
	
	bool ShaderManager::InitialiseShaders(
		ShaderProgramType programType
		);

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

	GLenum LinkPrograms(
		const ShaderProgramType programType // The type of this program ie Phong etc
		);

	void OutputDebugShaderAttributeInfo(
		const ShaderProgramType programType
		);

	ShaderProgramType m_currentProgramType;

	std::map<ShaderProgramType, Program> m_shaderProgramByProgramType;

	//std::map<std::string, GLuint> &m_phongShaders;
	//std::map<std::string, GLuint> &m_lambertShaders;
	//std::map<std::string, GLuint> m_shaderPathToIds;
	//std::map<ShaderType, GLuint> m_shaderTypeToProgramId;
};

}


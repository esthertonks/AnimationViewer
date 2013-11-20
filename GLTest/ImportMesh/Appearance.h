#pragma once

#include <vector>
#include <glm\glm.hpp>

namespace render
{

class Appearance
{

public:

	Appearance();
	~Appearance(){};

	inline void SetShaderPath(
	std::string &shaderPath
	)
	{
		m_shaderPath = shaderPath;
	}

	inline std::string &GetShaderPath()
	{
		return m_shaderPath;
	}

void AddShaderInput(
	glm::vec3 &shaderInput
	);

private:
	// The shader associated with this appearance
	std::string m_shaderPath;

	// opengl states

	// Shader Inputs
	std::vector<glm::vec3> m_shaderInputs;

};
}
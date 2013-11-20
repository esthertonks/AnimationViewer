#pragma once

#include <vector>
#include <string>
#include <glm\glm.hpp>

namespace render
{

class Appearance abstract
{

public:

	Appearance(){};
	virtual ~Appearance(){};

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

void AddTexture(
	std::string &texturePath
	)
{
	m_texturePaths.push_back(texturePath);
};

private:
	// The shader associated with this appearance
	std::string m_shaderPath;

	// opengl states

	std::vector<std::string>m_texturePaths;

};
}
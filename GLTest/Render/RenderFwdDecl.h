#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <fbxsdk.h> //TODO global header file?

namespace render
{

enum ShaderProgramType
{
	Phong,
	Lambert,
	Overlay,
	None
};

}
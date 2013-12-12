#pragma once

#include <glm\glm.hpp>
#include <fbxsdk.h> //TODO global header file?

namespace utils
{

class MathsUtils
{
public:

static void TranslateRotateScale(
	glm::vec3 &position, 
	glm::mat4x4 &rotation,
	glm::vec3 &scale,
	glm::mat4x4 &result
	);

static void Decompose();

static void ConvertFBXToGLMatrix(
	const FbxAMatrix &fbxMatrix,
	glm::mat4x4 &glmMatrix
	);
};

}

#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fbxsdk.h> //TODO global header file?

namespace utils
{

class MathsUtils
{
public:

static void TranslateRotateScale(
	const glm::vec3 &position, 
	const glm::mat4x4 &rotation,
	const glm::vec3 &scale,
	glm::mat4x4 &result
	);

static void Decompose();

static void ConvertFBXToGLMatrix(
	const FbxAMatrix &fbxMatrix,
	glm::mat4x4 &glmMatrix
	);

static int Clamp(
	int value,
	int minValue,
	int maxValue
	)
{
	if(value < minValue)
	{
		value = minValue;
	}
	else if(value > maxValue)
	{
		value = maxValue;
	}

	return value;
}

static float Clamp(
	float value,
	float minValue,
	float maxValue
	)
{
	if(value < minValue)
	{
		value = minValue;
	}
	else if(value > maxValue)
	{
		value = maxValue;
	}

	return value;
}

};

}

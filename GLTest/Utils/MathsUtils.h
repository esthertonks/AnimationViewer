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

static void ConvertGLToFBXMatrix(
	const glm::mat4x4 &glmMatrix, // Matrix to convert
	FbxAMatrix &fbxMatrix // Converted matrix
	);

/*
	Takes the current time and the time at the keyframe before and after this time.
	Returns a normalised time value between 0.0 and 1.0 where 0.0 is the time at the last
	keyframe and 1.0 is the time at the next keyframe.
*/
static float NormalizeValue(
	const long currentValue, // The current value
	const long lastValue, // Start value
	const long nextValue	 // End Value
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

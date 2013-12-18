#include "MathsUtils.h"

namespace utils
{
	// GL column matrices.

	// 00 10 20 30
	// 01 11 21 31
	// 02 12 22 32
	// 03 13 23 33

	//matrix[3][0] = pos.x; 
	//matrix[3][1] = pos.y;
	//matrix[3][2] = pos.z;

/**
	@return A matrix combined matrix containing the result of translating, rotation and then scaling
	M = T * R * S
*/
void MathsUtils::TranslateRotateScale(
	const glm::vec3 &position, 
	const glm::mat4x4 &rotation,
	const glm::vec3 &scale,
	glm::mat4x4 &result
	)
{
	result[3][0] = position.x;
	result[3][1] = position.y;
	result[3][2] = position.z;

	result[0][0] = rotation[0][0] * scale.x;
	result[0][1] = rotation[0][1] * scale.x;
	result[0][2] = rotation[0][2] * scale.x;

	result[1][0] = rotation[1][0] * scale.y;
	result[1][1] = rotation[1][1] * scale.y;
	result[1][2] = rotation[1][2] * scale.y;

	result[2][0] = rotation[2][0] * scale.z;
	result[2][1] = rotation[2][1] * scale.z;
	result[2][2] = rotation[2][2] * scale.z;
}

void MathsUtils::Decompose()
{
	assert("Not implemented");
}

void MathsUtils::ConvertFBXToGLMatrix(
	const FbxAMatrix &fbxMatrix,
	glm::mat4x4 &glmMatrix
	)
{
	glmMatrix[0][0] = static_cast<float>(fbxMatrix[0][0]);
	glmMatrix[0][1] = static_cast<float>(fbxMatrix[0][1]);
	glmMatrix[0][2] = static_cast<float>(fbxMatrix[0][2]);
	glmMatrix[0][3] = static_cast<float>(fbxMatrix[0][3]);

	glmMatrix[1][0] = static_cast<float>(fbxMatrix[1][0]);
	glmMatrix[1][1] = static_cast<float>(fbxMatrix[1][1]);
	glmMatrix[1][2] = static_cast<float>(fbxMatrix[1][2]);
	glmMatrix[1][3] = static_cast<float>(fbxMatrix[1][3]);

	glmMatrix[2][0] = static_cast<float>(fbxMatrix[2][0]);
	glmMatrix[2][1] = static_cast<float>(fbxMatrix[2][1]);
	glmMatrix[2][2] = static_cast<float>(fbxMatrix[2][2]);
	glmMatrix[2][3] = static_cast<float>(fbxMatrix[2][3]);

	glmMatrix[3][0] = static_cast<float>(fbxMatrix[3][0]);
	glmMatrix[3][1] = static_cast<float>(fbxMatrix[3][1]);
	glmMatrix[3][2] = static_cast<float>(fbxMatrix[3][2]);
	glmMatrix[3][3] = static_cast<float>(fbxMatrix[3][3]);

}

}
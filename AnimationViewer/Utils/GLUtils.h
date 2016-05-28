#pragma once

#include "RenderFwdDecl.h"

namespace render
{

class GLUtils
{
public:

static bool CheckOpenGLError(
	const char * file, 
	int line
	);

static void DebugPrintGLInfo();

static void ConvertFBXToGLMatrix(
	const FbxAMatrix &fbxMatrix,
	glm::mat4x4 &glmMatrix
	);

};

}


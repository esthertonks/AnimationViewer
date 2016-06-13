#pragma once

#include "RenderFwdDecl.h"

namespace utils
{

class GLUtils
{
public:

	static bool CheckOpenGLError(
		const char * file, 
		int line
		);

	static void DebugPrintGLInfo();

	};

}


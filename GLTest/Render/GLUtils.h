#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

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

};

}


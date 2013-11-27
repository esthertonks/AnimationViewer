#include "GLUtils.h"
#include <wx/log.h>

namespace render
{

/**
	@return True if an error has occured, false otherwise
*/
bool GLUtils::CheckOpenGLError(
	const char * file, 
	int line
	)
{
	GLenum glErr = glGetError();
	if(glErr != GL_NO_ERROR)
	{
		wxLogDebug("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		glErr = glGetError();
		return true;
	}
	return false;
}

void GLUtils::DebugPrintGLInfo()
{
	const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* glslVersion = reinterpret_cast<const char*>(glGetString( GL_SHADING_LANGUAGE_VERSION));

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	wxLogDebug("Renderer info: %s version %s from %s", renderer, version, vendor);
	wxLogDebug("GL version major: %s minor: %s", major, minor);

	wxLogDebug("GLSL version: %s", glslVersion);

	GLint numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	for(int extension = 0; extension < numExtensions; extension++)
	{
		wxLogDebug("Extension: %s\n", glGetStringi(GL_EXTENSIONS, extension));
	}
}

}
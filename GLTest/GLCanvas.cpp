#include "GLCanvas.h"
#include "ShaderComponent.h"

#include <wx/log.h>

#include <glm/gtc/matrix_transform.hpp>

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_PAINT(GLCanvas::Render)
	EVT_SIZE(GLCanvas::OnSize)
	EVT_SET_FOCUS(GLCanvas::OnSetFocus)
END_EVENT_TABLE()

GLCanvas::GLCanvas(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint& position,
	const wxSize& size,
	long style, 
	const wxString& name
	)
	: wxGLCanvas(parent, id, position, size, style|wxFULL_REPAINT_ON_RESIZE, name), 
	m_context(NULL),
	m_shaderComponent(NULL)
{
}

void GLCanvas::InitGL()
{
	if(m_context || m_shaderComponent)
	{
		wxLogDebug("GL already initialised");
		return;
	}

	m_shaderComponent = new ShaderComponent();

	m_context = new wxGLContext(this);

	SetCurrent(*m_context);

	//TODO calling this here causes a GL expection - why???
	//DebugPrintGLInfo();

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if(status != GLEW_OK) //TODO - quit here and it leaks like a sieve.
	{
		wxLogDebug("Error initializing GLEW: ");
		char *errorText = (char*)glewGetErrorString(status);
		wxLogDebug(errorText);
		return;
	}

	CheckOpenGLError(__FILE__,__LINE__);

	m_shaderComponent->LoadShaders();

	glClearColor(0.2f,0.2f,0.2f,1.0f);

	DebugPrintGLInfo();
}

void GLCanvas::OnSize(
	wxSizeEvent& event
	)
{
    // Necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // Reset the OpenGL view aspect
    //ResetProjectionMode();
}

void GLCanvas::OnSetFocus(
	wxFocusEvent& WXUNUSED(event)
	)
{
}

int GLCanvas::CheckOpenGLError(const char * file, int line) {
	//
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	//
	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		wxLogDebug("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void GLCanvas::DebugPrintGLInfo()
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

void GLCanvas::Render(
	wxPaintEvent& WXUNUSED(event)
	)
{
	wxPaintDC(this);

	// TODO wxwidgets has not init callback so this has to be done here. bleugh.
	if(!m_context)
	{
		//wxLogDebug("Error - no gl canvas available for rendering");
		return;
	}

	if(!m_shaderComponent || !m_shaderComponent->Loaded())
	{
		//wxLogDebug("Error - no shaders loaded");
		return;
	}

	CheckOpenGLError(__FILE__,__LINE__);

	SetCurrent(*m_context);

	glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);

	float angle = 30.0f;
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	GLint location = glGetUniformLocation(m_shaderComponent->GetProgramHandle(), "RotationMatrix");
	if(location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
	

		glBindVertexArray(m_shaderComponent->GetVertexArrayHandle());
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
	}

	glFlush();
	SwapBuffers();
}

GLCanvas::~GLCanvas()
{
	delete m_context;
	m_glContext = NULL;

	delete m_shaderComponent;
	m_shaderComponent = NULL;
}

#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../Batch/BatchFwdDecl.h"

namespace render
{
	class Batch;
	class OrbitCamera;

class GLRenderer : public wxGLCanvas
{
public:

	GLRenderer(
		wxWindow *parent, 
		wxWindowID id, 
		const wxPoint& position,
		const wxSize& size, 
		long style, 
		const wxString& name
	);

	~GLRenderer();

	OrbitCamera& GetCamera()
	{
		return *m_camera;
	}

	void Paint(
		wxPaintEvent& event
		);

	void RenderImmediate(
		);

	void OnSize(
		wxSizeEvent& event
		);

	void OnEraseBackground(
		wxEraseEvent& event
		);

	void OnSetFocus(
		wxFocusEvent& event
		);

	void OnKeyDown(
		wxKeyEvent& event
		);

	void OnLeftDown(
		wxMouseEvent& event
		);

	void OnRightDown(
		wxMouseEvent& event
		);

	void OnMouseMove(
		wxMouseEvent& event
		);

	void OnMouseWheelScroll(
		wxMouseEvent& event
		);

	void OnMouseWheelDown(
		wxMouseEvent& event
		);

	void InitGL();

	int CheckOpenGLError(
		const char * file, 
		int line
		);

	void DebugPrintGLInfo();

	inline GLuint GetProgramHandle()
	{
		return m_programHandle;
	}

	bool Initialised()
	{
		return m_initialised;
	}

	bool LoadShaders(
		const std::map<std::string, GLuint> &defaultShaderList
		);

	void SetBatches(
		const render::BatchList *batches
		)
	{
		m_batches = batches;
	}

protected:
	DECLARE_EVENT_TABLE()

private:
	void Render();
		
	GLuint LoadShader(
		const std::string &shaderName, // The name of the shader to load
		const GLenum shaderType	// The type of the shader ie GL_VERTEX_SHADER etc
	);

	const char* ReadShaderSourceFile(
		const std::string &shaderName
	);

	GLenum CompileShader(
		const std::string &shaderName, // The name of the shader to load
		const GLuint shaderId //GLuint id for the shader source
	);

	GLenum LinkProgram(
		const std::vector<GLuint> &shaders
		);

	void OutputDebugShaderAttributeInfo();

	wxGLContext* m_context;

	GLint m_programHandle;

	std::vector<GLuint> shaders;

	bool m_initialised;

	OrbitCamera *m_camera;

	const render::BatchList *m_batches;
};

}


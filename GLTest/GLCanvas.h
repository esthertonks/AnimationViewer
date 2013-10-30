#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

class ShaderComponent;

class GLCanvas : public wxGLCanvas
{
public:

	GLCanvas(
		wxWindow *parent, 
		wxWindowID id, 
		const wxPoint& position,
		const wxSize& size, 
		long style, 
		const wxString& name
	);

	~GLCanvas();

	void Render(
		wxPaintEvent& event
		);

	void OnSize(
		wxSizeEvent& event
		);

	void OnSetFocus(
		wxFocusEvent& event
		);

	void InitGL();

	void LinkShaders(
		GLint vertShader, 
		GLint fragShader
		);

	int CheckOpenGLError(
		const char * file, 
		int line
		);

	void DebugPrintGLInfo();

protected:
	DECLARE_EVENT_TABLE()

private:
	wxGLContext* m_context;
	ShaderComponent *m_shaderComponent;
};


#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

namespace mesh
{
	class RenderMeshNode;
}

namespace render
{
	class Renderer;

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

	void GLCanvas::SetMeshNode(
		mesh::RenderMeshNode &renderMeshNode
		);

	void Paint(
		wxPaintEvent& event
		);

	void RenderImmediate(
		);

	void OnSize(
		wxSizeEvent& event
		);

	void OnSetFocus(
		wxFocusEvent& event
		);

	void InitGL();

	int CheckOpenGLError(
		const char * file, 
		int line
		);

	void DebugPrintGLInfo();

protected:
	DECLARE_EVENT_TABLE()

private:
	void Render();

	wxGLContext* m_context;
	Renderer *m_renderer;
};

}


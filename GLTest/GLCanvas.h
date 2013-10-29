#pragma once

#define GLEW_STATIC 1

#include <GL/glew.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>

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

protected:
	DECLARE_EVENT_TABLE()

private:
	wxGLContext* m_context;
};


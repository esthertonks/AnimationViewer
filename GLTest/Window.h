#pragma once

#include "wx/wx.h"
#include <wx/log.h>
#include "GLCanvas.h"

class Window : public wxFrame
{

public:
	Window(
		wxFrame *frame, 
		const wxString& title, 
		const wxPoint& pos,
		const wxSize& size, 
		long style
		);

    void OnQuit(
		wxCommandEvent& event
		);

    void OnAbout(
		wxCommandEvent& event
		);

	void OnShow(
		wxShowEvent& event
		);
 
	DECLARE_EVENT_TABLE()

private:
	GLCanvas *m_canvas;

};


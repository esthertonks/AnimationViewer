#pragma once

#include "wx/wx.h"
#include <wx/log.h>

namespace Render
{
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

	void OnOpen(
		wxCommandEvent& event
		);

    void OnAbout(
		wxCommandEvent& event
		);

	void OnShow(
		wxShowEvent& event
		);

	void OnClose(
		wxCloseEvent& event
		);
 
	DECLARE_EVENT_TABLE()

};
}


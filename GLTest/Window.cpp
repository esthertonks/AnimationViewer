#include "Window.h"

namespace Render
{

BEGIN_EVENT_TABLE(Window, wxFrame)
	EVT_SHOW(Window::OnShow)
	EVT_MENU(wxID_EXIT, Window::OnQuit)
	EVT_MENU(wxID_ABOUT, Window::OnAbout)
END_EVENT_TABLE()

Window::Window(
	wxFrame *frame, 
	const wxString& title, 
	const wxPoint& pos,
    const wxSize& size, 
	long style
	)
	: wxFrame(frame, wxID_ANY, title, pos, size, style)
{
    // Make the "File" menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, wxT("&Open..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("&Exit\tALT-X"));
    // Make the "Help" menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About..."));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));
    SetMenuBar(menuBar);

	//CreateStatusBar();
	//SetStatusText( "Welcome to wxWindows!" );

	m_canvas = new GLCanvas(this, wxID_ANY, wxDefaultPosition,
        wxSize(300, 300), wxSUNKEN_BORDER, "Animation App");
}
 
void Window::OnShow(
	wxShowEvent& WXUNUSED(event)
	)
{
	m_canvas->InitGL();
	wxLogDebug("Frame shown");
}

void Window::OnQuit(
	wxCommandEvent& WXUNUSED(event)
	)
{
	Close(TRUE);
}
 
void Window::OnAbout(
	wxCommandEvent& WXUNUSED(event)
	)
{

	wxMessageBox("This is a wxWindows Hello world sample",
		"About Hello World", wxOK | wxICON_INFORMATION, this);
}
}
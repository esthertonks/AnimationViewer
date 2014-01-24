#include "Window.h"
#include "../AnimationApp.h"
#include "../Render/GLRenderCanvas.h"

namespace gui
{
	enum
	{
		OpenFBX = wxID_HIGHEST + 400,
		CloseFBX
	};

BEGIN_EVENT_TABLE(Window, wxFrame)
	EVT_SHOW(Window::OnShow)
	EVT_CLOSE(Window::OnClose)
	EVT_MENU(OpenFBX, Window::OnOpenFBX)
	EVT_MENU(CloseFBX, Window::OnCloseFBX)
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
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(OpenFBX, wxT("&Open FBX"));
	fileMenu->Append(CloseFBX, wxT("&Close FBX"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("&Exit"));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, wxT("&About..."));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));
	SetMenuBar(menuBar);
}
 
void Window::OnShow(
	wxShowEvent& WXUNUSED(event)
	)
{
	wxGetApp().GetRenderer().InitGL();
	wxLogDebug("Frame shown");
}

void Window::OnClose(
	wxCloseEvent& event
	)
	{
		wxGetApp().StopRendering();
		wxGetApp().Destroy();
		event.Skip(); // Continue closing the window
	}

// Menu events:
void Window::OnOpenFBX(
	wxCommandEvent& WXUNUSED(event)
	)
{
wxFileDialog* openDialog = new wxFileDialog(
		this, _("Choose an FBX file to open"), wxEmptyString, wxEmptyString, 
		_("FBX files (*.fbx)|*.fbx"),
		wxFD_OPEN, wxDefaultPosition);
 
	if (openDialog->ShowModal() == wxID_OK)
	{
		wxString fbxFilePath = openDialog->GetPath();
		wxGetApp().ImportFBX(std::string(fbxFilePath.c_str()));
	}
 
	openDialog->Destroy();
}

void Window::OnCloseFBX(
	wxCommandEvent& event
	)
{
	wxGetApp().CloseFBX();
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

	wxMessageBox("Animation Viewer By Esther Tonks",
		"Animation Viewer", wxOK | wxICON_INFORMATION, this);
}

}
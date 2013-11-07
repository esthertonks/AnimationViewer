#include "Window.h"
#include "../AnimationApp.h"
#include "GLCanvas.h"

namespace render
{

BEGIN_EVENT_TABLE(Window, wxFrame)
	EVT_SHOW(Window::OnShow)
	EVT_CLOSE(Window::OnClose)
	EVT_MENU(wxID_OPEN, Window::OnOpen)
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
	fileMenu->Append(wxID_OPEN, wxT("&Open FBX"));
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
}
 
void Window::OnShow(
	wxShowEvent& WXUNUSED(event)
	)
{
	wxGetApp().GetCanvas().InitGL();
	wxLogDebug("Frame shown");
}

void Window::OnClose(
	wxCloseEvent& event
	)
	{
		wxGetApp().StopRendering();
		wxGetApp().DeleteMesh();
		event.Skip(); // Continue closing the window
	}

// Menu events:
void Window::OnOpen(
	wxCommandEvent& event
	)
{
wxFileDialog* openDialog = new wxFileDialog(
		this, _("Choose an FBX file to open"), wxEmptyString, wxEmptyString, 
		_("FBX files (*.fbx)|*.fbx"),
		wxFD_OPEN, wxDefaultPosition);
 
	// Creates a "open file" dialog with 4 file types
	if (openDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		wxString fbxFilePath = openDialog->GetPath();
		// Sets our current document to the file the user selected
		wxGetApp().ImportFBX(fbxFilePath.c_str());
	}
 
	// Clean up after ourselves
	openDialog->Destroy();
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
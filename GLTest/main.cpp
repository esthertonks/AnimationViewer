#include "wx/wx.h" 

#include "Window.h"
 
class MyApp: public wxApp
{
    virtual bool OnInit();
};
 
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxFrame *frame = new Window(NULL, wxT("Testing"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);

	frame->Show(TRUE);

	SetTopWindow(frame);
	return TRUE;
}

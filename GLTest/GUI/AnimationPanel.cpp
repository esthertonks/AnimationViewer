#include "AnimationPanel.h"
#include "../AnimationApp.h"
#include "wx\statline.h"
#include "wx\stattext.h"

const int ID_PLAY_ANIMATION_BUTTON = wxID_HIGHEST + 1;
const int ID_PAUSE_ANIMATION_BUTTON = wxID_HIGHEST + 2;
const int ID_STOP_ANIMATION_BUTTON = wxID_HIGHEST + 3;

namespace gui
{

BEGIN_EVENT_TABLE(AnimationPanel, wxPanel)
	EVT_BUTTON(ID_PLAY_ANIMATION_BUTTON, AnimationPanel::OnPlayClicked)
	EVT_BUTTON(ID_PAUSE_ANIMATION_BUTTON, AnimationPanel::OnPauseClicked)
	EVT_BUTTON(ID_STOP_ANIMATION_BUTTON, AnimationPanel::OnStopClicked)
END_EVENT_TABLE()

AnimationPanel::AnimationPanel(
	wxWindow *parent
	)
	: wxPanel(parent)
{	
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *panelTitleText = new wxStaticText(this, wxID_ANY, wxT("ANIMATION:"));
	panelTitleText->SetForegroundColour(*wxWHITE);
	sizer->Add(panelTitleText, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *playAnimButton = new wxButton(this, ID_PLAY_ANIMATION_BUTTON, _T("Play Animation"));
	playAnimButton->SetMinSize(wxSize(100, 20));
	sizer->Add(playAnimButton, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *pauseAnimButton = new wxButton(this, ID_PAUSE_ANIMATION_BUTTON, _T("Pause Animation"));
	pauseAnimButton->SetMinSize(wxSize(100, 20));
	sizer->Add(pauseAnimButton, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *stopAnimButton = new wxButton(this, ID_STOP_ANIMATION_BUTTON, _T("Stop Animation"));
	stopAnimButton->SetMinSize(wxSize(100, 20));
	sizer->Add(stopAnimButton, 0, wxALIGN_LEFT | wxALL, 10);

	SetSizer(sizer);
}

void AnimationPanel::OnPlayClicked(
	wxCommandEvent& event
	)
{
	wxGetApp().PlayAnimation();
}

void AnimationPanel::OnPauseClicked(
	wxCommandEvent& event
	)
{
	wxGetApp().PauseAnimation();
}

void AnimationPanel::OnStopClicked(
	wxCommandEvent& event
	)
{
	wxGetApp().StopAnimation();
}

}


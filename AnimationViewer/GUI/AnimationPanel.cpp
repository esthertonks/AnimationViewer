#include "AnimationPanel.h"
#include "../AnimationApp.h"
#include "wx\statline.h"
#include "wx\stattext.h"

namespace gui
{
	enum
{
	PlayAnimationButton = wxID_HIGHEST + 100,
	PauseAnimationButton,
	StopAnimationButton,
	LoopCheckbox
};

BEGIN_EVENT_TABLE(AnimationPanel, wxPanel)
	EVT_BUTTON(PlayAnimationButton, AnimationPanel::OnPlayClicked)
	EVT_BUTTON(PauseAnimationButton, AnimationPanel::OnPauseClicked)
	EVT_BUTTON(StopAnimationButton, AnimationPanel::OnStopClicked)
	EVT_CHECKBOX(LoopCheckbox, AnimationPanel::OnLoopAnimation)
END_EVENT_TABLE()

AnimationPanel::AnimationPanel(
	wxWindow *parent
	)
	: wxPanel(parent)
{	
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticLine *separator = new wxStaticLine(this);
	separator->SetForegroundColour(*wxWHITE);
	separator->SetMinSize(wxSize(450, 3));
	sizer->Add(separator, 0, wxALIGN_CENTER | wxALL);

	wxStaticText *panelTitleText = new wxStaticText(this, wxID_ANY, wxT("Animation:"));
	panelTitleText->SetForegroundColour(AnimationApp::m_guiTextColour);
	sizer->Add(panelTitleText, 0, wxALIGN_LEFT | wxALL, 10);

	wxCheckBox *loopCheckBox = new wxCheckBox(this, LoopCheckbox, wxT("Loop Animation"));
	loopCheckBox->SetForegroundColour(AnimationApp::m_guiTextColour);
	loopCheckBox->SetValue(true);
	loopCheckBox->SetToolTip(wxT("Set the animation to loop when playing."));
	sizer->Add(loopCheckBox, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *playAnimButton = new wxButton(this, PlayAnimationButton, _T("Play"));
	playAnimButton->SetMinSize(wxSize(100, 20));
	playAnimButton->SetToolTip(wxT("Play the animation."));
	sizer->Add(playAnimButton, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *pauseAnimButton = new wxButton(this, PauseAnimationButton, _T("Pause"));
	pauseAnimButton->SetMinSize(wxSize(100, 20));
	pauseAnimButton->SetToolTip(wxT("Pause the animation."));
	sizer->Add(pauseAnimButton, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *stopAnimButton = new wxButton(this, StopAnimationButton, _T("Stop"));
	stopAnimButton->SetMinSize(wxSize(100, 20));
	stopAnimButton->SetToolTip(wxT("Stop the animation."));
	sizer->Add(stopAnimButton, 0, wxALIGN_LEFT | wxALL, 10);

	SetMinSize(wxSize(450, 220));
	SetMaxSize(wxSize(600, 220));

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

void AnimationPanel::OnLoopAnimation(
	wxCommandEvent& event
	)
{
	wxGetApp().LoopAnimation(event.IsChecked());
}

}


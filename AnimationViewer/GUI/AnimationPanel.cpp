#include "AnimationPanel.h"
#include "../AnimationApp.h"
#include "wx\statline.h"
#include "wx\stattext.h"
#include "../Utils/WXUtils.h"
#include "../Colours.h"

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
	wxWindow *parent,
	wxSize minSize,
	wxSize maxSize
	)
	: wxPanel(parent)
{	
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *panelTitleText = new wxStaticText(this, wxID_ANY, wxT("ANIMATION"));
	utils::WXUtils::SetFont(panelTitleText, 10, wxFONTWEIGHT_BOLD);
	panelTitleText->SetForegroundColour(wxColor(theme::Colours::m_lightGrey.r, theme::Colours::m_lightGrey.g, theme::Colours::m_lightGrey.b));
	sizer->Add(panelTitleText, 0, wxALIGN_CENTER | wxALL, 20);

	wxButton *playAnimButton = new wxButton(this, PlayAnimationButton, _T("PLAY"));
	playAnimButton->SetMinSize(wxSize(100, 30));
	playAnimButton->SetToolTip(wxT("Play the animation."));
	utils::WXUtils::SetFont(playAnimButton, 8, wxFONTWEIGHT_BOLD);
	playAnimButton->SetForegroundColour(wxColor(theme::Colours::m_lightGrey.r, theme::Colours::m_lightGrey.g, theme::Colours::m_lightGrey.b));
	playAnimButton->SetBackgroundColour(wxColor(theme::Colours::m_darkGrey.r, theme::Colours::m_darkGrey.g, theme::Colours::m_darkGrey.b));
	sizer->Add(playAnimButton, 0, wxALIGN_CENTER | wxALL, 8);

	wxButton *pauseAnimButton = new wxButton(this, PauseAnimationButton, _T("PAUSE"));
	pauseAnimButton->SetMinSize(wxSize(100, 30));
	pauseAnimButton->SetToolTip(wxT("Pause the animation."));
	utils::WXUtils::SetFont(pauseAnimButton, 8, wxFONTWEIGHT_BOLD);
	pauseAnimButton->SetForegroundColour(wxColor(theme::Colours::m_lightGrey.r, theme::Colours::m_lightGrey.g, theme::Colours::m_lightGrey.b));
	pauseAnimButton->SetBackgroundColour(wxColor(theme::Colours::m_darkGrey.r, theme::Colours::m_darkGrey.g, theme::Colours::m_darkGrey.b));
	sizer->Add(pauseAnimButton, 0, wxALIGN_CENTER | wxALL, 8);

	wxButton *stopAnimButton = new wxButton(this, StopAnimationButton, _T("STOP"));
	stopAnimButton->SetMinSize(wxSize(100, 30));
	stopAnimButton->SetToolTip(wxT("Stop the animation."));
	utils::WXUtils::SetFont(stopAnimButton, 8, wxFONTWEIGHT_BOLD);
	stopAnimButton->SetForegroundColour(wxColor(theme::Colours::m_lightGrey.r, theme::Colours::m_lightGrey.g, theme::Colours::m_lightGrey.b));
	stopAnimButton->SetBackgroundColour(wxColor(theme::Colours::m_darkGrey.r, theme::Colours::m_darkGrey.g, theme::Colours::m_darkGrey.b));
	sizer->Add(stopAnimButton, 0, wxALIGN_CENTER | wxALL, 8);

	wxCheckBox *loopCheckBox = new wxCheckBox(this, LoopCheckbox, wxT("Loop Animation"));
	loopCheckBox->SetForegroundColour(wxColor(theme::Colours::m_lightGrey.r, theme::Colours::m_lightGrey.g, theme::Colours::m_lightGrey.b));
	loopCheckBox->SetValue(true);
	loopCheckBox->SetToolTip(wxT("Set the animation to loop when playing."));
	sizer->Add(loopCheckBox, 0, wxALIGN_CENTER | wxALL, 10);

	SetMinSize(minSize);
	SetMaxSize(maxSize);

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


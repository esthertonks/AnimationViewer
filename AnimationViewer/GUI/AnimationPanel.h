#pragma once

#include "wx/wx.h"

namespace gui
{

class AnimationPanel : public wxPanel
{
	public:

	AnimationPanel::AnimationPanel(
		wxWindow *parent,
		wxSize minSize,
		wxSize maxSize
	);

	void OnPlayClicked(
		wxCommandEvent& event
		);

	void OnPauseClicked(
		wxCommandEvent& event
		);

	void OnStopClicked(
		wxCommandEvent& event
		);

	void OnLoopAnimation(
		wxCommandEvent& event
		);

	protected:
		DECLARE_EVENT_TABLE()
};

}


#pragma once

#include "wx/wx.h"

namespace gui
{

class AnimationPanel : public wxPanel
{
	public:

	AnimationPanel::AnimationPanel(
		wxWindow *parent
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

	protected:
		DECLARE_EVENT_TABLE()
};

}


#pragma once

#include "wx/wx.h"

namespace gui
{

class ControlsPanel : public wxPanel
{
	public:

	ControlsPanel::ControlsPanel(
		wxWindow *parent,
		wxWindowID winid,
		const wxPoint& pos,
		const wxSize& size,
		long style
	);

	void OnCheckboxClicked(
		wxCommandEvent& event
		);

	protected:
		DECLARE_EVENT_TABLE()
};

}


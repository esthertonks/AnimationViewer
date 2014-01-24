#pragma once

#include "wx/wx.h"

namespace gui
{

class ViewPanel : public wxPanel
{
	public:

	ViewPanel::ViewPanel(
		wxWindow *parent
	);

	void OnCheckboxClicked(
		wxCommandEvent& event
		);

	void OnCentreCameraClicked(
		wxCommandEvent& event
		);

	protected:
		DECLARE_EVENT_TABLE()
};

}


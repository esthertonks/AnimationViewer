#include "ControlsPanel.h"

#include "ViewPanel.h"
#include "AnimationPanel.h"

namespace gui
{

ControlsPanel::ControlsPanel(
	wxWindow *parent,
	wxWindowID windowId,
	const wxPoint& position,
	const wxSize& size,
	long style
	)
	: wxPanel(parent, windowId, position, size, style)
{	
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	ViewPanel *viewPanel = new ViewPanel(this);
	AnimationPanel *animationPanel = new AnimationPanel(this);

	sizer->Add(viewPanel, 1, wxEXPAND, 0);
	sizer->Add(animationPanel, 1, wxEXPAND, 0);
	SetSizer(sizer);

}

}


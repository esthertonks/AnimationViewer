#include "ControlsPanel.h"

#include "ViewPanel.h"
#include "AnimationPanel.h"
#include "HierarchyPanel.h"

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
	viewPanel = new ViewPanel(this);
	animationPanel = new AnimationPanel(this);
	m_hierarchyPanel = new HierarchyPanel(this);

	sizer->Add(viewPanel, 20, wxCENTER, 10);
	sizer->Add(animationPanel, 20, wxCENTER, 10);
	sizer->Add(m_hierarchyPanel, 40, wxCENTER, 10);

	SetSizer(sizer);

}

HierarchyPanel& ControlsPanel::GetHierarchyPanel()
{
	return *m_hierarchyPanel;
}

}


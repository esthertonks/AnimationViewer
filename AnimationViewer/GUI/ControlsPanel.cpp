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

	m_animationPanel = new AnimationPanel(this, wxSize(340, 240), wxSize(360, 240));
	m_viewPanel = new ViewPanel(this, wxSize(340, 220), wxSize(360, 220));
	m_hierarchyPanel = new HierarchyPanel(this, wxSize(340, 500), wxSize(360, 500));

	sizer->Add(m_animationPanel, 30, wxCENTER, 10);
	sizer->Add(m_viewPanel, 20, wxCENTER, 10);
	sizer->Add(m_hierarchyPanel, 40, wxCENTER, 10);

	SetSizer(sizer);

}

HierarchyPanel& ControlsPanel::GetHierarchyPanel()
{
	return *m_hierarchyPanel;
}

ViewPanel& ControlsPanel::GetViewPanel()
{
	return *m_viewPanel;
}

AnimationPanel& ControlsPanel::GetAnimationPanel()
{
	return *m_animationPanel;
}

}


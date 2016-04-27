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
	m_viewPanel = new ViewPanel(this);
	m_animationPanel = new AnimationPanel(this);
	m_hierarchyPanel = new HierarchyPanel(this);

	sizer->Add(m_viewPanel, 20, wxCENTER, 10);
	sizer->Add(m_animationPanel, 20, wxCENTER, 10);
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


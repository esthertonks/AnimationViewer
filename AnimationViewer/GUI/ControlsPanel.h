#pragma once

#include "wx/wx.h"

namespace gui
{
	class HierarchyPanel;
	class AnimationPanel;
	class ViewPanel;

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

	HierarchyPanel& GetHierarchyPanel();
	AnimationPanel& GetAnimationPanel();
	ViewPanel& GetViewPanel();

private:
	ViewPanel *m_viewPanel;
	AnimationPanel *m_animationPanel;
	HierarchyPanel *m_hierarchyPanel;
};

}


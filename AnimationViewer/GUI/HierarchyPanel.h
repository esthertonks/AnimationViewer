#pragma once

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/treectrl.h>
#include <map>
#include <fbxsdk.h>

namespace mesh
{
	class BoneNode;
};

namespace gui
{

class HierarchyPanel : public wxPanel
{
public:

	HierarchyPanel::HierarchyPanel(
		wxWindow *parent,
		wxSize minSize,
		wxSize maxSize
	);

	void ClearData();

	void SetBoneHierarchy(
		mesh::BoneNode *root
		);

	void LoadIcons();

	void UpdateTransformText(
		wxString boneName,
		FbxAMatrix globalTransform
	);

	void OnBoneSelected(
		wxTreeEvent& event
		);

	protected:
		DECLARE_EVENT_TABLE()

private:

	void SetHierarchyInternal(
		mesh::BoneNode *root,
		wxTreeItemId *parentItem
		);

	enum
    {
		FileIcon,
		FileSelectedIcon,
		FolderIcon,
		FolderSelectedIcon,
		FolderOpenedIcon
    };

	wxTreeCtrl *m_animationTree;

	//wxStaticText *m_boneNameText;
	wxStaticText *m_positionText;
	wxStaticText *m_rotateText;
	wxStaticText *m_scaleText;

	wxStaticText *m_positionTextX;
	wxStaticText *m_rotateTextX;
	wxStaticText *m_scaleTextX;

	wxStaticText *m_positionTextY;
	wxStaticText *m_rotateTextY;
	wxStaticText *m_scaleTextY;

	wxStaticText *m_positionTextZ;
	wxStaticText *m_rotateTextZ;
	wxStaticText *m_scaleTextZ;
};

}


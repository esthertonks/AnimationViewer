#include "HierarchyPanel.h"
#include "../AnimationApp.h"
#include "../Mesh/BoneNode.h"
#include "../Colours.h"
#include "../Utils/WXUtils.h"
#include "../Icons/icon1.xpm"
#include "../Icons/icon2.xpm"
#include "../Icons/icon3.xpm"
#include "../Icons/icon4.xpm"
#include "../Icons/icon5.xpm"

#include "wx\statline.h"
#include "wx\stattext.h"

enum
{
	BoneSelected = wxID_HIGHEST + 1
};

namespace gui
{

BEGIN_EVENT_TABLE(HierarchyPanel, wxPanel)
	EVT_TREE_SEL_CHANGED(BoneSelected, HierarchyPanel::OnBoneSelected)
END_EVENT_TABLE()

class BoneItemData : public wxTreeItemData
{
public:
	BoneItemData(
		const wxString& boneName, 
		FbxAMatrix &globalTransform
		)
		: m_boneName(boneName),
		m_globalTransform(globalTransform)
	{
	}

	const wxString GetBoneName() const 
	{ 
		return m_boneName; 
	}

	const FbxAMatrix &GetGlobalTransform() const 
	{ 
		return m_globalTransform; 
	}

private:
	wxString m_boneName;
	FbxAMatrix m_globalTransform;
};

HierarchyPanel::HierarchyPanel(
	wxWindow *parent,
	wxSize minSize,
	wxSize maxSize
	)
	: wxPanel(parent)
{	
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticLine *separator = new wxStaticLine(this);
	separator->SetForegroundColour(*wxWHITE);
	separator->SetMinSize(wxSize(340, 3));
	sizer->Add(separator, 0, wxALIGN_CENTER | wxALL, 10);

	wxStaticText *hierarchyTitleText = new wxStaticText(this, wxID_ANY, wxT("BONE HIERARCHY"));
	hierarchyTitleText->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	utils::WXUtils::SetFont(hierarchyTitleText, 10, wxFONTWEIGHT_BOLD);
	sizer->Add(hierarchyTitleText, 0, wxALIGN_CENTER | wxALL, 10);

	// Set up the tree
	m_animationTree = new wxTreeCtrl(this, BoneSelected, wxDefaultPosition, wxSize(400, 250), wxTR_DEFAULT_STYLE | wxRAISED_BORDER);
	m_animationTree->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_animationTree->SetBackgroundColour(theme::Colours::m_darkGrey.GetAsUIColour());
	m_animationTree->SetMinSize(wxSize(340, 250));
	m_animationTree->SetToolTip("Select a bone to show its transforms in the box below.");

	LoadIcons();

	sizer->Add(m_animationTree, 0, wxALIGN_CENTER | wxALL, 10);

	wxStaticText *transformsTitleText = new wxStaticText(this, wxID_ANY, wxT("BONE TRANSFORMS"));
	transformsTitleText->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	utils::WXUtils::SetFont(transformsTitleText, 10, wxFONTWEIGHT_BOLD);
	sizer->Add(transformsTitleText, 0, wxALIGN_CENTER | wxALL, 10);

	// Add the bone stats panel:
	wxPanel *boneTransformsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(360, 250), wxTR_DEFAULT_STYLE | wxRAISED_BORDER , "");
	boneTransformsPanel->SetBackgroundColour(theme::Colours::m_darkGrey.GetAsUIColour());
	boneTransformsPanel->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	boneTransformsPanel->SetMinSize(wxSize(340, 250));
	boneTransformsPanel->SetToolTip("Shows the global transforms of the selected bone");

	//m_boneNameText = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	//sizer->Add(m_boneNameText, 0, wxALIGN_LEFT | wxALL, 10);

	wxSizer *transformPanelSizer = new wxGridSizer(3, 4, 2, 2);

	// Set up the text for the bone transforms. Separate strings so they can be positioned by the layout.
	m_positionText = new wxStaticText(boneTransformsPanel, wxID_ANY, "POSITION ");
	utils::WXUtils::SetFont(m_positionText, 8, wxFONTWEIGHT_BOLD);
	m_positionText->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_positionTextX = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_positionTextX->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_positionTextY = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_positionTextY->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_positionTextZ = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_positionTextZ->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());

	m_rotateText = new wxStaticText(boneTransformsPanel, wxID_ANY, "ROTATION ");
	utils::WXUtils::SetFont(m_rotateText, 8, wxFONTWEIGHT_BOLD);
	m_rotateText->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_rotateTextX = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_rotateTextX->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_rotateTextY = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_rotateTextY->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_rotateTextZ = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_rotateTextZ->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());

	m_scaleText = new wxStaticText(boneTransformsPanel, wxID_ANY, "SCALE ");
	utils::WXUtils::SetFont(m_scaleText, 8, wxFONTWEIGHT_BOLD);
	m_scaleText->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_scaleTextX = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_scaleTextX->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_scaleTextY = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_scaleTextY->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());
	m_scaleTextZ = new wxStaticText(boneTransformsPanel, wxID_ANY, "");
	m_scaleTextZ->SetForegroundColour(theme::Colours::m_lightGrey.GetAsUIColour());

	transformPanelSizer->Add(m_positionText, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_positionTextX, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_positionTextY, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_positionTextZ, 0, wxALIGN_LEFT | wxALL, 10);

	transformPanelSizer->Add(m_rotateText, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_rotateTextX, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_rotateTextY, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_rotateTextZ, 0, wxALIGN_LEFT | wxALL, 10);

	transformPanelSizer->Add(m_scaleText, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_scaleTextX, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_scaleTextY, 0, wxALIGN_LEFT | wxALL, 10);
	transformPanelSizer->Add(m_scaleTextZ, 0, wxALIGN_LEFT | wxALL, 10);

	boneTransformsPanel->SetSizer(transformPanelSizer);

	sizer->Add(boneTransformsPanel, 0, wxALIGN_CENTER | wxALL, 10);

	SetMinSize(minSize);
	SetMaxSize(maxSize);
	SetSizer(sizer);
}

void HierarchyPanel::ClearData()
{
	SetEvtHandlerEnabled(false);
	m_animationTree->DeleteAllItems();
	SetEvtHandlerEnabled(true);

	m_positionTextX->SetLabelText("");
	m_positionTextY->SetLabelText("");
	m_positionTextZ->SetLabelText("");

	m_rotateTextX->SetLabelText("");
	m_rotateTextY->SetLabelText("");
	m_rotateTextZ->SetLabelText("");

	m_scaleTextX->SetLabelText("");
	m_scaleTextY->SetLabelText("");
	m_scaleTextZ->SetLabelText("");
}

void HierarchyPanel::SetBoneHierarchy(
	mesh::BoneNode *root
	)
{
	if(root == NULL || root->m_firstChild == NULL)
	{
		return;
	}

	wxString rootName(root->GetName().c_str());
	SetHierarchyInternal(root->m_firstChild, &m_animationTree->AddRoot(rootName, FolderIcon, FolderIcon, new BoneItemData(rootName, root->GetGlobalTransform())));
}

void HierarchyPanel::SetHierarchyInternal(
	mesh::BoneNode *boneNode,
	wxTreeItemId *parentItem
	)
{
	for(mesh::BoneNode *node = boneNode; node != NULL; node = node->m_next)
	{

		wxString boneName(node->GetName().c_str());

		int iconId = node->m_parent != NULL ? FileIcon : FolderIcon;

		wxTreeItemId item = m_animationTree->AppendItem(*parentItem, boneName, iconId, iconId, new BoneItemData(boneName, node->GetGlobalTransform()));

		for(mesh::BoneNode *childNode = node->m_firstChild; childNode != NULL; childNode = childNode->m_next)
		{
			SetHierarchyInternal(childNode, &item);
		}
	}
}

void HierarchyPanel::LoadIcons()
{
	wxIcon icons[5];
	int iconSize = 16;
	wxImageList *imageList = new wxImageList(iconSize, iconSize, true);
	icons[FileIcon] = wxIcon(icon1_xpm);
	icons[FileSelectedIcon] = wxIcon(icon2_xpm);
	icons[FolderIcon] = wxIcon(icon3_xpm);
	icons[FolderSelectedIcon] = wxIcon(icon4_xpm);
	icons[FolderOpenedIcon] = wxIcon(icon5_xpm);

	for (int iconIndex = 0; iconIndex < WXSIZEOF(icons); iconIndex++ )
	{
		int width = icons[iconIndex].GetWidth();
		int height = icons[iconIndex].GetHeight();
		if(iconSize == width && iconSize == height)
		{
			imageList->Add(icons[iconIndex]);
		}
		else
		{
			imageList->Add(wxBitmap(wxBitmap(icons[iconIndex]).ConvertToImage().Rescale(iconSize, iconSize)));
		}
	}

	m_animationTree->AssignImageList(imageList);
}

void HierarchyPanel::UpdateTransformText(
	wxString boneName,
	FbxAMatrix globalTransform
	)
{
	FbxVector4 position = globalTransform.GetT();
	FbxVector4 rotation = globalTransform.GetR();
	FbxVector4 scale = globalTransform.GetS();

	wxString positionXText(wxString(wxString::Format("%.3f", position[0])));
	wxString positionYText(wxString(wxString::Format("%.3f", position[1])));
	wxString positionZText(wxString(wxString::Format("%.3f", position[2])));

	wxString rotationXText(wxString(wxString::Format("%.3f", rotation[0])));
	wxString rotationYText(wxString(wxString::Format("%.3f", rotation[1])));
	wxString rotationZText(wxString(wxString::Format("%.3f", rotation[2])));

	wxString scaleXText(wxString(wxString::Format("%.3f", scale[0])));
	wxString scaleYText(wxString(wxString::Format("%.3f", scale[1])));
	wxString scaleZText(wxString(wxString::Format("%.3f", scale[2])));

	m_positionTextX->SetLabelText(positionXText);
	m_positionTextY->SetLabelText(positionYText);
	m_positionTextZ->SetLabelText(positionZText);

	m_rotateTextX->SetLabelText(rotationXText);
	m_rotateTextY->SetLabelText(rotationYText);
	m_rotateTextZ->SetLabelText(rotationZText);

	m_scaleTextX->SetLabelText(scaleXText);
	m_scaleTextY->SetLabelText(scaleYText);
	m_scaleTextZ->SetLabelText(scaleZText);
}

void HierarchyPanel::OnBoneSelected(
	wxTreeEvent& event
	)
{
	wxTreeItemId selectedBone = event.GetItem();
	BoneItemData *selectedBoneData = static_cast<BoneItemData *>(m_animationTree->GetItemData(selectedBone.GetID()));
	if(selectedBoneData)
	{
		UpdateTransformText(selectedBoneData->GetBoneName(), selectedBoneData->GetGlobalTransform());
	}
}

}


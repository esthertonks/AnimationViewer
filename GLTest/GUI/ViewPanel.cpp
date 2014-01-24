#include "ViewPanel.h"
#include "../AnimationApp.h"
#include "wx\statline.h"
#include "wx\stattext.h"

namespace gui
{
	enum
	{
		BonesCheckbox = wxID_HIGHEST + 200,
		MeshCheckbox,
		CentreCamera
	};

BEGIN_EVENT_TABLE(ViewPanel, wxPanel)
	EVT_CHECKBOX(BonesCheckbox, ViewPanel::OnCheckboxClicked)
	EVT_CHECKBOX(MeshCheckbox, ViewPanel::OnCheckboxClicked)
	EVT_CHECKBOX(CentreCamera, ViewPanel::OnCentreCameraClicked)
END_EVENT_TABLE()

ViewPanel::ViewPanel(
	wxWindow *parent
	)
	: wxPanel(parent)
{	
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *viewOptionsText = new wxStaticText(this, wxID_ANY, wxT("View Options:"));
	viewOptionsText->SetForegroundColour(AnimationApp::m_guiTextColour);
	sizer->Add(viewOptionsText, 0, wxALIGN_LEFT | wxALL, 10);

	wxCheckBox *bonesCheckBox = new wxCheckBox(this, BonesCheckbox, wxT("Show Bones"));
	bonesCheckBox->SetForegroundColour(AnimationApp::m_guiTextColour);
	bonesCheckBox->SetValue(false);
	bonesCheckBox->SetToolTip(wxT("Show bone overlay for current mesh"));
	sizer->Add(bonesCheckBox, 0, wxALIGN_LEFT | wxALL, 10);

	wxCheckBox *meshCheckBox = new wxCheckBox(this, MeshCheckbox, wxT("Show Mesh"));
	meshCheckBox->SetForegroundColour(AnimationApp::m_guiTextColour);
	meshCheckBox->SetValue(true);
	meshCheckBox->SetToolTip(wxT("Show the current mesh"));

	sizer->Add(meshCheckBox, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *centreCameraButton = new wxButton(this, CentreCamera, wxT("Centre Camera"));
	centreCameraButton->SetToolTip(wxT("Reset the camera to look directly at the mesh"));
	centreCameraButton->SetMinSize(wxSize(100, 20));
	sizer->Add(centreCameraButton, 0, wxALIGN_LEFT | wxALL, 10);

	SetMinSize(wxSize(450, 150));
	SetMaxSize(wxSize(600, 150));

	SetSizer(sizer);
}

void ViewPanel::OnCheckboxClicked(
	wxCommandEvent& event
	)
{
	//wxCheckBox* checkbox = (wxCheckBox*)event.GetEventObject();
	switch (event.GetId())
	{
	case BonesCheckbox:
		wxGetApp().ShowBones(event.GetInt() ? true : false);
		break;

	//case ID_NORMALS_CHECKBOX:
	//	wxGetApp().ShowNormals(event.GetInt() ? true : false);
	//	break;

	case MeshCheckbox:
		wxGetApp().ShowMesh(event.GetInt() ? true : false);
		break;
	}
}

void ViewPanel::OnCentreCameraClicked(
	wxCommandEvent& event
	)
{
	wxGetApp().CentreCamera();
}

}


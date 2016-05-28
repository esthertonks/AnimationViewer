#include "ViewPanel.h"
#include "../AnimationApp.h"
#include "wx\statline.h"
#include "wx\stattext.h"
#include "wx\valnum.h"

namespace gui
{
	enum
	{
		BonesCheckbox = wxID_HIGHEST + 200,
		MeshCheckbox,
		NormalsCheckbox,
		CentreCamera,
		SetLightPosition
	};

BEGIN_EVENT_TABLE(ViewPanel, wxPanel)
	EVT_CHECKBOX(BonesCheckbox, ViewPanel::OnCheckboxClicked)
	EVT_CHECKBOX(MeshCheckbox, ViewPanel::OnCheckboxClicked)
	EVT_CHECKBOX(NormalsCheckbox, ViewPanel::OnCheckboxClicked)
	EVT_BUTTON(CentreCamera, ViewPanel::OnCentreCameraClicked)
	EVT_BUTTON(SetLightPosition, ViewPanel::OnSetLightPositionClicked)
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

	m_bonesCheckBox = new wxCheckBox(this, BonesCheckbox, wxT("Show Bones"));
	m_bonesCheckBox->SetForegroundColour(AnimationApp::m_guiTextColour);
	//m_bonesCheckBox->SetValue(false);
	m_bonesCheckBox->SetToolTip(wxT("Show bone overlay for current mesh"));
	sizer->Add(m_bonesCheckBox, 0, wxALIGN_LEFT | wxALL, 10);

	m_meshCheckBox = new wxCheckBox(this, MeshCheckbox, wxT("Show Mesh"));
	m_meshCheckBox->SetForegroundColour(AnimationApp::m_guiTextColour);
	//m_meshCheckBox->SetValue(true);
	m_meshCheckBox->SetToolTip(wxT("Show the current mesh"));

	sizer->Add(m_meshCheckBox, 0, wxALIGN_LEFT | wxALL, 10);

	m_normalsCheckBox = new wxCheckBox(this, NormalsCheckbox, wxT("Show Normals"));
	m_normalsCheckBox->SetForegroundColour(AnimationApp::m_guiTextColour);
	//m_normalsCheckBox->SetValue(false);
	m_normalsCheckBox->SetToolTip(wxT("Show normals overaly for the current mesh"));

	sizer->Add(m_normalsCheckBox, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *centreCameraButton = new wxButton(this, CentreCamera, wxT("Centre Camera"));
	centreCameraButton->SetToolTip(wxT("Reset the camera to look directly at the mesh"));
	centreCameraButton->SetMinSize(wxSize(120, 20));
	sizer->Add(centreCameraButton, 0, wxALIGN_LEFT | wxALL, 10);

	wxButton *lightPositionButton = new wxButton(this, SetLightPosition, wxT("Set Light Position"));
	lightPositionButton->SetToolTip(wxT("Set the position of the light"));
	lightPositionButton->SetMinSize(wxSize(120, 20));

	wxFloatingPointValidator<float> positionValidatorX(3, &m_xPositionValue, wxNUM_VAL_ZERO_AS_BLANK);
	wxFloatingPointValidator<float> positionValidatorY(3, &m_yPositionValue, wxNUM_VAL_ZERO_AS_BLANK);
	wxFloatingPointValidator<float> positionValidatorZ(3, &m_zPositionValue, wxNUM_VAL_ZERO_AS_BLANK);

	m_lightPositionX = new wxTextCtrl(this, wxID_ANY, "0.0", wxDefaultPosition, wxDefaultSize, 0, positionValidatorX, "0.0");
	m_lightPositionY = new wxTextCtrl(this, wxID_ANY, "0.0", wxDefaultPosition, wxDefaultSize, 0, positionValidatorY, "0.0");
	m_lightPositionZ = new wxTextCtrl(this, wxID_ANY, "0.0", wxDefaultPosition, wxDefaultSize, 0, positionValidatorZ, "0.0");
	m_lightPositionX->SetForegroundColour(AnimationApp::m_guiTextColour);
	m_lightPositionY->SetForegroundColour(AnimationApp::m_guiTextColour);
	m_lightPositionZ->SetForegroundColour(AnimationApp::m_guiTextColour);

	m_lightPositionX->SetBackgroundColour(AnimationApp::m_guiBackgroundColour);
	m_lightPositionY->SetBackgroundColour(AnimationApp::m_guiBackgroundColour);
	m_lightPositionZ->SetBackgroundColour(AnimationApp::m_guiBackgroundColour);

	m_lightPositionX->SetMinSize(wxSize(50, 20));
	m_lightPositionY->SetMinSize(wxSize(50, 20));
	m_lightPositionZ->SetMinSize(wxSize(50, 20));
	
	m_lightPositionX->SetMaxSize(wxSize(30, 20));
	m_lightPositionY->SetMaxSize(wxSize(30, 20));
	m_lightPositionZ->SetMaxSize(wxSize(30, 20));

	wxSizer *positionSizer = new wxBoxSizer(wxHORIZONTAL);
	positionSizer->Add(lightPositionButton, 0, wxALIGN_LEFT | wxALL, 2);
	positionSizer->Add(m_lightPositionX, 0, wxALIGN_LEFT | wxALL, 2);
	positionSizer->Add(m_lightPositionY, 0, wxALIGN_LEFT | wxALL, 2);
	positionSizer->Add(m_lightPositionZ, 0, wxALIGN_LEFT | wxALL, 2);

	SetMinSize(wxSize(450, 200));
	SetMaxSize(wxSize(600, 200));

	sizer->Add(positionSizer, 0, wxALIGN_LEFT | wxALL, 8);

	SetSizer(sizer);
}

void ViewPanel::Initialise(
	glm::vec4 lightPosition,
	bool showBones,
	bool showMesh,
	bool showNormals
)
{
	m_xPositionValue = lightPosition.x;
	m_yPositionValue = lightPosition.y;
	m_zPositionValue = lightPosition.z;

	m_lightPositionX->GetValidator()->TransferToWindow();
	m_lightPositionY->GetValidator()->TransferToWindow();
	m_lightPositionZ->GetValidator()->TransferToWindow();

	m_bonesCheckBox->SetValue(showBones);
	m_meshCheckBox->SetValue(showMesh);
	m_normalsCheckBox->SetValue(showNormals);
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

	case NormalsCheckbox:
		wxGetApp().ShowNormals(event.GetInt() ? true : false);
		break;

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

void ViewPanel::OnSetLightPositionClicked(
	wxCommandEvent& event
	)
{
	m_lightPositionX->GetValidator()->TransferFromWindow();
	m_lightPositionY->GetValidator()->TransferFromWindow();
	m_lightPositionZ->GetValidator()->TransferFromWindow();

	wxGetApp().SetLightPosition(glm::vec4(m_xPositionValue, m_yPositionValue, m_zPositionValue, 1.0f));
}

}


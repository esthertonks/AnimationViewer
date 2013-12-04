#include "ControlsPanel.h"
#include "../AnimationApp.h"

const int ID_BONES_CHECKBOX = 100;
const int ID_NORMALS_CHECKBOX = 101;

namespace gui
{

BEGIN_EVENT_TABLE(ControlsPanel, wxPanel)
	EVT_CHECKBOX(ID_BONES_CHECKBOX, ControlsPanel::OnCheckboxClicked)
	EVT_CHECKBOX(ID_NORMALS_CHECKBOX, ControlsPanel::OnCheckboxClicked)
END_EVENT_TABLE()

ControlsPanel::ControlsPanel(
	wxWindow *parent,
	wxWindowID windowId,
	const wxPoint& position,
	const wxSize& size,
	long style
	)
	: wxPanel(parent, windowId, position, size, style)
{	
	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *checkBoxSizer = new wxBoxSizer(wxVERTICAL);

	wxCheckBox *bonesCheckBox = new wxCheckBox(this, ID_BONES_CHECKBOX, "Show Bones");
	bonesCheckBox->SetValue(false);
	bonesCheckBox->SetToolTip("Show bone overlay for current mesh");
	checkBoxSizer->Add(bonesCheckBox, 1, wxALIGN_LEFT | wxALL, 10);
	wxCheckBox *normalsCheckBox = new wxCheckBox(this, ID_NORMALS_CHECKBOX, "Show Normals");
	normalsCheckBox->SetValue(false);
	normalsCheckBox->SetToolTip("Show normals overlay for current mesh");
	checkBoxSizer->Add(normalsCheckBox, 1, wxALIGN_LEFT | wxALL, 10);
	verticalSizer->Add(checkBoxSizer, 30, wxALIGN_TOP | wxALL, 10);
	SetSizer(verticalSizer);
}

void ControlsPanel::OnCheckboxClicked(
	wxCommandEvent& event
	)
{
	wxCheckBox* checkbox = (wxCheckBox*)event.GetEventObject();
	switch (event.GetId())
	{
	case ID_BONES_CHECKBOX:
		wxGetApp().ShowBones(event.GetInt() ? true : false);
		break;

	case ID_NORMALS_CHECKBOX:
		wxGetApp().ShowNormals(event.GetInt() ? true : false);
		break;
	}
}

}


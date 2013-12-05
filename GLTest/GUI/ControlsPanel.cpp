#include "ControlsPanel.h"
#include "../AnimationApp.h"
#include "wx\statline.h"
#include "wx\stattext.h"

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
	wxBoxSizer *leftPanelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *topLeftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *bottomLeftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *checkBoxSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *viewOptionsText = new wxStaticText(this, wxID_ANY, wxT("VIEW OPTIONS:"));
	viewOptionsText->SetForegroundColour(*wxWHITE);
	checkBoxSizer->Add(viewOptionsText , 20, wxALIGN_LEFT | wxALL, 10);

	wxCheckBox *bonesCheckBox = new wxCheckBox(this, ID_BONES_CHECKBOX, wxT("Show Bones"));
	bonesCheckBox->SetForegroundColour(*wxWHITE);
	bonesCheckBox->SetValue(false);
	bonesCheckBox->SetToolTip(wxT("Show bone overlay for current mesh"));
	checkBoxSizer->Add(bonesCheckBox, 20, wxALIGN_LEFT | wxALL, 10);

	wxCheckBox *normalsCheckBox = new wxCheckBox(this, ID_NORMALS_CHECKBOX, wxT("Show Normals"));
	normalsCheckBox->SetForegroundColour(*wxWHITE);
	normalsCheckBox->SetValue(false);
	normalsCheckBox->SetToolTip(wxT("Show normals overlay for current mesh"));

	checkBoxSizer->Add(normalsCheckBox, 20, wxALIGN_LEFT | wxALL, 10);

	//wxStaticLine *separator = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(100, 1));
	wxStaticLine *separator = new wxStaticLine(this);
	separator->SetForegroundColour(*wxWHITE);
	separator->SetMinSize(wxSize(450, 3));
	topLeftSizer->Add(checkBoxSizer, 0, wxALIGN_TOP);
	topLeftSizer->Add(separator, 0, wxALIGN_CENTER | wxALL, 10);
	leftPanelSizer->Add(topLeftSizer, 30, wxALIGN_CENTRE | wxALL, 10);
	leftPanelSizer->Add(bottomLeftSizer, 70);
	SetSizer(leftPanelSizer);
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


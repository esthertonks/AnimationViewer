#pragma once

#include "wx/wx.h"
#include "glm\glm.hpp"

namespace gui
{

class ViewPanel : public wxPanel
{
	public:

	ViewPanel::ViewPanel(
		wxWindow *parent
	);

	void Initialise(
		glm::vec4 lightPosition,
		bool showBones,
		bool showMesh,
		bool showNormals
	);

	void OnCheckboxClicked(
		wxCommandEvent& event
		);

	void OnCentreCameraClicked(
		wxCommandEvent& event
		);

	void OnSetLightPositionClicked(
		wxCommandEvent& event
		);

	protected:
		DECLARE_EVENT_TABLE()

	private:

	float m_xPositionValue;
	float m_yPositionValue;
	float m_zPositionValue;

	wxCheckBox *m_meshCheckBox;
	wxCheckBox *m_bonesCheckBox;
	wxCheckBox *m_normalsCheckBox;

	wxTextCtrl *m_lightPositionX;
	wxTextCtrl *m_lightPositionY;
	wxTextCtrl *m_lightPositionZ;
};

}


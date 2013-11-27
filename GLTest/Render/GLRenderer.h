#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../Batch/BatchFwdDecl.h"

namespace render
{
	class Batch;
	class OrbitCamera;
	class RenderEntity;
	class ShaderManager;

class GLRenderer : public wxGLCanvas
{
public:

	GLRenderer(
		wxWindow *parent, 
		wxWindowID id, 
		const wxPoint& position,
		const wxSize& size, 
		long style, 
		const wxString& name
	);

	~GLRenderer();

	OrbitCamera& GetCamera()
	{
		return *m_camera;
	}

	void Paint(
		wxPaintEvent& event
		);

	void RenderImmediate(
		);

	void OnSize(
		wxSizeEvent& event
		);

	void OnEraseBackground(
		wxEraseEvent& event
		);

	void OnSetFocus(
		wxFocusEvent& event
		);

	void OnKeyDown(
		wxKeyEvent& event
		);

	void OnLeftDown(
		wxMouseEvent& event
		);

	void OnRightDown(
		wxMouseEvent& event
		);

	void OnMouseMove(
		wxMouseEvent& event
		);

	void OnMouseWheelScroll(
		wxMouseEvent& event
		);

	void OnMouseWheelDown(
		wxMouseEvent& event
		);

	void InitGL();

	void AddEntity(
		RenderEntity *entity
		)
	{
		m_renderEntity = entity;
	}

	void RemoveEntity(
		RenderEntity *entity
		)
	{
		if(entity == m_renderEntity)
		{
			delete m_renderEntity;
			m_renderEntity = NULL;
		}
	}

protected:
	DECLARE_EVENT_TABLE()

private:
	void Render();

	wxGLContext* m_context;

	OrbitCamera *m_camera;

	RenderEntity *m_renderEntity;

	ShaderManager *m_shaderManager;

	bool m_initialised;
};

}


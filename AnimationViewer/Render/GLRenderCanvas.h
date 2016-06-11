#pragma once

#include "../Render/RenderFwdDecl.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <map>
#include <string>
#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>
#include "../Batch/BatchFwdDecl.h"
#include "../Renderable/RenderableFwdDecl.h"

namespace render
{
	class Batch;
	class OrbitCamera;
	class Renderable;
	class RenderableMesh;
	class ShaderManager;

class GLRenderCanvas : public wxGLCanvas
{
public:

	GLRenderCanvas(
		wxWindow *parent,
		wxGLAttributes &attributeList
	);

	~GLRenderCanvas();

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

	void CentreCamera();

	void SetLightPosition(
		glm::vec4 position
		);

	const glm::vec4 &GetLightPosition()
	{
		return m_lightPosition;
	}

	void InitGL();

	void AddRenderable(
		RenderablePtr &renderable
		)
	{
		m_renderables.push_front(renderable);
	}

	void AddOverlay(
		RenderablePtr &renderable
		)
	{
		m_renderables.push_back(renderable);
	}

	void RemoveRenderable(
		RenderablePtr &renderable
		)
	{
		m_renderables.remove(renderable);
	}

protected:
	DECLARE_EVENT_TABLE()

private:
	void Render();

	wxGLContext* m_context;

	OrbitCamera *m_camera;

	typedef std::list<RenderablePtr> Renderables;
	Renderables m_renderables;

	ShaderManager *m_shaderManager;

	bool m_initialised;

	glm::vec4 m_lightPosition;
};

}


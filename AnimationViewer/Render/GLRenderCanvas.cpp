#include "GLRenderCanvas.h"

#include <wx/log.h>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderable/RenderableMesh.h"
#include "../Batch/VertexFormat.h"
#include "../Batch/Batch.h"
#include "OrbitCamera.h"
#include "../Batch/Appearance.h"
#include "../Batch/PhongAppearance.h"
#include "../Batch/LambertAppearance.h"
#include "ShaderManager.h"
#include "GLUtils.h"
#include "../Colours.h"

#include "GL/wglew.h"

namespace render
{

BEGIN_EVENT_TABLE(GLRenderCanvas, wxGLCanvas)
	EVT_PAINT(GLRenderCanvas::Paint)
	EVT_SIZE(GLRenderCanvas::OnSize)
	EVT_ERASE_BACKGROUND(GLRenderCanvas::OnEraseBackground)
	EVT_SET_FOCUS(GLRenderCanvas::OnSetFocus)

	EVT_LEFT_DOWN(GLRenderCanvas::OnLeftDown)
	EVT_RIGHT_DOWN(GLRenderCanvas::OnRightDown)
	EVT_MOTION(GLRenderCanvas::OnMouseMove)
	EVT_MOUSEWHEEL(GLRenderCanvas::OnMouseWheelScroll)
	EVT_MIDDLE_DOWN(GLRenderCanvas::OnMouseWheelDown)

	EVT_KEY_DOWN(GLRenderCanvas::OnKeyDown)
END_EVENT_TABLE()

// GL Perspective parameters. TODO move and add UI controls
const float GLRenderCanvas::m_fovy = 40.0f;
const float GLRenderCanvas::m_nearPlane = 1.0f;
const float GLRenderCanvas::m_farPlane = 2000.0f;

GLRenderCanvas::GLRenderCanvas(
	wxWindow *parent,
	wxGLAttributes &attributeList
	)
	: wxGLCanvas(parent, attributeList), 
	m_context(NULL),
	m_camera(new OrbitCamera(glm::vec3(1000.0f, 0.0f, 0.0f))),
	m_shaderManager(new ShaderManager()),
	m_initialised(false),
	m_lightPosition(1000.0f, 1000.0f, 1000.0f, 1.0f)
{
}

void GLRenderCanvas::OnSize(
	wxSizeEvent& event
	)
{
	if(m_context)
	{
		SetCurrent(*m_context);
	}

	int width = event.GetSize().GetWidth();
	int height = event.GetSize().GetHeight();

//	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
//	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	glViewport(0, 0, (GLint)width, (GLint)height);

	//Refresh();
	Render();
}

void GLRenderCanvas::OnEraseBackground(
	wxEraseEvent& WXUNUSED(event)
	)
{
	// Make sure this does nothing to prevent flickering on resize
}

void GLRenderCanvas::OnSetFocus(
	wxFocusEvent& WXUNUSED(event)
	)
{
}

void GLRenderCanvas::OnKeyDown(
	wxKeyEvent& event
	)
{
	wxString key;
	long keycode = event.GetKeyCode();
	switch (keycode)
	{
		case WXK_NUMPAD_PAGEUP:

			break;
		case WXK_NUMPAD_PAGEDOWN:

			break;

		case 'F':
			//TODO focus on mesh if the mesh has moved from zero
			CentreCamera();
			break;
	}
}

static float lastPosX = -1;
static float lastPosY = -1;

void GLRenderCanvas::OnLeftDown(
	wxMouseEvent& event
	)
{
	if(m_renderables.size() == 0)
	{
		return;
	}

	lastPosX = event.m_x;
	lastPosY = event.m_y;

	event.Skip();
}

void GLRenderCanvas::OnRightDown(
	wxMouseEvent& event
	)
{
	if(m_renderables.size() == 0)
	{
		return;
	}

	lastPosX = event.m_x;
	lastPosY = event.m_y;
}

void GLRenderCanvas::OnMouseWheelScroll(
	wxMouseEvent& event
	)
{
	if(m_renderables.size() == 0)
	{
		return;
	}
	int wheelRotation = event.GetWheelRotation();
	m_camera->Zoom(-(wheelRotation / event.GetWheelDelta()) * 10);
}

void GLRenderCanvas::OnMouseWheelDown(
	wxMouseEvent& event
	)
{
	if(m_renderables.size() == 0)
	{
		return;
	}

	lastPosX = event.m_x;
	lastPosY = event.m_y;
}

void GLRenderCanvas::OnMouseMove(
	wxMouseEvent& event
	)
{
	if(m_renderables.size() == 0)
	{
		return;
	}

	//Check that a mouse button was actually pressed before mouse move
	if(lastPosX == -1 || lastPosY == -1)
	{
		return;
	}

	float posX = event.m_x;
	float posY = event.m_y;

	if(event.AltDown()) // Manipulate the camera
	{
		float diffX = posX - lastPosX;
		float diffY = posY - lastPosY;

		if(event.LeftIsDown()) // Rotate around pivot
		{
			if(diffX != 0)
			{
				m_camera->RotateAroundY((diffX / wxSystemSettings::GetMetric (wxSYS_SCREEN_X)) * 100);
			}

			if(diffY != 0)
			{
				m_camera->RotateAroundX((diffY / wxSystemSettings::GetMetric (wxSYS_SCREEN_Y)) * 100);
			}
		}
		else if(event.RightIsDown()) // Zoom
		{
			if(diffY != 0)
			{
				m_camera->Zoom(-diffY);
			}
		}
		else if(event.MiddleIsDown())
		{
			if(diffX != 0)
			{
				int width = GetSize().GetWidth();
				int height = GetSize().GetHeight();

				m_camera->PanX(-diffX, (float)width / (float)height);
			}
			if(diffY != 0)
			{
				int width = GetSize().GetWidth();
				int height = GetSize().GetHeight();

				m_camera->PanY(diffY, (float)width / (float)height);
			}
		}
	}
	else if(event.LeftIsDown()) // Rotate around pivot
	{
		float diffX = posX - lastPosX;
		float diffY = posY - lastPosY;

		if(diffX != 0 || diffY != 0)
		{
			if(diffX != 0)
			{
				m_camera->RotateAroundY((diffX / wxSystemSettings::GetMetric (wxSYS_SCREEN_X)) * 100);
			}

			if(diffY != 0)
			{
				m_camera->RotateAroundX((diffY / wxSystemSettings::GetMetric (wxSYS_SCREEN_Y)) * 100);
			}
		}
	}

	lastPosX = posX;
	lastPosY = posY;

}

void GLRenderCanvas::CentreCamera()
{
	m_camera->Reset();
}

void GLRenderCanvas::SetLightPosition(
	glm::vec4 position
	)
{
	m_lightPosition = position;
}

void GLRenderCanvas::InitGL()
{
	if(m_context)
	{
		wxLogDebug("GL already initialised");
		return;
	}

	wxGLContextAttrs contextAttributes;
	contextAttributes.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();

	m_context = new wxGLContext(this, NULL, &contextAttributes);

	if (!m_context->IsOK())
	{
		wxMessageBox("This sample needs an OpenGL 3.3 capable driver.\nThe application will now close.",
			"OpenGL version error", wxOK | wxICON_INFORMATION, this);
		delete m_context;
		m_context = NULL;
		return;
	}


	SetCurrent(*m_context);

	//TODO calling this here causes a GL expection - why???
	//DebugPrintGLInfo();

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if(status != GLEW_OK) //TODO - quit here and it leaks like a sieve.
	{
		wxLogDebug("Error initializing GLEW: ");
		char *errorText = (char*)glewGetErrorString(status);
		wxLogDebug(errorText);
		return;
	}

	utils::GLUtils::CheckOpenGLError(__FILE__,__LINE__);

	m_shaderManager->InitialiseShaders(); //TODO failure message when failed?

	utils::GLUtils::CheckOpenGLError(__FILE__,__LINE__);

	glm::vec3 backbroundColour = theme::Colours::m_grey.GetAsRenderColour();
	glClearColor(backbroundColour.r, backbroundColour.g, backbroundColour.b, 1.0f);

	int width = GetSize().GetWidth();
	int height = GetSize().GetHeight();

//	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
//	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glCullFace(GL_BACK);

	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	m_initialised = true;

	utils::GLUtils::DebugPrintGLInfo();
}

void GLRenderCanvas::Paint(
	wxPaintEvent& WXUNUSED(event)
	)
{
	wxPaintDC(this);
	Render();
}

void GLRenderCanvas::RenderImmediate()
{
	wxClientDC(this);
	Render();
}

void GLRenderCanvas::Render(
	)
{
	// TODO wxwidgets has no init callback so this has to be done here. bleugh.
	if(!m_context)
	{
		wxLogDebug("Error - no gl canvas available for rendering");
		return;
	}

	if(!m_initialised)
	{
		wxLogDebug("openGL not initialised");
		return;
	}

	utils::GLUtils::CheckOpenGLError(__FILE__,__LINE__);

	SetCurrent(*m_context);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_renderables.size() == 0)
	{
		glFlush();
		SwapBuffers();
		return;
	}

	glm::mat4x4& viewMatrix = m_camera->GetViewMatrix();

	int width = GetSize().GetWidth();
	int height = GetSize().GetHeight();

	glm::mat4x4 projectionMatrix = glm::perspective(m_fovy, (float)width / (float) height, m_nearPlane, m_farPlane);

	glm::vec4 lightPosition = m_lightPosition;
	
	Renderables::const_iterator renderableIterator;
	for(renderableIterator = m_renderables.begin(); renderableIterator != m_renderables.end(); renderableIterator++)
	{
		(*renderableIterator)->Render(*m_shaderManager, viewMatrix, projectionMatrix, lightPosition);
	}

	glFlush();

	SwapBuffers();
}

GLRenderCanvas::~GLRenderCanvas()
{
	delete m_context;
	m_context = NULL;

	delete m_camera;
	m_camera = NULL;

	delete m_shaderManager;
	m_shaderManager = NULL;
}
}
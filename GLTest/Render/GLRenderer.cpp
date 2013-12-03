#include "GLRenderer.h"

#include <wx/log.h>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderEntity.h"
#include "../Batch/VertexFormat.h"
#include "../Batch/Batch.h"
#include "OrbitCamera.h"
#include "../Batch/Appearance.h"
#include "../Batch/PhongAppearance.h"
#include "../Batch/LambertAppearance.h"
#include "RenderEntity.h"
#include "ShaderManager.h"
#include "GLUtils.h"

namespace render
{

BEGIN_EVENT_TABLE(GLRenderer, wxGLCanvas)
	EVT_PAINT(GLRenderer::Paint)
	EVT_SIZE(GLRenderer::OnSize)
	EVT_ERASE_BACKGROUND(GLRenderer::OnEraseBackground)
	EVT_SET_FOCUS(GLRenderer::OnSetFocus)

	EVT_LEFT_DOWN(GLRenderer::OnLeftDown)
	EVT_RIGHT_DOWN(GLRenderer::OnRightDown)
	EVT_MOTION(GLRenderer::OnMouseMove)
	EVT_MOUSEWHEEL(GLRenderer::OnMouseWheelScroll)
	EVT_MIDDLE_DOWN(GLRenderer::OnMouseWheelDown)

	EVT_KEY_DOWN(GLRenderer::OnKeyDown)
END_EVENT_TABLE()

GLRenderer::GLRenderer(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint& position,
	const wxSize& size,
	long style, 
	const wxString& name
	)
	: wxGLCanvas(parent, id, position, size, style|wxFULL_REPAINT_ON_RESIZE, name), 
	m_context(NULL),
	m_camera(new OrbitCamera(glm::vec3(100.0f, 0.0f, 0.0f))),
	m_renderEntity(NULL),
	m_shaderManager(new ShaderManager()),
	m_initialised(false)
{
}

void GLRenderer::OnSize(
	wxSizeEvent& event
	)
{
    // Necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

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

void GLRenderer::OnEraseBackground(
	wxEraseEvent& WXUNUSED(event)
	)
{
	// Make sure this does nothing to prevent flickering on resize
}

void GLRenderer::OnSetFocus(
	wxFocusEvent& WXUNUSED(event)
	)
{
}

void GLRenderer::OnKeyDown(
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
			m_camera->Reset();
			break;
	}
}

static float lastPosX = -1;
static float lastPosY = -1;

void GLRenderer::OnLeftDown(
	wxMouseEvent& event
	)
{
	if(!m_renderEntity)
	{
		return;
	}

	lastPosX = event.m_x;
	lastPosY = event.m_y;

	event.Skip();
}

void GLRenderer::OnRightDown(
	wxMouseEvent& event
	)
{
	if(!m_renderEntity)
	{
		return;
	}

	lastPosX = event.m_x;
	lastPosY = event.m_y;
}

void GLRenderer::OnMouseWheelScroll(
	wxMouseEvent& event
	)
{
	if(!m_renderEntity)
	{
		return;
	}
	int wheelRotation = event.GetWheelRotation();
	m_camera->Zoom(-(wheelRotation / event.GetWheelDelta()) * 10);
}

void GLRenderer::OnMouseWheelDown(
	wxMouseEvent& event
	)
{
	if(!m_renderEntity)
	{
		return;
	}

	lastPosX = event.m_x;
	lastPosY = event.m_y;
}

void GLRenderer::OnMouseMove(
	wxMouseEvent& event
	)
{
	if(!m_renderEntity)
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
			m_renderEntity->Rotate((diffX / wxSystemSettings::GetMetric (wxSYS_SCREEN_X)) * 400, (diffY / wxSystemSettings::GetMetric (wxSYS_SCREEN_Y)) * 400);
		}
	}

	lastPosX = posX;
	lastPosY = posY;


}

void GLRenderer::InitGL()
{
	if(m_context)
	{
		wxLogDebug("GL already initialised");
		return;
	}

	m_context = new wxGLContext(this);

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

	GLUtils::CheckOpenGLError(__FILE__,__LINE__);

	m_shaderManager->InitialiseShaders(); //TODO failure message when failed?

	GLUtils::CheckOpenGLError(__FILE__,__LINE__);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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

	//DebugPrintGLInfo();
}

void GLRenderer::Paint(
	wxPaintEvent& WXUNUSED(event)
	)
{
	wxPaintDC(this);
	Render();
}

void GLRenderer::RenderImmediate()
{
	wxClientDC(this);
	Render();
}

void GLRenderer::Render(
	)
{
	// TODO wxwidgets has not init callback so this has to be done here. bleugh.
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

	GLUtils::CheckOpenGLError(__FILE__,__LINE__);

	SetCurrent(*m_context);

		// for each in batchlist
	// set transform matrix
	// if different shader bind shader
	// if different shader params setup params
	// if different mesh bind vertex array
	// draw

	//glClearColor(1.0f,0.5f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);

	if(!m_renderEntity)
	{
		glFlush();
		SwapBuffers();
		return;
	}

	glm::mat4x4& viewMatrix = m_camera->GetViewMatrix();

	int width = GetSize().GetWidth();
	int height = GetSize().GetHeight();

	glm::mat4x4& projectionMatrix = glm::perspective(40.0f, (float)width / (float) height, 1.0f, 600.f);

	
	// Update inputs for current shader
	m_renderEntity->Render(*m_shaderManager, viewMatrix, projectionMatrix);

	glFlush();

	SwapBuffers();
}

GLRenderer::~GLRenderer()
{
	delete m_context;
	m_glContext = NULL;

	delete m_camera;
	m_camera = NULL;

	delete m_shaderManager;
	m_shaderManager = NULL;
}
}
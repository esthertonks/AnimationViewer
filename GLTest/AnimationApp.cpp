#include "AnimationApp.h"
#include "Render/GLRenderCanvas.h"
#include "Render/Window.h"
#include "Import/FBXImport.h"
#include "ImportMesh/Mesh.h"
#include "Render\Renderable.h"
#include "Render\RenderableMesh.h"

IMPLEMENT_APP(AnimationApp)

bool AnimationApp::OnInit()
{	
	wxFrame *frame = new render::Window(NULL, wxT("Testing"), wxDefaultPosition, wxSize(800, 800), wxDEFAULT_FRAME_STYLE);
	
	m_renderCanvas = new render::GLRenderCanvas(frame, wxID_ANY, wxDefaultPosition, wxSize(800, 800), wxSUNKEN_BORDER, "Animation App");
	m_fbxImporter = boost::shared_ptr<import::FBXImport>(new import::FBXImport());

	m_currentMesh = NULL;

	frame->Show(TRUE);

	SetTopWindow(frame);

	StartRendering();
	return TRUE;
}

void AnimationApp::OnIdle(
	wxIdleEvent& evt
	)
{
	//Prepare Mesh - update animation
	DWORD timeNow = timeGetTime();
	//BatchList
	//if(m_renderCanvas->hasEntities())
	//{
		float delta = 0.001f * timeNow - m_lastTime;

		float fps = 1/delta;

		//m_input->Do SomethingWithCamera();
		//m_camera->GetView();
		//animate(delta);
		m_renderCanvas->RenderImmediate();
	//}
	m_lastTime = timeNow;

	evt.RequestMore(); // Request continuous rendering, rather than just once on idle
}

void AnimationApp::StartRendering()
{
	Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(AnimationApp::OnIdle));
}

void AnimationApp::StopRendering()
{
	Disconnect(wxEVT_IDLE, wxIdleEventHandler(AnimationApp::OnIdle));
}

void AnimationApp::ImportFBX(
	std::string filePath
	)
{
	//TODO should the render mesh be a different type of mesh?
	//TODO should the processing be done right away by the importer and the rest of the app only knows about the render compatible mesh?
	mesh::Mesh* importMesh = m_fbxImporter->Import(filePath);
	if(importMesh)
	{
		render::RenderableMeshPtr renderableMesh = render::RenderableMeshPtr(new render::RenderableMesh());

		if(renderableMesh->Create(*importMesh))
		{
			render::RenderablePtr renderable = boost::static_pointer_cast<render::Renderable>(renderableMesh);
			m_renderCanvas->AddRenderable(renderable);
			m_currentMesh = renderable;
		}
		delete importMesh;
	}
	else
	{
		m_renderCanvas->RemoveRenderable(m_currentMesh);
		m_currentMesh = NULL;
	}

}

void AnimationApp::Destroy()
{
	delete m_renderCanvas;//TODO putting this in a shared ptr doesnt work but may have been unrelated memory issue - try again?
}


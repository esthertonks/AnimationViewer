#include "AnimationApp.h"
#include "Render/GLRenderer.h"
#include "Render/Window.h"
#include "Import/FBXImport.h"
#include "ImportMesh/Mesh.h"
#include "Batch/BatchProcessor.h"
#include "Batch/BatchList.h"
#include "Batch/Batch.h"

IMPLEMENT_APP(AnimationApp)

bool AnimationApp::OnInit()
{
	m_batchList = NULL;
	
	wxFrame *frame = new render::Window(NULL, wxT("Testing"), wxDefaultPosition, wxSize(800, 800), wxDEFAULT_FRAME_STYLE);
	
	m_renderer = new render::GLRenderer(frame, wxID_ANY, wxDefaultPosition, wxSize(800, 800), wxSUNKEN_BORDER, "Animation App");
	m_fbxImporter = new import::FBXImport();

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
	if(m_batchList)
	{
		float delta = 0.001f * timeNow - m_lastTime;

		float fps = 1/delta;

		//m_input->Do SomethingWithCamera();
		//m_camera->GetView();
		//animate(delta);
		m_renderer->RenderImmediate();
	}
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
		if(m_batchList)
		{
			delete m_batchList;
			m_batchList = NULL;
		}
		batch::BatchProcessor meshBatchProcessor;
		m_batchList = meshBatchProcessor.CreateBatches(*importMesh);

		for(render::Batch *node = m_batchList->GetNodeHierarchy(); node != NULL; node = node->m_next)
		{
			m_renderer->Prepare(*node);
		}
	}

}

void AnimationApp::DeleteMesh()
{
	if(m_batchList)
	{
		delete m_batchList;
		m_batchList = NULL;
	}
}

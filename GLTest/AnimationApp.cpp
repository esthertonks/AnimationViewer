#include "AnimationApp.h"
#include "Render/GLCanvas.h"
#include "Render/Window.h"
#include "Import/FBXImport.h"
#include "ImportMesh/Mesh.h"
#include "Batch/BatchProcessor.h"

IMPLEMENT_APP(AnimationApp)

bool AnimationApp::OnInit()
{
	m_renderMesh = NULL;

	wxFrame *frame = new Render::Window(NULL, wxT("Testing"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	
	m_canvas = new Render::GLCanvas(frame, wxID_ANY, wxDefaultPosition, wxSize(300, 300), wxSUNKEN_BORDER, "Animation App");
	m_fbxImporter = new Import::FBXImport();

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

	//RenderMesh
	m_canvas->GetRenderer()->SetMesh(m_renderMesh);
	m_canvas->RenderImmediate();
	evt.RequestMore(); // render continuously, not only once on idle
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
		batch::BatchProcessor meshBatchProcessor;
		m_renderMesh = &meshBatchProcessor.ProcessImportMesh(*importMesh);
	}

}

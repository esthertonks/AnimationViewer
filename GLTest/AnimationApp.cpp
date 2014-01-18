#include "AnimationApp.h"
#include "Render/GLRenderCanvas.h"
#include "Render/Window.h"
#include "Import/FBXImport.h"
#include "Mesh/Mesh.h"
#include "RenderableMesh/Renderable.h"
#include "RenderableMesh/RenderableMesh.h"
#include "RenderableMesh/RenderableBoneList.h"
#include "GUI/ControlsPanel.h"
#include "Mesh/AnimationInfo.h"
#include "Animation/AnimationController.h"

const int ID_BONES_CHECKBOX = 100;
const int ID_NORMALS_CHECKBOX = 101;

IMPLEMENT_APP(AnimationApp)

bool AnimationApp::OnInit()
{	
	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	wxFrame *frame = new render::Window(NULL, wxT("Animation App"), wxDefaultPosition, wxSize(width, height), wxDEFAULT_FRAME_STYLE);
	frame->SetBackgroundColour(wxColour(25.0f, 25.0f, 25.0f));
	frame->SetMinSize(wxSize(800, 800));
	frame->SetMaxSize(wxSize(width, height));
	//m_renderCanvas = new render::GLRenderCanvas(frame, wxID_ANY, wxDefaultPosition, wxSize(800, 800), wxSUNKEN_BORDER, "Animation App");
	m_renderCanvas = new render::GLRenderCanvas(frame);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *leftVerticalSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rightVerticalSizer = new wxBoxSizer(wxVERTICAL);

	gui::ControlsPanel *leftPanel = new gui::ControlsPanel(frame, -1, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER);//TODO create class

	leftVerticalSizer->Add(leftPanel, 1, wxALIGN_LEFT | wxEXPAND, 0);
	rightVerticalSizer->Add(m_renderCanvas, 1, wxALIGN_RIGHT | wxEXPAND, 0);
	horizontalSizer->Add(leftVerticalSizer, 25, wxEXPAND);
	horizontalSizer->Add(rightVerticalSizer, 75, wxEXPAND);
	frame->SetSizer(horizontalSizer);
	//frame->SetAutoLayout(true);

	m_fbxImporter = boost::shared_ptr<import::FBXImport>(new import::FBXImport());
	m_meshAnimator = boost::shared_ptr<animation::AnimationController>(new animation::AnimationController());

	m_currentMeshInfo.m_renderMesh = NULL;
	m_currentMeshInfo.m_mesh = NULL;

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
	if(m_currentMeshInfo.m_renderMesh)
	{
		// Update animation
		if(m_currentMeshInfo.m_mesh && m_meshAnimator && m_meshAnimator->IsAnimating())
		{
			//float delta = timeNow - m_lastTime;
			m_meshAnimator->Update(m_currentMeshInfo.m_mesh, timeNow, false);

			if(m_currentMeshInfo.m_renderMesh)// TODO should just be an array of renderables to update? What about the array of renderable in the render component???
			{
				m_currentMeshInfo.m_renderMesh->Update(m_currentMeshInfo.m_mesh->GetNodeHierarchy());
			}

			// Update any render meshes with the new bone hierarchy
			if(m_boneOverlay)
			{
				// TODO this should have the bone hierarchy passed in? Certainly shouldn't be storing the mesh....
				m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetNodeHierarchy()); // Update the render mesh with the new bone info
			}
		}

		//m_input->Do SomethingWithCamera();
		//m_camera->GetView();
		//animate(delta);
	}
	m_renderCanvas->RenderImmediate();
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
	m_currentMeshInfo.m_mesh = m_fbxImporter->Import(filePath);
	if(m_currentMeshInfo.m_mesh)
	{
		render::RenderableMeshPtr renderableMesh = render::RenderableMeshPtr(new render::RenderableMesh());

		if(renderableMesh->Create(m_currentMeshInfo.m_mesh))
		{
			render::RenderablePtr renderable = boost::static_pointer_cast<render::Renderable>(renderableMesh);
			m_renderCanvas->AddRenderable(renderable);
			m_currentMeshInfo.m_renderMesh = renderable;

			// Make sure the bone hierarchy is correct //TODO but it always will be yes? May not be necessary
			m_meshAnimator->Update(m_currentMeshInfo.m_mesh, 0, false);
			m_currentMeshInfo.m_renderMesh->Update(m_currentMeshInfo.m_mesh->GetNodeHierarchy());
		}
		else
		{
			m_currentMeshInfo.m_renderMesh = NULL;
			m_currentMeshInfo.m_mesh = NULL;
		}
	}
	else
	{
		m_renderCanvas->RemoveRenderable(m_currentMeshInfo.m_renderMesh);
		m_currentMeshInfo.m_renderMesh = NULL;
	}

}

void AnimationApp::ShowBones(
	bool show
	)
{
	//TODO actually better to create this on mesh load maybe???

	if(show && m_currentMeshInfo.m_mesh) // If there is no mesh do nothing
	{
		render::RenderablePtr renderable = boost::static_pointer_cast<render::Renderable>(render::RenderableBoneListPtr(new render::RenderableBoneList()));

		if(renderable->Create(m_currentMeshInfo.m_mesh))
		{
			m_renderCanvas->AddRenderable(renderable);
			m_boneOverlay = renderable;
			m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetNodeHierarchy());
		}
		else
		{
			m_boneOverlay = NULL;
		}
	}
	else
	{
		m_renderCanvas->RemoveRenderable(m_boneOverlay);
		m_boneOverlay = NULL;
	}

}

void AnimationApp::ShowNormals(
	bool show
	)
{

}

void AnimationApp::ShowMesh(
	bool show
	)
{
	if(show && m_currentMeshInfo.m_renderMesh)
	{
		m_renderCanvas->AddRenderable(m_currentMeshInfo.m_renderMesh);
	}
	else
	{
		m_renderCanvas->RemoveRenderable(m_currentMeshInfo.m_renderMesh);
	}
}

void AnimationApp::PlayAnimation()
{
	if(!m_currentMeshInfo.m_mesh)
	{
		return;
	}

	mesh::AnimationInfoPtr animationInfo = m_currentMeshInfo.m_mesh->GetAnimationInfo();
	if(m_meshAnimator)
	{
		long animStartTime = animationInfo->ConvertFrameToMilliseconds(animationInfo->GetStartSample());
		long animEndTime = animationInfo->ConvertFrameToMilliseconds(animationInfo->GetEndSample());

		m_meshAnimator->StartAnimation(timeGetTime(), animStartTime, animEndTime);
	}
}

void AnimationApp::PauseAnimation()
{
	if(m_meshAnimator)
	{
		m_meshAnimator->PauseAnimation();
	}
}

void AnimationApp::StopAnimation()
{
	if(m_meshAnimator)
	{
		m_meshAnimator->StopAnimation();
	}
}

void AnimationApp::Destroy()
{
	delete m_renderCanvas;//TODO putting this in a shared ptr doesnt work but may have been unrelated memory issue - try again?
}


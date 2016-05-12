#include "AnimationApp.h"
#include "Render/GLRenderCanvas.h"
#include "GUI/Window.h"
#include "Import/FBXImport.h"
#include "Mesh/Mesh.h"
#include "Mesh/BoneNode.h"
#include "Mesh/MeshNode.h" //TODO why?
#include "RenderableMesh/Renderable.h"
#include "RenderableMesh/RenderableMesh.h"
#include "RenderableMesh/RenderableBoneList.h"
#include "RenderableMesh/RenderableNormalsList.h"
#include "GUI/ControlsPanel.h"
#include "GUI/HierarchyPanel.h"
#include "GUI/ViewPanel.h"
#include "Mesh/AnimationInfo.h"
#include "Animation/AnimationController.h"

IMPLEMENT_APP(AnimationApp)

wxColour AnimationApp::m_guiBackgroundColour(50, 50, 50);
wxColour AnimationApp::m_guiTextColour(190, 190, 170);

bool AnimationApp::OnInit()
{	
	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	wxFrame *frame = new gui::Window(NULL, wxT("Animation App"), wxDefaultPosition, wxSize(width, height), wxDEFAULT_FRAME_STYLE);
	frame->SetBackgroundColour(m_guiBackgroundColour);
	frame->SetMinSize(wxSize(800, 800));
	frame->SetMaxSize(wxSize(width, height));
	//m_renderCanvas = new render::GLRenderCanvas(frame, wxID_ANY, wxDefaultPosition, wxSize(800, 800), wxSUNKEN_BORDER, "Animation App");
	wxGLAttributes attributes;

	// Defaults should be accepted
	attributes.PlatformDefaults().Defaults().EndList();
	attributes.AddAttribute(WX_GL_CORE_PROFILE);

	bool accepted = wxGLCanvas::IsDisplaySupported(attributes);

	if (!accepted)
	{
		// Try again without sample buffers
		attributes.Reset();
		attributes.AddAttribute(WX_GL_CORE_PROFILE);
		attributes.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();
		accepted = wxGLCanvas::IsDisplaySupported(attributes);

		if (!accepted)
		{
			wxMessageBox("Error - Graphics card does not support required openGL attributes. The application will now exit.", "openGL error", wxOK | wxICON_ERROR);
			return FALSE;
		}
	}

	m_renderCanvas = new render::GLRenderCanvas(frame, attributes);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *leftVerticalSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rightVerticalSizer = new wxBoxSizer(wxVERTICAL);

	m_controlsPanel = new gui::ControlsPanel(frame, -1, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER);//TODO create class
	m_controlsPanel->GetViewPanel().Initialise(m_renderCanvas->GetLightPosition(), m_boneOverlay != NULL ? true : false, true, false);
	leftVerticalSizer->Add(m_controlsPanel, 1, wxEXPAND, 0);
	rightVerticalSizer->Add(m_renderCanvas, 1, wxEXPAND, 0);
	horizontalSizer->Add(leftVerticalSizer, 25, wxEXPAND);
	horizontalSizer->Add(rightVerticalSizer, 75, wxEXPAND);
	frame->SetSizer(horizontalSizer);
	//frame->SetAutoLayout(true);

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

	if(m_currentMeshInfo.m_renderMesh)
	{
		// Update animation
		if(m_currentMeshInfo.m_mesh && m_meshAnimator && m_meshAnimator->IsAnimating())
		{
			//float delta = timeNow - m_lastTime;
			m_meshAnimator->Update(m_currentMeshInfo.m_mesh, timeNow);

			if(m_currentMeshInfo.m_renderMesh)// TODO should just be an array of renderables to update? What about the array of renderable in the render component???
			{
				m_currentMeshInfo.m_renderMesh->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
			}

			// Update any render meshes with the new bone hierarchy
			if(m_boneOverlay)
			{
				// TODO this should have the bone hierarchy passed in? Certainly shouldn't be storing the mesh....
				m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy()); // Update the render mesh with the new bone info
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
	if(m_currentMeshInfo.m_renderMesh != NULL)
	{
		CloseFBX();
	}
	//TODO should the render mesh be a different type of mesh?
	//TODO should the processing be done right away by the importer and the rest of the app only knows about the render compatible mesh?
	
	boost::shared_ptr<import::FBXImport> m_fbxImporter(new import::FBXImport());
	m_currentMeshInfo.m_mesh = m_fbxImporter->Import(filePath);
	if(m_currentMeshInfo.m_mesh)
	{
		render::RenderableMeshPtr renderableMesh = render::RenderableMeshPtr(new render::RenderableMesh(m_currentMeshInfo.m_mesh));

		if(renderableMesh->Create())
		{
			render::RenderablePtr renderable = boost::static_pointer_cast<render::Renderable>(renderableMesh);
			m_renderCanvas->AddRenderable(renderable);
			m_currentMeshInfo.m_renderMesh = renderable;

			// Make sure the bone hierarchy is correct //TODO but it always will be yes? May not be necessary
			m_meshAnimator->Update(m_currentMeshInfo.m_mesh, 0);
			m_currentMeshInfo.m_renderMesh->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());

			m_controlsPanel->GetHierarchyPanel().SetBoneHierarchy(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
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

void AnimationApp::CloseFBX()
{
	m_meshAnimator->StopAnimation();

	if(m_currentMeshInfo.m_renderMesh)
	{
		m_renderCanvas->RemoveRenderable(m_currentMeshInfo.m_renderMesh);
		m_currentMeshInfo.m_renderMesh = NULL;
	}
	m_currentMeshInfo.m_mesh = NULL;

	if(m_boneOverlay)
	{
		m_renderCanvas->RemoveRenderable(m_boneOverlay);
		m_boneOverlay = NULL;
	}

	m_controlsPanel->GetHierarchyPanel().ClearData();
}

void AnimationApp::ShowBones(
	bool show
	)
{
	if(show && m_currentMeshInfo.m_mesh) // If there is no mesh do nothing
	{
		render::RenderablePtr renderable = boost::static_pointer_cast<render::Renderable>(render::RenderableBoneListPtr(new render::RenderableBoneList()));

		if(renderable->Create())
		{
			m_renderCanvas->AddOverlay(renderable);
			m_boneOverlay = renderable;
			m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
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

void AnimationApp::ShowNormals(
	bool show
)
{
	if (show && m_currentMeshInfo.m_mesh) // If there is no mesh do nothing
	{
		render::RenderablePtr renderable = boost::static_pointer_cast<render::Renderable>(render::RenderableNormalsListPtr(new render::RenderableNormalsList()));

		if (renderable->Create())
		{
			m_renderCanvas->AddOverlay(renderable);
			m_normalsOverlay = renderable;
			//m_normalsOverlay->Update(m_currentMeshInfo.m_renderMesh->);
		}
		else
		{
			m_normalsOverlay = NULL;
		}
	}
	else
	{
		m_renderCanvas->RemoveRenderable(m_normalsOverlay);
		m_normalsOverlay = NULL;
	}
}

void AnimationApp::PlayAnimation()
{
	if(!m_currentMeshInfo.m_mesh)
	{
		return;
	}

	if(m_meshAnimator)
	{
		if(m_meshAnimator->IsPaused())
		{
			m_meshAnimator->ResumeAnimation();
		}
		else
		{
			mesh::AnimationInfoPtr animationInfo = m_currentMeshInfo.m_mesh->GetAnimationInfo();

			long animStartTime = animationInfo->ConvertFrameToMilliseconds(animationInfo->GetStartSample());
			long animEndTime = animationInfo->ConvertFrameToMilliseconds(animationInfo->GetEndSample());

			m_meshAnimator->StartAnimation(timeGetTime(), animStartTime, animEndTime);
		}
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
	if(m_meshAnimator && m_currentMeshInfo.m_mesh)
	{
		m_meshAnimator->StopAnimation();
		m_meshAnimator->Update(m_currentMeshInfo.m_mesh, 0);

		if(m_currentMeshInfo.m_renderMesh)
		{
			m_currentMeshInfo.m_renderMesh->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
		}
	}
}

void AnimationApp::LoopAnimation(
	bool looping
	)
{
	if(m_meshAnimator)
	{
		m_meshAnimator->SetLooping(looping);
	}
}

void AnimationApp::CentreCamera()
{
	//TODO focus on mesh if the mesh has moved from zero
	m_renderCanvas->CentreCamera();
}

void AnimationApp::SetLightPosition(
	glm::vec4 lightPosition
	)
{
	m_renderCanvas->SetLightPosition(lightPosition);
}

void AnimationApp::Destroy()
{
	delete m_renderCanvas;//TODO putting this in a shared ptr doesnt work but may have been unrelated memory issue - try again?
}


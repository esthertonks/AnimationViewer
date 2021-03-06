#include "AnimationApp.h"
#include "Render/GLRenderCanvas.h"
#include "GUI/Window.h"
#include "Import/FBXImport.h"
#include "Mesh/Mesh.h"
#include "Mesh/BoneNode.h"
#include "Mesh/MeshNode.h" //TODO why?
#include "Renderable/Renderable.h"
#include "Renderable/RenderableMesh.h"
#include "GUI/ControlsPanel.h"
#include "GUI/HierarchyPanel.h"
#include "GUI/ViewPanel.h"
#include "Mesh/AnimationInfo.h"
#include "Animation/AnimationController.h"
#include "Renderable/RenderableVertexListFactory.h"
#include "Renderable/RenderableVertexList.h"
#include "Colours.h"

const int AnimationApp::m_boneOverlayPointSize = 10.0f;

IMPLEMENT_APP(AnimationApp)

bool AnimationApp::OnInit()
{	
	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	wxFrame *frame = new gui::Window(NULL, wxT("Animation App"), wxDefaultPosition, wxSize(width, height), wxDEFAULT_FRAME_STYLE);
	frame->SetBackgroundColour(theme::Colours::m_darkGrey.GetAsUIColour());
	frame->SetMinSize(wxSize(800, 800));
	frame->SetMaxSize(wxSize(width, height));
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

	m_controlsPanel = new gui::ControlsPanel(frame, -1, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER);
	m_controlsPanel->GetViewPanel().Initialise(m_renderCanvas->GetLightPosition(), m_boneOverlay != NULL ? true : false, true, false);
	leftVerticalSizer->Add(m_controlsPanel, 1, wxEXPAND, 0);
	rightVerticalSizer->Add(m_renderCanvas, 1, wxEXPAND, 0);
	horizontalSizer->Add(leftVerticalSizer, 10, wxEXPAND);
	horizontalSizer->Add(rightVerticalSizer, 90, wxEXPAND);
	frame->SetSizer(horizontalSizer);

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
	//PrepareForRendering Mesh - update animation
	DWORD timeNow = timeGetTime();

	if(m_currentMeshInfo.m_renderMesh)
	{
		// Update animation
		if(m_currentMeshInfo.m_mesh && m_meshAnimator && m_meshAnimator->IsAnimating())
		{
			//float delta = timeNow - m_lastTime;
			m_meshAnimator->Update(m_currentMeshInfo.m_mesh, timeNow);

			// Update any render meshes with the new bone hierarchy
			if(m_currentMeshInfo.m_renderMesh)// TODO should just be an array of renderables to update? What about the array of renderable in the render component???
			{
				m_currentMeshInfo.m_renderMesh->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
			}

			// Update the bones if visible
			if(m_boneOverlay)
			{
				m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy()); // Update the render mesh with the new bone info
			}

			// Update the normals if visible
			if (m_normalsOverlay)
			{
				m_normalsOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy()); // Update the render mesh with the new bone info

			}
		}
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
		render::RenderableVertexListPtr renderableVertexBoneListPtr = render::RenderableVertexListFactory::CreateBoneVertexList(glm::vec3(theme::Colours::m_duskyBlue.GetAsRenderColour()), m_boneOverlayPointSize);
		render::RenderablePtr renderable = boost::dynamic_pointer_cast<render::Renderable>(renderableVertexBoneListPtr);

		m_renderCanvas->AddOverlay(renderable);
		m_boneOverlay = renderable;
		m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
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
		const glm::vec3 whiteColour(1.0f, 1.0f, 1.0f);
		const float normalLength = 1.7f;
		const int pointSize = 1.0f;

		render::RenderableVertexListPtr renderableVertexNormalsListPtr = render::RenderableVertexListFactory::CreateNormalsVertexList(m_currentMeshInfo.m_mesh, whiteColour, normalLength, pointSize);
		render::RenderablePtr renderable = boost::dynamic_pointer_cast<render::Renderable>(renderableVertexNormalsListPtr);

		m_renderCanvas->AddOverlay(renderable);
		m_normalsOverlay = renderable;
		m_normalsOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
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
			if(m_boneOverlay)
			{
				m_boneOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
			}
			if (m_normalsOverlay)
			{
				m_normalsOverlay->Update(m_currentMeshInfo.m_mesh->GetBoneNodeHierarchy());
			}
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


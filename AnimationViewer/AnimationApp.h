#pragma once

//#include <wchar.h>
//#include "vld.h"
#include "wx/wx.h"
#include "Batch/BatchFwdDecl.h"
#include "Mesh/MeshFwdDecl.h"
#include "Animation/AnimationFwdDecl.h"
#include "Renderable/RenderableFwdDecl.h"

namespace render
{
	class GLRenderCanvas;
	class Batch;
}

namespace gui
{
	class ControlsPanel;
}

namespace import
{
	class FBXImport;
}

class AnimationApp: public wxApp
{
	public:

	void Destroy();
	void StartRendering();
	void StopRendering();
	render::GLRenderCanvas& GetRenderer()
	{
		return *m_renderCanvas;
	};

	// Menu controls
	void ImportFBX(
		std::string filePath
	);

	void CloseFBX();

	// GUI controls
	void ShowBones(
		bool show
		);

	void ShowNormals(
		bool show
		);

	void ShowMesh(
		bool show
		);

	void PlayAnimation();
	void PauseAnimation();
	void StopAnimation();
	void LoopAnimation(
		bool looping
		);

	void CentreCamera();

	void SetLightPosition(
		glm::vec4 lightPosition
	);

	private:

	virtual bool OnInit();
	void OnIdle(
		wxIdleEvent& evt
		);

	render::GLRenderCanvas *m_renderCanvas;
	DWORD m_lastTime;

	// Grouping of the original import mesh and the converted renderable mesh
	struct CurrentMeshInfo
	{
		render::RenderablePtr m_renderMesh;
		mesh::MeshPtr m_mesh;
	};

	CurrentMeshInfo m_currentMeshInfo;

	const static int m_boneOverlayPointSize;
	render::RenderablePtr m_boneOverlay;
	render::RenderablePtr m_normalsOverlay;
	animation::AnimationControllerPtr m_meshAnimator;

	gui::ControlsPanel *m_controlsPanel;
};

DECLARE_APP(AnimationApp)

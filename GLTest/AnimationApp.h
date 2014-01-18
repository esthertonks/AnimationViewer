#pragma once

//#include <wchar.h>
//#include "vld.h"
#include "wx/wx.h"
#include "Batch\BatchFwdDecl.h"
#include "Mesh\MeshFwdDecl.h"
#include "Animation\AnimationFwdDecl.h"

namespace render
{
	class GLRenderCanvas;
	class Batch;
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

	void ImportFBX(
		std::string filePath
	);

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

	private:
	virtual bool OnInit();
	void OnIdle(
		wxIdleEvent& evt
		);

	render::GLRenderCanvas *m_renderCanvas;
	boost::shared_ptr<import::FBXImport> m_fbxImporter;
	DWORD m_lastTime;

	// Grouping of the original import mesh and the converted renderable mesh
	struct CurrentMeshInfo
	{
		render::RenderablePtr m_renderMesh;
		mesh::MeshPtr m_mesh;
	};

	CurrentMeshInfo m_currentMeshInfo;

	render::RenderablePtr m_boneOverlay;//TODO so....where does this live? Needs storing so we can remove it from the renderer when we check bones off
	animation::AnimationControllerPtr m_meshAnimator;
};

DECLARE_APP(AnimationApp)

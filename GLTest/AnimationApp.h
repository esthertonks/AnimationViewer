#pragma once

#include "wx/wx.h"
#include <vector>
#include <boost\shared_ptr.hpp>
#include "Batch\BatchFwdDecl.h"

namespace render
{
	class GLRenderer;
	class RenderEntity;
}

namespace import
{
	class FBXImport;
}

namespace render
{
	class Batch;
	class RenderEntitiy;
}


class AnimationApp: public wxApp
{
	public:
	void StartRendering();
	void StopRendering();
	render::GLRenderer& GetRenderer()
	{
		return *m_renderer;
	};

	void ImportFBX(
		std::string filePath
	);

	void DeleteMesh();

	private:
	virtual bool OnInit();
	void OnIdle(
		wxIdleEvent& evt
		);

	render::GLRenderer *m_renderer;
	import::FBXImport *m_fbxImporter;
	DWORD m_lastTime;
	render::RenderEntity *m_renderEntity;
};

DECLARE_APP(AnimationApp)

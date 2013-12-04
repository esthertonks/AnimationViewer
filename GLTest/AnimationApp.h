#pragma once

#include <wchar.h>
#include "vld.h"
#include "wx/wx.h"
#include "Batch\BatchFwdDecl.h"

namespace render
{
	class GLRenderer;
}

namespace import
{
	class FBXImport;
}

namespace render
{
	class Batch;
}


class AnimationApp: public wxApp
{
	public:

	void Destroy();
	void StartRendering();
	void StopRendering();
	render::GLRenderer& GetRenderer()
	{
		return *m_renderer;
	};

	void ImportFBX(
		std::string filePath
	);

	private:
	virtual bool OnInit();
	void OnIdle(
		wxIdleEvent& evt
		);

	render::GLRenderer *m_renderer;
	boost::shared_ptr<import::FBXImport> m_fbxImporter;
	DWORD m_lastTime;
	render::RenderablePtr m_currentMesh;
};

DECLARE_APP(AnimationApp)

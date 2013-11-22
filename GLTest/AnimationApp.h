#pragma once

#include "wx/wx.h"
#include <vector>
#include <boost\shared_ptr.hpp>
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
	render::BatchList m_renderBatches;
	DWORD m_lastTime;
};

DECLARE_APP(AnimationApp)

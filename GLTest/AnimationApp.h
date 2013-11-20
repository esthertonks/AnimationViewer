#pragma once

#include "wx/wx.h"

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
	class BatchList;
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
	render::BatchList *m_batchList; // List of batches to render sorts by material
	DWORD m_lastTime;
};

DECLARE_APP(AnimationApp)

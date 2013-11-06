#pragma once

#include "wx/wx.h"

namespace Render
{
	class GLCanvas;
}

namespace Import
{
	class FBXImport;
}


class AnimationApp: public wxApp
{
	public:
	void StartRendering();
	void StopRendering();
	Render::GLCanvas& GetCanvas()
	{
		return *m_canvas;
	};

	void ImportFBX(
		std::string filePath
	);

	private:
	virtual bool OnInit();
	void OnIdle(
		wxIdleEvent& evt
		);

	Render::GLCanvas *m_canvas;
	Import::FBXImport *m_fbxImporter;
	mesh::Mesh *m_renderMesh; // Only ever one mesh to render at once right now
};

DECLARE_APP(AnimationApp)

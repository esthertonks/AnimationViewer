#pragma once

#include "wx/wx.h"

namespace render
{
	class GLCanvas;
}

namespace import
{
	class FBXImport;
}

namespace mesh
{
	class RenderMesh;
	class RenderMeshNode;
}


class AnimationApp: public wxApp
{
	public:
	void StartRendering();
	void StopRendering();
	render::GLCanvas& GetCanvas()
	{
		return *m_canvas;
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

	render::GLCanvas *m_canvas;
	import::FBXImport *m_fbxImporter;
	mesh::RenderMesh *m_renderMesh; // Only ever one mesh to render at once right now
};

DECLARE_APP(AnimationApp)

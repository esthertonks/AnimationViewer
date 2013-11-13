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
	mesh::RenderMesh *m_renderMesh; // Only ever one mesh to render at once right now
	DWORD m_lastTime;
};

DECLARE_APP(AnimationApp)

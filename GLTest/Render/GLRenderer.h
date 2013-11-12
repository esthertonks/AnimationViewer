#pragma once

// TODO general includes in its own header?
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <map>
#include <string>
#include <vector>

namespace mesh
{
	class RenderMeshNode;
}

namespace render
{
	class Camera;

class GLRenderer : public wxGLCanvas
{
public:

	GLRenderer(
		wxWindow *parent, 
		wxWindowID id, 
		const wxPoint& position,
		const wxSize& size, 
		long style, 
		const wxString& name
	);

	~GLRenderer();

	Camera& GetCamera()
	{
		return *m_camera;
	}

	void Paint(
		wxPaintEvent& event
		);

	void RenderImmediate(
		);

	void OnSize(
		wxSizeEvent& event
		);

	void OnEraseBackground(
		wxEraseEvent& event
		);

	void OnSetFocus(
		wxFocusEvent& event
		);

	void OnKeyDown(
		wxKeyEvent& event
		);

	void OnLeftDown(
		wxMouseEvent& event
		);

	void OnRightDown(
		wxMouseEvent& event
		);

	void OnMouseMove(
		wxMouseEvent& event
		);

	void OnMouseWheel(
		wxMouseEvent& event
		);

	void InitGL();

	int CheckOpenGLError(
		const char * file, 
		int line
		);

	void DebugPrintGLInfo();

		inline GLuint GetProgramHandle()
	{
		return m_programHandle;
	}

	inline GLuint GetVertexArrayHandle()
	{
		return m_vertexArrayHandle;
	}

	bool Initialised()
	{
		return m_initialised;
	}

	bool LoadShaders(
		const std::map<std::string, GLuint> &defaultShaderList
		);

	void Prepare(
		mesh::RenderMeshNode &renderMeshNode
		/*BatchList renderBatchList*/
		);

protected:
	DECLARE_EVENT_TABLE()

private:
	void Render();
		
	GLuint LoadShader(
		const std::string &shaderName, // The name of the shader to load
		const GLenum shaderType	// The type of the shader ie GL_VERTEX_SHADER etc
	);

	const char* ReadShaderSourceFile(
		const std::string &shaderName
	);

	GLenum CompileShader(
		const std::string &shaderName, // The name of the shader to load
		const GLuint shaderId //GLuint id for the shader source
	);

	GLenum LinkProgram(
		const std::vector<GLuint> &shaders
		);

	void OutputDebugShaderAttributeInfo();

	wxGLContext* m_context;

	GLint m_programHandle;
	GLuint m_vertexArrayHandle;

	GLuint m_indexBufferHandle;
	GLuint m_positionBufferHandle;

	std::vector<GLuint> shaders;

	bool m_initialised;
	bool m_meshLoaded;

	int currentNumIndices; //temp!!!!

	Camera *m_camera;
};

}


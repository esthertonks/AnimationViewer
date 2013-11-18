#include "GLRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <wx/log.h>
#include <fstream>

#include "../BatchMesh/RenderVertex.h"
#include "../BatchMesh/RenderMeshNode.h"
#include "OrbitCamera.h"

namespace render
{

BEGIN_EVENT_TABLE(GLRenderer, wxGLCanvas)
	EVT_PAINT(GLRenderer::Paint)
	EVT_SIZE(GLRenderer::OnSize)
	EVT_ERASE_BACKGROUND(GLRenderer::OnEraseBackground)
	EVT_SET_FOCUS(GLRenderer::OnSetFocus)

	EVT_LEFT_DOWN(GLRenderer::OnLeftDown)
	EVT_RIGHT_DOWN(GLRenderer::OnRightDown)
	EVT_MOTION(GLRenderer::OnMouseMove)
	EVT_MOUSEWHEEL(GLRenderer::OnMouseWheelScroll)
	EVT_MIDDLE_DOWN(GLRenderer::OnMouseWheelDown)

	EVT_KEY_DOWN(GLRenderer::OnKeyDown)
END_EVENT_TABLE()

GLRenderer::GLRenderer(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint& position,
	const wxSize& size,
	long style, 
	const wxString& name
	)
	: wxGLCanvas(parent, id, position, size, style|wxFULL_REPAINT_ON_RESIZE, name), 
	m_context(NULL),
	m_camera(new OrbitCamera(glm::vec3(100.0f, 0.0f, 0.0f)))
{
}

void GLRenderer::InitGL()
{
	if(m_context)
	{
		wxLogDebug("GL already initialised");
		return;
	}

	m_initialised = false;
	m_meshLoaded = false;
	currentNumIndices = -1;

	m_context = new wxGLContext(this);

	SetCurrent(*m_context);

	//TODO calling this here causes a GL expection - why???
	//DebugPrintGLInfo();

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if(status != GLEW_OK) //TODO - quit here and it leaks like a sieve.
	{
		wxLogDebug("Error initializing GLEW: ");
		char *errorText = (char*)glewGetErrorString(status);
		wxLogDebug(errorText);
		return;
	}

	CheckOpenGLError(__FILE__,__LINE__);

	std::map<std::string, GLuint> defaultShaderList; //TODO load from file system
	defaultShaderList.insert(std::pair<std::string, GLuint>("Shaders/phong.vert", GL_VERTEX_SHADER));
	defaultShaderList.insert(std::pair<std::string, GLuint>("Shaders/phong.frag", GL_FRAGMENT_SHADER));

	LoadShaders(defaultShaderList);
	CheckOpenGLError(__FILE__,__LINE__);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	int width = GetSize().GetWidth();
	int height = GetSize().GetHeight();

//	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
//	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glCullFace(GL_BACK);

	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	//DebugPrintGLInfo();
}

void GLRenderer::OnSize(
	wxSizeEvent& event
	)
{
    // Necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

	if(m_context)
	{
		SetCurrent(*m_context);
	}

	int width = event.GetSize().GetWidth();
	int height = event.GetSize().GetHeight();

//	int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
//	int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
	glViewport(0, 0, (GLint)width, (GLint)height);

	//Refresh();
	Render();
}

void GLRenderer::OnEraseBackground(
	wxEraseEvent& WXUNUSED(event)
	)
{
	// Make sure this does nothing to prevent flickering on resize
}

void GLRenderer::OnSetFocus(
	wxFocusEvent& WXUNUSED(event)
	)
{
}

void GLRenderer::OnKeyDown(
	wxKeyEvent& event
	)
{
	wxString key;
	long keycode = event.GetKeyCode();
	switch (keycode)
	{
		case WXK_NUMPAD_PAGEUP:

			break;
		case WXK_NUMPAD_PAGEDOWN:

			break;

		case 'F':
			//TODO focus on mesh if the mesh has moved from zero
			m_camera->Reset();
			break;
	}
}

static float lastPosX = -1;
static float lastPosY = -1;

void GLRenderer::OnLeftDown(
	wxMouseEvent& event
	)
{
	lastPosX = event.m_x;
	lastPosY = event.m_y;

	event.Skip();
}

void GLRenderer::OnRightDown(
	wxMouseEvent& event
	)
{
	lastPosX = event.m_x;
	lastPosY = event.m_y;
}

void GLRenderer::OnMouseWheelScroll(
	wxMouseEvent& event
	)
{
	int wheelRotation = event.GetWheelRotation();
	m_camera->Zoom(-(wheelRotation / event.GetWheelDelta()) * 10);
}

void GLRenderer::OnMouseWheelDown(
	wxMouseEvent& event
	)
{
	lastPosX = event.m_x;
	lastPosY = event.m_y;
}

void GLRenderer::OnMouseMove(
	wxMouseEvent& event
	)
{
	//Check that a mouse button was actually pressed before mouse move
	if(lastPosX == -1 || lastPosY == -1)
	{
		return;
	}

	float posX = event.m_x;
	float posY = event.m_y;

	if(event.AltDown())
	{
		float diffX = posX - lastPosX;
		float diffY = posY - lastPosY;

		if(event.LeftIsDown()) // Rotate around pivot
		{
			if(diffX != 0)
			{
				m_camera->RotateAroundY((diffX / wxSystemSettings::GetMetric (wxSYS_SCREEN_X)) * 100);
			}

			if(diffY != 0)
			{
				m_camera->RotateAroundX((diffY / wxSystemSettings::GetMetric (wxSYS_SCREEN_Y)) * 100);
			}
		}
		else if(event.RightIsDown()) // Zoom
		{
			if(diffY != 0)
			{
				m_camera->Zoom(-diffY);
			}
		}
		else if(event.MiddleIsDown())
		{
			if(diffX != 0)
			{
				int width = GetSize().GetWidth();
				int height = GetSize().GetHeight();

				m_camera->PanX(-diffX, (float)width / (float)height);
			}
			if(diffY != 0)
			{
				int width = GetSize().GetWidth();
				int height = GetSize().GetHeight();

				m_camera->PanY(diffY, (float)width / (float)height);
			}
		}
	}

	lastPosX = posX;
	lastPosY = posY;


}

int GLRenderer::CheckOpenGLError(
	const char * file, 
	int line
	)
{
	//
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	//
	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		wxLogDebug("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void GLRenderer::DebugPrintGLInfo()
{
	const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* glslVersion = reinterpret_cast<const char*>(glGetString( GL_SHADING_LANGUAGE_VERSION));

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	wxLogDebug("Renderer info: %s version %s from %s", renderer, version, vendor);
	wxLogDebug("GL version major: %s minor: %s", major, minor);

	wxLogDebug("GLSL version: %s", glslVersion);

	GLint numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	for(int extension = 0; extension < numExtensions; extension++)
	{
		wxLogDebug("Extension: %s\n", glGetStringi(GL_EXTENSIONS, extension));
	}
}

void GLRenderer::Paint(
	wxPaintEvent& WXUNUSED(event)
	)
{
	wxPaintDC(this);
	Render();
}

void GLRenderer::RenderImmediate()
{
	wxClientDC(this);
	Render();
}

bool GLRenderer::LoadShaders(
	const std::map<std::string, GLuint> &defaultShaderList
	)
{
	GLuint vertexShaderId = LoadShader("Shaders/phong.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = LoadShader("Shaders/phong.frag", GL_FRAGMENT_SHADER);

	if(!CompileShader("phong.vert", vertexShaderId))
	{
		return false;
	}

	if(!CompileShader("phong.frag", fragmentShaderId))
	{
		return false;
	}

	shaders.push_back(vertexShaderId);
	shaders.push_back(fragmentShaderId);

	if(!LinkProgram(shaders))
	{
		return false;
	}

	glUseProgram(m_programHandle);

	OutputDebugShaderAttributeInfo();

	m_initialised = true;

	return true;
}

/**
	@brief Loads a shader of the specified name and type
	@return GL_FALSE if failed or GL_TRUE if successfully loaded
*/
GLuint GLRenderer::LoadShader(
	const std::string &shaderName, // The name of the shader to load
	const GLenum shaderType	// The type of the shader ie GL_VERTEX_SHADER etc
	)
{
	const char *shaderTextBuffer = ReadShaderSourceFile(shaderName);
	if(!shaderTextBuffer)
	{
		return GL_FALSE;
	}

	// Create the shader object
	GLuint shaderId = glCreateShader(shaderType);
	if(shaderId)
	{
		// Pass the shader source to GL
		glShaderSource(shaderId, 1, &shaderTextBuffer, NULL);
	}
	else
	{
		wxLogDebug("Error creating vertex shader.\n");
	}

	delete[] shaderTextBuffer;

	return shaderId;
}

const char* GLRenderer::ReadShaderSourceFile(
	const std::string &shaderName
	)
{
	std::ifstream shaderStream(shaderName, std::ifstream::in);
	if(!shaderStream) {
		wxLogDebug("Error opening file: %s\n", shaderName);
		return NULL;
	}

	// TODO something better than a text buffer?
   char *shaderTextBuffer = new GLchar[10000];
	int i = 0;
	while(shaderStream.good()) 
	{
		int c = shaderStream.get();
		if(c > 0)
		{
			shaderTextBuffer[i++] = c;
		}
	}
	shaderStream.close();
	shaderTextBuffer[i++] = '\0';

	//wxLogDebug(buffer);
	return shaderTextBuffer;
}

/**
	@brief Compiles the shader with this shaderHandle and name
	@return GL_FALSE if failed or GL_TRUE if successfully loaded
*/
GLenum GLRenderer::CompileShader(
	const std::string &shaderName, // The name of the shader to load
	const GLuint shaderId //GLuint id for the shader source
	)
{
	// Compile the shader
	glCompileShader(shaderId);

	// Check whether the shader compiled successfully
	GLint compiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		wxLogDebug("Shader compilation failed for shader %s\n", shaderName.c_str());

		GLint errorTextLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorTextLength);

		std::string errorText(errorTextLength, ' ');

		GLsizei written;
		glGetShaderInfoLog(shaderId, errorTextLength, &written, &errorText[0]);

		wxLogDebug("Shader compilation errors: \n%s", errorText.c_str());
	}

	return compiled;
}

GLenum GLRenderer::LinkProgram(
	const std::vector<GLuint> &shaders // List of shaders needed for this shader program
	)
{
	// Create the program object
	m_programHandle = glCreateProgram();
	if(!m_programHandle)
	{
		wxLogDebug("Error creating program object.\n");
		return false;
    }

//    // Bind index 0 to the shader input variable "VertexPosition"
//    glBindAttribLocation(programHandle, 0, "VertexPosition");
//    // Bind index 1 to the shader input variable "VertexColor"
//    glBindAttribLocation(programHandle, 1, "VertexColor");

	// Attach the shaders to the program object
	std::vector<GLuint>::const_iterator iter;

	for(iter = shaders.begin(); iter != shaders.end(); iter++)
	{
		glAttachShader(m_programHandle, *iter);
	}

	// Link the program
	glLinkProgram(m_programHandle);

	//// Detach the vertex and fragment shaders from the program.
	//glDetachShader(m_programHandle, m_vertexShader);
	//glDetachShader(m_programHandle, m_fragmentShader);

	//// Delete the vertex and fragment shaders.
	//glDeleteShader(m_vertexShader);
	//glDeleteShader(m_fragmentShader);

	// Check for successful linking
	GLint linked;
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &linked);

	if(!linked)
	{
		wxLogDebug("Failed to link shader program\n" );

		GLint errorTextLength;
		glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &errorTextLength);

		std::string errorText(errorTextLength, ' ');

		GLsizei written;
		glGetShaderInfoLog(m_programHandle, errorTextLength, &written, &errorText[0]);

		wxLogDebug("Link shader errors: \n%s", errorText);
	}

	return linked;
}

void GLRenderer::OutputDebugShaderAttributeInfo()
{
	GLint maxlength, noofAttributes;
	glGetProgramiv(m_programHandle, GL_ACTIVE_ATTRIBUTES, &noofAttributes);
	glGetProgramiv(m_programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxlength);
 
	GLchar *name = (GLchar *)malloc(maxlength);
 
	GLint written, size, location;
	GLenum type;
	wxLogDebug("Index | Name\n");
	wxLogDebug("---------------------------------");
	for(int i = 0; i < noofAttributes; i++)
	{
		glGetActiveAttrib(m_programHandle, i, maxlength, &written, &size, &type, name);
 
		location = glGetAttribLocation(m_programHandle, name);
		wxLogDebug("Location %d name %s\n", location, name);
	}
 
	GLint noofUniforms;
	glGetProgramiv(m_programHandle, GL_ACTIVE_UNIFORMS, &noofUniforms);
	glGetProgramiv(m_programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlength);
 
	GLchar *uniformName = (GLchar *)malloc(maxlength);
 
	wxLogDebug("Location | Name\n");
	wxLogDebug("---------------------------------");
	for(int i = 0; i < noofUniforms; i++)
	{
		glGetActiveUniform(m_programHandle, i, maxlength, &written, &size, &type, uniformName);
 
		location = glGetUniformLocation(m_programHandle, uniformName);
		wxLogDebug("Location %d, uniformName %s\n", location, uniformName);
	}
}

void GLRenderer::Prepare(
	mesh::RenderMeshNode &renderMeshNode
	/*BatchList renderBatchList*/
	)
{
	//for each in batchlist
	//create and bind vbo - store id's in batchlist
	//load textures - store in batch list


	/////////////////// Create the VBO ////////////////////
	// Create and set-up the vertex array object
	glGenVertexArrays( 1, &m_vertexArrayHandle);
	glBindVertexArray(m_vertexArrayHandle);

	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	m_positionBufferHandle = vboHandles[0];
	m_indexBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh::RenderVertex) * renderMeshNode.GetNumVertices(), renderMeshNode.GetVertices().get(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::RenderVertex), (GLubyte *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::RenderVertex), (GLubyte *)sizeof(glm::vec3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::RenderVertex), (GLubyte *)(sizeof(glm::vec3) * 2));

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex color
	glEnableVertexAttribArray(2);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * renderMeshNode.GetNumIndices(), renderMeshNode.GetIndices().get(), GL_STATIC_DRAW);

	m_meshLoaded = true;
	currentNumIndices = renderMeshNode.GetNumIndices();
	//boost::shared_array<mesh::RenderVertex>&vertices = renderMeshNode.GetVertices();
	//for(int i = 0; i < renderMeshNode.GetNumVertices(); i++)	
	//{
	//	wxLogDebug("vert %d pos %f\n", i, vertices[i].m_position[0]);
	//	wxLogDebug("vert %d pos %f\n", i, vertices[i].m_position[1]);
	//	wxLogDebug("vert %d pos %f\n", i, vertices[i].m_position[2]);
	//}

	//boost::shared_array<unsigned int>&indices = renderMeshNode.GetIndices();
	//for(int i = 0; i < renderMeshNode.GetNumIndices(); i++)	
	//{
	//	wxLogDebug("index %d is vert index %d \n", i, indices[i]);
	//}

	//TODO load from mesh
	GLint materialAmbientLocation = glGetUniformLocation(GetProgramHandle(), "material.ambient");
	glUniform3f(materialAmbientLocation, 0.3f, 0.5f, 0.3f);

	GLint materialDiffuseLocation = glGetUniformLocation(GetProgramHandle(), "material.diffuse");
	glUniform3f(materialDiffuseLocation, 0.3f, 0.5f, 0.3f);

	GLint materialSpecularLocation = glGetUniformLocation(GetProgramHandle(), "material.specular");
	glUniform3f(materialSpecularLocation, 0.8f, 0.8f, 0.8f);

	GLint materialShininessLocation = glGetUniformLocation(GetProgramHandle(), "material.shininess");
	glUniform1f(materialShininessLocation, 100.3f);

	// TODO need to be able to add more than one light
	GLint lightAmbientLocation = glGetUniformLocation(GetProgramHandle(), "light.ambient");
	glUniform3f(lightAmbientLocation, 0.4f, 0.4f, 0.4f);

	GLint lightDiffuseLocation = glGetUniformLocation(GetProgramHandle(), "light.diffuse");
	glUniform3f(lightDiffuseLocation, 1.0f, 1.0f, 1.0f);

	GLint lightSpecularLocation = glGetUniformLocation(GetProgramHandle(), "light.specular");
	glUniform3f(lightSpecularLocation, 0.3f, 0.5f, 0.3f);

}

//void GLRenderer::Update(
//	glm::mat4x4 &projectionMatrix, 
//	glm::mat4x4 &viewMatrix
//	)
//{
//	m_cameraMatrix = cameraMatrix;
//}

void GLRenderer::Render(
	)
{
	// TODO wxwidgets has not init callback so this has to be done here. bleugh.
	if(!m_context)
	{
		//wxLogDebug("Error - no gl canvas available for rendering");
		return;
	}

	if(!Initialised())
	{
		//wxLogDebug("Error - no shaders loaded");
		return;
	}

	CheckOpenGLError(__FILE__,__LINE__);

	SetCurrent(*m_context);

		// for each in batchlist
	// set transform matrix
	// if different shader bind shader
	// if different shader params setup params
	// if different mesh bind vertex array
	// draw

	//glClearColor(1.0f,0.5f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);

	if(m_meshLoaded)
	{
		float angle = 30.0f;
		glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		//glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -20.f));
		//glm::mat4x4 modelMatrix = glm::mat4x4(1.0f);
		glm::mat4x4& viewMatrix = m_camera->GetViewMatrix();

		//int width = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
		//int height = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);

		int width = GetSize().GetWidth();
		int height = GetSize().GetHeight();

		glm::mat4x4& projectionMatrix = glm::perspective(40.0f, (float)width / (float) height, 1.0f, 600.f); 

		GLint modelMatrixLocation = glGetUniformLocation(GetProgramHandle(), "modelMatrix");
		GLint viewMatrixLocation = glGetUniformLocation(GetProgramHandle(), "viewMatrix");
		GLint projectionMatrixLocation = glGetUniformLocation(GetProgramHandle(), "projectionMatrix"); //TODO only needs setting on resize
		GLint normalMatrixLocation = glGetUniformLocation(GetProgramHandle(), "normalMatrix");
		if(modelMatrixLocation >= 0 && viewMatrixLocation >= 0 && projectionMatrixLocation >= 0)
		{
			glm::mat4x4 modelViewMatrix = viewMatrix * modelMatrix;
			glm::mat3x3 normalMatrix = glm::mat3x3(glm::vec3(modelViewMatrix[0]), glm::vec3(modelViewMatrix[1]), glm::vec3(modelViewMatrix[2]));

			GLint lightPositionLocation = glGetUniformLocation(GetProgramHandle(), "light.position");

			glm::vec4 lightPositionMatrix = viewMatrix * glm::vec4(50.0f,5.0f,2.0f,1.0f);
			glUniform4f(lightPositionLocation, lightPositionMatrix.x, lightPositionMatrix.y, lightPositionMatrix.z, lightPositionMatrix.w);

			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
			glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);

			//wxLogDebug("%u\n", GetVertexArrayHandle());
			glBindVertexArray(GetVertexArrayHandle());

			//m_indexBufferHandle
			glDrawElements(GL_TRIANGLES, currentNumIndices, GL_UNSIGNED_SHORT, (GLvoid*)0);

		}
	}

	glFlush();

	SwapBuffers();
}

GLRenderer::~GLRenderer()
{
	if(!Initialised())
	{
		return;
	}

	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(0, &m_indexBufferHandle);

	// Release the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_positionBufferHandle);

	// Release the vertex array object.
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vertexArrayHandle);

	std::vector<GLuint>::iterator iter;
	for(iter = shaders.begin(); iter != shaders.end(); iter++)
	{
		// Detach and delete any shaders
		glDetachShader(m_programHandle, *iter);
		glDeleteShader(*iter);
	}

	// Delete the shader program.
	glDeleteProgram(m_programHandle);

	delete m_context;
	m_glContext = NULL;
}
}
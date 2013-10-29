#include "GLCanvas.h"
//#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/log.h>
#include <iostream>

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_PAINT(GLCanvas::Render)
	EVT_SIZE(GLCanvas::OnSize)
	EVT_SET_FOCUS(GLCanvas::OnSetFocus)
END_EVENT_TABLE()

GLCanvas::GLCanvas(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint& position,
	const wxSize& size,
	long style, 
	const wxString& name
	)
    : wxGLCanvas(parent, id, position, size, style|wxFULL_REPAINT_ON_RESIZE, name), 
	m_context(NULL)
{
}

void GLCanvas::InitGL()
{
	if(m_context)
	{
		wxLogDebug("Context already initialised on InitGL");
		return;
	}

	m_context = new wxGLContext(this);

	SetCurrent(*m_context);

	const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	wxLogDebug("Renderer info: %s version %s from %s", renderer, version, vendor);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err ) //TODO - quit here and it leaks like a sieve.
	{
		wxLogDebug("Error initializing GLEW: ");
		char *errorText = (char*)glewGetErrorString(err);
		wxLogDebug(errorText);
		exit(1);
	}
	//GLUtils::checkForOpenGLError(__FILE__,__LINE__);

	//GLUtils::dumpGLInfo();

	glClearColor(0.2f,0.2f,0.2f,1.0f);

	    //////////////////////////////////////////////////////
    /////////// Vertex shader //////////////////////////
    //////////////////////////////////////////////////////

    GLchar * shaderCode;

	// Load the vertex shader
	wxFileInputStream input("basic.vert");
	if(!input.IsOk())
	{
		fprintf(stderr, "Error opening file: shader/basic.vert\n" );
		exit(1);
	}

	wxTextInputStream text(input);

	shaderCode = (char *)malloc(10000);

	int i = 0;
	while(input.IsOk() && !input.Eof())
	{
		int c = text.GetChar();
		if(c > 0)
		shaderCode[i++] = c;
	}

	shaderCode[i++] = '\0';
	////////////////////////////////////////////

	// Create the shader object
	GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
	if( 0 == vertShader )
	{
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(1);
	}

    // Load the source code into the shader object
    const GLchar* codeArray[] = {shaderCode};
    glShaderSource( vertShader, 1, codeArray, NULL );
    free(shaderCode); // can be removed from book.

    // Compile the shader
    glCompileShader( vertShader );

    // Check compilation status
    GLint result;
    glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result )
    {

       fprintf( stderr, "Vertex shader compilation failed!\n" );

       GLint logLen;
       glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLen );

       if( logLen > 0 )
       {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           glGetShaderInfoLog(vertShader, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }

    //////////////////////////////////////////////////////
    /////////// Fragment shader //////////////////////////
    //////////////////////////////////////////////////////

    //GLchar * shaderCode;

    // Load contents of file into shaderCode hereo
    //ifstream fragFile( "../chapter01/shader/basic.frag", ifstream::in );
    //if( !fragFile ) {
    //    fprintf(stderr, "Error opening file: shader/basic.frag\n" );
    //    exit(1);
    //}

    //shaderCode = (char *)malloc(10000);
    //i = 0;
    //while( fragFile.good() ) {
    //    int c = fragFile.get();
    //    if(c > 0)
    //        shaderCode[i++] = c;
    //}
    //inFile.close();
    //shaderCode[i++] = '\0';
    //////////////////////////////////////////////

    //// Create the shader object
    //GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
    //if( 0 == fragShader )
    //{
    //  fprintf(stderr, "Error creating fragment shader.\n");
    //  exit(1);
    //}

    //// Load the source code into the shader object
    ////const GLchar* codeArray[] = {shaderCode};
    //codeArray[0] = shaderCode;
    //glShaderSource( fragShader, 1, codeArray, NULL );
    //free(shaderCode); // can be removed from book.

    //// Compile the shader
    //glCompileShader( fragShader );

    //// Check compilation status
    ////GLint result;
    //glGetShaderiv( fragShader, GL_COMPILE_STATUS, &result );
    //if( GL_FALSE == result )
    //{

    //   fprintf( stderr, "Fragment shader compilation failed!\n" );

    //   GLint logLen;
    //   glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &logLen );

    //   if( logLen > 0 )
    //   {
    //       char * log = (char *)malloc(logLen);

    //       GLsizei written;
    //       glGetShaderInfoLog(fragShader, logLen, &written, log);

    //       fprintf(stderr, "Shader log: \n%s", log);

    //       free(log);
    //   }
    //}

    //linkMe(vertShader, fragShader);

    ///////////////////// Create the VBO ////////////////////
    //// Create and set-up the vertex array object
    //glGenVertexArrays( 1, &m_vaoHandle );
    //glBindVertexArray(m_vaoHandle);

    //// Create and populate the buffer objects
    //GLuint vboHandles[2];
    //glGenBuffers(2, vboHandles);
    //GLuint positionBufferHandle = vboHandles[0];
    //GLuint indexBufferHandle = vboHandles[1];

    //glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    //glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vert), verts, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLubyte *)0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLubyte *)sizeof(glm::vec3));

    //glEnableVertexAttribArray(0);  // Vertex position
    //glEnableVertexAttribArray(1);  // Vertex color

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLCanvas::OnSize(
	wxSizeEvent& event
	)
{
    // Necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // Reset the OpenGL view aspect
    //ResetProjectionMode();
}

void GLCanvas::OnSetFocus(
	wxFocusEvent& event
	)
{
}

void GLCanvas::Render(
	wxPaintEvent& WXUNUSED(event)
	)
{
	// TODO wxwidgets has not init callback so this has to be done here. bleugh.
	if(!m_context)
	{
		wxLogDebug("Error - no gl canvas available for rendering");
		return;
	}

    SetCurrent(*m_context);
    wxPaintDC(this);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);
 
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(-0.5, -0.5);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, -0.5);
        glColor3f(0.4, 0.5, 0.4);
        glVertex2f(0.0, -0.8);
    glEnd();
 
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.1, 0.1);
        glVertex2f(-0.1, 0.1);
        glVertex2f(-0.1, -0.1);
        glVertex2f(0.1, -0.1);
    glEnd();
 
// using a little of glut
    glColor4f(0,0,1,1);
 
    glLoadIdentity();
    glColor4f(2,0,1,1);

// done using glut
 
    glFlush();
    SwapBuffers();
}

GLCanvas::~GLCanvas()
{
	delete m_context;
	m_glContext = NULL;
}

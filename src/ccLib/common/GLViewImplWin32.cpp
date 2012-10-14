#include "GLViewImplWin32.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace CCPlayer
{

const char* CCGLViewImplWin32::ourClassNameA = "CCPlayer_GLView";
int CCGLViewImplWin32::ourViewCount = 0;

CCGLViewImplWin32::CCGLViewImplWin32(HWND hParent, int x, int y, int width, int height)
{
    if(ourViewCount++ == 0)
    {
        RegisterViewClass();
    }

    m_posX = x;
    m_posY = y;
    m_width = width;
    m_height = height;

    m_hWnd = ::CreateWindowA(ourClassNameA, "",
                        WS_CHILDWINDOW | WS_VISIBLE,
                        x, y, width, height,
                        hParent, NULL,
                        ::GetModuleHandle(NULL), NULL);
}

CCGLViewImplWin32::~CCGLViewImplWin32()
{
    if(--ourViewCount == 0)
    {
        UnRegisterViewClass();
    }
}

void CCGLViewImplWin32::RegisterViewClass()
{
    WNDCLASSA WindowClass;
    WindowClass.style         = 0;
    WindowClass.lpfnWndProc   = GlobalUIObjectsEvent;
    WindowClass.cbClsExtra    = 0;
    WindowClass.cbWndExtra    = 0;
    WindowClass.hInstance     = GetModuleHandle(NULL);
    WindowClass.hIcon         = NULL;
    WindowClass.hCursor       = 0;
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WindowClass.lpszMenuName  = NULL;
    WindowClass.lpszClassName = ourClassNameA;
    ::RegisterClassA(&WindowClass);
}

void CCGLViewImplWin32::UnRegisterViewClass()
{
    ::UnregisterClassA(ourClassNameA, GetModuleHandle(NULL));
}

void CCGLViewImplWin32::InitGL()
{
    glViewport(0,0,740,520);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)740/(GLfloat)520,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


//below fuctions performed in gl render thread
int CCGLViewImplWin32::CreateGLContext()
{
    int PixelFormat;
    PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if (!(m_hDC=::GetDC(m_hWnd)))
	{
        return -1;
	}

	if (!(PixelFormat=::ChoosePixelFormat(m_hDC,&pfd)))
	{
		return -1;
	}

	if(!::SetPixelFormat(m_hDC,PixelFormat,&pfd))
	{
		return -1;
	}

	if (!(m_hRC=::wglCreateContext(m_hDC)))
	{
		return -1;
	}

	if(!::wglMakeCurrent(m_hDC,m_hRC))
	{
		return -1;
	}

    //Init the render rect
	InitGL();

	return 0;
}

int CCGLViewImplWin32::SwapBuffers()
{
    if(::SwapBuffers(m_hDC))
    {
        return 0;
    }else
    {
        return -1;
    }
}

int CCGLViewImplWin32::DrawFrame()
{
    static long double x = 0.0;
	const static GLfloat MatYellowDiffuse[] = {0.86f, 0.74f, 0.14f, 1.0f};
	const static GLfloat MatOrangeDiffuse[] = {0.78f, 0.59f, 0.0f, 1.0f};

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// clear screen and depth buffers
	glLoadIdentity();									// reset modelview matrix

	// get the angle we wish to use
	if(x >= 360.0f) x = 0.0f;

	// move and rotate the cube
	glPushMatrix();
		glTranslatef(-5, -5, -35);
		glRotated(x, 0.0f, 1.0f, 0.0f);

		glColor3ub(220, 190, 35);		// yellow
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatYellowDiffuse);

		// draw the cube
		glBegin(GL_TRIANGLES);
			glNormal3f(0, 0, -1);
			glVertex3f(-5, -5, -1);			// left
			glVertex3f(0, 5, -1);			// top
			glVertex3f(5, -5, -1);			// right

			glNormal3f(0, 0, 1);
			glVertex3f(-5, -5, 1);			// left
			glVertex3f(0, 5, 1);			// top
			glVertex3f(5, -5, 1);			// right
		glEnd();

		glColor3ub(200, 150, 0);			// dark yellow
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatOrangeDiffuse);

		glBegin(GL_QUADS);
			glNormal3f(-1, 0, 0);
			glVertex3f(-5, -5, -1);			// bottom left
			glVertex3f(0, 5, -1);			// top left
			glVertex3f(0, 5, 1);			// top right
			glVertex3f(-5, -5, 1);			// bottom right

			glNormal3f(1, 0, 0);
			glVertex3f(5, -5, 1);			// bottom left
			glVertex3f(0, 5, 1);			// top left
			glVertex3f(0, 5, -1);			// top right
			glVertex3f(5, -5, -1);			// bottom right
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(5, -5, -35);
		glRotated(x, 0.0f, 1.0f, 0.0f);

		glColor3ub(220, 190, 35);		// yellow
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatYellowDiffuse);

		// draw the cube
		glBegin(GL_TRIANGLES);
			glNormal3f(0, 0, -1);
			glVertex3f(-5, -5, -1);			// left
			glVertex3f(0, 5, -1);			// top
			glVertex3f(5, -5, -1);			// right

			glNormal3f(0, 0, 1);
			glVertex3f(-5, -5, 1);			// left
			glVertex3f(0, 5, 1);			// top
			glVertex3f(5, -5, 1);			// right
		glEnd();

		glColor3ub(200, 150, 0);			// dark yellow
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatOrangeDiffuse);

		glBegin(GL_QUADS);
			glNormal3f(-1, 0, 0);
			glVertex3f(-5, -5, -1);			// bottom left
			glVertex3f(0, 5, -1);			// top left
			glVertex3f(0, 5, 1);			// top right
			glVertex3f(-5, -5, 1);			// bottom right

			glNormal3f(1, 0, 0);
			glVertex3f(5, -5, 1);			// bottom left
			glVertex3f(0, 5, 1);			// top left
			glVertex3f(0, 5, -1);			// top right
			glVertex3f(5, -5, -1);			// bottom right
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 5, -35);
		glRotated(x, 0.0f, 1.0f, 0.0f);

		glColor3ub(220, 190, 35);		// yellow
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatYellowDiffuse);

		// draw the cube
		glBegin(GL_TRIANGLES);
			glNormal3f(0, 0, -1);
			glVertex3f(-5, -5, -1);			// left
			glVertex3f(0, 5, -1);			// top
			glVertex3f(5, -5, -1);			// right

			glNormal3f(0, 0, 1);
			glVertex3f(-5, -5, 1);			// left
			glVertex3f(0, 5, 1);			// top
			glVertex3f(5, -5, 1);			// right
		glEnd();

		glColor3ub(200, 150, 0);			// dark yellow
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatOrangeDiffuse);

		glBegin(GL_QUADS);
			glNormal3f(-1, 0, 0);
			glVertex3f(-5, -5, -1);			// bottom left
			glVertex3f(0, 5, -1);			// top left
			glVertex3f(0, 5, 1);			// top right
			glVertex3f(-5, -5, 1);			// bottom right

			glNormal3f(1, 0, 0);
			glVertex3f(5, -5, 1);			// bottom left
			glVertex3f(0, 5, 1);			// top left
			glVertex3f(0, 5, -1);			// top right
			glVertex3f(5, -5, -1);			// bottom right
		glEnd();
	glPopMatrix();

	// rotate 45 degrees every second
	x += (double)45.0;

    SwapBuffers();

	return 0;
}


} // end namespace CCPlayer

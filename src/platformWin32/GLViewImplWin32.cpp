#include "GLViewImplWin32.h"
#include "VideoDef.h"

namespace CCPlayer
{

const char* CCGLViewImplWin32::ourClassNameA = "CCPlayer_GLView";
int CCGLViewImplWin32::ourViewCount = 0;

LRESULT CALLBACK PrivateGLViewEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

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

    m_selfHandle = ::CreateWindowA(ourClassNameA, "",
                        WS_CHILDWINDOW | WS_VISIBLE,
                        x, y, width, height,
                        hParent, NULL,
                        ::GetModuleHandle(NULL), NULL);

    CreateGLContext();
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
    WindowClass.lpfnWndProc   = PrivateGLViewEvent;
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

	if (!(m_hDC=::GetDC(m_selfHandle)))
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

LRESULT CALLBACK PrivateGLViewEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    return DefWindowProcA(Handle, Message, WParam, LParam);
}

} // end namespace CCPlayer

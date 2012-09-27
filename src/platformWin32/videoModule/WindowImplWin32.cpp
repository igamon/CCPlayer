#include "WindowImplWin32.h"

namespace CCPlayer
{
const char* CCWindowImplWin32::ourClassNameA = "CCPlayer_Window";
int CCWindowImplWin32::ourWindowCount = 0;

CCWindowImplWin32::CCWindowImplWin32(int x, int y, int width, int height)
{
    if(ourWindowCount++ == 0)
    {
        RegisterWindowClass();
    }

    m_hWnd = ::CreateWindowA(ourClassNameA, "",
                                   WS_OVERLAPPEDWINDOW,
                                   x, y, width, height,
                                   NULL, NULL,
                                   ::GetModuleHandle(NULL), NULL);

    //After Create it , we should show it
    Show(true);

    Update();
}

CCWindowImplWin32::~CCWindowImplWin32()
{
    if(--ourWindowCount == 0)
    {
        UnRegisterWindowClass();
    }
}

void CCWindowImplWin32::RegisterWindowClass()
{
    WNDCLASSA WindowClass;
    WindowClass.style         = 0;
    WindowClass.lpfnWndProc   = GlobalUIObjectsEvent;
    WindowClass.cbClsExtra    = 0;
    WindowClass.cbWndExtra    = 0;
    WindowClass.hInstance     = GetModuleHandle(NULL);
    WindowClass.hIcon         = NULL;
    WindowClass.hCursor       = 0;
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WindowClass.lpszMenuName  = NULL;
    WindowClass.lpszClassName = ourClassNameA;
    ::RegisterClassA(&WindowClass);
}

void CCWindowImplWin32::UnRegisterWindowClass()
{
    ::UnregisterClassA(ourClassNameA, GetModuleHandle(NULL));
}

void CCWindowImplWin32::Show(bool bShow)
{
    if(bShow)
    {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }else
    {
        ::ShowWindow(m_hWnd, SW_HIDE);
    }
}

void CCWindowImplWin32::Update()
{
    ::UpdateWindow(m_hWnd);
}

HWND CCWindowImplWin32::GetWndHandle()
{
    return m_hWnd;
}

int CCWindowImplWin32::MainLoop()
{
    MSG msg;
    while(::GetMessage(&msg,NULL,0,0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return msg.wParam;
}

} // end namespace CCPlayer

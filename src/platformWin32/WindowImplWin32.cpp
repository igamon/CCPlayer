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

    m_selfHandle = ::CreateWindowA(ourClassNameA, "",
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
        ::ShowWindow(m_selfHandle, SW_SHOW);
    }else
    {
        ::ShowWindow(m_selfHandle, SW_HIDE);
    }
}

void CCWindowImplWin32::Update()
{
    ::UpdateWindow(m_selfHandle);
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

void CCWindowImplWin32::AddSubUIObject(CCUIObject* pSubObject)
{
    CCUIWindow::AddSubUIObjectHelp(pSubObject);

    LayoutSubViews();
}

void CCWindowImplWin32::InsertUIObejct(int index, CCUIObject* pSubObject)
{
    CCUIWindow::InsertUIObejctHelp(index, pSubObject);

    LayoutSubViews();
}

void CCWindowImplWin32::LayoutSubViews()
{
    std::vector<CCUIObject*>::iterator it = m_subUIObjects.begin();

    while(it != m_subUIObjects.end())
    {
        ::SetWindowPos((*it)->GetUIObjectHandle(),
                     HWND_TOP,
                     10,
                     10,
                     100,
                     200,
                     SWP_SHOWWINDOW);

        it ++;
    }
}

} // end namespace CCPlayer

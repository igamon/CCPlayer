#include "App.h"
#include "Win32Def.h"
#include "WindowImplWin32.h"
#include "GLViewImplWin32.h"
#include "Player.h"

namespace CCPlayer
{

CCApp::CCApp(int argc, char* argv[])
:m_bInited(true)
{
}

CCApp::~CCApp()
{
}

int CCApp::Exec()
{
    if(!m_bInited)
    {
        return -1;
    }

    CCWindowImplWin32 mainWindow(WINDOW_X_POS, WINDOW_Y_POS, WINDOW_WIDTH, WINDOW_HEIGHT);
    CCGLViewImplWin32 glRenderView(
                        mainWindow.GetWndHandle(),
                        10,
                        10,
                        WINDOW_WIDTH - 40,
                        WINDOW_HEIGHT - 60);

    CCPlayer playerInstance;

    //while(1)
    {
        playerInstance.Open("test.flv");
        Sleep(15);
    }

    return mainWindow.MainLoop();
}

LRESULT CALLBACK GlobalUIObjectsEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    return DefWindowProcA(Handle, Message, WParam, LParam);
}

}

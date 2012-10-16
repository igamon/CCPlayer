#include "App.h"
#include "Win32Def.h"
#include "Player.h"
#include "WindowImplWin32.h"
#include "GLViewImplWin32.h"

namespace CCPlayer
{

CCApp::CCApp(int argc, char* argv[])
:m_bInited(true)
{
}

CCApp::~CCApp()
{
}

void CCApp::OpenResponse(int ErrCode)
{
    m_pPlayerInstance->InitGLWindow(dynamic_cast<CCUIGLView*>(m_pGLRenderView));
}

int CCApp::Exec()
{
    if(!m_bInited)
    {
        return -1;
    }

    m_pMainWindow = new CCWindowImplWin32(WINDOW_X_POS,
                                          WINDOW_Y_POS,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT);
    m_pGLRenderView = new CCGLViewImplWin32(
                        m_pMainWindow->GetUIObjectHandle(),
                        10,
                        10,
                        WINDOW_WIDTH - 40,
                        WINDOW_HEIGHT - 60);

    m_pPlayerInstance = new CCPlayer();
    m_pPlayerInstance->SetRspCommandObject(this);
    m_pPlayerInstance->Open("test.flv");

    return m_pMainWindow->MainLoop();
}

LRESULT CALLBACK GlobalUIObjectsEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    return DefWindowProcA(Handle, Message, WParam, LParam);
}

}

#include "App.h"
#include "Win32Def.h"
#include "Player.h"
#include "WindowImplWin32.h"
#include "GLViewImplWin32.h"

namespace CCPlayer
{

#define MESSAGE_TYPE_OPEN   WM_USER+1

CCApp::CCApp(int argc, char* argv[])
:m_bInited(true)
{
}

CCApp::~CCApp()
{
}

void CCApp::OpenResponse(int ErrCode)
{
    ::SendMessage(m_pMainWindow->GetUIObjectHandle(), MESSAGE_TYPE_OPEN, 0, 0);

    //m_pPlayerInstance->InitGLWindow(dynamic_cast<CCUIGLView*>(m_pGLRenderView));
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
                                          WINDOW_HEIGHT,
                                          this);
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

void CCApp::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
    switch(wParam)
    {
        case 'P':
        {
            m_pPlayerInstance->Pause();
            std::cout << "Key P Pressed" << std::endl;
        }
        break;
        case 'C':
        {
            m_pPlayerInstance->Continue();
            std::cout << "Key C Pressed" << std::endl;
        }
        break;
        case 'S':
        {
            m_pPlayerInstance->Stop();
            std::cout << "Key S Pressed" << std::endl;
        }
        break;
        case 'G':
        {
            int64_t totalDuration = 0, currentPostion = 0;
            m_pPlayerInstance->GetTotalDurationBySecond(&totalDuration);
            m_pPlayerInstance->GetCurrentPostionBySecond(&currentPostion);
            std::cout << currentPostion <<":" << totalDuration << std::endl;
        }
        break;
    }
}

void CCApp::OnPlayerOpen(WPARAM wParam, LPARAM lParam)
{
    m_pPlayerInstance->InitGLWindow(dynamic_cast<CCUIGLView*>(m_pGLRenderView));

    int64_t totalDuration = 0.0;

    m_pPlayerInstance->GetTotalDurationBySecond(&totalDuration);
    std::cout << "Total time duration is : " << totalDuration << std::endl;
}

LRESULT CALLBACK GlobalUIObjectsEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch(Message)
    {
        case WM_CREATE:
        {
            //提取出对象指针
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(LParam);

            //CCApp* pApp = (CCApp*)((LPCREATESTRUCT)LParam)->lpCreateParams;

            //pApp->func();

            SetWindowLong(Handle, GWL_USERDATA, (unsigned int)pCreateStruct->lpCreateParams);
        }
        break;
        case WM_KEYDOWN :
        {
            CCApp* pApp = reinterpret_cast<CCApp*>(GetWindowLong(Handle, GWL_USERDATA));
            pApp->OnKeyUp(WParam, LParam);
        }
        break;
        case MESSAGE_TYPE_OPEN:
        {
            CCApp* pApp = reinterpret_cast<CCApp*>(GetWindowLong(Handle, GWL_USERDATA));
            pApp->OnPlayerOpen(WParam, LParam);
        }
        break;
    }

    return DefWindowProcA(Handle, Message, WParam, LParam);
}

}

#ifndef APP_H
#define APP_H

#include "IRSPCommand.h"
#include "Common.h"

namespace CCPlayer
{

class CCPlayer;
class CCGLViewImplWin32;
class CCWindowImplWin32;

class CCApp: public IRSPCommand
{
public:
    CCApp(int argc, char* argv[]);
    int Exec();
    virtual ~CCApp();

public:
    virtual void OpenResponse(int ErrCode);

public:
    void OnKeyUp(WPARAM wParam, LPARAM lParam);

private:
    CCWindowImplWin32* m_pMainWindow;
    CCGLViewImplWin32* m_pGLRenderView;
    CCPlayer* m_pPlayerInstance;
    bool m_bInited;
};

}

#endif // APP_H

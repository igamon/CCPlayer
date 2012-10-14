#ifndef APP_H
#define APP_H

#include "IRSPCommand.h"

namespace CCPlayer
{

class CCPlayer;
class CCWindowImplWin32;

class CCApp: public IRSPCommand
{
public:
    CCApp(int argc, char* argv[]);
    int Exec();
    virtual ~CCApp();

public:
    virtual void OpenResponse(int ErrCode);

private:
    CCWindowImplWin32* m_pMainWindow;
    CCPlayer* m_pPlayerInstance;
    bool m_bInited;
};

}

#endif // APP_H

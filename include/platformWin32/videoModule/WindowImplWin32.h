#ifndef WINDOWIMPLWIN32_H
#define WINDOWIMPLWIN32_H

#include "Win32Def.h"

namespace CCPlayer
{
class CCWindowImplWin32
{
public:
    CCWindowImplWin32(int x, int y, int width, int height);
    virtual ~CCWindowImplWin32();

public:
    void Show(bool bShow);
    void Update();
    HWND GetWndHandle();
    int MainLoop();

private:
    void RegisterWindowClass();
    void UnRegisterWindowClass();

private:
    HWND m_hWnd;

private:
    static const char* ourClassNameA;
    static int ourWindowCount;
};

} // end namespace CCPlayer

#endif // WINDOWIMPLWIN32_H

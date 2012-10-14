#ifndef WINDOWIMPLWIN32_H
#define WINDOWIMPLWIN32_H

#include "Win32Def.h"
#include "UIWindow.h"

namespace CCPlayer
{
class CCWindowImplWin32: public CCUIWindow
{
public:
    CCWindowImplWin32(int x, int y, int width, int height);
    virtual ~CCWindowImplWin32();

public:
    void Show(bool bShow);
    void Update();
    HWND GetWndHandle();
    int MainLoop();

public:
    virtual void AddSubUIObject(CCUIObject* pSubObject);
    virtual void InsertUIObejct(int index, CCUIObject* pSubObject);
    virtual void LayoutSubViews();

private:
    void RegisterWindowClass();
    void UnRegisterWindowClass();

private:
    static const char* ourClassNameA;
    static int ourWindowCount;
};

} // end namespace CCPlayer

#endif // WINDOWIMPLWIN32_H

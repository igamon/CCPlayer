#ifndef UIGLVIEWIMPLWIN32_H
#define UIGLVIEWIMPLWIN32_H

#include "Win32Def.h"
#include "UIGLView.h"

namespace CCPlayer
{

class CCGLViewImplWin32 : public CCUIGLView
{
public:
    CCGLViewImplWin32(HWND hParent, int x, int y, int width, int height);
    virtual ~CCGLViewImplWin32();

public:
    virtual int CreateGLContext();
    virtual int SwapBuffers();

private:
    void RegisterViewClass();
    void UnRegisterViewClass();
    void InitGL();

private:
    static const char* ourClassNameA;
    static int ourViewCount;

private:
    int m_posX;
    int m_posY;
    int m_width;
    int m_height;

private:
    HDC			m_hDC;
    HGLRC		m_hRC;
};

} // end namespace CCPlayer

#endif // UIGLVIEWIMPLWIN32_H

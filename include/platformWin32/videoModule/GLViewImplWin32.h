#ifndef UIGLVIEWIMPLWIN32_H
#define UIGLVIEWIMPLWIN32_H

#include "Win32Def.h"
#include "IGLRenderObject.h"

namespace CCPlayer
{

class CCGLViewImplWin32 : public IGLRender
{
public:
    CCGLViewImplWin32(HWND hParent, int x, int y, int width, int height);
    virtual ~CCGLViewImplWin32();

public:
    virtual int CreateGLContext();
    virtual int SwapBuffers();
    virtual int DrawFrame();

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
    HWND        m_hWnd;
    HDC			m_hDC;
    HGLRC		m_hRC;
};

} // end namespace CCPlayer

#endif // UIGLVIEWIMPLWIN32_H

#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "Common.h"

namespace CCPlayer
{

class CCUIGLView;

class CCGLWrapper
{
public:
    CCGLWrapper();
    virtual ~CCGLWrapper();

public:
    void SetGLRenderView(CCUIGLView* pGLRenderView);
    int CreateGLContext();
    int DrawFrame();

private:
    CCUIGLView* m_pGLRenderView;
};

}
#endif // GLWRAPPER_H

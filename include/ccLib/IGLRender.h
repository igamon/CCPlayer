#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

#include "VideoDef.h"

class IGLRender
{
public:
    virtual int SwapBuffers() = 0;
    virtual int CreateGLContext() = 0;
    virtual int DrawFrame() = 0;
};

#endif

#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

class IGLRenderObject
{
public:
    virtual int SwapBuffers() = 0;
    virtual int CreateGLContext() = 0;
    virtual int DrawFrame() = 0;
};

#endif

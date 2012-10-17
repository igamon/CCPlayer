#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "Common.h"

namespace CCPlayer
{

class CCUIGLView;
class VideoFrame;

class CCGLWrapper
{
public:
    CCGLWrapper();
    virtual ~CCGLWrapper();

public:
    void SetGLRenderView(CCUIGLView* pGLRenderView);
    int CreateGLContext();
    int DrawFrame(VideoFrame* pVideoFrame, int width, int height);

private:
    CCUIGLView* m_pGLRenderView;
    GLuint m_glTexture;
};

}
#endif // GLWRAPPER_H

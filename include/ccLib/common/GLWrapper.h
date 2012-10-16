#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "UIGLView.h"

namespace CCPlayer
{

class CCGLWrapper
{
public:
    CCGLWrapper();
    virtual ~CCGLWrapper();

private:
    CCUIGLView m_glRenderHandle;
};

}
#endif // GLWRAPPER_H

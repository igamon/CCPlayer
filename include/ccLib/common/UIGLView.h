#ifndef UIGLVIEW_H
#define UIGLVIEW_H

#include "Common.h"
#include "UIObject.h"

namespace CCPlayer
{

class CCUIGLView: public CCUIObject
{
public:
    CCUIGLView();
    virtual ~CCUIGLView();
public:
    virtual int CreateGLContext() = 0;
    virtual int SwapBuffers() = 0;
};

}

#endif // UIGLVIEW_H

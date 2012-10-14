#ifndef UIGLVIEW_H
#define UIGLVIEW_H

#include "Common.h"
#include "UIObject.h"

namespace CCPlayer
{

class CCUIWindow;
class CCGLViewImplWin32;

class CCUIGLView: public CCUIObject
{
public:
    CCUIGLView();
    virtual ~CCUIGLView();
public:
    int DrawFrame();

public:
    void CreateRenderRect(CCUIWindow* pParent, int x, int y, int width, int height);
private:
    CCGLViewImplWin32* m_pGLViewImplDependOS;
};

}

#endif // UIGLVIEW_H

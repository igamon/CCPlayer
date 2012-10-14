#include "UIGLView.h"
#include "UIWindow.h"

//current only support win32
#include "GLViewImplWin32.h"
typedef CCPlayer::CCGLViewImplWin32 CCGLViewImplDependOS;
//#define CCGLViewImplDependOS CCPlayer::CCGLViewImplWin32

namespace CCPlayer
{

CCUIGLView::CCUIGLView()
:m_pGLViewImplDependOS(NULL)
{
    //ctor
}

CCUIGLView::~CCUIGLView()
{
    //dtor
}

void CCUIGLView::CreateRenderRect(CCUIWindow* pParent,
                                  int x,
                                  int y,
                                  int width,
                                  int height)
{
    if(m_pGLViewImplDependOS == NULL)
    {
        m_pGLViewImplDependOS = new CCGLViewImplDependOS(pParent->GetUIObjectHandle(),
                                                         x,
                                                         y,
                                                         width,
                                                         height);
        pParent->InsertUIObejct(0, m_pGLViewImplDependOS);

        //m_pGLViewImplDependOS->CreateGLContext();
    }
}

int CCUIGLView::DrawFrame()
{
    m_pGLViewImplDependOS->DrawFrame1();
    return 0;
}

}

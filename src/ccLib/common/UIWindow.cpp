#include "UIWindow.h"

namespace CCPlayer
{

CCUIWindow::CCUIWindow()
{
    //ctor
}

CCUIWindow::~CCUIWindow()
{
    //dtor
}

void CCUIWindow::AddSubUIObjectHelp(CCUIObject* pSubObject)
{
    m_subUIObjects.push_back(pSubObject);
}

void CCUIWindow::InsertUIObejctHelp(int index, CCUIObject* pSubObject)
{
    std::vector<CCUIObject*>::iterator pos = m_subUIObjects.begin();

    while(index-- && ++pos != m_subUIObjects.end());

    m_subUIObjects.insert(pos, pSubObject);
}

}

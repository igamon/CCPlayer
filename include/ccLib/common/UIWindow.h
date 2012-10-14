#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "UIObject.h"

namespace CCPlayer
{

class CCUIWindow: public CCUIObject
{
public:
    CCUIWindow();
    virtual ~CCUIWindow();

public:
    virtual void AddSubUIObject(CCUIObject* pSubObject) = 0;
    virtual void InsertUIObejct(int index, CCUIObject* pSubObject) = 0;
    virtual void LayoutSubViews() = 0;

protected:
    void AddSubUIObjectHelp(CCUIObject* pSubObject);
    void InsertUIObejctHelp(int index, CCUIObject* pSubObject);

protected:
    std::vector<CCUIObject*> m_subUIObjects;
};

}

#endif // UIWINDOW_H

#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

#include "Common.h"

class CCUIObject
{
public:
    void AddSubUIObject(CCUIObject* pSubObject)
    {
        m_subUIObjects.push_back(pSubObject);
    }

    void InsertUIObejct(int index, CCUIObject* pSubObject)
    {
        std::vector<CCUIObject*>::iterator pos = m_subUIObjects.begin();

        while(index -- && pos != m_subUIObjects.end())
        {
            pos ++;
        }

        m_subUIObjects.insert(pos, pSubObject);
    }

    UIObjectHandle GetUIObjectHandle()
    {
        return m_selfHandle;
    };

private:
    UIObjectHandle m_selfHandle;

private:
    std::vector<CCUIObject*> m_subUIObjects;
};

#endif

#ifndef GLRENDEROBJECT_H
#define GLRENDEROBJECT_H

#include "Common.h"

namespace CCPlayer
{

class CCUIObject
{
public:
    UIObjectHandle GetUIObjectHandle()
    {
        return m_selfHandle;
    }

protected:
    UIObjectHandle m_selfHandle;
};

}

#endif

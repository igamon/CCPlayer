#ifndef WIN32DEF_H
#define WIN32DEF_H

#include <Windows.h>

namespace CCPlayer
{
#define WINDOW_X_POS    100
#define WINDOW_Y_POS    100
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

extern LRESULT CALLBACK GlobalUIObjectsEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

}


#endif

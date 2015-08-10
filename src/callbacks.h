#ifndef CALLBACKS_H_INC
#define CALLBACKS_H_INC

#include "Dialogs.h"

int CALLBACK FolderProc(HWND, UINT, LPARAM, LPARAM);
INT_PTR CALLBACK DialogFunc(HWND, UINT, WPARAM, LPARAM);
TCHAR* replacechar(TCHAR*, TCHAR, TCHAR);

#endif
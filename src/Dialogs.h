#ifndef DIALOGS_H_INCLUDED
#define DIALOGS_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <nsis/pluginapi.h> // nsis plugin
#ifndef _TCHAR_DEFINED
#include <tchar.h>
#endif
#include "resource.h"

typedef struct {
	wchar_t sTitle[64];
	wchar_t sText[64];
	wchar_t sOk[12];
	wchar_t sCancel[12];
	int nRetButton;
	int nRetVal;
	int bPassMask;
	int buff_size;
} IPBData, *LPIPBDATA;

#define PluginEntry(foo)	void __declspec(dllexport) foo(HWND hwndParent,int string_size,TCHAR *variables,stack_t **stacktop,extra_parameters *extra)

#endif
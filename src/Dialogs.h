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
#include <strsafe.h>
#include "resource.h"

typedef struct {
	TCHAR sTitle[64];
	TCHAR sText[64];
	TCHAR sOk[12];
	TCHAR sCancel[12];
	int nRetButton;
	int nRetVal;
	int bPassMask;
	int buff_size;
} IPBData, *LPIPBDATA;

typedef struct {
	TCHAR sTitle[64];
	TCHAR sText[64];
	TCHAR sOk[12];
	TCHAR sCancel[12];
	int nRetButton;
	int nRetVal;
} DriveDLGData, *LPDRIVEDLGDATA;

#define PluginEntry(foo)	void __declspec(dllexport) foo(HWND hwndParent,int string_size,TCHAR *variables,stack_t **stacktop,extra_parameters *extra)

#endif
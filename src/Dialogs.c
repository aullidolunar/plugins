#include "callbacks.h"

HINSTANCE hInstance; // global, meh

PluginEntry(InputBox)
{
	wchar_t sztype[3];
	wchar_t retButton[3];
	wchar_t retVar[3];
	EXDLL_INIT();
	{
		IPBData pPBdata;
		HWND hDlg;
		MSG msg;
		PopStringW (sztype); // type 
		PopStringW (pPBdata.sTitle); // dialog title
		PopStringW (pPBdata.sText); // dialog secundary text
		PopStringW (pPBdata.sOk); // label for ok button
		PopStringW (pPBdata.sCancel); // label for cancel button
		PopStringW (retButton); // return var of the button
		PopStringW (retVar); // return var of the text
		pPBdata.bPassMask = _ttoi (sztype);
		pPBdata.nRetButton = _ttoi (retButton);
		pPBdata.nRetVal = _ttoi (retVar);
		pPBdata.buff_size = string_size;
		hDlg = CreateDialogParam (hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwndParent, DialogFunc, (LPARAM)&pPBdata);
		while(GetMessage(&msg, NULL, 0, 0)) {
			if(!IsDialogMessage(hDlg, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (IsWindow(hDlg)) DestroyWindow(hDlg);
	}
}

PluginEntry(About)
{
	TCHAR *buff; // buffer
	buff = HeapAlloc (GetProcessHeap(), 0, string_size);
	EXDLL_INIT();
	{
		_sntprintf (buff, string_size, _T("Plugin name: %s\nVersion: %s\nAuthor: %s"), _T(PACKAGE_NAME), _T(PACKAGE_VERSION), _T(SELFIE));
		MessageBox (hwndParent, buff, _T("About Dialogs"), MB_OK|MB_ICONINFORMATION);
	}
	HeapFree (GetProcessHeap(), 0, buff);
}

PluginEntry(Open)
{
	TCHAR *buff; // buffer
	TCHAR init_dir[MAX_PATH]; // initial directory
	TCHAR filters[MAX_PATH]; // filters for the open dialog
	TCHAR retButton[3]; // return var of the button
	TCHAR retFile[3]; // return var of the file
	buff = HeapAlloc (GetProcessHeap(), HEAP_ZERO_MEMORY, string_size);
	EXDLL_INIT ();
	{
		OPENFILENAME ofn;
		int nRetButton;
		PopStringW (init_dir);
		PopStringW (filters);
		PopStringW (retButton);
		PopStringW (retFile);
		ZeroMemory (&ofn, sizeof(OPENFILENAME));
		ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		ofn.hwndOwner = hwndParent;
		ofn.lpstrInitialDir = init_dir;
		ofn.lpstrFile = buff;
		ofn.lpstrFilter = replacechar (filters, '|', '\0');
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = string_size;
		nRetButton = _ttoi (retButton);
		if (GetOpenFileName(&ofn)) {
			int nRetVal;
			nRetVal = _ttoi (retFile);
			SetUserVariableW (nRetVal, buff);
			SetUserVariableW (nRetButton, _T("1"));
		}
		else {
			SetUserVariableW (nRetButton, _T("0"));
		}
	}
	HeapFree (GetProcessHeap(), HEAP_ZERO_MEMORY, buff);
}

PluginEntry(Save)
{
	TCHAR *buff; // buffer
	TCHAR init_dir[MAX_PATH]; // initial directory
	TCHAR filters[MAX_PATH]; // filters for the open dialog
	TCHAR retButton[3]; // return var of the button
	TCHAR retFile[3]; // return var of the file
	buff = HeapAlloc (GetProcessHeap(), HEAP_ZERO_MEMORY, string_size);
	EXDLL_INIT ();
	{
		OPENFILENAME ofn;
		int nRetButton;
		PopStringW (init_dir);
		PopStringW (filters);
		PopStringW (retButton);
		PopStringW (retFile);
		ZeroMemory (&ofn, sizeof(OPENFILENAME));
		ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT;
		ofn.hwndOwner = hwndParent;
		ofn.lpstrInitialDir = init_dir;
		ofn.lpstrFile = buff;
		ofn.lpstrFilter = replacechar (filters, '|', '\0');
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = string_size;
		nRetButton = _ttoi (retButton);
		if (GetSaveFileName(&ofn)) {
			int nRetVal;
			nRetVal = _ttoi (retFile);
			SetUserVariableW (nRetVal, buff);
			SetUserVariableW (nRetButton, _T("1"));
		}
		else {
			SetUserVariableW (nRetButton, _T("0"));
		}
	}
	HeapFree (GetProcessHeap(), HEAP_ZERO_MEMORY, buff);
}

PluginEntry(Ver)
{
	TCHAR retVar[5]; // return var
	EXDLL_INIT();
	{
		int nRetVal;
		PopStringW (retVar);
		nRetVal = _ttoi (retVar);
		SetUserVariableW (nRetVal, _T(PACKAGE_VERSION));
	}
}

PluginEntry(FolderDlg)
{
	TCHAR info[MAX_PATH]; // secundary text
	TCHAR dir[MAX_PATH]; // init directory
	TCHAR retButton[3]; // return var of the button
	TCHAR retDir[3]; // return var of the directory
	TCHAR *buff; // buffer
	buff = HeapAlloc (GetProcessHeap(), 0, string_size);
	EXDLL_INIT();
	{
		BROWSEINFO bi;
		LPITEMIDLIST pidl;
		int nRetVal;
		int nRetButton;
		PopStringW(info);
		PopStringW(dir);
		PopStringW(retButton);
		PopStringW(retDir);
		ZeroMemory (&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = hwndParent;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = buff;
		bi.lpszTitle = info;
		bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_DONTGOBELOWDOMAIN|BIF_USENEWUI;
		bi.lParam = (LPARAM)dir;
		bi.lpfn = FolderProc;
		nRetVal = _ttoi (retDir);
		nRetButton = _ttoi (retButton);
		pidl = SHBrowseForFolder(&bi);
		if (SHGetPathFromIDList(pidl, buff)) {
			SetUserVariableW (nRetVal, buff);
			SetUserVariableW (nRetButton, _T("1"));
			CoTaskMemFree (pidl);
		} else {
			SetUserVariableW (nRetButton, _T("0"));
		}
	}
	HeapFree (GetProcessHeap(), 0, buff);
}

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	hInstance = hInst;
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		{
			
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
	}
	return TRUE;
}
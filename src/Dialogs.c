#include "callbacks.h"

HINSTANCE hInstance; // global, meh

PluginEntry(InputBox)
{
	TCHAR sztype[3];
	TCHAR retButton[3];
	TCHAR retVar[3];
	EXDLL_INIT();
	{
		IPBData pPBdata;
		HWND hDlg;
		MSG msg;
		popstring (sztype); // type 
		popstring (pPBdata.sTitle); // dialog title
		popstring (pPBdata.sText); // dialog secundary text
		popstring (pPBdata.sOk); // label for ok button
		popstring (pPBdata.sCancel); // label for cancel button
		popstring (retButton); // return var of the button
		popstring (retVar); // return var of the text
		pPBdata.bPassMask = myatoi (sztype);
		pPBdata.nRetButton = myatoi (retButton);
		pPBdata.nRetVal = myatoi (retVar);
		pPBdata.buff_size = sizeof(TCHAR)*string_size;
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
	buff = HeapAlloc (GetProcessHeap(), 0, sizeof(TCHAR)*string_size);
	EXDLL_INIT();
	{
		wnsprintf (buff, string_size, _T("Plugin name: %s\nCharset: %s\nVersion: %s\nAuthor: %s"), _T(PACKAGE_NAME), _T(PACKAGE_CHARSET), _T(PACKAGE_VERSION), _T(SELFIE));
		MessageBox (hwndParent, buff, _T("About Dialogs"), MB_OK|MB_ICONINFORMATION);
	}
	HeapFree (GetProcessHeap(), 0, buff);
}

PluginEntry(FontDlg)
{
	TCHAR retButton[3]; // return var of the button
	TCHAR retBuffer[3]; // return var of the buffer
	EXDLL_INIT();
	{
		CHOOSEFONT cf;
		LOGFONT lf;
		HFONT hFont;
		
		hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		GetObject (hFont, sizeof(LOGFONT), &lf);
		SecureZeroMemory (&cf, sizeof(CHOOSEFONT));
		cf.lStructSize = sizeof (cf);
		cf.hwndOwner = hwndParent;
		cf.lpLogFont = &lf;
		cf.Flags = CF_SCREENFONTS|CF_SCRIPTSONLY|CF_INITTOLOGFONTSTRUCT;
		popstring (retButton);
		popstring (retBuffer);
		
		if (ChooseFont(&cf)) {
			TCHAR *szBuffer;
			int nFontSize;
			HDC hDC;
			szBuffer = HeapAlloc (GetProcessHeap(), 0, string_size);
			hDC = GetDC(hwndParent);
			setuservariable (myatoi(retButton), _T("1"));
			nFontSize = -MulDiv(lf.lfHeight, 72, GetDeviceCaps (hDC, LOGPIXELSY));
			wnsprintf (szBuffer, string_size, _T("%s,%i,%i,%i"), lf.lfFaceName, lf.lfWeight, (lf.lfItalic) ? 1 : 0, nFontSize);
			setuservariable (myatoi(retBuffer), szBuffer);
			ReleaseDC (hwndParent, hDC);
			HeapFree (GetProcessHeap(), 0, szBuffer);
		} else {
			setuservariable (myatoi(retButton), _T("0"));
		}
	}
}

PluginEntry(ColorDlg)
{
	TCHAR sztype[3]; // type of the return value
	TCHAR retButton[3]; // return var of the button
	TCHAR retBuffer[3]; // return var of the buffer
	EXDLL_INIT();
	{
		CHOOSECOLOR cc;
		int nType;
		int i;
		COLORREF col_array[16];
		
		popstring (sztype);
		popstring (retButton);
		popstring (retBuffer);
		nType = myatoi (sztype);
			
		for (i = 0; i < 16; i++) col_array[i] = RGB(255,255,255);

		SecureZeroMemory (&cc, sizeof(CHOOSECOLOR));
		cc.lpCustColors = col_array;
		cc.lStructSize = sizeof(CHOOSECOLOR);
		cc.hwndOwner = hwndParent;
		cc.Flags = CC_FULLOPEN|CC_SOLIDCOLOR;
				
		if (ChooseColor(&cc)) {
			TCHAR szBuffer[32]; // just to be sure :v
			switch (nType) {
				case 0: // decimal format
				{
					wnsprintf (szBuffer, sizeof(szBuffer), _T("%i"), cc.rgbResult);
					break;
				}
				case 1: // #rrggbb format
				{
					wnsprintf (szBuffer, sizeof(szBuffer), _T("#%02X%02X%02X"), GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
					break;
				}
				case 2: // rr,gg,bb format
				{
					wnsprintf (szBuffer, sizeof(szBuffer), _T("%i,%i,%i"), GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
					break;
				}
				case 3: // 0x00rrggbb format
				{
					wnsprintf (szBuffer, sizeof(szBuffer), _T("0x00%02X%02X%02X"), GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult));
					break;
				}
			}
			setuservariable (myatoi(retButton), _T("1"));
			setuservariable (myatoi(retBuffer), szBuffer);
		} else {
			setuservariable (myatoi(retButton), _T("0"));
		}
	}
}

PluginEntry(Open)
{
	int buff_size;
	TCHAR *buff; // buffer
	TCHAR init_dir[MAX_PATH]; // initial directory
	TCHAR filters[MAX_PATH]; // filters for the open dialog
	TCHAR retButton[3]; // return var of the button
	TCHAR retFile[3]; // return var of the file
	buff_size = sizeof(TCHAR)*string_size;
	buff = HeapAlloc (GetProcessHeap(), HEAP_ZERO_MEMORY, buff_size);
	EXDLL_INIT ();
	{
		OPENFILENAME ofn;
		int nRetButton;
		popstring (init_dir);
		popstring (filters);
		popstring (retButton);
		popstring (retFile);
		SecureZeroMemory (&ofn, sizeof(OPENFILENAME));
		ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		ofn.hwndOwner = hwndParent;
		ofn.lpstrInitialDir = init_dir;
		ofn.lpstrFile = buff;
		ofn.lpstrFilter = replacechar (filters, '|', '\0');
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = buff_size;
		nRetButton = myatoi (retButton);
		if (GetOpenFileName(&ofn)) {
			int nRetVal;
			nRetVal = myatoi (retFile);
			setuservariable (nRetVal, buff);
			setuservariable (nRetButton, _T("1"));
		}
		else {
			setuservariable (nRetButton, _T("0"));
		}
	}
	HeapFree (GetProcessHeap(), HEAP_ZERO_MEMORY, buff);
}

PluginEntry(Save)
{
	int buff_size;
	TCHAR *buff; // buffer
	TCHAR init_dir[MAX_PATH]; // initial directory
	TCHAR filters[MAX_PATH]; // filters for the open dialog
	TCHAR retButton[3]; // return var of the button
	TCHAR retFile[3]; // return var of the file
	buff_size = sizeof(TCHAR)*string_size;
	buff = HeapAlloc (GetProcessHeap(), HEAP_ZERO_MEMORY, buff_size);
	EXDLL_INIT ();
	{
		OPENFILENAME ofn;
		int nRetButton;
		popstring (init_dir);
		popstring (filters);
		popstring (retButton);
		popstring (retFile);
		SecureZeroMemory (&ofn, sizeof(OPENFILENAME));
		ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT;
		ofn.hwndOwner = hwndParent;
		ofn.lpstrInitialDir = init_dir;
		ofn.lpstrFile = buff;
		ofn.lpstrFilter = replacechar (filters, '|', '\0');
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = buff_size;
		nRetButton = myatoi (retButton);
		if (GetSaveFileName(&ofn)) {
			int nRetVal;
			nRetVal = myatoi (retFile);
			setuservariable (nRetVal, buff);
			setuservariable (nRetButton, _T("1"));
		}
		else {
			setuservariable (nRetButton, _T("0"));
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
		popstring (retVar);
		nRetVal = myatoi (retVar);
		setuservariable (nRetVal, _T(PACKAGE_VERSION));
	}
}

PluginEntry(FolderDlg)
{
	TCHAR info[MAX_PATH]; // secundary text
	TCHAR dir[MAX_PATH]; // init directory
	TCHAR retButton[3]; // return var of the button
	TCHAR retDir[3]; // return var of the directory
	TCHAR *buff; // buffer
	int buff_size;
	buff_size = sizeof(TCHAR)*string_size;
	buff = HeapAlloc (GetProcessHeap(), 0, buff_size);
	EXDLL_INIT();
	{
		BROWSEINFO bi;
		LPITEMIDLIST pidl;
		int nRetVal;
		int nRetButton;
		popstring(info);
		popstring(dir);
		popstring(retButton);
		popstring(retDir);
		ZeroMemory (&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = hwndParent;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = buff;
		bi.lpszTitle = info;
		bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_DONTGOBELOWDOMAIN|BIF_USENEWUI;
		bi.lParam = (LPARAM)dir;
		bi.lpfn = FolderProc;
		nRetVal = myatoi (retDir);
		nRetButton = myatoi (retButton);
		pidl = SHBrowseForFolder(&bi);
		if (SHGetPathFromIDList(pidl, buff)) {
			setuservariable (nRetVal, buff);
			setuservariable (nRetButton, _T("1"));
			CoTaskMemFree (pidl);
		} else {
			setuservariable (nRetButton, _T("0"));
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
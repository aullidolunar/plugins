#include "callbacks.h"

int CALLBACK FolderProc(HWND hwnd, UINT msg, LPARAM lParam, LPARAM lpData) {
	switch (msg) {
		case BFFM_INITIALIZED:
		{
			SendMessage (hwnd, BFFM_SETSELECTION, TRUE, lpData);
			break;
		}
	}
	return 0;
}

INT_PTR CALLBACK DialogDrivesFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static LPDRIVEDLGDATA pDDData;
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			TCHAR szLogicalDrives[MAX_PATH] = {0};
			DWORD dwSize = MAX_PATH;
			DWORD dwResult;
			pDDData = (LPDRIVEDLGDATA)lParam;
			
			SetWindowText (hDlg, pDDData->sTitle);
			SetDlgItemText (hDlg, IDOK, pDDData->sOk);
			SetDlgItemText (hDlg, IDCANCEL, pDDData->sCancel);
			dwResult = GetLogicalDriveStrings (dwSize, szLogicalDrives);
			SendDlgItemMessage (hDlg, IDC_COMBOBOX, CB_ADDSTRING, 0, (LPARAM)pDDData->sText);
			if (dwResult > 0 && dwResult <= MAX_PATH) {
				TCHAR* szSingleDrive = szLogicalDrives;
				while (*szSingleDrive) {
					//printf("Drive: %s\n", szSingleDrive);
					SendDlgItemMessage (hDlg, IDC_COMBOBOX, CB_ADDSTRING, 0, (LPARAM)szSingleDrive);
					szSingleDrive += lstrlen (szSingleDrive) + 1;
				}
			}
			SendDlgItemMessage (hDlg, IDC_COMBOBOX, CB_SETCURSEL, 0, 0);
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_COMBOBOX:
				{
					switch (HIWORD(wParam)) 
					{
						case CBN_SELCHANGE:
						{
							int npos;
							npos = SendMessage ((HWND)lParam, CB_GETCURSEL, 0, 0);
							EnableWindow (GetDlgItem (hDlg, IDOK), (BOOL)npos);
							break;
						}
					}
					break;
				}
				case IDOK:
				{
					TCHAR buff[8];
					int npos;
					npos = SendDlgItemMessage (hDlg, IDC_COMBOBOX, CB_GETCURSEL, 0, 0);
					SendDlgItemMessage (hDlg, IDC_COMBOBOX, CB_GETLBTEXT, npos, (LPARAM)&buff);
					setuservariable (pDDData->nRetButton, _T("1"));
					setuservariable (pDDData->nRetVal, buff);
					DestroyWindow (hDlg);
					break;
				}
				case IDCANCEL:
				{
					setuservariable (pDDData->nRetButton, _T("0"));
					DestroyWindow (hDlg);
					break;
				}
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hDlg);
			return TRUE;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return TRUE;
		}
	}
	return FALSE;
}

INT_PTR CALLBACK DialogFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static LPIPBDATA pPBData;
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			pPBData = (LPIPBDATA)lParam;
			SetWindowText (hDlg, pPBData->sTitle);
			SetDlgItemText (hDlg, IDC_LABEL, pPBData->sText);
			SetDlgItemText (hDlg, IDOK, pPBData->sOk);
			SetDlgItemText (hDlg, IDCANCEL, pPBData->sCancel);
			if (pPBData->bPassMask){
				HWND hEdit;
				long current_styles;
				hEdit = GetDlgItem (hDlg, IDC_ENTRY);
				current_styles = GetWindowLongPtr (hEdit, GWL_STYLE);
				SetWindowLongPtr (hEdit, GWL_STYLE, current_styles|ES_PASSWORD);
				SendMessage (hEdit, EM_SETPASSWORDCHAR, (WPARAM)'*', 0);
				InvalidateRect (hEdit, 0, 0);
			}
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDOK:
				{
					TCHAR *buff;
					buff = HeapAlloc (GetProcessHeap(), 0, pPBData->buff_size);
					GetDlgItemText (hDlg, IDC_ENTRY, buff, pPBData->buff_size);
					setuservariable (pPBData->nRetButton, _T("1"));
					setuservariable (pPBData->nRetVal, buff);
					HeapFree (GetProcessHeap(), 0, buff);
					DestroyWindow (hDlg);
					break;
				}
				case IDCANCEL:
				{
					setuservariable (pPBData->nRetButton, _T("0"));
					DestroyWindow (hDlg);
					break;
				}
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hDlg);
			return TRUE;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return TRUE;
		}
	}
	return FALSE;
}

TCHAR* replacechar(TCHAR* buff, TCHAR findchr, TCHAR replace) {
	int len;
	int z;
	len = lstrlen(buff);
	for(z = 0; z < len; z++) {
		if(buff[z] == findchr) {
			buff[z] = replace;
		}
	}
	z++;
	buff[z] = '\0';
	return buff;

}
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
					wchar_t *buff;
					buff = HeapAlloc (GetProcessHeap(), 0, pPBData->buff_size);
					GetDlgItemText (hDlg, IDC_ENTRY, buff, pPBData->buff_size);
					SetUserVariableW (pPBData->nRetButton, _T("1"));
					SetUserVariableW (pPBData->nRetVal, buff);
					HeapFree (GetProcessHeap(), 0, buff);
					DestroyWindow (hDlg);
					break;
				}
				case IDCANCEL:
				{
					SetUserVariableW (pPBData->nRetButton, _T("0"));
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

wchar_t* replacechar(wchar_t* buff, wchar_t findchr, wchar_t replace) {
	int len;
	int z;
	len = _tcslen(buff);
	for(z = 0; z < len; z++) {
		if(buff[z] == findchr) {
			buff[z] = replace;
		}
	}
	z++;
	buff[z] = '\0';
	return buff;

}
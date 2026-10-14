#include "module2b.h"

#define IDD_DIALOG2B 103
#define IDC_BTN_BACK 1003

static INT_PTR CALLBACK DlgProc2B(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_BACK)
        {
            EndDialog(hDlg, -1);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDOK)
        {
            EndDialog(hDlg, 1);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, 0);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

extern int Func_MOD2B(HWND hWnd)
{
    return (int)DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2B), hWnd, DlgProc2B);
}

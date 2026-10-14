#include "module2a.h"

#define IDD_DIALOG2A 102
#define IDC_BTN_NEXT 1002

static INT_PTR CALLBACK DlgProc2A(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_NEXT)
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

extern int Func_MOD2A(HWND hWnd)
{
    return (int)DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2A), hWnd, DlgProc2A);
}

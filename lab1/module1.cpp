#include "module1.h"

#define IDD_DIALOG1 101
#define IDC_SCROLLBAR1 1001

static int g_scrollPos = 1;

static INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetScrollRange(GetDlgItem(hDlg, IDC_SCROLLBAR1), SB_CTL, 1, 100, TRUE);
        SetScrollPos(GetDlgItem(hDlg, IDC_SCROLLBAR1), SB_CTL, g_scrollPos, TRUE);
        return (INT_PTR)TRUE;

    case WM_HSCROLL:
    {
        HWND hScroll = (HWND)lParam;
        int curPos = GetScrollPos(hScroll, SB_CTL);
        switch (LOWORD(wParam))
        {
        case SB_LINELEFT:
            curPos--;
            break;
        case SB_LINERIGHT:
            curPos++;
            break;
        case SB_PAGELEFT:
            curPos -= 10;
            break;
        case SB_PAGERIGHT:
            curPos += 10;
            break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            curPos = HIWORD(wParam);
            break;
        }
        if (curPos < 1)
            curPos = 1;
        if (curPos > 100)
            curPos = 100;
        g_scrollPos = curPos;
        SetScrollPos(hScroll, SB_CTL, g_scrollPos, TRUE);
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

extern int Func_MOD1(HWND hWnd, int *outValue)
{
    INT_PTR res = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc1);
    if (res == IDOK)
    {
        if (outValue)
            *outValue = g_scrollPos;
        return 1;
    }
    return 0;
}

// my_table.cpp - реалізація вікна таблиці об'єктів
#include "my_table.h"

#include <cstdio>
#include <cstring>

namespace {

const int IDC_TBL_LIST   = 5001;
const int IDC_TBL_DELETE = 5002;

const int DELETE_BAR_HEIGHT = 40;

}  // namespace

MyTable::MyTable()
    : hwnd(NULL), hList(NULL), hDelete(NULL), hInst(NULL),
      lastSelected(-1), updating(false),
      selectCb(NULL), selectCtx(NULL), deleteCb(NULL), deleteCtx(NULL)
{
}

MyTable::~MyTable()
{
    Close();
}

void MyTable::SetSelectHandler(Callback cb, void* context)
{
    selectCb = cb;
    selectCtx = context;
}

void MyTable::SetDeleteHandler(Callback cb, void* context)
{
    deleteCb = cb;
    deleteCtx = context;
}

// Немодальний діалог створюється з шаблону в пам'яті (окремий файл ресурсів
// не потрібен); елементи керування створюються в WM_INITDIALOG.
bool MyTable::Open(HWND owner, HINSTANCE instance)
{
    if (hwnd != NULL) {
        SetForegroundWindow(hwnd);
        return true;
    }
    hInst = instance;

    alignas(DWORD) BYTE buffer[256];
    ZeroMemory(buffer, sizeof(buffer));
    DLGTEMPLATE* dt = (DLGTEMPLATE*)buffer;
    dt->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
    dt->cdit = 0;
    dt->cx = 250;
    dt->cy = 170;
    WORD* p = (WORD*)(dt + 1);
    *p++ = 0;                                  // без меню
    *p++ = 0;                                  // стандартний клас діалогу
    wcscpy((wchar_t*)p, L"Таблиця об\u2019єктів");

    hwnd = CreateDialogIndirectParamW(hInst, dt, owner, DlgProc, (LPARAM)this);
    if (hwnd == NULL)
        return false;

    // Розташування: біля правого краю головного вікна
    RECT ow, me;
    GetWindowRect(owner, &ow);
    GetWindowRect(hwnd, &me);
    int x = ow.right - (me.right - me.left) - 30;
    int y = ow.top + 90;
    SetWindowPos(hwnd, NULL, x < 0 ? 0 : x, y, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    ShowWindow(hwnd, SW_SHOW);
    return true;
}

void MyTable::Close()
{
    if (hwnd != NULL && IsWindow(hwnd))
        DestroyWindow(hwnd);
    hwnd = hList = hDelete = NULL;
}

bool MyTable::PreTranslate(MSG* msg)
{
    return hwnd != NULL && IsDialogMessageW(hwnd, msg) != FALSE;
}

INT_PTR CALLBACK MyTable::DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    MyTable* self;
    if (msg == WM_INITDIALOG) {
        self = (MyTable*)lp;
        SetWindowLongPtrW(hDlg, DWLP_USER, (LONG_PTR)self);
        self->hwnd = hDlg;
    } else {
        self = (MyTable*)GetWindowLongPtrW(hDlg, DWLP_USER);
    }
    if (self == NULL)
        return FALSE;
    return self->HandleMessage(msg, wp, lp);
}

INT_PTR MyTable::HandleMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        OnInit();
        return TRUE;
    case WM_SIZE:
        OnSize();
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wp) == IDC_TBL_DELETE)
            RequestDelete();
        else if (LOWORD(wp) == IDCANCEL)          // клавіша Esc
            DestroyWindow(hwnd);
        return TRUE;
    case WM_NOTIFY:
        OnNotify((NMHDR*)lp);
        return TRUE;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return TRUE;
    case WM_DESTROY:
        hwnd = hList = hDelete = NULL;
        lastSelected = -1;
        return TRUE;
    }
    return FALSE;
}

void MyTable::OnInit()
{
    HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    hList = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL |
        LVS_SHOWSELALWAYS,
        0, 0, 0, 0, hwnd, (HMENU)(UINT_PTR)IDC_TBL_LIST, hInst, NULL);
    ListView_SetExtendedListViewStyle(hList,
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    SendMessageW(hList, WM_SETFONT, (WPARAM)font, TRUE);

    const wchar_t* titles[] = { L"\u2116", L"Тип об\u2019єкта", L"x1", L"y1",
                                L"x2", L"y2" };
    const int widths[] = { 36, 150, 50, 50, 50, 50 };
    for (int i = 0; i < 6; i++) {
        LVCOLUMNW col;
        ZeroMemory(&col, sizeof(col));
        col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        col.pszText = (LPWSTR)titles[i];
        col.cx = widths[i];
        col.iSubItem = i;
        SendMessageW(hList, LVM_INSERTCOLUMNW, i, (LPARAM)&col);
    }

    hDelete = CreateWindowExW(0, L"BUTTON", L"Вилучити вибраний об\u2019єкт",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        0, 0, 0, 0, hwnd, (HMENU)(UINT_PTR)IDC_TBL_DELETE, hInst, NULL);
    SendMessageW(hDelete, WM_SETFONT, (WPARAM)font, TRUE);

    OnSize();
}

void MyTable::OnSize()
{
    if (hList == NULL || hDelete == NULL)
        return;
    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right, h = rc.bottom;
    MoveWindow(hList, 0, 0, w, h > DELETE_BAR_HEIGHT ? h - DELETE_BAR_HEIGHT : 0,
               TRUE);
    MoveWindow(hDelete, w - 190, h - DELETE_BAR_HEIGHT + 7, 180, 26, TRUE);
}

void MyTable::OnNotify(NMHDR* hdr)
{
    if (hdr->hwndFrom != hList)
        return;

    if (hdr->code == LVN_ITEMCHANGED && !updating) {
        NMLISTVIEW* nm = (NMLISTVIEW*)hdr;
        if ((nm->uChanged & LVIF_STATE) &&
            ((nm->uNewState ^ nm->uOldState) & LVIS_SELECTED)) {
            int sel = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
            if (sel != lastSelected) {
                lastSelected = sel;
                if (selectCb != NULL)
                    selectCb(sel, selectCtx);
            }
        }
    } else if (hdr->code == LVN_KEYDOWN) {
        if (((NMLVKEYDOWN*)hdr)->wVKey == VK_DELETE)
            RequestDelete();
    }
}

void MyTable::RequestDelete()
{
    if (hList == NULL)
        return;
    int row = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
    if (row >= 0 && deleteCb != NULL)
        deleteCb(row, deleteCtx);
}

void MyTable::Clear()
{
    if (hList == NULL)
        return;
    updating = true;
    ListView_DeleteAllItems(hList);
    lastSelected = -1;
    updating = false;
}

void MyTable::AddRow(const wchar_t* type, long x1, long y1, long x2, long y2)
{
    if (hList == NULL)
        return;
    updating = true;
    int row = ListView_GetItemCount(hList);
    wchar_t buf[32];

    wsprintfW(buf, L"%d", row + 1);
    LVITEMW item;
    ZeroMemory(&item, sizeof(item));
    item.mask = LVIF_TEXT;
    item.iItem = row;
    item.pszText = buf;
    SendMessageW(hList, LVM_INSERTITEMW, 0, (LPARAM)&item);

    ListView_SetItemText(hList, row, 1, (LPWSTR)type);
    const long values[4] = { x1, y1, x2, y2 };
    for (int i = 0; i < 4; i++) {
        wsprintfW(buf, L"%ld", values[i]);
        ListView_SetItemText(hList, row, 2 + i, buf);
    }
    updating = false;
}

void MyTable::RemoveRow(int row)
{
    if (hList == NULL)
        return;
    updating = true;
    ListView_DeleteItem(hList, row);
    Renumber();
    lastSelected = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
    updating = false;
}

void MyTable::Renumber()
{
    int n = ListView_GetItemCount(hList);
    for (int i = 0; i < n; i++) {
        wchar_t buf[16];
        wsprintfW(buf, L"%d", i + 1);
        ListView_SetItemText(hList, i, 0, buf);
    }
}

// app_toolbar.cpp - реалізація панелі інструментів (лаб. №4)
#include "app_toolbar.h"
#include "variant.h"

namespace {

const int ICON_SIZE = 24;
const int BUTTON_COUNT = 6;
const COLORREF MASK_COLOR = RGB(255, 0, 255);

struct ButtonInfo {
    int id;
    const wchar_t* tip;
};

const ButtonInfo BUTTONS[BUTTON_COUNT] = {
    { IDM_POINT,   L"Крапка"      },
    { IDM_LINE,    L"Лінія"       },
    { IDM_RECT,    L"Прямокутник" },
    { IDM_ELLIPSE, L"Еліпс"       },
    { IDM_LINEOO,  L"Лінія з кружечками" },
    { IDM_CUBE,    L"Каркас куба" }
};

}  // namespace

AppToolbar::AppToolbar() : hwnd(NULL), himl(NULL)
{
}

AppToolbar::~AppToolbar()
{
    if (himl != NULL)
        ImageList_Destroy(himl);
}

// Піктограми малюються програмно, окремі файли ресурсів не потрібні
HIMAGELIST AppToolbar::CreateButtonImages() const
{
    HDC screen = GetDC(NULL);
    HDC mem = CreateCompatibleDC(screen);
    HBITMAP bmp = CreateCompatibleBitmap(screen, ICON_SIZE * BUTTON_COUNT,
                                         ICON_SIZE);
    HGDIOBJ oldBmp = SelectObject(mem, bmp);

    RECT all = { 0, 0, ICON_SIZE * BUTTON_COUNT, ICON_SIZE };
    HBRUSH maskBrush = CreateSolidBrush(MASK_COLOR);
    FillRect(mem, &all, maskBrush);
    DeleteObject(maskBrush);

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN thickPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH blueBrush = CreateSolidBrush(RGB(135, 206, 250));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN greenPen = CreatePen(PS_SOLID, 2, Variant::LINEOO_LINE_COLOR);
    HBRUSH yellowBrush = CreateSolidBrush(Variant::LINEOO_CIRCLE_FILL);
    HPEN redPen = CreatePen(PS_SOLID, 1, Variant::CUBE_EDGE_COLOR);
    HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

    HGDIOBJ oldPen = SelectObject(mem, blackPen);
    HGDIOBJ oldBrush = SelectObject(mem, blackBrush);

    // Крапка
    int x = 0;
    Ellipse(mem, x + 8, 8, x + 17, 17);

    // Лінія
    x += ICON_SIZE;
    SelectObject(mem, thickPen);
    MoveToEx(mem, x + 4, 20, NULL);
    LineTo(mem, x + 20, 4);
    SelectObject(mem, blackPen);

    // Прямокутник
    x += ICON_SIZE;
    SelectObject(mem, blueBrush);
    Rectangle(mem, x + 3, 6, x + 21, 18);

    // Еліпс
    x += ICON_SIZE;
    SelectObject(mem, whiteBrush);
    Ellipse(mem, x + 2, 5, x + 22, 19);

    // Лінія з кружечками
    x += ICON_SIZE;
    SelectObject(mem, greenPen);
    SelectObject(mem, yellowBrush);
    MoveToEx(mem, x + 6, 18, NULL);
    LineTo(mem, x + 18, 6);
    Ellipse(mem, x + 2, 14, x + 10, 22);
    Ellipse(mem, x + 14, 2, x + 22, 10);

    // Каркас куба
    x += ICON_SIZE;
    SelectObject(mem, redPen);
    SelectObject(mem, nullBrush);
    Rectangle(mem, x + 9, 3, x + 21, 15);     // задня грань
    Rectangle(mem, x + 3, 9, x + 15, 21);     // передня грань
    MoveToEx(mem, x + 3, 9, NULL);   LineTo(mem, x + 9, 3);
    MoveToEx(mem, x + 15, 9, NULL);  LineTo(mem, x + 21, 3);
    MoveToEx(mem, x + 3, 21, NULL);  LineTo(mem, x + 9, 15);
    MoveToEx(mem, x + 15, 21, NULL); LineTo(mem, x + 21, 15);

    SelectObject(mem, oldBrush);
    SelectObject(mem, oldPen);
    SelectObject(mem, oldBmp);

    DeleteObject(blackPen);
    DeleteObject(thickPen);
    DeleteObject(blackBrush);
    DeleteObject(blueBrush);
    DeleteObject(whiteBrush);
    DeleteObject(greenPen);
    DeleteObject(yellowBrush);
    DeleteObject(redPen);
    DeleteDC(mem);
    ReleaseDC(NULL, screen);

    HIMAGELIST list = ImageList_Create(ICON_SIZE, ICON_SIZE,
                                       ILC_COLOR24 | ILC_MASK, BUTTON_COUNT, 0);
    if (list != NULL)
        ImageList_AddMasked(list, bmp, MASK_COLOR);
    DeleteObject(bmp);
    return list;
}

bool AppToolbar::Create(HWND parent, HINSTANCE hInst)
{
    hwnd = CreateWindowExW(0, TOOLBARCLASSNAMEW, NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_TOP |
        TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0, parent, (HMENU)(UINT_PTR)IDC_TOOLBAR, hInst, NULL);
    if (hwnd == NULL)
        return false;

    SendMessageW(hwnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

    himl = CreateButtonImages();
    SendMessageW(hwnd, TB_SETIMAGELIST, 0, (LPARAM)himl);

    TBBUTTON tb[BUTTON_COUNT];
    ZeroMemory(tb, sizeof(tb));
    for (int i = 0; i < BUTTON_COUNT; i++) {
        tb[i].iBitmap = i;
        tb[i].idCommand = BUTTONS[i].id;
        tb[i].fsState = TBSTATE_ENABLED;
        tb[i].fsStyle = TBSTYLE_CHECKGROUP;
    }
    SendMessageW(hwnd, TB_ADDBUTTONS, BUTTON_COUNT, (LPARAM)tb);
    SendMessageW(hwnd, TB_AUTOSIZE, 0, 0);
    return true;
}

void AppToolbar::Resize()
{
    if (hwnd != NULL)
        SendMessageW(hwnd, TB_AUTOSIZE, 0, 0);
}

void AppToolbar::CheckType(int commandId)
{
    if (hwnd == NULL)
        return;
    for (int i = 0; i < BUTTON_COUNT; i++) {
        SendMessageW(hwnd, TB_CHECKBUTTON, BUTTONS[i].id,
                     MAKELONG(BUTTONS[i].id == commandId, 0));
    }
}

const wchar_t* AppToolbar::GetTipText(int commandId) const
{
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (BUTTONS[i].id == commandId)
            return BUTTONS[i].tip;
    }
    return L"";
}

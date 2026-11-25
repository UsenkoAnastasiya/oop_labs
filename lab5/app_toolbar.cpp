// app_toolbar.cpp - реалізація панелі інструментів
#include "app_toolbar.h"
#include "shape_registry.h"

#include <vector>

namespace {

const COLORREF MASK_COLOR = RGB(255, 0, 255);

}  // namespace

AppToolbar::AppToolbar() : hwnd(NULL), himl(NULL)
{
}

AppToolbar::~AppToolbar()
{
    if (himl != NULL)
        ImageList_Destroy(himl);
}

// Піктограми малює кожен модуль фігури (ShapeInfo::drawIcon) на спільній
// смузі-бітмапі з прозорим (пурпуровим) фоном.
HIMAGELIST AppToolbar::CreateButtonImages() const
{
    const ShapeRegistry& registry = ShapeRegistry::Instance();
    const int count = registry.Count();
    const int size = SHAPE_ICON_SIZE;

    HDC screen = GetDC(NULL);
    HDC mem = CreateCompatibleDC(screen);
    HBITMAP bmp = CreateCompatibleBitmap(screen, size * count, size);
    HGDIOBJ oldBmp = SelectObject(mem, bmp);

    RECT all = { 0, 0, size * count, size };
    HBRUSH maskBrush = CreateSolidBrush(MASK_COLOR);
    FillRect(mem, &all, maskBrush);
    DeleteObject(maskBrush);

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ oldPen = SelectObject(mem, blackPen);
    for (int i = 0; i < count; i++)
        registry.At(i).drawIcon(mem, i * size);
    SelectObject(mem, oldPen);
    DeleteObject(blackPen);

    SelectObject(mem, oldBmp);
    DeleteDC(mem);
    ReleaseDC(NULL, screen);

    HIMAGELIST list = ImageList_Create(size, size, ILC_COLOR24 | ILC_MASK,
                                       count, 0);
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

    const int count = ShapeRegistry::Instance().Count();
    std::vector<TBBUTTON> tb(count);
    for (int i = 0; i < count; i++) {
        ZeroMemory(&tb[i], sizeof(TBBUTTON));
        tb[i].iBitmap = i;
        tb[i].idCommand = IDM_SHAPE_BASE + i;
        tb[i].fsState = TBSTATE_ENABLED;
        tb[i].fsStyle = TBSTYLE_CHECKGROUP;
    }
    if (count > 0)
        SendMessageW(hwnd, TB_ADDBUTTONS, count, (LPARAM)&tb[0]);
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
    const int count = ShapeRegistry::Instance().Count();
    for (int i = 0; i < count; i++) {
        int id = IDM_SHAPE_BASE + i;
        SendMessageW(hwnd, TB_CHECKBUTTON, id, MAKELONG(id == commandId, 0));
    }
}

const wchar_t* AppToolbar::GetTipText(int commandId) const
{
    const ShapeRegistry& registry = ShapeRegistry::Instance();
    int index = commandId - IDM_SHAPE_BASE;
    if (index < 0 || index >= registry.Count())
        return L"";
    return registry.At(index).title;
}

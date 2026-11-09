// main_window.cpp - реалізація головного вікна
#include "main_window.h"
#include "ids.h"
#include "variant.h"

namespace {
const wchar_t CLASS_NAME[] = L"Lab3EditorWindow";
const wchar_t WINDOW_TITLE[] =
    L"Lab3 - Графічний редактор - Усенко Анастасія (ІМ-51)";
}

MainWindow::MainWindow()
    : hwnd(NULL), hInst(NULL), hObjectsMenu(NULL)
{
}

bool MainWindow::Register(HINSTANCE hInstance)
{
    WNDCLASSEXW wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindow::WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIconW(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursorW(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = CLASS_NAME;
    return RegisterClassExW(&wc) != 0;
}

// Меню: Файл | Об'єкти | Довідка
HMENU MainWindow::BuildMenu()
{
    HMENU bar = CreateMenu();

    HMENU file = CreatePopupMenu();
    AppendMenuW(file, MF_STRING, IDM_CLEAR, L"&Очистити");
    AppendMenuW(file, MF_SEPARATOR, 0, NULL);
    AppendMenuW(file, MF_STRING, IDM_EXIT, L"&Вихід");

    hObjectsMenu = CreatePopupMenu();
    AppendMenuW(hObjectsMenu, MF_STRING, IDM_POINT, L"&Крапка");
    AppendMenuW(hObjectsMenu, MF_STRING, IDM_LINE, L"&Лінія");
    AppendMenuW(hObjectsMenu, MF_STRING, IDM_RECT, L"&Прямокутник");
    AppendMenuW(hObjectsMenu, MF_STRING, IDM_ELLIPSE, L"&Еліпс");

    HMENU help = CreatePopupMenu();
    AppendMenuW(help, MF_STRING, IDM_ABOUT, L"&Про програму");

    AppendMenuW(bar, MF_POPUP, (UINT_PTR)file, L"&Файл");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)hObjectsMenu, L"&Об\u2019єкти");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)help, L"&Довідка");
    return bar;
}

bool MainWindow::Create(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HMENU menu = BuildMenu();

    HWND h = CreateWindowExW(0, CLASS_NAME, WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 620,
        NULL, menu, hInstance, this);
    if (h == NULL) {
        DestroyMenu(menu);
        return false;
    }
    ShowWindow(h, nCmdShow);
    UpdateWindow(h);
    return true;
}

// Статична віконна процедура лише передає повідомлення об'єкту класу
LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    MainWindow* self;
    if (msg == WM_NCCREATE) {
        CREATESTRUCTW* cs = (CREATESTRUCTW*)lp;
        self = (MainWindow*)cs->lpCreateParams;
        self->hwnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)self);
    } else {
        self = (MainWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    }
    if (self == NULL)
        return DefWindowProcW(hWnd, msg, wp, lp);
    return self->HandleMessage(msg, wp, lp);
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_CREATE:
        return OnCreate() ? 0 : -1;
    case WM_SIZE:
        OnSize();
        return 0;
    case WM_COMMAND:
        OnCommand(LOWORD(wp));
        return 0;
    case WM_NOTIFY:
        return OnNotify(lp);
    case WM_INITMENUPOPUP:
        OnInitMenuPopup((HMENU)wp, LOWORD(lp), (BOOL)HIWORD(lp));
        return 0;
    case WM_LBUTTONDOWN:
        OnLButtonDown(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
        return 0;
    case WM_LBUTTONUP:
        OnLButtonUp(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
        return 0;
    case WM_MOUSEMOVE:
        OnMouseMove(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
        return 0;
    case WM_CAPTURECHANGED:
        OnCaptureChanged();
        return 0;
    case WM_ERASEBKGND:
        return 1;   // фон малюється в OnPaint (подвійна буферизація)
    case WM_PAINT:
        OnPaint();
        return 0;
    case WM_DESTROY:
        OnDestroy();
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

bool MainWindow::OnCreate()
{
    if (!toolbar.Create(hwnd, hInst))
        return false;
    toolbar.CheckType(IDM_POINT + editor.GetType());
    return true;
}

void MainWindow::OnSize()
{
    toolbar.Resize();
    InvalidateRect(hwnd, NULL, FALSE);
}

void MainWindow::SelectObjectType(int id)
{
    editor.SetType((ShapeType)(id - IDM_POINT));
    toolbar.CheckType(id);
}

void MainWindow::OnCommand(int id)
{
    switch (id) {
    case IDM_POINT:
    case IDM_LINE:
    case IDM_RECT:
    case IDM_ELLIPSE:
        SelectObjectType(id);
        break;
    case IDM_CLEAR:
        editor.Clear();
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case IDM_EXIT:
        DestroyWindow(hwnd);
        break;
    case IDM_ABOUT: {
        wchar_t text[256];
        wsprintfW(text,
            L"Lab3 - Графічний редактор\n"
            L"Виконала: Усенко Анастасія, група ІМ-51\n\n"
            L"Ж = %d, розмір масиву вказівників N = %d",
            Variant::ZH, Variant::N);
        MessageBoxW(hwnd, text, L"Про програму", MB_OK | MB_ICONINFORMATION);
        break;
    }
    }
}

// Підказки (tooltips) кнопок панелі інструментів
LRESULT MainWindow::OnNotify(LPARAM lp)
{
    NMHDR* hdr = (NMHDR*)lp;
    if (hdr->code == TTN_GETDISPINFOW) {
        NMTTDISPINFOW* info = (NMTTDISPINFOW*)lp;
        info->lpszText = (LPWSTR)toolbar.GetTipText((int)hdr->idFrom);
    }
    return 0;
}

// Позначка поточного типу об'єкта в меню "Об'єкти" (Ж mod 2 = 0)
void MainWindow::OnInitMenuPopup(HMENU hPopup, UINT, BOOL isSystemMenu)
{
    if (isSystemMenu)
        return;
    if (hPopup == hObjectsMenu) {
        CheckMenuRadioItem(hPopup, IDM_POINT, IDM_ELLIPSE,
                           IDM_POINT + editor.GetType(), MF_BYCOMMAND);
    }
}

void MainWindow::OnLButtonDown(int x, int y)
{
    if (!editor.OnLBdown(x, y)) {
        wchar_t text[128];
        wsprintfW(text, L"Масив заповнено: максимум %d об\u2019єктів.",
                  Variant::N);
        MessageBoxW(hwnd, text, L"Увага", MB_OK | MB_ICONWARNING);
        return;
    }
    SetCapture(hwnd);
}

void MainWindow::OnLButtonUp(int x, int y)
{
    if (!editor.IsDrawing())
        return;
    editor.OnLBup(x, y);
    ReleaseCapture();
    InvalidateRect(hwnd, NULL, FALSE);
}

void MainWindow::OnMouseMove(int x, int y)
{
    if (editor.IsDrawing()) {
        editor.OnMouseMove(x, y);
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void MainWindow::OnCaptureChanged()
{
    if (editor.IsDrawing()) {
        editor.Cancel();
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void MainWindow::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);
    if (rc.right > 0 && rc.bottom > 0) {
        HDC mem = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        HGDIOBJ oldBmp = SelectObject(mem, bmp);

        FillRect(mem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
        editor.OnPaint(mem);
        BitBlt(hdc, 0, 0, rc.right, rc.bottom, mem, 0, 0, SRCCOPY);

        SelectObject(mem, oldBmp);
        DeleteObject(bmp);
        DeleteDC(mem);
    }
    EndPaint(hwnd, &ps);
}

void MainWindow::OnDestroy()
{
    PostQuitMessage(0);
}

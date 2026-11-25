// main_window.cpp - реалізація головного вікна (лаб. №5)
#include "main_window.h"
#include "ids.h"
#include "shape_registry.h"
#include "variant.h"

#include <commdlg.h>

namespace
{
    const wchar_t CLASS_NAME[] = L"Lab5EditorWindow";
    const wchar_t WINDOW_TITLE[] =
        L"Lab5 - Графічний редактор - Усенко Анастасія (ІМ-51)";
}

MainWindow::MainWindow()
    : hwnd(NULL), hInst(NULL), hObjectsMenu(NULL), hWindowMenu(NULL),
      editor(MyEditor::Instance())
{
    // Клієнт реєструє у таблиці функції зворотного виклику
    table.SetSelectHandler(TableSelectThunk, this);
    table.SetDeleteHandler(TableDeleteThunk, this);
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

// Меню: Файл | Об'єкти | Вікно | Довідка.
// Пункти меню "Об'єкти" будуються за реєстром типів фігур.
HMENU MainWindow::BuildMenu()
{
    HMENU bar = CreateMenu();

    HMENU file = CreatePopupMenu();
    AppendMenuW(file, MF_STRING, IDM_OPEN, L"&Відкрити...");
    AppendMenuW(file, MF_STRING, IDM_SAVE, L"&Зберегти...");
    AppendMenuW(file, MF_SEPARATOR, 0, NULL);
    AppendMenuW(file, MF_STRING, IDM_CLEAR, L"&Очистити");
    AppendMenuW(file, MF_SEPARATOR, 0, NULL);
    AppendMenuW(file, MF_STRING, IDM_EXIT, L"Ви&хід");

    hObjectsMenu = CreatePopupMenu();
    const ShapeRegistry &registry = ShapeRegistry::Instance();
    for (int i = 0; i < registry.Count(); i++)
        AppendMenuW(hObjectsMenu, MF_STRING, IDM_SHAPE_BASE + i,
                    registry.At(i).title);

    hWindowMenu = CreatePopupMenu();
    AppendMenuW(hWindowMenu, MF_STRING, IDM_TABLE, L"&Таблиця об\u2019єктів");

    HMENU help = CreatePopupMenu();
    AppendMenuW(help, MF_STRING, IDM_ABOUT, L"&Про програму");

    AppendMenuW(bar, MF_POPUP, (UINT_PTR)file, L"&Файл");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)hObjectsMenu, L"&Об\u2019єкти");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)hWindowMenu, L"&Вікно");
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
    if (h == NULL)
    {
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
    MainWindow *self;
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCTW *cs = (CREATESTRUCTW *)lp;
        self = (MainWindow *)cs->lpCreateParams;
        self->hwnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)self);
    }
    else
    {
        self = (MainWindow *)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    }
    if (self == NULL)
        return DefWindowProcW(hWnd, msg, wp, lp);
    return self->HandleMessage(msg, wp, lp);
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
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
        return OnEraseBkgnd((HDC)wp) ? 1 : 0;
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
    toolbar.CheckType(IDM_SHAPE_BASE + editor.GetType());
    return true;
}

void MainWindow::OnSize()
{
    toolbar.Resize();
    InvalidateRect(hwnd, NULL, FALSE);
}

void MainWindow::SelectObjectType(int id)
{
    editor.SetType(id - IDM_SHAPE_BASE);
    toolbar.CheckType(id);
}

void MainWindow::OnCommand(int id)
{
    if (id >= IDM_SHAPE_BASE &&
        id < IDM_SHAPE_BASE + ShapeRegistry::Instance().Count())
    {
        SelectObjectType(id);
        return;
    }
    switch (id)
    {
    case IDM_OPEN:
        OnOpenFile();
        break;
    case IDM_SAVE:
        OnSaveFile();
        break;
    case IDM_CLEAR:
        editor.Clear();
        table.Clear();
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case IDM_EXIT:
        DestroyWindow(hwnd);
        break;
    case IDM_TABLE:
        ToggleTable();
        break;
    case IDM_ABOUT:
    {
        wchar_t text[256];
        wsprintfW(text,
                  L"Lab5 - Графічний редактор\n"
                  L"Виконала: Усенко Анастасія, група ІМ-51\n\n"
                  L"Ж = %d, розмір масиву вказівників N = %d\n"
                  L"MyEditor - Singleton Меєрса",
                  Variant::ZH, Variant::N);
        MessageBoxW(hwnd, text, L"Про програму", MB_OK | MB_ICONINFORMATION);
        break;
    }
    }
}

// Підказки (tooltips) кнопок панелі інструментів
LRESULT MainWindow::OnNotify(LPARAM lp)
{
    NMHDR *hdr = (NMHDR *)lp;
    if (hdr->code == TTN_GETDISPINFOW)
    {
        NMTTDISPINFOW *info = (NMTTDISPINFOW *)lp;
        info->lpszText = (LPWSTR)toolbar.GetTipText((int)hdr->idFrom);
    }
    return 0;
}

// Позначки в меню: поточний тип об'єкта та стан вікна таблиці
void MainWindow::OnInitMenuPopup(HMENU hPopup, UINT, BOOL isSystemMenu)
{
    if (isSystemMenu)
        return;
    if (hPopup == hObjectsMenu)
    {
        int count = ShapeRegistry::Instance().Count();
        if (count > 0)
            CheckMenuRadioItem(hPopup, IDM_SHAPE_BASE,
                               IDM_SHAPE_BASE + count - 1,
                               IDM_SHAPE_BASE + editor.GetType(), MF_BYCOMMAND);
    }
    else if (hPopup == hWindowMenu)
    {
        CheckMenuItem(hPopup, IDM_TABLE,
                      MF_BYCOMMAND | (table.IsOpen() ? MF_CHECKED : MF_UNCHECKED));
    }
}

void MainWindow::OnLButtonDown(int x, int y)
{
    if (!editor.OnLBdown(x, y))
    {
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
    bool created = editor.OnLBup(x, y);
    ReleaseCapture();
    if (created && table.IsOpen())
        AddTableRow(editor.Count() - 1);
    InvalidateRect(hwnd, NULL, FALSE);
}

void MainWindow::OnMouseMove(int x, int y)
{
    if (editor.IsDrawing())
    {
        editor.OnMouseMove(x, y);
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void MainWindow::OnCaptureChanged()
{
    if (editor.IsDrawing())
    {
        editor.Cancel();
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

bool MainWindow::OnEraseBkgnd(HDC hdc)
{
    RECT rc;
    GetClipBox(hdc, &rc);
    FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
    return true;
}

void MainWindow::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);
    if (rc.right > 0 && rc.bottom > 0)
    {
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

// Вікно таблиці автоматично закривається при виході з програми
void MainWindow::OnDestroy()
{
    table.Close();
    PostQuitMessage(0);
}

// ----------------------------------------------------------------- Файли ---

void MainWindow::OnSaveFile()
{
    wchar_t path[MAX_PATH] = L"objects.txt";
    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Текстові файли (*.txt)\0*.txt\0Усі файли (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = L"txt";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    if (!GetSaveFileNameW(&ofn))
        return;
    if (!editor.SaveToFile(path))
        MessageBoxW(hwnd, L"Не вдалося записати файл.", L"Помилка",
                    MB_OK | MB_ICONERROR);
}

void MainWindow::OnOpenFile()
{
    wchar_t path[MAX_PATH] = L"";
    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Текстові файли (*.txt)\0*.txt\0Усі файли (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (!GetOpenFileNameW(&ofn))
        return;

    int loaded = editor.LoadFromFile(path);
    if (loaded < 0)
    {
        MessageBoxW(hwnd, L"Файл має неправильний формат.", L"Помилка",
                    MB_OK | MB_ICONERROR);
        return;
    }
    SyncTable();
    InvalidateRect(hwnd, NULL, FALSE);

    wchar_t text[128];
    wsprintfW(text, L"Завантажено об\u2019єктів: %d", loaded);
    MessageBoxW(hwnd, text, L"Відкрити", MB_OK | MB_ICONINFORMATION);
}

// --------------------------------------------------------------- Таблиця ---

void MainWindow::ToggleTable()
{
    if (table.IsOpen())
    {
        table.Close();
        editor.SetSelected(-1);
        InvalidateRect(hwnd, NULL, FALSE);
    }
    else if (table.Open(hwnd, hInst))
    {
        SyncTable();
    }
}

void MainWindow::AddTableRow(int index)
{
    const Shape *shape = editor.At(index);
    if (shape == NULL)
        return;
    long x1, y1, x2, y2;
    shape->Get(x1, y1, x2, y2);
    table.AddRow(shape->Title(), x1, y1, x2, y2);
}

void MainWindow::SyncTable()
{
    if (!table.IsOpen())
        return;
    table.Clear();
    for (int i = 0; i < editor.Count(); i++)
        AddTableRow(i);
}

// Бонус 1: рядок таблиці виділено - відповідний об'єкт виділяється в
// головному вікні (товсте помаранчеве перо)
void MainWindow::OnTableSelect(int row)
{
    editor.SetSelected(row);
    InvalidateRect(hwnd, NULL, FALSE);
}

// Бонус 2: користувач вилучає виділений рядок - об'єкт видаляється з масиву
void MainWindow::OnTableDelete(int row)
{
    if (editor.Remove(row))
    {
        table.RemoveRow(row);
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void MainWindow::TableSelectThunk(int row, void *context)
{
    static_cast<MainWindow *>(context)->OnTableSelect(row);
}

void MainWindow::TableDeleteThunk(int row, void *context)
{
    static_cast<MainWindow *>(context)->OnTableDelete(row);
}

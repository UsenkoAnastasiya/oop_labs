#include <windows.h>
#include <cstdio>
#include "module1.h"
#include "module2a.h"
#include "module2b.h"

#define IDM_WORK1 1001
#define IDM_WORK2 1002

static wchar_t g_displayText[256] = L"Результат відобразиться тут";

void RunWork2Sequence(HWND hWnd)
{
    int step = 1;
    while (step > 0)
    {
        if (step == 1)
        {
            int resA = Func_MOD2A(hWnd);
            if (resA == 1)
            {
                step = 2;
            }
            else
            {
                step = 0;
                break;
            }
        }
        else if (step == 2)
        {
            int resB = Func_MOD2B(hWnd);
            if (resB == -1)
            {
                step = 1;
            }
            else if (resB == 1)
            {
                swprintf_s(g_displayText, L"Робота 2: Завершено успішно!");
                InvalidateRect(hWnd, NULL, TRUE);
                step = 0;
            }
            else
            {
                step = 0;
                break;
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDM_WORK1)
        {
            int val = 0;
            if (Func_MOD1(hWnd, &val))
            {
                swprintf_s(g_displayText, L"Обране число: %d", val);
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else if (LOWORD(wParam) == IDM_WORK2)
        {
            RunWork2Sequence(hWnd);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOutW(hdc, 20, 20, g_displayText, static_cast<int>(wcslen(g_displayText)));
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Lab2Class";
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassW(&wc))
    {
        MessageBoxW(NULL, L"Помилка реєстрації класу вікна!", L"Помилка", MB_ICONERROR);
        return 0;
    }

    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenuW(hSubMenu, MF_STRING, IDM_WORK1, L"Робота 1");
    AppendMenuW(hSubMenu, MF_STRING, IDM_WORK2, L"Робота 2");
    AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), L"Завдання");

    HWND hWnd = CreateWindowW(
        L"Lab2Class",
        L"Лабораторна 1 (Варіант 25)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, hMenu, hInstance, NULL);

    if (!hWnd)
    {
        MessageBoxW(NULL, L"Помилка створення вікна!", L"Помилка", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

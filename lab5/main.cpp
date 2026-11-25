// main.cpp - точка входу програми Lab5
#include "common.h"
#include "main_window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    if (!MainWindow::Register(hInstance))
        return 1;

    MainWindow window;
    if (!window.Create(hInstance, nCmdShow))
        return 1;

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        if (window.PreTranslate(&msg))      // повідомлення немодальної таблиці
            continue;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

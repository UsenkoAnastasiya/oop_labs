// main_window.h - головне вікно графічного редактора
#pragma once
#include "common.h"
#include "app_toolbar.h"
#include "objects_editor.h"

class MainWindow {
public:
    MainWindow();

    static bool Register(HINSTANCE hInst);
    bool Create(HINSTANCE hInst, int nCmdShow);

private:
    MainWindow(const MainWindow&);
    MainWindow& operator=(const MainWindow&);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp);

    // Методи-обробники повідомлень - функції-члени класу
    bool OnCreate();
    void OnSize();
    void OnCommand(int id);
    LRESULT OnNotify(LPARAM lp);
    void OnInitMenuPopup(HMENU hPopup, UINT pos, BOOL isSystemMenu);
    void OnLButtonDown(int x, int y);
    void OnLButtonUp(int x, int y);
    void OnMouseMove(int x, int y);
    void OnCaptureChanged();
    void OnPaint();
    void OnDestroy();

    HMENU BuildMenu();
    void SelectObjectType(int id);

    HWND hwnd;
    HINSTANCE hInst;
    HMENU hObjectsMenu;     // підменю "Об'єкти"
    AppToolbar toolbar;
    ObjectsEditor editor;
};

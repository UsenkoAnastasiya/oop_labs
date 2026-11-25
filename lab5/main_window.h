// main_window.h - головне вікно графічного редактора (лаб. №5)
#pragma once
#include "common.h"
#include "app_toolbar.h"
#include "my_editor.h"
#include "my_table.h"

class MainWindow
{
public:
    MainWindow();

    static bool Register(HINSTANCE hInst);
    bool Create(HINSTANCE hInst, int nCmdShow);

    // Для циклу повідомлень: клавіатура немодального вікна таблиці
    bool PreTranslate(MSG *msg) { return table.PreTranslate(msg); }

private:
    MainWindow(const MainWindow &);
    MainWindow &operator=(const MainWindow &);

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
    bool OnEraseBkgnd(HDC hdc);
    void OnPaint();
    void OnDestroy();

    HMENU BuildMenu();
    void SelectObjectType(int id);

    // Файли
    void OnOpenFile();
    void OnSaveFile();

    // Таблиця та її зворотні виклики (callback)
    void ToggleTable();
    void SyncTable();                  // заповнити таблицю даними редактора
    void AddTableRow(int index);
    void OnTableSelect(int row);       // бонус 1: виділення об'єкта
    void OnTableDelete(int row);       // бонус 2: вилучення об'єкта
    static void TableSelectThunk(int row, void *context);
    static void TableDeleteThunk(int row, void *context);

    HWND hwnd;
    HINSTANCE hInst;
    HMENU hObjectsMenu;  // підменю "Об'єкти"
    HMENU hWindowMenu;   // підменю "Вікно"
    MyEditor &editor;    // єдиний екземпляр - Singleton Меєрса
    AppToolbar toolbar;
    MyTable table;
};

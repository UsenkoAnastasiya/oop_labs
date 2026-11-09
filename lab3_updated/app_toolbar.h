// app_toolbar.h - вікно Toolbar з кнопками типів об'єктів
#pragma once
#include "common.h"
#include "ids.h"

class AppToolbar {
public:
    AppToolbar();
    ~AppToolbar();

    bool Create(HWND parent, HINSTANCE hInst);
    void Resize();                      // TB_AUTOSIZE після зміни розмірів вікна
    void CheckType(int commandId);      // позначити натиснуту кнопку
    const wchar_t* GetTipText(int commandId) const;   // текст підказки (tooltip)

private:
    AppToolbar(const AppToolbar&);
    AppToolbar& operator=(const AppToolbar&);

    HIMAGELIST CreateButtonImages() const;

    HWND hwnd;
    HIMAGELIST himl;
};

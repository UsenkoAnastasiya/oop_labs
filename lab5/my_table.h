// my_table.h - вікно таблиці об'єктів (немодальне вікно діалогу).
// Модуль не залежить від інших модулів програми: про дії користувача він
// повідомляє клієнта через функції зворотного виклику (callback, патерн
// Observer), які клієнт реєструє сам.
#pragma once
#include "common.h"

class MyTable {
public:
    // row - номер рядка таблиці (з нуля), -1 - виділення знято
    typedef void (*Callback)(int row, void* context);

    MyTable();
    ~MyTable();

    bool Open(HWND owner, HINSTANCE hInst);   // створити й показати вікно
    void Close();                             // закрити вікно
    bool IsOpen() const { return hwnd != NULL; }

    void Clear();
    void AddRow(const wchar_t* type, long x1, long y1, long x2, long y2);
    void RemoveRow(int row);

    void SetSelectHandler(Callback cb, void* context);   // користувач виділив рядок
    void SetDeleteHandler(Callback cb, void* context);   // користувач просить вилучити

    // Для циклу повідомлень: обробка клавіатури немодального діалогу
    bool PreTranslate(MSG* msg);

private:
    MyTable(const MyTable&);
    MyTable& operator=(const MyTable&);

    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
    INT_PTR HandleMessage(UINT msg, WPARAM wp, LPARAM lp);

    void OnInit();
    void OnSize();
    void OnNotify(NMHDR* hdr);
    void RequestDelete();
    void Renumber();

    HWND hwnd;        // вікно діалогу
    HWND hList;       // елемент ListView
    HWND hDelete;     // кнопка "Вилучити"
    HINSTANCE hInst;

    int lastSelected;
    bool updating;    // true - зміни програмні, повідомлення клієнту не надсилаються

    Callback selectCb;
    void* selectCtx;
    Callback deleteCb;
    void* deleteCtx;
};

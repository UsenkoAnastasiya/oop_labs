// rect_shape.h - прямокутник: ввід по двом протилежним кутам
#pragma once
#include "shape.h"

class RectShape : public virtual Shape {
public:
    void Show(HDC hdc, bool rubber) override;
    const char* Key() const override { return "RECT"; }
    const wchar_t* Title() const override { return L"Прямокутник"; }
};

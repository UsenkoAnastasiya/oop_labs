// ellipse_shape.h - еліпс: ввід від центру до кута охоплюючого прямокутника
#pragma once
#include "shape.h"

class EllipseShape : public virtual Shape {
public:
    void Show(HDC hdc, bool rubber) override;
    const char* Key() const override { return "ELLIPSE"; }
    const wchar_t* Title() const override { return L"Еліпс"; }
};

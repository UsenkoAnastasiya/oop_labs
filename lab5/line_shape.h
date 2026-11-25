// line_shape.h - лінія: від точки натискання до точки відпускання
#pragma once
#include "shape.h"

class LineShape : public virtual Shape {
public:
    void Show(HDC hdc, bool rubber) override;
    const char* Key() const override { return "LINE"; }
    const wchar_t* Title() const override { return L"Лінія"; }
};

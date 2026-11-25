// cube_shape.h - каркас куба: множинне успадкування (лінія + прямокутник)
#pragma once
#include "line_shape.h"
#include "rect_shape.h"

class CubeShape : public LineShape, public RectShape {
public:
    CubeShape();
    void Show(HDC hdc, bool rubber) override;
    const char* Key() const override { return "CUBE"; }
    const wchar_t* Title() const override { return L"Каркас куба"; }
};

// point_shape.h - крапка: малюється в точці відпускання кнопки миші
#pragma once
#include "shape.h"

class PointShape : public Shape {
public:
    void Show(HDC hdc, bool rubber) override;
    const char* Key() const override { return "POINT"; }
    const wchar_t* Title() const override { return L"Крапка"; }
};

// lineoo_shape.h - лінія з кружечками: множинне успадкування (лінія + еліпс)
#pragma once
#include "line_shape.h"
#include "ellipse_shape.h"

class LineOOShape : public LineShape, public EllipseShape {
public:
    LineOOShape();
    void Show(HDC hdc, bool rubber) override;
    const char* Key() const override { return "LINEOO"; }
    const wchar_t* Title() const override { return L"Лінія з кружечками"; }
};

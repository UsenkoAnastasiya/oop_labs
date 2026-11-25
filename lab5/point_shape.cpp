// point_shape.cpp - реалізація крапки та її реєстрація
#include "point_shape.h"
#include "pen_brush.h"
#include "shape_registry.h"

void PointShape::Show(HDC hdc, bool rubber)
{
    PenBrush tools(hdc, rubber, highlighted, true, outlineColor, outlineColor);
    Ellipse(hdc, xe - 3, ye - 3, xe + 4, ye + 4);
}

namespace {

Shape* Create() { return new PointShape; }

void DrawIcon24(HDC hdc, int x)
{
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    HGDIOBJ old = SelectObject(hdc, brush);
    Ellipse(hdc, x + 8, 8, x + 17, 17);
    SelectObject(hdc, old);
    DeleteObject(brush);
}

const ShapeRegistrar registrar({ "POINT", L"Крапка", 10, Create, DrawIcon24 });

}  // namespace

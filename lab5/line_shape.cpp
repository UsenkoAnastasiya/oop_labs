// line_shape.cpp - реалізація лінії та її реєстрація
#include "line_shape.h"
#include "pen_brush.h"
#include "shape_registry.h"

void LineShape::Show(HDC hdc, bool rubber)
{
    PenBrush tools(hdc, rubber, highlighted, false, 0, outlineColor);
    MoveToEx(hdc, xs, ys, NULL);
    LineTo(hdc, xe, ye);
}

namespace {

Shape* Create() { return new LineShape; }

void DrawIcon24(HDC hdc, int x)
{
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HGDIOBJ old = SelectObject(hdc, pen);
    MoveToEx(hdc, x + 4, 20, NULL);
    LineTo(hdc, x + 20, 4);
    SelectObject(hdc, old);
    DeleteObject(pen);
}

const ShapeRegistrar registrar({ "LINE", L"Лінія", 20, Create, DrawIcon24 });

}  // namespace

// rect_shape.cpp - реалізація прямокутника та його реєстрація
#include "rect_shape.h"
#include "pen_brush.h"
#include "shape_registry.h"

void RectShape::Show(HDC hdc, bool rubber)
{
    COLORREF fill = (fillColor == CLR_INVALID) ? Variant::RECT_FILL_COLOR
                                               : fillColor;
    PenBrush tools(hdc, rubber, highlighted, !hollow, fill, outlineColor);

    // Дві протилежні вершини; порядок координат довільний
    long left = xs < xe ? xs : xe;
    long right = xs < xe ? xe : xs;
    long top = ys < ye ? ys : ye;
    long bottom = ys < ye ? ye : ys;
    Rectangle(hdc, left, top, right, bottom);
}

namespace {

Shape* Create() { return new RectShape; }

void DrawIcon24(HDC hdc, int x)
{
    HBRUSH brush = CreateSolidBrush(Variant::RECT_FILL_COLOR);
    HGDIOBJ old = SelectObject(hdc, brush);
    Rectangle(hdc, x + 3, 6, x + 21, 18);
    SelectObject(hdc, old);
    DeleteObject(brush);
}

const ShapeRegistrar registrar({ "RECT", L"Прямокутник", 30, Create, DrawIcon24 });

}  // namespace

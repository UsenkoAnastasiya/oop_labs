// ellipse_shape.cpp - реалізація еліпса та його реєстрація
#include "ellipse_shape.h"
#include "pen_brush.h"
#include "shape_registry.h"

#include <cstdlib>

void EllipseShape::Show(HDC hdc, bool rubber)
{
    COLORREF fill = (fillColor == CLR_INVALID) ? Variant::ELLIPSE_FILL_COLOR
                                               : fillColor;
    PenBrush tools(hdc, rubber, highlighted, !hollow, fill, outlineColor);

    // (xs, ys) - центр, (xe, ye) - кут охоплюючого прямокутника
    long dx = labs(xe - xs);
    long dy = labs(ye - ys);
    Ellipse(hdc, xs - dx, ys - dy, xs + dx, ys + dy);
}

namespace {

Shape* Create() { return new EllipseShape; }

void DrawIcon24(HDC hdc, int x)
{
    HBRUSH brush = CreateSolidBrush(Variant::ELLIPSE_FILL_COLOR);
    HGDIOBJ old = SelectObject(hdc, brush);
    Ellipse(hdc, x + 2, 5, x + 22, 19);
    SelectObject(hdc, old);
    DeleteObject(brush);
}

const ShapeRegistrar registrar({ "ELLIPSE", L"Еліпс", 40, Create, DrawIcon24 });

}  // namespace

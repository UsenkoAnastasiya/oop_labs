// lineoo_shape.cpp - лінія з кружечками (множинне успадкування) та реєстрація
#include "lineoo_shape.h"
#include "shape_registry.h"
#include "variant.h"

LineOOShape::LineOOShape()
{
    outlineColor = Variant::LINEOO_LINE_COLOR;
    fillColor = Variant::LINEOO_CIRCLE_FILL;
}

// Відображення складається з викликів Show базових класів: лінію малює
// LineShape::Show, кружечки на кінцях - EllipseShape::Show. Спільні
// координати тимчасово підміняються, а потім відновлюються.
void LineOOShape::Show(HDC hdc, bool rubber)
{
    const long x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    const long r = Variant::LINEOO_RADIUS;

    LineShape::Show(hdc, rubber);

    Set(x1, y1, x1 + r, y1 + r);
    EllipseShape::Show(hdc, rubber);
    Set(x2, y2, x2 + r, y2 + r);
    EllipseShape::Show(hdc, rubber);

    Set(x1, y1, x2, y2);
}

namespace {

Shape* Create() { return new LineOOShape; }

void DrawIcon24(HDC hdc, int x)
{
    HPEN pen = CreatePen(PS_SOLID, 2, Variant::LINEOO_LINE_COLOR);
    HBRUSH brush = CreateSolidBrush(Variant::LINEOO_CIRCLE_FILL);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    MoveToEx(hdc, x + 6, 18, NULL);
    LineTo(hdc, x + 18, 6);
    Ellipse(hdc, x + 2, 14, x + 10, 22);
    Ellipse(hdc, x + 14, 2, x + 22, 10);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
}

const ShapeRegistrar registrar(
    { "LINEOO", L"Лінія з кружечками", 50, Create, DrawIcon24 });

}  // namespace

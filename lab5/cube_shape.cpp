// cube_shape.cpp - каркас куба (множинне успадкування) та його реєстрація
#include "cube_shape.h"
#include "shape_registry.h"
#include "variant.h"

CubeShape::CubeShape()
{
    outlineColor = Variant::CUBE_EDGE_COLOR;
    hollow = true;                       // каркас - без заповнення
}

// Передня грань - прямокутник за двома протилежними кутами, задня грань
// зміщена вгору й праворуч на k = половина меншої сторони. Грані малює
// RectShape::Show, чотири бокові ребра - LineShape::Show.
void CubeShape::Show(HDC hdc, bool rubber)
{
    const long x1 = xs, y1 = ys, x2 = xe, y2 = ye;

    long left = x1 < x2 ? x1 : x2;
    long right = x1 < x2 ? x2 : x1;
    long top = y1 < y2 ? y1 : y2;
    long bottom = y1 < y2 ? y2 : y1;
    long w = right - left;
    long h = bottom - top;
    long k = (w < h ? w : h) / 2;

    Set(left + k, top - k, right + k, bottom - k);   // задня грань
    RectShape::Show(hdc, rubber);
    Set(left, top, right, bottom);                   // передня грань
    RectShape::Show(hdc, rubber);

    Set(left, top, left + k, top - k);               // бокові ребра
    LineShape::Show(hdc, rubber);
    Set(right, top, right + k, top - k);
    LineShape::Show(hdc, rubber);
    Set(left, bottom, left + k, bottom - k);
    LineShape::Show(hdc, rubber);
    Set(right, bottom, right + k, bottom - k);
    LineShape::Show(hdc, rubber);

    Set(x1, y1, x2, y2);
}

namespace {

Shape* Create() { return new CubeShape; }

void DrawIcon24(HDC hdc, int x)
{
    HPEN pen = CreatePen(PS_SOLID, 1, Variant::CUBE_EDGE_COLOR);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x + 9, 3, x + 21, 15);
    Rectangle(hdc, x + 3, 9, x + 15, 21);
    MoveToEx(hdc, x + 3, 9, NULL);   LineTo(hdc, x + 9, 3);
    MoveToEx(hdc, x + 15, 9, NULL);  LineTo(hdc, x + 21, 3);
    MoveToEx(hdc, x + 3, 21, NULL);  LineTo(hdc, x + 9, 15);
    MoveToEx(hdc, x + 15, 21, NULL); LineTo(hdc, x + 21, 15);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

const ShapeRegistrar registrar({ "CUBE", L"Каркас куба", 60, Create, DrawIcon24 });

}  // namespace

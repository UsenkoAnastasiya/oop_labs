// shape.cpp - реалізація класів геометричних фігур
#include "shape.h"
#include "variant.h"

#include <cstdlib>

// ---------------------------------------------------------------- Shape ----

void Shape::Set(long x1, long y1, long x2, long y2)
{
    xs = x1;
    ys = y1;
    xe = x2;
    ye = y2;
}

bool Shape::GetFillColor(COLORREF&) const
{
    return false;
}

void Shape::Show(HDC hdc) const
{
    HPEN pen = CreatePen(PS_SOLID, 1, Variant::OUTLINE_COLOR);
    COLORREF fill;
    bool filled = GetFillColor(fill);
    HBRUSH brush = filled ? CreateSolidBrush(fill)
                          : (HBRUSH)GetStockObject(NULL_BRUSH);

    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    Draw(hdc);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    DeleteObject(pen);
    if (filled)
        DeleteObject(brush);
}

void Shape::ShowRubber(HDC hdc) const
{
    // Суцільна лінія синього кольору, без заповнення
    HPEN pen = CreatePen(PS_SOLID, 1, Variant::RUBBER_COLOR);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Draw(hdc);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

// ----------------------------------------------------------- PointShape ----

void PointShape::Draw(HDC hdc) const
{
    Ellipse(hdc, xe - 3, ye - 3, xe + 4, ye + 4);
}

bool PointShape::GetFillColor(COLORREF& color) const
{
    color = Variant::OUTLINE_COLOR;
    return true;
}

// ------------------------------------------------------------ LineShape ----

void LineShape::Draw(HDC hdc) const
{
    MoveToEx(hdc, xs, ys, NULL);
    LineTo(hdc, xe, ye);
}

// ------------------------------------------------------------ RectShape ----

void RectShape::Draw(HDC hdc) const
{
    // Дві протилежні вершини; порядок координат довільний
    long left = xs < xe ? xs : xe;
    long right = xs < xe ? xe : xs;
    long top = ys < ye ? ys : ye;
    long bottom = ys < ye ? ye : ys;
    Rectangle(hdc, left, top, right, bottom);
}

bool RectShape::GetFillColor(COLORREF& color) const
{
    color = Variant::RECT_FILL_COLOR;
    return true;
}

// --------------------------------------------------------- EllipseShape ----

void EllipseShape::Draw(HDC hdc) const
{
    // (xs, ys) - центр, (xe, ye) - кут охоплюючого прямокутника
    long dx = labs(xe - xs);
    long dy = labs(ye - ys);
    Ellipse(hdc, xs - dx, ys - dy, xs + dx, ys + dy);
}

bool EllipseShape::GetFillColor(COLORREF& color) const
{
    color = Variant::ELLIPSE_FILL_COLOR;
    return true;
}

// -------------------------------------------------------------- Фабрика ----

Shape* CreateShape(ShapeType type)
{
    switch (type) {
    case ST_POINT:   return new PointShape;
    case ST_LINE:    return new LineShape;
    case ST_RECT:    return new RectShape;
    case ST_ELLIPSE: return new EllipseShape;
    }
    return NULL;
}

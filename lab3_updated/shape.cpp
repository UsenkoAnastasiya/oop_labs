// shape.cpp - реалізація класів геометричних фігур
#include "shape.h"
#include "variant.h"

#include <cstdlib>

namespace {

// Вибирає перо та пензель у контекст пристрою, а в деструкторі повертає
// старі об'єкти й видаляє створені.
// Для "гумового" сліду перо синє, пензель порожній.
class PenBrush {
public:
    PenBrush(HDC hdc, bool rubber, bool filled, COLORREF fillColor)
        : dc(hdc), ownBrush(filled && !rubber)
    {
        pen = CreatePen(PS_SOLID, 1,
                        rubber ? Variant::RUBBER_COLOR : Variant::OUTLINE_COLOR);
        brush = ownBrush ? CreateSolidBrush(fillColor)
                         : (HBRUSH)GetStockObject(NULL_BRUSH);
        oldPen = SelectObject(dc, pen);
        oldBrush = SelectObject(dc, brush);
    }
    ~PenBrush()
    {
        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(pen);
        if (ownBrush)
            DeleteObject(brush);
    }
private:
    PenBrush(const PenBrush&);
    PenBrush& operator=(const PenBrush&);

    HDC dc;
    bool ownBrush;
    HPEN pen;
    HBRUSH brush;
    HGDIOBJ oldPen;
    HGDIOBJ oldBrush;
};

}  // namespace

// ---------------------------------------------------------------- Shape ----

void Shape::Set(long x1, long y1, long x2, long y2)
{
    xs = x1;
    ys = y1;
    xe = x2;
    ye = y2;
}

// ----------------------------------------------------------- PointShape ----

void PointShape::Show(HDC hdc, bool rubber) const
{
    PenBrush tools(hdc, rubber, true, Variant::OUTLINE_COLOR);
    Ellipse(hdc, xe - 3, ye - 3, xe + 4, ye + 4);
}

// ------------------------------------------------------------ LineShape ----

void LineShape::Show(HDC hdc, bool rubber) const
{
    PenBrush tools(hdc, rubber, false, 0);
    MoveToEx(hdc, xs, ys, NULL);
    LineTo(hdc, xe, ye);
}

// ------------------------------------------------------------ RectShape ----

void RectShape::Show(HDC hdc, bool rubber) const
{
    PenBrush tools(hdc, rubber, true, Variant::RECT_FILL_COLOR);

    // Дві протилежні вершини; порядок координат довільний
    long left = xs < xe ? xs : xe;
    long right = xs < xe ? xe : xs;
    long top = ys < ye ? ys : ye;
    long bottom = ys < ye ? ye : ys;
    Rectangle(hdc, left, top, right, bottom);
}

// --------------------------------------------------------- EllipseShape ----

void EllipseShape::Show(HDC hdc, bool rubber) const
{
    PenBrush tools(hdc, rubber, true, Variant::ELLIPSE_FILL_COLOR);

    // (xs, ys) - центр, (xe, ye) - кут охоплюючого прямокутника
    long dx = labs(xe - xs);
    long dy = labs(ye - ys);
    Ellipse(hdc, xs - dx, ys - dy, xs + dx, ys + dy);
}

// -------------------------------------------------------------- Фабрика ----

Shape* CreateShape(ShapeType type)
{
    switch (type) {
    case ST_POINT: return new PointShape;
    case ST_LINE:  return new LineShape;
    case ST_RECT:  return new RectShape;
    default:       return new EllipseShape;
    }
}

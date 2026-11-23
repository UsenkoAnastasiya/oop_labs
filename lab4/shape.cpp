// shape.cpp - реалізація класів геометричних фігур (лаб. №4)
#include "shape.h"
#include "variant.h"

#include <cstdlib>

namespace {

// Вибирає перо та пензель у контекст пристрою, а в деструкторі повертає
// старі об'єкти й видаляє створені.
// Для "гумового" сліду перо синє пунктирне, пензель порожній.
class PenBrush {
public:
    PenBrush(HDC hdc, bool rubber, bool filled, COLORREF fillColor,
             COLORREF outlineColor)
        : dc(hdc), ownBrush(filled && !rubber)
    {
        pen = rubber ? CreatePen(PS_DASH, 1, Variant::RUBBER_COLOR)
                     : CreatePen(PS_SOLID, 1, outlineColor);
        brush = ownBrush ? CreateSolidBrush(fillColor)
                         : (HBRUSH)GetStockObject(NULL_BRUSH);
        oldPen = SelectObject(dc, pen);
        oldBrush = SelectObject(dc, brush);
        // Проміжки пунктиру не повинні зафарбовуватися фоном
        oldBkMode = SetBkMode(dc, TRANSPARENT);
    }
    ~PenBrush()
    {
        SetBkMode(dc, oldBkMode);
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
    int oldBkMode;
};

}  // namespace

// ---------------------------------------------------------------- Shape ----

Shape::Shape()
    : xs(0), ys(0), xe(0), ye(0),
      outlineColor(Variant::OUTLINE_COLOR), fillColor(CLR_INVALID),
      hollow(false)
{
}

void Shape::Set(long x1, long y1, long x2, long y2)
{
    xs = x1;
    ys = y1;
    xe = x2;
    ye = y2;
}

// ----------------------------------------------------------- PointShape ----

void PointShape::Show(HDC hdc, bool rubber)
{
    PenBrush tools(hdc, rubber, true, outlineColor, outlineColor);
    Ellipse(hdc, xe - 3, ye - 3, xe + 4, ye + 4);
}

// ------------------------------------------------------------ LineShape ----

void LineShape::Show(HDC hdc, bool rubber)
{
    PenBrush tools(hdc, rubber, false, 0, outlineColor);
    MoveToEx(hdc, xs, ys, NULL);
    LineTo(hdc, xe, ye);
}

// ------------------------------------------------------------ RectShape ----

void RectShape::Show(HDC hdc, bool rubber)
{
    COLORREF fill = (fillColor == CLR_INVALID) ? Variant::RECT_FILL_COLOR
                                               : fillColor;
    PenBrush tools(hdc, rubber, !hollow, fill, outlineColor);

    // Дві протилежні вершини; порядок координат довільний
    long left = xs < xe ? xs : xe;
    long right = xs < xe ? xe : xs;
    long top = ys < ye ? ys : ye;
    long bottom = ys < ye ? ye : ys;
    Rectangle(hdc, left, top, right, bottom);
}

// --------------------------------------------------------- EllipseShape ----

void EllipseShape::Show(HDC hdc, bool rubber)
{
    COLORREF fill = (fillColor == CLR_INVALID) ? Variant::ELLIPSE_FILL_COLOR
                                               : fillColor;
    PenBrush tools(hdc, rubber, !hollow, fill, outlineColor);

    // (xs, ys) - центр, (xe, ye) - кут охоплюючого прямокутника
    long dx = labs(xe - xs);
    long dy = labs(ye - ys);
    Ellipse(hdc, xs - dx, ys - dy, xs + dx, ys + dy);
}

// ---------------------------------------------------------- LineOOShape ----

LineOOShape::LineOOShape()
{
    outlineColor = Variant::LINEOO_LINE_COLOR;
    fillColor = Variant::LINEOO_CIRCLE_FILL;
}

// Множинне успадкування: відображення складається з викликів Show
// базових класів. Спільні координати тимчасово підміняються, а потім
// відновлюються.
void LineOOShape::Show(HDC hdc, bool rubber)
{
    const long x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    const long r = Variant::LINEOO_RADIUS;

    LineShape::Show(hdc, rubber);                 // лінія (x1,y1)-(x2,y2)

    Set(x1, y1, x1 + r, y1 + r);                  // кружечок на початку
    EllipseShape::Show(hdc, rubber);
    Set(x2, y2, x2 + r, y2 + r);                  // кружечок на кінці
    EllipseShape::Show(hdc, rubber);

    Set(x1, y1, x2, y2);
}

// ------------------------------------------------------------ CubeShape ----

CubeShape::CubeShape()
{
    outlineColor = Variant::CUBE_EDGE_COLOR;
    hollow = true;                                // каркас - без заповнення
}

// Передня грань - прямокутник за двома протилежними кутами, задня грань
// зміщена вгору й праворуч на k = половина меншої сторони.
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

// -------------------------------------------------------------- Фабрика ----

Shape* CreateShape(ShapeType type)
{
    switch (type) {
    case ST_POINT:  return new PointShape;
    case ST_LINE:   return new LineShape;
    case ST_RECT:   return new RectShape;
    case ST_ELLIPSE:return new EllipseShape;
    case ST_LINEOO: return new LineOOShape;
    default:        return new CubeShape;
    }
}

// shape.cpp - реалізація базового класу Shape
#include "shape.h"
#include "variant.h"

Shape::Shape()
    : xs(0), ys(0), xe(0), ye(0),
      outlineColor(Variant::OUTLINE_COLOR), fillColor(CLR_INVALID),
      hollow(false), highlighted(false)
{
}

void Shape::Set(long x1, long y1, long x2, long y2)
{
    xs = x1;
    ys = y1;
    xe = x2;
    ye = y2;
}

void Shape::Get(long& x1, long& y1, long& x2, long& y2) const
{
    x1 = xs;
    y1 = ys;
    x2 = xe;
    y2 = ye;
}

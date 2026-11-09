// shape.h - ієрархія класів геометричних фігур
#pragma once
#include "common.h"

enum ShapeType { ST_POINT = 0, ST_LINE, ST_RECT, ST_ELLIPSE };

// Абстрактний базовий клас фігури.
// Set() запам'ятовує дві точки, введені користувачем:
//   (xs, ys) - точка натискання, (xe, ye) - точка відпускання кнопки миші.
// Кожен нащадок сам малює себе у методі Show().
class Shape {
public:
    Shape() : xs(0), ys(0), xe(0), ye(0) {}
    virtual ~Shape() {}

    void Set(long x1, long y1, long x2, long y2);

    // rubber == false - остаточне зображення фігури,
    // rubber == true  - "гумовий" слід (синя суцільна лінія, без заповнення)
    virtual void Show(HDC hdc, bool rubber) const = 0;

protected:
    long xs, ys, xe, ye;
};

// Крапка: малюється в точці відпускання кнопки миші
class PointShape : public Shape {
public:
    void Show(HDC hdc, bool rubber) const override;
};

// Лінія: від точки натискання до точки відпускання
class LineShape : public Shape {
public:
    void Show(HDC hdc, bool rubber) const override;
};

// Прямокутник: ввід по двом протилежним кутам
class RectShape : public Shape {
public:
    void Show(HDC hdc, bool rubber) const override;
};

// Еліпс: ввід від центру до одного з кутів охоплюючого прямокутника
class EllipseShape : public Shape {
public:
    void Show(HDC hdc, bool rubber) const override;
};

// Фабрика: створює в динамічній пам'яті фігуру потрібного типу
Shape* CreateShape(ShapeType type);

// shape.h - ієрархія класів геометричних фігур
#pragma once
#include "common.h"

enum ShapeType { ST_POINT = 0, ST_LINE, ST_RECT, ST_ELLIPSE };

// Абстрактний базовий клас фігури.
// Set() запам'ятовує дві точки, введені користувачем:
//   (xs, ys) - точка натискання, (xe, ye) - точка відпускання кнопки миші.
// Як саме ці точки перетворюються на фігуру, визначає Draw() у нащадках.
class Shape {
public:
    Shape() : xs(0), ys(0), xe(0), ye(0) {}
    virtual ~Shape() {}

    void Set(long x1, long y1, long x2, long y2);
    void Show(HDC hdc) const;        // остаточне зображення фігури
    void ShowRubber(HDC hdc) const;  // "гумовий" слід під час вводу

    virtual const wchar_t* Name() const = 0;

protected:
    virtual void Draw(HDC hdc) const = 0;
    virtual bool GetFillColor(COLORREF& color) const;  // false - без заповнення

    long xs, ys, xe, ye;
};

// Крапка: малюється в точці відпускання кнопки миші
class PointShape : public Shape {
public:
    const wchar_t* Name() const override { return L"Крапка"; }
protected:
    void Draw(HDC hdc) const override;
    bool GetFillColor(COLORREF& color) const override;
};

// Лінія: від точки натискання до точки відпускання
class LineShape : public Shape {
public:
    const wchar_t* Name() const override { return L"Лінія"; }
protected:
    void Draw(HDC hdc) const override;
};

// Прямокутник: ввід по двом протилежним кутам
class RectShape : public Shape {
public:
    const wchar_t* Name() const override { return L"Прямокутник"; }
protected:
    void Draw(HDC hdc) const override;
    bool GetFillColor(COLORREF& color) const override;
};

// Еліпс: ввід від центру до одного з кутів охоплюючого прямокутника
class EllipseShape : public Shape {
public:
    const wchar_t* Name() const override { return L"Еліпс"; }
protected:
    void Draw(HDC hdc) const override;
    bool GetFillColor(COLORREF& color) const override;
};

// Фабрика: створює в динамічній пам'яті фігуру потрібного типу
Shape* CreateShape(ShapeType type);

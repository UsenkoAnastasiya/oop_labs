// shape.h - ієрархія класів геометричних фігур (лаб. №4)
#pragma once
#include "common.h"

enum ShapeType {
    ST_POINT = 0, ST_LINE, ST_RECT, ST_ELLIPSE, ST_LINEOO, ST_CUBE
};

// Абстрактний базовий клас фігури.
// Set() запам'ятовує дві точки, введені користувачем:
//   (xs, ys) - точка натискання, (xe, ye) - точка відпускання кнопки миші.
// Кожен нащадок сам малює себе у методі Show().
//
// Фігури LineShape, RectShape та EllipseShape успадковують Shape ВІРТУАЛЬНО.
// Тому в об'єкті, що має кількох предків (лінія з кружечками, каркас куба),
// існує лише одна спільна підоб'єкт Shape з єдиними координатами, і вказівник
// на нащадка однозначно перетворюється на Shape*.
class Shape {
public:
    Shape();
    virtual ~Shape() {}

    void Set(long x1, long y1, long x2, long y2);

    // rubber == false - остаточне зображення фігури,
    // rubber == true  - "гумовий" слід (синя пунктирна лінія, без заповнення).
    // Метод не const: складені фігури тимчасово змінюють спільні координати
    // і викликають Show базових класів, після чого відновлюють їх.
    virtual void Show(HDC hdc, bool rubber) = 0;

protected:
    long xs, ys, xe, ye;

    // Кольори та режим заповнення. Для фігур лаб. №3 діють значення за
    // замовчуванням; нові фігури задають власні кольори в конструкторі.
    COLORREF outlineColor;     // колір контуру / лінії
    COLORREF fillColor;        // колір заповнення; CLR_INVALID - за замовчуванням
    bool hollow;               // true - заповнення не виконується (каркас)
};

// Крапка: малюється в точці відпускання кнопки миші
class PointShape : public Shape {
public:
    void Show(HDC hdc, bool rubber) override;
};

// Лінія: від точки натискання до точки відпускання
class LineShape : public virtual Shape {
public:
    void Show(HDC hdc, bool rubber) override;
};

// Прямокутник: ввід по двом протилежним кутам
class RectShape : public virtual Shape {
public:
    void Show(HDC hdc, bool rubber) override;
};

// Еліпс: ввід від центру до одного з кутів охоплюючого прямокутника
class EllipseShape : public virtual Shape {
public:
    void Show(HDC hdc, bool rubber) override;
};

// Лінія з кружечками - множинне успадкування: лінія + еліпс (кружечок).
// Лінію малює LineShape::Show, кружечки на обох кінцях - EllipseShape::Show.
class LineOOShape : public LineShape, public EllipseShape {
public:
    LineOOShape();
    void Show(HDC hdc, bool rubber) override;
};

// Каркас куба - множинне успадкування: лінія + прямокутник.
// Передня і задня грані - RectShape::Show, 4 бокових ребра - LineShape::Show.
class CubeShape : public LineShape, public RectShape {
public:
    CubeShape();
    void Show(HDC hdc, bool rubber) override;
};

// Фабрика: створює в динамічній пам'яті фігуру потрібного типу
Shape* CreateShape(ShapeType type);

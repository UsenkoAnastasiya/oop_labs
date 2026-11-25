// shape.h - абстрактний базовий клас геометричних фігур (лаб. №5)
#pragma once
#include "common.h"

// Set() запам'ятовує дві точки, введені користувачем:
//   (xs, ys) - точка натискання, (xe, ye) - точка відпускання кнопки миші.
// Кожен нащадок сам малює себе (Show), повідомляє ключ для файлу (Key) та
// назву для інтерфейсу (Title). Конкретні фігури оголошені в окремих
// модулях *_shape.h / *_shape.cpp й реєструються в ShapeRegistry самі.
class Shape {
public:
    Shape();
    virtual ~Shape() {}

    void Set(long x1, long y1, long x2, long y2);
    void Get(long& x1, long& y1, long& x2, long& y2) const;
    void SetHighlight(bool on) { highlighted = on; }

    // rubber == false - остаточне зображення, true - "гумовий" слід
    virtual void Show(HDC hdc, bool rubber) = 0;
    virtual const char* Key() const = 0;        // ASCII-ключ типу у файлі
    virtual const wchar_t* Title() const = 0;   // назва типу українською

protected:
    long xs, ys, xe, ye;
    COLORREF outlineColor;   // колір контуру / лінії
    COLORREF fillColor;      // колір заповнення; CLR_INVALID - за замовчуванням
    bool hollow;             // true - без заповнення (каркас)
    bool highlighted;        // фігуру вибрано в таблиці
};

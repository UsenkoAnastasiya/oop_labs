// variant.h - параметри варіанта завдання
// Лаб. №3: Ж = Жлаб2 + 1 = 25 + 1 = 26; кольори та стилі збережено в лаб. №4
// Лаб. №4: номер у списку = 25 (непарний) -> глобальний статичний об'єкт MyEditor
#pragma once
#include "common.h"

namespace Variant {

const int ZH = 26;          // Ж
const int N  = ZH + 100;    // кількість елементів масиву вказівників = 126

// "Гумовий" слід (лаб. №4): пунктирна лінія; колір, як у лаб. №3, - синій
const COLORREF RUBBER_COLOR = RGB(0, 0, 255);

// Контур усіх фігур лаб. №3 - чорний
const COLORREF OUTLINE_COLOR = RGB(0, 0, 0);

// Прямокутник: чорний контур, блакитне заповнення
const COLORREF RECT_FILL_COLOR = RGB(135, 206, 250);

// Еліпс: чорний контур, біле заповнення
const COLORREF ELLIPSE_FILL_COLOR = RGB(255, 255, 255);

// Нові фігури лаб. №4 (кольори обрано довільно)
// Лінія з кружечками: темно-зелена лінія та контур, жовті кружечки
const COLORREF LINEOO_LINE_COLOR   = RGB(0, 128, 0);
const COLORREF LINEOO_CIRCLE_FILL  = RGB(255, 215, 0);
// Каркас куба: темно-червоні ребра
const COLORREF CUBE_EDGE_COLOR     = RGB(160, 0, 0);

const int LINEOO_RADIUS = 7;   // радіус кружечків на кінцях лінії, пікселів

}  // namespace Variant

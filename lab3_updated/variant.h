// variant.h - параметри варіанта завдання
// Ж = Жлаб2 + 1 = 25 + 1 = 26
#pragma once
#include "common.h"

namespace Variant {

const int ZH = 26;          // Ж
const int N  = ZH + 100;    // кількість елементів масиву вказівників = 126

// Ж mod 4 = 2 -> "гумовий" слід: суцільна лінія синього кольору
const COLORREF RUBBER_COLOR = RGB(0, 0, 255);

// Контур усіх фігур - чорний
const COLORREF OUTLINE_COLOR = RGB(0, 0, 0);

// Ж mod 5 = 1, Ж mod 6 = 2 -> прямокутник: чорний контур, блакитне заповнення
const COLORREF RECT_FILL_COLOR = RGB(135, 206, 250);

// Ж mod 5 = 1 -> еліпс: чорний контур, біле заповнення
const COLORREF ELLIPSE_FILL_COLOR = RGB(255, 255, 255);

}  // namespace Variant

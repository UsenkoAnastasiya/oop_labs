// variant.h - параметри варіанта завдання
// Лаб. №3: Ж = Жлаб2 + 1 = 25 + 1 = 26; кольори та стилі збережено в лаб. №4, 5
// Лаб. №5: номер у списку = 25 (непарний) -> Singleton Меєрса для MyEditor
#pragma once
#include "common.h"

namespace Variant {

const int ZH = 26;          // Ж
const int N  = ZH + 100;    // кількість елементів масиву вказівників = 126

// "Гумовий" слід: синя пунктирна лінія
const COLORREF RUBBER_COLOR = RGB(0, 0, 255);

// Контур усіх фігур лаб. №3 - чорний
const COLORREF OUTLINE_COLOR = RGB(0, 0, 0);
const COLORREF RECT_FILL_COLOR = RGB(135, 206, 250);     // блакитний
const COLORREF ELLIPSE_FILL_COLOR = RGB(255, 255, 255);  // білий

// Лінія з кружечками та каркас куба (лаб. №4)
const COLORREF LINEOO_LINE_COLOR   = RGB(0, 128, 0);
const COLORREF LINEOO_CIRCLE_FILL  = RGB(255, 215, 0);
const COLORREF CUBE_EDGE_COLOR     = RGB(160, 0, 0);
const int LINEOO_RADIUS = 7;

// Виділення об'єкта, вибраного в таблиці (бонус 1): товсте помаранчеве перо
const COLORREF HIGHLIGHT_COLOR = RGB(255, 128, 0);
const int HIGHLIGHT_WIDTH = 3;

}  // namespace Variant

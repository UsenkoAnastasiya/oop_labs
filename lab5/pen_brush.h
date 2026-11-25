// pen_brush.h - допоміжний клас: вибирає перо та пензель у контекст пристрою,
// а в деструкторі повертає старі об'єкти й видаляє створені (RAII).
#pragma once
#include "common.h"
#include "variant.h"

class PenBrush {
public:
    // rubber    - "гумовий" слід: синє пунктирне перо, без заповнення
    // highlight - виділений об'єкт: товсте помаранчеве перо
    PenBrush(HDC hdc, bool rubber, bool highlight, bool filled,
             COLORREF fillColor, COLORREF outlineColor)
        : dc(hdc), ownBrush(filled && !rubber)
    {
        if (rubber)
            pen = CreatePen(PS_DASH, 1, Variant::RUBBER_COLOR);
        else if (highlight)
            pen = CreatePen(PS_SOLID, Variant::HIGHLIGHT_WIDTH,
                            Variant::HIGHLIGHT_COLOR);
        else
            pen = CreatePen(PS_SOLID, 1, outlineColor);
        brush = ownBrush ? CreateSolidBrush(fillColor)
                         : (HBRUSH)GetStockObject(NULL_BRUSH);
        oldPen = SelectObject(dc, pen);
        oldBrush = SelectObject(dc, brush);
        oldBkMode = SetBkMode(dc, TRANSPARENT);   // проміжки пунктиру прозорі
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

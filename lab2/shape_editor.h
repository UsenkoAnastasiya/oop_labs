#pragma once
#include "shape.h"

const int N_CAPACITY = 125;

class ShapeObjectsEditor
{
private:
    Shape *pcshape[N_CAPACITY];
    int count;

    long x1, y1, x2, y2;
    bool isDrawing;
    int currentType; // 1-Крапка, 2-Лінія, 3-Прямокутник, 4-Еліпс

public:
    ShapeObjectsEditor();
    ~ShapeObjectsEditor();

    void StartPointEditor(HWND hWnd);
    void StartLineEditor(HWND hWnd);
    void StartRectEditor(HWND hWnd);
    void StartEllipseEditor(HWND hWnd);

    void OnLBdown(HWND hWnd);
    void OnLBup(HWND hWnd);
    void OnMouseMove(HWND hWnd);
    void OnPaint(HWND hWnd);
};
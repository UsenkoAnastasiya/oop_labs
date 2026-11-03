#include "framework.h"
#include "shape_editor.h"

ShapeObjectsEditor::ShapeObjectsEditor()
{
    count = 0;
    isDrawing = false;
    currentType = 1;
    for (int i = 0; i < N_CAPACITY; ++i)
    {
        pcshape[i] = nullptr;
    }
}

ShapeObjectsEditor::~ShapeObjectsEditor()
{
    for (int i = 0; i < count; ++i)
    {
        delete pcshape[i];
    }
}

void ShapeObjectsEditor::StartPointEditor(HWND hWnd)
{
    currentType = 1;
    SetWindowTextW(hWnd, L"Графічний редактор - [Крапка]");
}

void ShapeObjectsEditor::StartLineEditor(HWND hWnd)
{
    currentType = 2;
    SetWindowTextW(hWnd, L"Графічний редактор - [Лінія]");
}

void ShapeObjectsEditor::StartRectEditor(HWND hWnd)
{
    currentType = 3;
    SetWindowTextW(hWnd, L"Графічний редактор - [Прямокутник]");
}

void ShapeObjectsEditor::StartEllipseEditor(HWND hWnd)
{
    currentType = 4;
    SetWindowTextW(hWnd, L"Графічний редактор - [Еліпс]");
}

void ShapeObjectsEditor::OnLBdown(HWND hWnd)
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    x1 = x2 = pt.x;
    y1 = y2 = pt.y;
    isDrawing = true;
}

void ShapeObjectsEditor::OnLBup(HWND hWnd)
{
    if (!isDrawing)
        return;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    x2 = pt.x;
    y2 = pt.y;
    isDrawing = false;

    if (count < N_CAPACITY)
    {
        Shape *pShape = nullptr;
        switch (currentType)
        {
        case 1:
            pShape = new PointShape();
            break;
        case 2:
            pShape = new LineShape();
            break;
        case 3:
            pShape = new RectShape();
            break;
        case 4:
            pShape = new EllipseShape();
            break;
        }

        if (pShape)
        {
            pShape->SetCoord(x1, y1, x2, y2);
            pcshape[count++] = pShape;
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}

void ShapeObjectsEditor::OnMouseMove(HWND hWnd)
{
    if (!isDrawing)
        return;

    HDC hdc = GetDC(hWnd);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // Червоний слід
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    SetROP2(hdc, R2_NOTXORPEN);

    if (currentType == 2)
    {
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
    }
    else if (currentType == 3 || currentType == 4)
    {
        Rectangle(hdc, x1, y1, x2, y2);
    }

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    x2 = pt.x;
    y2 = pt.y;

    if (currentType == 2)
    {
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
    }
    else if (currentType == 3 || currentType == 4)
    {
        Rectangle(hdc, x1, y1, x2, y2);
    }

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void ShapeObjectsEditor::OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    for (int i = 0; i < count; ++i)
    {
        if (pcshape[i])
        {
            pcshape[i]->Draw(hdc);
        }
    }

    EndPaint(hWnd, &ps);
}
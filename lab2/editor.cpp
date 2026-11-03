#include "framework.h"
#include "editor.h"

#include <algorithm>

Editor::~Editor()
{
}

ShapeEditor::ShapeEditor(Shape **shapeArray, int *count)
    : shapes(shapeArray),
      shapeCount(count),
      drawing(false),
      previewVisible(false),
      xStart(0),
      yStart(0),
      xEnd(0),
      yEnd(0)
{
}

ShapeEditor::~ShapeEditor()
{
}

void ShapeEditor::AddShape(Shape *shape)
{
    if (shape == nullptr)
        return;

    if (*shapeCount < SHAPE_ARRAY_SIZE)
    {
        shapes[*shapeCount] = shape;
        ++(*shapeCount);
    }
    else
    {
        delete shape;
    }
}

void ShapeEditor::TogglePreview(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);

    int oldDrawingMode = SetROP2(hdc, R2_NOTXORPEN);

    HPEN previewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, previewPen));

    DrawPreview(hdc);

    SelectObject(hdc, oldPen);
    DeleteObject(previewPen);

    SetROP2(hdc, oldDrawingMode);
    ReleaseDC(hWnd, hdc);
}

void ShapeEditor::OnLBdown(HWND hWnd)
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hWnd, &point);

    xStart = point.x;
    yStart = point.y;
    xEnd = point.x;
    yEnd = point.y;

    drawing = true;
    previewVisible = false;

    SetCapture(hWnd);
}

void ShapeEditor::OnLBup(HWND hWnd)
{
    if (!drawing)
        return;

    if (previewVisible)
    {
        TogglePreview(hWnd);
        previewVisible = false;
    }

    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hWnd, &point);

    xEnd = point.x;
    yEnd = point.y;

    AddShape(CreateShape());

    drawing = false;
    ReleaseCapture();

    InvalidateRect(hWnd, nullptr, TRUE);
}

void ShapeEditor::OnMouseMove(HWND hWnd)
{
    if (!drawing)
        return;

    if (previewVisible)
        TogglePreview(hWnd);

    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hWnd, &point);

    xEnd = point.x;
    yEnd = point.y;

    TogglePreview(hWnd);
    previewVisible = true;
}

void ShapeEditor::OnPaint(HWND hWnd)
{
    PAINTSTRUCT paintStruct;
    HDC hdc = BeginPaint(hWnd, &paintStruct);

    for (int i = 0; i < *shapeCount; ++i)
    {
        if (shapes[i] != nullptr)
            shapes[i]->Show(hdc);
    }

    EndPaint(hWnd, &paintStruct);
}

// PointEditor

PointEditor::PointEditor(Shape **shapeArray, int *count)
    : ShapeEditor(shapeArray, count)
{
}

void PointEditor::DrawPreview(HDC hdc)
{
    UNREFERENCED_PARAMETER(hdc);
}

Shape *PointEditor::CreateShape()
{
    PointShape *point = new PointShape;
    point->Set(xStart, yStart, xStart, yStart);
    return point;
}

void PointEditor::OnLBdown(HWND hWnd)
{
    POINT cursor;
    GetCursorPos(&cursor);
    ScreenToClient(hWnd, &cursor);

    xStart = cursor.x;
    yStart = cursor.y;
    xEnd = cursor.x;
    yEnd = cursor.y;

    AddShape(CreateShape());
    InvalidateRect(hWnd, nullptr, TRUE);
}

// LineEditor

LineEditor::LineEditor(Shape **shapeArray, int *count)
    : ShapeEditor(shapeArray, count)
{
}

void LineEditor::DrawPreview(HDC hdc)
{
    MoveToEx(hdc, xStart, yStart, nullptr);
    LineTo(hdc, xEnd, yEnd);
}

Shape *LineEditor::CreateShape()
{
    LineShape *line = new LineShape;
    line->Set(xStart, yStart, xEnd, yEnd);
    return line;
}

// RectEditor

RectEditor::RectEditor(Shape **shapeArray, int *count)
    : ShapeEditor(shapeArray, count)
{
}

void RectEditor::DrawPreview(HDC hdc)
{
    long oppositeX = 2 * xStart - xEnd;
    long oppositeY = 2 * yStart - yEnd;

    long left = std::min(oppositeX, xEnd);
    long right = std::max(oppositeX, xEnd);
    long top = std::min(oppositeY, yEnd);
    long bottom = std::max(oppositeY, yEnd);

    HBRUSH oldBrush = static_cast<HBRUSH>(
        SelectObject(hdc, GetStockObject(NULL_BRUSH)));

    Rectangle(hdc, left, top, right, bottom);

    SelectObject(hdc, oldBrush);
}

Shape *RectEditor::CreateShape()
{
    long oppositeX = 2 * xStart - xEnd;
    long oppositeY = 2 * yStart - yEnd;

    long left = std::min(oppositeX, xEnd);
    long right = std::max(oppositeX, xEnd);
    long top = std::min(oppositeY, yEnd);
    long bottom = std::max(oppositeY, yEnd);

    RectShape *rectangle = new RectShape;
    rectangle->Set(left, top, right, bottom);

    return rectangle;
}

// EllipseEditor

EllipseEditor::EllipseEditor(Shape **shapeArray, int *count)
    : ShapeEditor(shapeArray, count)
{
}

void EllipseEditor::DrawPreview(HDC hdc)
{
    long left = std::min(xStart, xEnd);
    long right = std::max(xStart, xEnd);
    long top = std::min(yStart, yEnd);
    long bottom = std::max(yStart, yEnd);

    HBRUSH oldBrush = static_cast<HBRUSH>(
        SelectObject(hdc, GetStockObject(NULL_BRUSH)));

    Ellipse(hdc, left, top, right, bottom);

    SelectObject(hdc, oldBrush);
}

Shape *EllipseEditor::CreateShape()
{
    long left = std::min(xStart, xEnd);
    long right = std::max(xStart, xEnd);
    long top = std::min(yStart, yEnd);
    long bottom = std::max(yStart, yEnd);

    EllipseShape *ellipse = new EllipseShape;
    ellipse->Set(left, top, right, bottom);

    return ellipse;
}
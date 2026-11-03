#pragma once
#include "framework.h"
#include "shape.h"

// Максимальний розмір масиву об'єктів під варіант Ж = 25 (N = 25 + 100 = 125)
const int SHAPE_ARRAY_SIZE = 125;

class Editor
{
public:
    virtual ~Editor();
    virtual void OnLBdown(HWND hWnd) = 0;
    virtual void OnLBup(HWND hWnd) = 0;
    virtual void OnMouseMove(HWND hWnd) = 0;
    virtual void OnPaint(HWND hWnd) = 0;
};

class ShapeEditor : public Editor
{
protected:
    Shape **shapes;
    int *shapeCount;
    bool drawing;
    bool previewVisible;
    long xStart, yStart, xEnd, yEnd;

    virtual void DrawPreview(HDC hdc) = 0;
    virtual Shape *CreateShape() = 0;
    void TogglePreview(HWND hWnd);
    void AddShape(Shape *shape);

public:
    ShapeEditor(Shape **shapeArray, int *count);
    virtual ~ShapeEditor();

    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
    void OnPaint(HWND hWnd) override;
};

class PointEditor : public ShapeEditor
{
protected:
    void DrawPreview(HDC hdc) override;
    Shape *CreateShape() override;

public:
    PointEditor(Shape **shapeArray, int *count);
    void OnLBdown(HWND hWnd) override;
};

class LineEditor : public ShapeEditor
{
protected:
    void DrawPreview(HDC hdc) override;
    Shape *CreateShape() override;

public:
    LineEditor(Shape **shapeArray, int *count);
};

class RectEditor : public ShapeEditor
{
protected:
    void DrawPreview(HDC hdc) override;
    Shape *CreateShape() override;

public:
    RectEditor(Shape **shapeArray, int *count);
};

class EllipseEditor : public ShapeEditor
{
protected:
    void DrawPreview(HDC hdc) override;
    Shape *CreateShape() override;

public:
    EllipseEditor(Shape **shapeArray, int *count);
};
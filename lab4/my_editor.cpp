// my_editor.cpp - реалізація класу MyEditor (лаб. №4)
#include "my_editor.h"

MyEditor::MyEditor()
    : count(0), current(ST_POINT), drawing(false)
{
    for (int i = 0; i < N; i++)
        pcshape[i] = NULL;
    start.x = start.y = 0;
    cursor.x = cursor.y = 0;
}

MyEditor::~MyEditor()
{
    Clear();
}

bool MyEditor::OnLBdown(int x, int y)
{
    if (count >= N)
        return false;
    drawing = true;
    start.x = cursor.x = x;
    start.y = cursor.y = y;
    return true;
}

void MyEditor::OnMouseMove(int x, int y)
{
    if (drawing) {
        cursor.x = x;
        cursor.y = y;
    }
}

void MyEditor::OnLBup(int x, int y)
{
    if (!drawing)
        return;
    drawing = false;

    Shape* shape = CreateShape(current);
    shape->Set(start.x, start.y, x, y);
    pcshape[count++] = shape;
}

void MyEditor::Cancel()
{
    drawing = false;
}

void MyEditor::OnPaint(HDC hdc)
{
    for (int i = 0; i < count; i++)
        pcshape[i]->Show(hdc, false);

    if (drawing) {
        // Тимчасова фігура для "гумового" сліду
        Shape* rubber = CreateShape(current);
        rubber->Set(start.x, start.y, cursor.x, cursor.y);
        rubber->Show(hdc, true);
        delete rubber;
    }
}

void MyEditor::Clear()
{
    for (int i = 0; i < count; i++) {
        delete pcshape[i];
        pcshape[i] = NULL;
    }
    count = 0;
    drawing = false;
}

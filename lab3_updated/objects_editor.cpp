// objects_editor.cpp - реалізація редактора об'єктів
#include "objects_editor.h"

ObjectsEditor::ObjectsEditor()
    : count(0), current(ST_POINT), drawing(false)
{
    for (int i = 0; i < N; i++)
        pcshape[i] = NULL;
    start.x = start.y = 0;
    cursor.x = cursor.y = 0;
}

ObjectsEditor::~ObjectsEditor()
{
    Clear();
}

bool ObjectsEditor::OnLBdown(int x, int y)
{
    if (count >= N)
        return false;
    drawing = true;
    start.x = cursor.x = x;
    start.y = cursor.y = y;
    return true;
}

void ObjectsEditor::OnMouseMove(int x, int y)
{
    if (drawing) {
        cursor.x = x;
        cursor.y = y;
    }
}

void ObjectsEditor::OnLBup(int x, int y)
{
    if (!drawing)
        return;
    drawing = false;

    Shape* shape = CreateShape(current);
    shape->Set(start.x, start.y, x, y);
    pcshape[count++] = shape;
}

void ObjectsEditor::Cancel()
{
    drawing = false;
}

void ObjectsEditor::OnPaint(HDC hdc) const
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

void ObjectsEditor::Clear()
{
    for (int i = 0; i < count; i++) {
        delete pcshape[i];
        pcshape[i] = NULL;
    }
    count = 0;
    drawing = false;
}

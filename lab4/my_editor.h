// my_editor.h - клас MyEditor: зберігає фігури та керує вводом (лаб. №4)
#pragma once
#include "common.h"
#include "shape.h"
#include "variant.h"

class MyEditor {
public:
    static const int N = Variant::N;   // N = Ж + 100 = 126

    MyEditor();
    ~MyEditor();

    void SetType(ShapeType type) { current = type; }
    ShapeType GetType() const { return current; }
    bool IsDrawing() const { return drawing; }

    bool OnLBdown(int x, int y);   // false - масив заповнено
    void OnMouseMove(int x, int y);
    void OnLBup(int x, int y);
    void Cancel();                 // перервати ввід (втрата захоплення миші)

    void OnPaint(HDC hdc);
    void Clear();

private:
    MyEditor(const MyEditor&);             // копіювання заборонено
    MyEditor& operator=(const MyEditor&);

    Shape* pcshape[N];   // статичний масив вказівників на динамічні об'єкти
    int count;
    ShapeType current;

    bool drawing;
    POINT start;
    POINT cursor;
};

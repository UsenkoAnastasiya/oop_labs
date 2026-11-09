// objects_editor.h - редактор об'єктів: зберігає фігури та керує вводом
#pragma once
#include "common.h"
#include "shape.h"
#include "variant.h"

class ObjectsEditor {
public:
    static const int N = Variant::N;   // N = Ж + 100 = 126

    ObjectsEditor();
    ~ObjectsEditor();

    void SetType(ShapeType type) { current = type; }
    ShapeType GetType() const { return current; }
    bool IsDrawing() const { return drawing; }

    bool OnLBdown(int x, int y);   // false - масив заповнено
    void OnMouseMove(int x, int y);
    void OnLBup(int x, int y);
    void Cancel();                 // перервати ввід (втрата захоплення миші)

    void OnPaint(HDC hdc) const;
    void Clear();

private:
    ObjectsEditor(const ObjectsEditor&);             // копіювання заборонено
    ObjectsEditor& operator=(const ObjectsEditor&);

    Shape* pcshape[N];   // статичний масив вказівників на динамічні об'єкти
    int count;
    ShapeType current;

    bool drawing;
    POINT start;
    POINT cursor;
};

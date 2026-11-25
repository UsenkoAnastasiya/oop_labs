// my_editor.h - клас MyEditor: зберігає фігури, керує вводом, пише/читає файл.
// Номер у списку = 25 (непарний): MyEditor реалізовано як Singleton Меєрса -
// єдиний екземпляр створюється при першому виклику Instance() як статична
// локальна змінна, копіювання заборонено.
#pragma once
#include "common.h"
#include "shape.h"
#include "variant.h"

class MyEditor {
public:
    static const int N = Variant::N;   // N = Ж + 100 = 126

    static MyEditor& Instance();       // Singleton Меєрса

    void SetType(int typeIndex) { current = typeIndex; }   // індекс у ShapeRegistry
    int GetType() const { return current; }
    bool IsDrawing() const { return drawing; }

    bool OnLBdown(int x, int y);   // false - масив заповнено
    void OnMouseMove(int x, int y);
    bool OnLBup(int x, int y);     // true - створено новий об'єкт
    void Cancel();                 // перервати ввід (втрата захоплення миші)

    void OnPaint(HDC hdc);
    void Clear();

    int Count() const { return count; }
    const Shape* At(int index) const;
    bool Remove(int index);                 // вилучити об'єкт (бонус 2)
    void SetSelected(int index);            // виділений об'єкт (бонус 1), -1 - немає
    int GetSelected() const { return selected; }

    bool SaveToFile(const wchar_t* path) const;
    int LoadFromFile(const wchar_t* path);  // кількість об'єктів, -1 - помилка

private:
    MyEditor();
    ~MyEditor();
    MyEditor(const MyEditor&);             // копіювання заборонено
    MyEditor& operator=(const MyEditor&);

    Shape* pcshape[N];   // статичний масив вказівників на динамічні об'єкти
    int count;
    int current;
    int selected;

    bool drawing;
    Shape* rubber;       // фігура, що вводиться; створюється один раз
    POINT start;
};

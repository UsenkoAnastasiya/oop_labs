// my_editor.cpp - реалізація класу MyEditor
#include "my_editor.h"
#include "shape_registry.h"

#include <cstdio>
#include <cstring>
#include <vector>

MyEditor& MyEditor::Instance()
{
    static MyEditor instance;   // Singleton Меєрса
    return instance;
}

MyEditor::MyEditor()
    : count(0), current(0), selected(-1), drawing(false), rubber(NULL)
{
    for (int i = 0; i < N; i++)
        pcshape[i] = NULL;
    start.x = start.y = 0;
}

MyEditor::~MyEditor()
{
    Clear();
}

bool MyEditor::OnLBdown(int x, int y)
{
    if (count >= N)
        return false;
    Shape* shape = ShapeRegistry::Instance().Create(current);
    if (shape == NULL)
        return false;
    rubber = shape;
    drawing = true;
    start.x = x;
    start.y = y;
    rubber->Set(x, y, x, y);
    return true;
}

void MyEditor::OnMouseMove(int x, int y)
{
    if (drawing)
        rubber->Set(start.x, start.y, x, y);
}

bool MyEditor::OnLBup(int x, int y)
{
    if (!drawing)
        return false;
    rubber->Set(start.x, start.y, x, y);
    pcshape[count++] = rubber;     // та сама фігура стає остаточною
    rubber = NULL;
    drawing = false;
    return true;
}

void MyEditor::Cancel()
{
    delete rubber;
    rubber = NULL;
    drawing = false;
}

void MyEditor::OnPaint(HDC hdc)
{
    for (int i = 0; i < count; i++) {
        pcshape[i]->SetHighlight(i == selected);
        pcshape[i]->Show(hdc, false);
    }
    if (drawing)
        rubber->Show(hdc, true);
}

void MyEditor::Clear()
{
    Cancel();
    for (int i = 0; i < count; i++) {
        delete pcshape[i];
        pcshape[i] = NULL;
    }
    count = 0;
    selected = -1;
}

const Shape* MyEditor::At(int index) const
{
    if (index < 0 || index >= count)
        return NULL;
    return pcshape[index];
}

bool MyEditor::Remove(int index)
{
    if (index < 0 || index >= count)
        return false;
    delete pcshape[index];
    for (int i = index; i < count - 1; i++)
        pcshape[i] = pcshape[i + 1];
    pcshape[--count] = NULL;
    selected = -1;
    return true;
}

void MyEditor::SetSelected(int index)
{
    selected = (index >= 0 && index < count) ? index : -1;
}

// Формат файлу (текстовий):
//   LAB5-SHAPES 1        - заголовок і версія
//   <кількість об'єктів>
//   <КЛЮЧ> x1 y1 x2 y2   - по одному рядку на об'єкт
bool MyEditor::SaveToFile(const wchar_t* path) const
{
    FILE* f = _wfopen(path, L"w");
    if (f == NULL)
        return false;
    fprintf(f, "LAB5-SHAPES 1\n%d\n", count);
    for (int i = 0; i < count; i++) {
        long x1, y1, x2, y2;
        pcshape[i]->Get(x1, y1, x2, y2);
        fprintf(f, "%s %ld %ld %ld %ld\n", pcshape[i]->Key(), x1, y1, x2, y2);
    }
    bool ok = (ferror(f) == 0);
    fclose(f);
    return ok;
}

int MyEditor::LoadFromFile(const wchar_t* path)
{
    FILE* f = _wfopen(path, L"r");
    if (f == NULL)
        return -1;

    char line[256];
    int declared = 0;
    if (fgets(line, sizeof(line), f) == NULL ||
        strncmp(line, "LAB5-SHAPES", 11) != 0 ||
        fgets(line, sizeof(line), f) == NULL ||
        sscanf(line, "%d", &declared) != 1 || declared < 0) {
        fclose(f);
        return -1;
    }

    // Спочатку читаємо у тимчасовий список: за помилки редактор не змінюється
    const ShapeRegistry& registry = ShapeRegistry::Instance();
    std::vector<Shape*> loaded;
    while ((int)loaded.size() < declared && (int)loaded.size() < N &&
           fgets(line, sizeof(line), f) != NULL) {
        char key[32];
        long x1, y1, x2, y2;
        if (sscanf(line, "%31s %ld %ld %ld %ld", key, &x1, &y1, &x2, &y2) != 5)
            continue;
        int type = registry.IndexOfKey(key);
        if (type < 0)
            continue;                       // невідомий тип пропускається
        Shape* shape = registry.Create(type);
        shape->Set(x1, y1, x2, y2);
        loaded.push_back(shape);
    }
    fclose(f);

    Clear();
    for (size_t i = 0; i < loaded.size(); i++)
        pcshape[count++] = loaded[i];
    return count;
}

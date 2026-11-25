// shape_registry.h - реєстр типів фігур (Singleton Меєрса + фабрика).
// Кожен модуль фігури реєструє себе статичним об'єктом ShapeRegistrar,
// тому додавання нового типу не вимагає змін в уже наявних модулях:
// меню, панель інструментів, файл і таблиця будуються за реєстром.
#pragma once
#include "common.h"
#include "shape.h"
#include <vector>

const int SHAPE_ICON_SIZE = 24;

typedef Shape* (*ShapeCreator)();
typedef void (*ShapeIconDrawer)(HDC hdc, int x);   // піктограма 24x24 зі зсувом x

struct ShapeInfo {
    const char* key;          // ключ у файлі, наприклад "RECT"
    const wchar_t* title;     // назва для меню, підказки та таблиці
    int order;                // порядок кнопок і пунктів меню
    ShapeCreator create;      // створює фігуру в динамічній пам'яті
    ShapeIconDrawer drawIcon; // малює піктограму кнопки
};

class ShapeRegistry {
public:
    static ShapeRegistry& Instance();     // Singleton Меєрса

    void Register(const ShapeInfo& info);
    int Count() const { return (int)items.size(); }
    const ShapeInfo& At(int index) const { return items[index]; }
    int IndexOfKey(const char* key) const;   // -1, якщо ключа немає
    Shape* Create(int index) const;

private:
    ShapeRegistry() {}
    ShapeRegistry(const ShapeRegistry&);
    ShapeRegistry& operator=(const ShapeRegistry&);

    std::vector<ShapeInfo> items;
};

// Реєстрація типу фігури під час ініціалізації статичних об'єктів
struct ShapeRegistrar {
    explicit ShapeRegistrar(const ShapeInfo& info)
    {
        ShapeRegistry::Instance().Register(info);
    }
};

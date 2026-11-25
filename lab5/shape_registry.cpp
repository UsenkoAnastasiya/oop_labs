// shape_registry.cpp - реалізація реєстру типів фігур
#include "shape_registry.h"
#include <cstring>

ShapeRegistry& ShapeRegistry::Instance()
{
    static ShapeRegistry instance;   // створюється при першому зверненні
    return instance;
}

// Порядок ініціалізації статичних об'єктів різних .cpp не визначений,
// тому елементи вставляються в масив впорядковано за полем order.
void ShapeRegistry::Register(const ShapeInfo& info)
{
    size_t pos = 0;
    while (pos < items.size() && items[pos].order <= info.order)
        pos++;
    items.insert(items.begin() + pos, info);
}

int ShapeRegistry::IndexOfKey(const char* key) const
{
    for (int i = 0; i < Count(); i++) {
        if (strcmp(items[i].key, key) == 0)
            return i;
    }
    return -1;
}

Shape* ShapeRegistry::Create(int index) const
{
    if (index < 0 || index >= Count())
        return NULL;
    return items[index].create();
}

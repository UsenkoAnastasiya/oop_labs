# Lab4 - графічний редактор з множинним успадкуванням (Win32 API, C++)

Номер у списку = 25 (непарний) -> **глобальний статичний** об'єкт класу `MyEditor`.
Кольори та стилі як у Lab3 (Ж = 26, N = 126), "гумовий" слід - пунктирна лінія.
Шість типів фігур: крапка, лінія, прямокутник, еліпс, **лінія з кружечками**
(`LineOOShape : LineShape, EllipseShape`) та **каркас куба**
(`CubeShape : LineShape, RectShape`).

## Збірка
- MinGW-w64: `build.bat` (або `build.bat debug`) -> `build\Lab4.exe`
- Visual Studio: `build_msvc.bat`

## Структура
- `src/shape.*` - ієрархія фігур, віртуальне успадкування `Shape`
- `src/my_editor.*` - клас `MyEditor`: масив вказівників `Shape *pcshape[N]`, ввід
- `src/app_toolbar.*` - Toolbar із 6 кнопками й підказками
- `src/main_window.*` - головне вікно; тут визначено `static MyEditor editor;`
- `src/variant.h`, `ids.h`, `common.h`, `main.cpp`

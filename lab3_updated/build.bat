@echo off
rem Збірка Lab3 компілятором MinGW-w64 (g++). Режим налагодження: build.bat debug
setlocal
cd /d "%~dp0"
if not exist build mkdir build

set FLAGS=-std=c++17 -Wall -Wextra -municode -mwindows -static -static-libgcc -static-libstdc++ -finput-charset=UTF-8 -fexec-charset=UTF-8
if /i "%1"=="debug" (set FLAGS=%FLAGS% -g -O0) else (set FLAGS=%FLAGS% -O2)

windres app.rc -O coff -o build\app.res.o
if errorlevel 1 exit /b 1

g++ %FLAGS% src\main.cpp src\main_window.cpp src\app_toolbar.cpp src\objects_editor.cpp src\shape.cpp build\app.res.o -o build\Lab3.exe -lcomctl32 -lgdi32 -luser32
if errorlevel 1 exit /b 1

echo Build OK: build\Lab3.exe

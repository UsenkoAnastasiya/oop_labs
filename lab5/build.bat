@echo off
rem Збірка Lab5 компілятором MinGW-w64 (g++). Режим налагодження: build.bat debug
rem Усі модулі з папки src\ збираються автоматично: щоб додати новий тип
rem фігури, достатньо покласти до src\ його *.h та *.cpp.
setlocal
cd /d "%~dp0"
if not exist build mkdir build

set FLAGS=-std=c++17 -Wall -Wextra -municode -mwindows -static -static-libgcc -static-libstdc++ -finput-charset=UTF-8 -fexec-charset=UTF-8
if /i "%1"=="debug" (set FLAGS=%FLAGS% -g -O0) else (set FLAGS=%FLAGS% -O2)

set SRCS=
for %%f in (src\*.cpp) do call set SRCS=%%SRCS%% %%f

windres app.rc -O coff -o build\app.res.o
if errorlevel 1 exit /b 1

g++ %FLAGS% %SRCS% build\app.res.o -o build\Lab5.exe -lcomctl32 -lcomdlg32 -lgdi32 -luser32
if errorlevel 1 exit /b 1

echo Build OK: build\Lab5.exe

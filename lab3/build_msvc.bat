@echo off
rem Збірка Lab3 компілятором Microsoft Visual C++ (cl.exe), якщо встановлено Visual Studio
setlocal
cd /d "%~dp0"
if not exist build mkdir build

where cl >nul 2>nul
if errorlevel 1 (
  for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do call "%%i\VC\Auxiliary\Build\vcvars64.bat"
)

cl /nologo /std:c++17 /EHsc /W4 /utf-8 /DUNICODE /D_UNICODE /Fo:build\ /Fe:build\Lab3.exe src\main.cpp src\main_window.cpp src\app_toolbar.cpp src\objects_editor.cpp src\shape.cpp /link /SUBSYSTEM:WINDOWS
if errorlevel 1 exit /b 1
echo Build OK: build\Lab3.exe

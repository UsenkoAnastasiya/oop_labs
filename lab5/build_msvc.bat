@echo off
rem Збірка Lab5 компілятором Microsoft Visual C++ (cl.exe), якщо встановлено Visual Studio
setlocal
cd /d "%~dp0"
if not exist build mkdir build

where cl >nul 2>nul
if errorlevel 1 (
  for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do call "%%i\VC\Auxiliary\Build\vcvars64.bat"
)

set SRCS=
for %%f in (src\*.cpp) do call set SRCS=%%SRCS%% %%f

cl /nologo /std:c++17 /EHsc /W4 /utf-8 /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /Fo:build\ /Fe:build\Lab5.exe %SRCS% /link /SUBSYSTEM:WINDOWS
if errorlevel 1 exit /b 1
echo Build OK: build\Lab5.exe

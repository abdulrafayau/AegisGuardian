@echo off
echo Compiling File Integrity Guardian...
g++ main.cpp -o Guardian.exe -std=c++17
if %errorlevel% neq 0 (
    echo.
    echo [X] Compilation failed!
    pause
    exit /b %errorlevel%
)
echo [OK] Compilation successful.
echo.
Guardian.exe

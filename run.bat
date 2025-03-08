@echo off
mkdir build 2>nul
cd build
cmake ..
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build .
if %errorlevel% neq 0 exit /b %errorlevel%
cd ..
echo ===========================
echo ✅ Build Complete! Running...
echo ===========================
build\library_system.exe

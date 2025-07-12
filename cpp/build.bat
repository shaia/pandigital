@echo off
setlocal

set BUILD_TYPE=vs
if not "%1"=="" (
    if "%1"=="mingw" set BUILD_TYPE=mingw
    if "%1"=="vs" set BUILD_TYPE=vs
)

if "%BUILD_TYPE%"=="mingw" (
    echo Building with MinGW...
    if not exist "build_mingw" mkdir build_mingw
    cd build_mingw
    cmake -G "MinGW Makefiles" ..
    cmake --build .
    cd ..
) else (
    echo Building with Visual Studio 2022...
    if not exist "build_vs" mkdir build_vs
    cd build_vs
    cmake -G "Visual Studio 17 2022" ..
    cmake --build . --config Release
    cd ..
)

echo.
echo Build complete!
if "%BUILD_TYPE%"=="mingw" (
    echo Executable location: build_mingw\pandigital.exe
) else (
    echo Executable location: build_vs\Release\pandigital.exe
)
pause

endlocal

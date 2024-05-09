set CMakePath=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe

set QtPath=C:\Qt\6.4.3\msvc2019_64

rem See https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators
set CMakeGenerator=Visual Studio 17 2022

set PathToSource=%~dp0
set PathToBuild=%~dp0\build



pushd %PathToSource%
set PathToSource=%cd%
popd

if exist "%PathToBuild%" rmdir /s /q "%PathToBuild%"
mkdir "%PathToBuild%"

pushd %PathToBuild%
set PathToBuild=%cd%
popd

"%CMakePath%" -G "%CMakeGenerator%" -D CMAKE_PREFIX_PATH="%QtPath%" -D MANU_GENERATE_VS_SLN="ON" -S "%PathToSource%" -B %PathToBuild%



pause

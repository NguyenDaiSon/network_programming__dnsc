REM Nguyen Dai Son
if exist build ( rmdir /s/q build )
mkdir build
cd build
@cmake.exe .. -G "MinGW Makefiles" %*
mingw32-make
start cmd

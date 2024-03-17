@echo off

echo Compiling...
echo.

set mingw64=C:\winlibs-x86_64-posix-seh-gcc-10.1.0-llvm-10.0.0-mingw-w64-7.0.0-r3\mingw64\bin

%mingw64%\windres rc.rc -O coff -o res.res

%mingw64%\g++ -o bin/ToxInput.exe res.res source/*.cpp --std=c++20 -O3 -lXInput9_1_0 ^
bin/sfml-graphics-2.dll ^
bin/sfml-window-2.dll ^
bin/sfml-system-2.dll ^
-Wl,-subsystem,windows

if %errorlevel% == 0 (
	echo Program started!
	cd bin
	call ToxInput.exe
	cd ..
) else (
	pause
)

rem this will do until i write a proper makefile
@echo off

echo Compiling...
echo.

cd C:/Program Files/mingw-w64/x86_64-8.1.0-win32-seh-rt_v6-rev0/mingw64/bin

windres %1/../rc.rc -O coff -o %1/../res.res

x86_64-w64-mingw32-g++.exe -o %1/../bin/ToxInput.exe %1/../res.res %1/*.cpp -lXInput9_1_0 ^
%1/../bin/sfml-graphics-2.dll ^
%1/../bin/sfml-window-2.dll ^
%1/../bin/sfml-system-2.dll ^
-Wl,-subsystem,windows

if %errorlevel% == 0 (
	echo Program started!
	cd %1/../bin
	call ToxInput.exe
) else (
	pause
)

exit
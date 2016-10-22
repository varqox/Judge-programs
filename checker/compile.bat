@echo off
set DIR=%~dp0
echo %DIR%

copy /Y "TDM-GCC-32/bin/libiconv-2.dll" .
"TDM-GCC-32/bin/mingw32-make" -f Makefile-win CXX="TDM-GCC-32/bin/g++"
del "libiconv-2.dll"
pause && echo. && exit /b 1

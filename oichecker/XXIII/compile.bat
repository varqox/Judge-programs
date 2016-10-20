@echo off
set DIR=%~dp0
echo %DIR%

if exist "TDM-GCC-32" goto tdm

mingw32-make -f Makefile-win CXX="g++"
pause && echo. && exit /b 1

:tdm
"TDM-GCC-32/bin/mingw32-make" -f Makefile-win CXX="TDM-GCC-32/bin/g++"
pause && echo. && exit /b 1

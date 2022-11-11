@echo off
color 0E
echo ==================================
echo  wiikey fusion disc faker 0.2
echo      (c) 2011 by [infact]
echo              for [gc-forever.com]
echo ==================================
echo   mail: infact@quantentunnel.de
echo ==================================
echo  NO WARRANTY FOR THIS BATCH FILE!
echo        IT WORKS FOR ME (TM)
echo ==================================
IF !%1==! goto nofilegiven
if not exist %1 goto filenotexist
verify OTHER 2>nul
setlocal ENABLEEXTENSIONS
if errorlevel 1 goto noextensions
if %~z1 gtr 1048500 goto filetoobig
if not exist "%~dp0first_mib.img" goto wherearemyfiles
if not exist "%~dp0dd4win.exe" goto wherearemyfiles
echo.
echo copying title from filename ("%~n1")...
set /p =%~n1 for WKF>"%~dp0temp.txt"<nul
"%~dp0dd4win" bs=1 if=/dev/zero of="%~dp0first_mib.img" count=68 seek=32 >nul 2>nul
"%~dp0dd4win" bs=1 if="%~dp0temp.txt" of="%~dp0first_mib.img" count=68 seek=32 >nul 2>nul
echo.
set /a gcdiscid=89*%random%/32768+10
echo using random number (%gcdiscid%) for disc id...
set /p =%gcdiscid%>"%~dp0temp.txt"<nul
"%~dp0dd4win" bs=1 if="%~dp0temp.txt" of="%~dp0first_mib.img" count=2 seek=4 >nul 2>nul
echo.
echo creating faked iso file ("%~n1_WKF_%~x1")...
copy /B "%~dp0first_mib.img" + %1 "%~n1_WKF_%~x1" >nul
echo.
echo deleting temporary file...
del "%~dp0temp.txt"
echo ==================================
echo.
echo All done. Press any key to exit...
pause >nul
exit /B
:filenotexist
echo.
echo Error: Given file (%1) not found!
echo.
echo ==================================
echo.
echo Press any key to exit...
pause >nul
exit /B
:wherearemyfiles
echo.
echo Error: dd4win.exe
echo or first_mib.img not found!
echo.
echo ==================================
echo.
echo Press any key to exit...
pause >nul
exit /B
:nofilegiven
echo.
echo Error: No file given!
echo.
echo Usage:
echo %~n0%~x0 isofile.iso
echo.
echo ==================================
echo.
echo Press any key to exit...
pause >nul
exit /B
:noextensions
echo.
echo Error: No commandline extensions
echo available...Sorry!
echo.
echo ==================================
echo.
echo Press any key to exit...
pause >nul
exit /B
:filetoobig
echo.
echo Error: Your file (%1) is too big.
echo The maximum size is 1MiB.
echo.
echo ==================================
echo.
echo Press any key to exit...
pause >nul
exit /B

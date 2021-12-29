@echo off
IF NOT EXIST "..\240pSuite_cube.dol" GOTO NORELOC
echo ===========================================================
echo.
echo Relocating dol and move to bootldr
echo.
dollz3 ..\240pSuite_cube.dol disc\bootldr.dol -m
echo.
echo Creating disc... Please wait.
echo.
echo ===========================================================
mkisofs -R -J -G gbi.hdr -no-emul-boot -b bootldr.dol -o GameCube-240pSuite-1.X.iso disc/
echo.
echo ===========================================================
echo.
del disc\bootldr.dol
echo Look at the output above. If no error is shown, the bootable disc is 
echo successfully built as "GameCube-240pSuite-1.X.iso"
echo.
echo Press any key or close window to exit
pause >nul
exit

:NORELOC
echo FAILED "..\240pSuite_cube.dol" does not exist
pause >nul
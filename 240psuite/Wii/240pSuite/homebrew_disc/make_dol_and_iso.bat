@echo off
IF NOT EXIST "..\240pSuite.dol" GOTO NORELOC
echo ===========================================================
echo.
echo Relocating dol and move to bootldr
echo.
dollz3 ..\240pSuite.dol disc\bootldr.dol -m
:NORELOC
echo.
echo Creating disc... Please wait.
echo.
echo ===========================================================
mkisofs -R -J -G gbi.hdr -no-emul-boot -b bootldr.dol -o 240pTest.iso disc/
echo.
echo ===========================================================
echo.
echo Look at the output above. If no error is shown, the bootable disc is 
echo successfully built as "240pTest.iso"
echo.
echo Press any key or close window to exit
pause >nul
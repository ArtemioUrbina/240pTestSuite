@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set CYGWIN="nodosfilewarning"
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Compile, assemble, and link
CALL makeclean.bat
huc -s -t -O2 -fno-recursive -msmall 240pSuite.c font.c video.c patterns.c tests.c help.c tools.c || goto :error
pceas -s -raw 240pSuite.s || goto :error

:; exit 0
exit /b 0

:error
exit /b %errorlevel%

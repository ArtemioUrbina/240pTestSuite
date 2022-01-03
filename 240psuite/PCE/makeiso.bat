@REM Path to the Huc compiler, The suite uses the one at: https://github.com/jbrandwood/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

CALL makeclean.bat

@REM Compile, assemble, and link
@REM -fno-recursive makes globals fail
huc -DSCDROM -DCDROM -s -t -O2 -msmall -scd -overlay loader.c font.c video.c  || goto :error
huc -DSCDROM -DCDROM -s -t -O2 -msmall -scd -overlay tests_reflex.c font.c video.c tools.c help.c || goto :error
huc -DSCDROM -DCDROM -s -t -O2 -msmall -scd -overlay 240pSuite.c font.c video.c tests.c patterns.c tools.c help.c float.c || goto :error
pceas -s -scd -overlay loader.s  || goto :error
pceas -s -scd -overlay tests_reflex.s || goto :error
pceas -s -scd -overlay 240pSuite.s  || goto :error
isolink 240pSuite.iso loader.ovl 240pSuite.ovl tests_reflex.ovl adpcm.vox adpcmsweep.vox adpcm8khz.vox || goto :error

:; exit 0
exit /b 0

:error
exit /b %errorlevel%
@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Compile, assemble, and link
@REM -msmall causes scroll to crash
@REM -fno-recursive makes globals fail
huc -DCDROM -t -O2 -scd -overlay 240pSuite.c font.c video.c tests.c help.c tools.c
huc -DCDROM -t -O2 -scd -overlay patterns.c font.c video.c help.c tools.c
isolink 240pSuite.iso 240pSuite.ovl patterns.ovl

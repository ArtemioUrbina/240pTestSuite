@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Compile, assemble, and link
@REM -msmall causes scroll to crash
@REM -fno-recursive makes globals fail
huc -DCDROM -t -O2 -scd -overlay 240pSuite.c font.c video.c tests.c help.c tools.c
huc -DCDROM -t -O2 -scd -overlay patterns.c font.c video.c help.c tools.c
huc -DCDROM -t -O2 -scd -overlay loader.c
copy 240pSuite.c 240pArcade.c
huc -t -O2 -scd -overlay 240pArcade.c font.c video.c tests.c patterns.c help.c tools.c
isolink 240pSuite.iso loader.ovl 240pSuite.ovl patterns.ovl 240pArcade.ovl
del 240pArcade.c

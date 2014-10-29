@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Compile, assemble, and link
@REM -fno-recursive makes globals fail
huc -DCDROM -s -t -O2 -msmall -scd -overlay 240pSuite.c font.c video.c tests.c help.c tools.c
huc -DCDROM -s -t -O2 -msmall -scd -overlay patterns.c font.c video.c help.c tools.c
huc -DCDROM -s -t -O2 -msmall -scd -overlay loader.c font.c video.c
pceas -DCDROM1 -s -msmall -scd -overlay 240pSuite.s
pceas -DCDROM1 -s -msmall -scd -overlay patterns.s
pceas -DCDROM1 -s -msmall -scd -overlay loader.s
copy 240pSuite.c 240pArcade.c
huc -DARCADE -s -t -O2 -msmall -scd -overlay 240pArcade.c font.c video.c tests.c patterns.c tools.c help.c 
pceas -DCDROM1 -s -msmall -scd -overlay 240pArcade.s
isolink 240pSuite.iso loader.ovl 240pSuite.ovl patterns.ovl 240pArcade.ovl
del 240pArcade.c

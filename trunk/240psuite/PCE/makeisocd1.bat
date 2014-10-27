@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Compile, assemble, and link

@REM Create gdata.bin and gdata.h
CALL make.bat
copy 240pSuite.s cdrom_bin
CALL makeclean.bat
del gdata.*
cd cdrom_bin

grep "\.incspr" 240pSuite.s | grep -v "include" | sed 's/graphics/..\/graphics/g'> data.s 
pceas -raw -scd -overlay data.s
copy ..\graphics\*.bin .
pcecdpak 0 gdata.bin gdata.h *.bin

move gdata.* ..
@REM del *.bin
@REM del *.ovl
@REM del *.s
@REM del *.sym
cd ..
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -scd -overlay 240pSuite.c font.c video.c tests.c help.c tools.c
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -scd -overlay patterns.c font.c video.c help.c tools.c
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -scd -overlay loader.c video.c font.c
pceas -DCDROM -DCDROM1 -s -msmall -scd -overlay 240pSuite.s
pceas -DCDROM -DCDROM1 -s -msmall -scd -overlay patterns.s
pceas -DCDROM -DCDROM1 -s -msmall -scd -overlay loader.s
isolink 240pSuite.iso loader.ovl 240pSuite.ovl patterns.ovl gdata.bin
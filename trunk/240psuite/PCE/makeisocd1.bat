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
del *.ovl
del *.s
del *.sym
cd ..
huc -DCDROM1 -s -msmall -t -O2 -cd -overlay loader.c video.c font.c
huc -DCDROM1 -s -msmall -t -O2 -cd -overlay 240pSuite.c font.c video.c tests.c tools.c help.c 
huc -DCDROM1 -s -msmall -t -O2 -cd -overlay patterns.c font.c video.c tools.c help.c 
huc -DCDROM1 -s -msmall -t -O2 -cd -overlay tests_ext.c video.c font.c help.c 
pceas -DCDROM1 -s -msmall -cd -overlay loader.s
pceas -DCDROM1 -s -msmall -cd -overlay 240pSuite.s
pceas -DCDROM1 -s -msmall -cd -overlay patterns.s
pceas -DCDROM1 -s -msmall -cd -overlay tests_ext.s
isolink 240pSuite.iso loader.ovl 240pSuite.ovl patterns.ovl tests_ext.ovl gdata.bin
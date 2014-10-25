@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Compile, assemble, and link

@REM Cluid gdata.bin and gdata.h
@REM CALL make.bat
@REM copy 240pSuite.s cdrom_bin
@REM CALL makeclean.bat
@REM del gdata.*
@REM cd cdrom_bin
@REM grep "\.inc" 240pSuite.s | grep -v "include" | sed 's/graphics/..\/graphics/g'> data.s 
@REM pceas -raw -scd -overlay data.s
@REM copy ..\graphics\*.bin .
@REM pcecdpak 0 gdata.bin gdata.h *.bin
@REM move gdata.* ..
@REM del *.bin
@REM del *.ovl
@REM del *.s
@REM del *.sym
@REM cd ..
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -cd -overlay 240pSuite.c font.c video.c tests.c help.c tools.c
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -cd -overlay patterns.c font.c video.c help.c tools.c
pceas -DCDROM -DCDROM1 -s -msmall -raw -cd -overlay 240pSuite.s
pceas -DCDROM -DCDROM1 -s -msmall -raw -cd -overlay patterns.s
isolink 240pSuite.iso patterns.ovl 240pSuite.ovl gdata.bin
@REM Path to the Huc compiler, The suite uses the one at: https://github.com/pce-devel/huc
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/huc

@REM Compile, assemble, and link

@REM Create gdata.bin and gdata.h
CALL make.bat
copy 240pSuite.s cdrom_bin
CALL makeclean.bat
del gdata.*
cd cdrom_bin

grep "\.incspr" 240pSuite.s | grep -v "include" | sed 's/graphics/..\/graphics/g'> data.s 
pceas -raw -cd -overlay data.s  || goto :error
copy src\*.bin .
pcecdpak 0 gdata.bin gdata.h *.bin  || goto :error

move gdata.* ..
del *.bin
del *.ovl
del *.s
del *.sym
cd ..
huc -DCDROM -DSYSCARD1 -s -msmall -t -O2 -cd -overlay loader.c video.c font.c  || goto :error
huc -DCDROM -DSYSCARD1 -s -msmall -t -O2 -cd -overlay 240pSuite.c font.c video.c tests.c tools.c help.c float.c || goto :error
huc -DCDROM -DSYSCARD1 -s -msmall -t -O2 -cd -overlay patterns.c font.c video.c tools.c help.c float.c || goto :error
huc -DCDROM -DSYSCARD1 -DEXT_TOOLS -s -msmall -t -O2 -cd -overlay patterns_ext.c font.c video.c tools.c help.c float.c || goto :error
huc -DCDROM -DSYSCARD1 -DEXT_TOOLS -s -msmall -t -O2 -cd -overlay tests_ext.c video.c font.c tools.c help.c float.c  || goto :error
huc -DCDROM -DSYSCARD1 -s -msmall -t -O2 -cd -overlay tests_sound.c video.c font.c tools.c help.c || goto :error
huc -DCDROM -DHELP_OVL -s -msmall -t -O2 -cd -overlay help.c video.c font.c tools.c || goto :error
pceas -O -s -cd -overlay loader.s  || goto :error
pceas -O -s -cd -overlay 240pSuite.s  || goto :error
pceas -O -s -cd -overlay patterns.s  || goto :error
pceas -O -s -cd -overlay patterns_ext.s || goto :error
pceas -O -s -cd -overlay tests_ext.s || goto :error
pceas -O -s -cd -overlay tests_sound.s || goto :error
pceas -O -s -cd -overlay help.s  || goto :error
isolink 240pSuite.iso loader.ovl 240pSuite.ovl patterns.ovl patterns_ext.ovl tests_ext.ovl tests_sound.ovl gdata.bin help.ovl adpcm.vox adpcmsweep.vox adpcm8khz.vox || goto :error

:; exit 0
exit /b 0

:error
exit /b %errorlevel%
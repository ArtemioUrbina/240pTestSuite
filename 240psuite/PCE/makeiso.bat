@REM Path to the Huc compiler, The suite uses the one at: https://github.com/uli/huc
@set CYGWIN="nodosfilewarning"
@set HUC_HOME=C:\Devel\PCE\huc

@path=%path%;%HUC_HOME%\bin
@set PCE_INCLUDE=%HUC_HOME%/include/pce

@REM Make the CD ROM2 Target for Syetem Card 1 and 2

@REM Create gdata.bin and gdata.h from the HuCard version
CALL make.bat
copy 240pSuite.s cdrom_bin
CALL makeclean.bat
del gdata.*
cd cdrom_bin

grep "\.incspr" 240pSuite.s | grep -v "include" | sed 's/graphics/..\/graphics/g'> data.s 
pceas -raw -cd -overlay data.s
copy src\*.bin .
pcecdpak 0 gdata.bin gdata.h *.bin

move gdata.* ..
del *.bin
del *.ovl
del *.s
del *.sym
cd ..

@REM Assemble and compile the CD-ROM2 ovl files
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -cd -overlay loader.c video.c font.c

huc -DCDROM -DCDROM1 -s -msmall -t -O2 -cd -overlay 240pSuite.c font.c video.c tests.c tools.c help.c  -o 
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -cd -overlay patterns.c font.c video.c tools.c help.c 
huc -DCDROM -DCDROM1 -s -msmall -t -O2 -cd -overlay tests_ext.c video.c font.c help.c 
huc -DCDROM -DHELP_OVL -s -msmall -t -O2 -cd -overlay help.c video.c font.c tools.c

pceas -DCDROM -DCDROM1 -s -msmall -cd -overlay loader.s
pceas -DCDROM -DCDROM1 -s -msmall -cd -overlay 240pSuite.s
pceas -DCDROM -DCDROM1 -s -msmall -cd -overlay patterns.s 
pceas -DCDROM -DCDROM1 -s -msmall -cd -overlay tests_ext.s
pceas -DCDROM -DHELP_OVL -s -msmall -cd -overlay help.s

move 240pSuite.ovl 240pSuite_CD.ovl
move patterns.ovl patterns_CD.ovl
move tests_ext.ovl tests_ext_CD.ovl
move help.ovl help_CD.ovl

del *.s

@REM Make the Super CD ROM Target

huc -DSCDROM -DCDROM -s -t -O2 -msmall -scd -overlay 240pSuite.c font.c video.c tests.c patterns.c tools.c help.c 
pceas -DSCDROM -DCDROM -s -msmall -scd -overlay 240pSuite.s

isolink 240pSuite.iso loader.ovl 240pSuite.ovl adpcm.vox sweep.vox 240pSuite_CD.ovl patterns_CD.ovl tests_ext_CD.ovl gdata.bin help_CD.ovl
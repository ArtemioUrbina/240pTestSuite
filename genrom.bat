cp out/char.bin cart/char.bin
cp out/dev_p1.rom cart/202-p1.p1
cp out/fix.bin cart/202-s1.s1
cp out/m1.rom cart/202-m1.m1
cp out/v1.rom cart/202-v1.v1

cd cart
..\..\..\m68k\bin\romwak /f 202-p1.p1
..\..\..\m68k\bin\romwak /p 202-p1.p1 202-p1.p1 1024 255
..\..\..\m68k\bin\romwak /w char.bin 202-c1.c1 202-c2.c2
..\..\..\m68k\bin\romwak /f 202-c1.c1
..\..\..\m68k\bin\romwak /f 202-c2.c2
..\..\..\m68k\bin\romwak /p 202-c1.c1 202-c1.c1 1024 255
..\..\..\m68k\bin\romwak /p 202-c2.c2 202-c2.c2 1024 255
..\..\..\m68k\bin\romwak /p 202-s1.s1 202-s1.s1 128 255

del char.bin

del 240ptest.zip
..\..\..\tools\7zip\7za a 240ptest.zip 202-p1.p1 202-c1.c1 202-c2.c2 202-s1.s1 202-m1.m1 202-v1.v1

cd ..\..\..\tools\neobuilder
NeoBuilder -n 240ptest -m dasutinartemio -y 2022 -g Other -s 480 ..\..\projects\ng240ptest\cart\240ptest.zip
move 240ptest.neo ..\..\projects\ng240ptest\cart\neosd

cd ..\..\projects\ng240ptest\
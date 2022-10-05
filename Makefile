#######################################
# Base dir of your m68k gcc toolchain #
#######################################

NEODEV=/c/NeoDev
export PATH=$(NEODEV)\m68k\bin

MAMEDIR = $(NEODEV)\mame\roms\240ptestng

BASEDIR = $(NEODEV)
AS = as
LD = gcc
CC = gcc
OBJC = objcopy
BIN2O = bin2elf
GFXCC = gfxcc
FIXCNV = fixcnv
ROMWAK = romwak
BUILDCHAR = BuildChar
CHARSPLIT = CharSplit
RM = rm -f
CP = cp
7Z = $(NEODEV)/tools/7zip/7za
NBPATH = $(NEODEV)/tools/neobuilder/
NB = ./NeoBuilder.exe

PROM = dev_p1.rom
CROM = char.bin
SROM = fix.bin
MROM = m1.rom
VROM = v1.rom

#######################################
# Path to libraries and include files #
#######################################

INCDIR = -I$(BASEDIR)/m68k/include -Iinclude 
LIBDIR = $(BASEDIR)/m68k/lib

###################################
# Output: {cart, cd} *lower case* #
###################################
OUTPUT = cart

############################
# Settings for cart output #
############################
ROMSIZE = 0x100000
PADBYTE = 0xFF

##############################
# Object Files and Libraries #
##############################

OBJS = \
	crt0_$(OUTPUT).o \
	src/main.o \
	src/patterns.o \
	src/tests.o \
	src/help.o \
	src/charMaps.o \
	src/charPals.o \
	src/fixPals.o \
	src/string.o
LIBS = -lDATlib -lprocess -lc -lgcc

#####################
# Compilation Flags #
#####################

ASFLAGS = -m68000 --register-prefix-optional
LDFLAGS = -Wl,-T$(BASEDIR)/src/system/neo$(OUTPUT).x -Xlinker -Map=output.map
CCFLAGS = -m68000 -O3 -Wall -fomit-frame-pointer -ffast-math -fno-builtin -nostartfiles -nodefaultlibs -D__$(OUTPUT)__
ARFLAGS = cr
DEBUG = -g

##############
# Make rules #
##############

ifeq ($(OUTPUT),cart)
out/$(PROM) : prog.o
	$(OBJC) --gap-fill=$(PADBYTE) --pad-to=$(ROMSIZE) -R .data -O binary $< $@
#	$(OBJC) --gap-fill=$(PADBYTE) -R .data -O binary $< $@
else
dev_p1.prg : prog.o
	$(OBJC) -O binary $< $@
endif

prog.o : $(OBJS)
	$(LD) -L$(LIBDIR) $(CCFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(INCDIR) $(CCFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

neo:
	$(CP) out/char.bin cart/char.bin
	$(CP) out/dev_p1.rom cart/2501-p1.p1
	$(CP) out/fix.bin cart/2501-s1.s1
	$(CP) out/m1.rom cart/2501-m1.m1
	$(CP) out/v1.rom cart/2501-v1.v1
	cd cart && $(ROMWAK) //f 2501-p1.p1 2501-p1.p1
	cd cart && $(ROMWAK) //p 2501-p1.p1 2501-p1.p1 1024 255
	cd cart && $(ROMWAK) //p char.bin char.bin 2048 255
	cd cart && $(CHARSPLIT) char.bin -rom 2501
	$(CP) cart/2501.C1 cart/2501-c1.c1
	$(CP) cart/2501.C2 cart/2501-c2.c2
	cd cart && $(ROMWAK) //p 2501-s1.s1 2501-s1.s1 128 255
	$(RM) cart/char.bin
	$(RM) cart/2501.C1
	$(RM) cart/2501.C2
	$(RM) cart/240ptest.neo
	cd cart && $(7Z) a $(NBPATH)240ptest.zip 2501-p1.p1 2501-c1.c1 2501-c2.c2 2501-s1.s1 2501-m1.m1 2501-v1.v1
	cd $(NBPATH) && $(NB) -n 240ptest -m dasutinartemio -y 2022 -g Other -s 2501 240ptest.zip
	$(RM) $(NBPATH)240ptest.zip
	$(CP) $(NBPATH)240ptest.neo cart/neosd
	$(RM) $(NBPATH)240ptest.neo

copyroms: neo
	$(CP) cart/2501-p1.p1 $(MAMEDIR)
	$(CP) cart/2501-c1.c1 $(MAMEDIR)
	$(CP) cart/2501-c2.c2 $(MAMEDIR)
	$(CP) cart/2501-s1.s1 $(MAMEDIR)
	$(CP) cart/2501-m1.m1 $(MAMEDIR)
	$(CP) cart/2501-v1.v1 $(MAMEDIR)

clean:
	$(RM) *.o src/*.o out/$(PROM) output.map

char:
	$(BUILDCHAR) chardata.xml

fix:
	$(BUILDCHAR) fixdata.xml
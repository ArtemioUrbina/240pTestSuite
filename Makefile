#######################################
# Base dir of your m68k gcc toolchain #
#######################################

NEODEV=C:\NeoDev
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
ROMSIZE = 0x20000
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
#	$(OBJC) --gap-fill=$(PADBYTE) --pad-to=$(ROMSIZE) -R .data -O binary $< $@
	$(OBJC) --gap-fill=$(PADBYTE) -R .data -O binary $< $@
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

# Disabling makeroms use genroms.bat instead
#makeroms:
#	$(ROMWAK) -f out/${PROM} cart/202-p1.p1
#	$(ROMWAK) -p out/${PROM} cart/202-p1.p1 1024 255
#	$(ROMWAK) -w out/${CROM} 202-c1.c1 202-c2.c2
#	$(ROMWAK) -f 202-c1.c1
#	$(ROMWAK) -f 202-c2.c2
#	$(ROMWAK) -p 202-c1.c1 202-c1.c1 1024 255
#	$(ROMWAK) -p 202-c2.c2 202-c2.c2 1024 255
#	cp 202-c1.c1 cart/202-c1.c1
#	cp 202-c2.c2 cart/202-c2.c2
#	cp out/fix.bin cart/202-s1.s1
#	$(ROMWAK) -p cart/202-s1.s1 cart/202-s1.s1 128 255
#	cp out/m1.rom cart/202-m1.m1
#	cp out/v1.rom cart/202-v1.v1
#	$(ROMWAK) -p out/${SROM} 202-s1.s1 128 255
#	copy sounds\roms\202-v1.v1
#	$(ROMWAK) -p 202-v1.v1 202-v1.v1 512 255
#	copy sounds\roms\202-m1.bin

copyroms:
	$(CP) out/$(PROM) $(MAMEDIR)
	$(CP) out/$(CROM) $(MAMEDIR)
	$(CP) out/$(SROM) $(MAMEDIR)
	$(CP) out/$(MROM) $(MAMEDIR)
	$(CP) out/$(VROM) $(MAMEDIR)

clean:
	$(RM) *.o src/*.o out/$(PROM) output.map

char:
	$(BUILDCHAR) chardata.xml

fix:
	$(BUILDCHAR) fixdata.xml
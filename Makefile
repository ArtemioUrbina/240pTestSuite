#######################################
# Base dir of your m68k gcc toolchain #
#######################################

NEODEV=/c/NeoDev
export PATH=$(NEODEV)/m68k/bin

MAMEDIR = $(NEODEV)\mame\roms\240ptestng
MAMECDDIR = $(NEODEV)\mame\roms\neocd\240ptestngcd

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

PROM  = 2501-p1.p1
CROM  = 2501-cx.cx
C1ROM = 2501-c1.c1
C2ROM = 2501-c2.c2
SROM  = 2501-s1.s1
MROM  = 2501-m1.m1
VROM  = 2501-v1.v1


MKISOFS = $(NEODEV)/tools/cdtools/mkisofs
FLAGS_MKISOFS = -iso-level 1 -pad -N
CHDMAN = $(NEODEV)/tools/cdtools/chdman

# NGCD_IMAGENAME - output image/ISO name
NGCD_IMAGENAME = 240pTestSuite

# NGCD_DISCLABEL - Disc label (8 characters maximum)
NGCD_DISCLABEL = 240TEST

CD_BASE_NAME  = 240P
CDPRG_OUTFILE = $(CD_BASE_NAME).PRG
CDCHR_OUTFILE = $(CD_BASE_NAME).SPR
CDFIX_OUTFILE = $(CD_BASE_NAME).FIX
CDZ80_OUTFILE = $(CD_BASE_NAME).Z80
CDPCM_OUTFILE = $(CD_BASE_NAME).PCM

# NGCD_DISCFILES - List of files to put on the CD, passed to mkisofs
NGCD_DISCFILES = \
	ABS.TXT \
	BIB.TXT \
	CPY.TXT \
	IPL.TXT \
	TITLE_E.SYS \
	TITLE_J.SYS \
	TITLE_U.SYS \
	$(CDPRG_OUTFILE) \
	$(CDZ80_OUTFILE) \
	$(CDFIX_OUTFILE) \
	$(CDPCM_OUTFILE) \
	$(CDCHR_OUTFILE)

#######################################
# Path to libraries and include files #
#######################################

INCDIR = -I$(BASEDIR)/m68k/include -Iinclude 
LIBDIR = $(BASEDIR)/m68k/lib

###################################
# Output: {cart, cd} *lower case* #
###################################
#OUTPUT = cart
OUTPUT = cd

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
	src/string.o \
	src/tools.o
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
cd/$(CDPRG_OUTFILE) : prog.o
	$(OBJC) -O binary $< $@
endif

prog.o : $(OBJS)
	$(LD) -L$(LIBDIR) $(CCFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(INCDIR) $(CCFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

cart: z80
	$(CP) out/$(PROM) $(OUTPUT)/$(PROM)
	$(CP) out/$(SROM) $(OUTPUT)/$(SROM)
	$(CP) out/$(MROM) $(OUTPUT)/$(MROM)
	$(CP) out/$(VROM) $(OUTPUT)/$(VROM)
	cd out && $(ROMWAK) //p $(CROM) $(CROM) 2048 255
	cd out && $(CHARSPLIT) $(CROM) -rom 2501
	$(CP) out/2501.C1 $(OUTPUT)/$(C1ROM)
	$(CP) out/2501.C2 $(OUTPUT)/$(C2ROM)
	$(RM) out/2501.C1
	$(RM) out/2501.C2
	cd $(OUTPUT) && $(ROMWAK) //f $(PROM) $(PROM)
	cd $(OUTPUT) && $(ROMWAK) //p $(PROM) $(PROM) 1024 255
	cd $(OUTPUT) && $(ROMWAK) //p $(SROM) $(SROM) 128 255

ifeq ($(OUTPUT),cart)
neo: cart out/$(PROM)
	$(RM) $(OUTPUT)/240ptest.neo
	cd $(OUTPUT) && $(7Z) a $(NBPATH)240ptest.zip $(PROM) $(C1ROM) $(C2ROM) $(SROM) $(MROM) $(VROM)
	cd $(NBPATH) && $(NB) -n 240ptest -m dasutinartemio -y 2022 -g Other -s 2501 240ptest.zip
	$(RM) $(NBPATH)240ptest.zip
	$(CP) $(NBPATH)240ptest.neo $(OUTPUT)/neosd
	$(RM) $(NBPATH)240ptest.neo
else
neo: cdz80
	$(CP) out/$(CROM) cd/$(CROM)
	cd cd && $(CHARSPLIT) $(CROM) -cd $(CD_BASE_NAME)
	$(RM) cd/$(CROM)
	$(CP) out/$(SROM) cd/$(CDFIX_OUTFILE)
	cd cd && $(MKISOFS) $(FLAGS_MKISOFS) -o $(NGCD_IMAGENAME).iso -V "$(NGCD_DISCLABEL)" $(NGCD_DISCFILES)
	cd cd && $(CHDMAN) createcd -i 240pTestSuite.cue -o 240pTestSuite.chd
	$(CP) cd/240pTestSuite.iso cd/iso/240pTestSuite.iso
	$(CP) cd/240pTestSuite.chd cd/chd/240pTestSuite.chd
	$(RM) cd/240pTestSuite.iso
	$(RM) cd/240pTestSuite.chd
endif

ifeq ($(OUTPUT),cart)
copyroms: cart
	$(CP) cart/$(PROM) $(MAMEDIR)
	$(CP) cart/$(C1ROM) $(MAMEDIR)
	$(CP) cart/$(C2ROM) $(MAMEDIR)
	$(CP) cart/$(SROM) $(MAMEDIR)
	$(CP) cart/$(MROM) $(MAMEDIR)
	$(CP) cart/$(VROM) $(MAMEDIR)
else
copyroms: neo
	$(CP) cd/chd/240pTestSuite.chd $(MAMECDDIR)
endif

clean:
	$(RM) *.o src/*.o output.map
	$(RM) $(OUTPUTDIR_CART)/*.p1 $(OUTPUTDIR_CART)/*.m1 $(OUTPUTDIR_CART)/*.v* 
	$(RM) $(OUTPUT)/*.p1 $(OUTPUT)/*.m1 $(OUTPUT)/*.v* 
	$(RM) $(OUTPUT)/*.SPR $(OUTPUT)/*.FIX $(OUTPUT)/*.PRG $(OUTPUT)/*.iso
	$(RM) $(OUTPUTDIR_CD)/*.Z80 $(OUTPUTDIR_CD)/*.PCM
	$(RM) $(SND)/samples_*.inc

char:
	$(BUILDCHAR) chardata.xml

fix:
	$(BUILDCHAR) fixdata.xml

############################# AUDIO #######################################

Z80_TOOLS  = /c/NeoDev/z80/bin

SND        = sound

# VASM_Z80 - name of vasm executable targeting z80 with oldstyle syntax
VASM_Z80 = $(Z80_TOOLS)/vasmz80

# TOOL_ADPCMA - name of ADPCM-A encoder executable
TOOL_ADPCMA = $(Z80_TOOLS)/adpcma

# TOOL_ADPCMB - name of ADPCM-B encoder executable
TOOL_ADPCMB = $(Z80_TOOLS)/adpcmb

# LUA - name of Lua interpreter executable (used for Sailor VROM)
# This is a terrible hack, but since we did an overrride of PATH above...
LUA = /mingw64/bin/lua

# Z80_FILE - main Z80 asm file (still one file because I don't do any bankswitching)
Z80_FILE = $(SND)/sound.asm

# PCMA_LIST - path to list of ADPCM-A files (used by Sailor VROM)
PCMA_LIST = $(SND)/pcm/pcma.txt

# PCMB_LIST - path to list of ADPCM-B files (used by Sailor VROM)
PCMB_LIST = $(SND)/pcm/pcmb.txt

# CDPCM_LIST - path to list of ADPCM-A files for CD (used by Sailor VROM)
CDPCM_LIST = $(SND)/pcm/pcma_cd.txt

OUTPUTDIR_CART = out
OUTPUTDIR_CD = cd

Z80_OUTFILE = $(MROM)

PCMOUT_CART = $(OUTPUTDIR_CART)/$(VROM)
PCMOUT_CART_INC = $(SND)/samples_cart.inc

PCMOUT_CD = $(OUTPUTDIR_CD)/$(CDPCM_OUTFILE)
PCMOUT_CD_INC = $(SND)/samples_cd.inc

# FLAGS_VASMZ80 - Flags for vasm Z80
FLAGS_VASMZ80 = -Fbin -nosym
FLAGS_ROMWAK_Z80 = //p

FLAGS_CART = TARGET_CART
FLAGS_CD = TARGET_CD

# Z80OUT_CART_SIZE - output size of M1 rom in kilobytes
Z80OUT_CART_SIZE = 64

# FLAGS_ROMWAK_PCM - Pad output file
FLAGS_ROMWAK_PCM = //p

# PCMOUT_CART_SIZE - output size of V1 rom in kilobytes
PCMOUT_CART_SIZE = 128

##############################################################################

z80: pcm
	$(VASM_Z80) $(FLAGS_VASMZ80) -D$(FLAGS_CART) -o $(OUTPUTDIR_CART)/$(Z80_OUTFILE) $(Z80_FILE)
	$(ROMWAK) $(FLAGS_ROMWAK_Z80) $(OUTPUTDIR_CART)/$(Z80_OUTFILE) $(OUTPUTDIR_CART)/$(Z80_OUTFILE) $(Z80OUT_CART_SIZE)

cdz80: cdpcm
	$(VASM_Z80) $(FLAGS_VASMZ80) -D$(FLAGS_CD) -o $(OUTPUTDIR_CD)/$(CDZ80_OUTFILE) $(Z80_FILE)

pcm:
	$(LUA) $(SND)/pcm/svrom.lua --pcma=$(PCMA_LIST) --pcmb=$(PCMB_LIST) --outname=$(PCMOUT_CART) --samplelist=$(PCMOUT_CART_INC)
	$(ROMWAK) $(FLAGS_ROMWAK_PCM) $(PCMOUT_CART) $(PCMOUT_CART) $(PCMOUT_CART_SIZE)

cdpcm:
	$(LUA) $(SND)/pcm/svrom.lua --mode=cd --pcma=$(CDPCM_LIST) --outname=$(PCMOUT_CD) --samplelist=$(PCMOUT_CD_INC)


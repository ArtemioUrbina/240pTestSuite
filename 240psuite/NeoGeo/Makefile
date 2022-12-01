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

NGH = 2501

PROM  = $(NGH)-p1.p1
CROM  = $(NGH)-cx.cx
C1ROM = $(NGH)-c1.c1
C2ROM = $(NGH)-c2.c2
SROM  = $(NGH)-s1.s1
MROM  = $(NGH)-m1.m1
VROM  = $(NGH)-v1.v1
# Comment teh above and uncomment teh two below and all other VROMB references
# For Split VROM (Not using PCB chip)
#VROM  = $(NGH)-v11.v11
#VROMB = $(NGH)-v21.v21


MKISOFS = $(NEODEV)/tools/cdtools/mkisofs
FLAGS_MKISOFS = -iso-level 1 -pad -N
CHDMAN = $(NEODEV)/tools/cdtools/chdman

# FLAGS_ROMWAK Pad output file
FLAGS_ROMWAK_PAD = //p

# FLAGS_ROMWAK flip hi/lo output file
FLAGS_ROMWAK_FLIP = //f

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
OUTPUT = cart
#OUTPUT = cd

############################
# Settings for cart output #
############################
ROMSIZE   = 0x100000
PADBYTE   = 0xFF
ROMSIZEKB = 1024
CHARSSIZE = 2048
PADBYTED  = 255
FIXSIZE   = 128

# Z80OUT_CART_SIZE - output size of M1 rom in kilobytes
Z80OUT_CART_SIZE = 64

# PCMOUT_CART_SIZE - output size of V1 rom in kilobytes
PCMOUT_CART_SIZE = 256

##############################
# Object Files and Libraries #
##############################

OBJS = \
	crt0_$(OUTPUT).o \
	src/main.o \
	src/patterns.o \
	src/tests.o \
	src/hardware.o \
	src/audio.o \
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

ifeq ($(OUTPUT),cart)
cart: z80 out/$(PROM) 
	$(CP) out/$(PROM) $(OUTPUT)/$(PROM)
	$(CP) out/$(SROM) $(OUTPUT)/$(SROM)
	$(CP) out/$(MROM) $(OUTPUT)/$(MROM)
	$(CP) out/$(VROM) $(OUTPUT)/$(VROM)
	#$(CP) out/$(VROMB) $(OUTPUT)/$(VROMB)
	cd out && $(ROMWAK) $(FLAGS_ROMWAK_PAD) $(CROM) $(CROM).spl $(CHARSSIZE) $(PADBYTED)
	cd out && $(CHARSPLIT) $(CROM).spl -rom $(NGH)
	$(CP) out/$(NGH).C1 $(OUTPUT)/$(C1ROM)
	$(CP) out/$(NGH).C2 $(OUTPUT)/$(C2ROM)
	$(RM) out/$(NGH).C1
	$(RM) out/$(NGH).C2
	$(RM) out/$(CROM).spl
	cd $(OUTPUT) && $(ROMWAK) $(FLAGS_ROMWAK_FLIP) $(PROM) $(PROM)
	cd $(OUTPUT) && $(ROMWAK) $(FLAGS_ROMWAK_PAD) $(PROM) $(PROM) $(ROMSIZEKB) $(PADBYTED)
	cd $(OUTPUT) && $(ROMWAK) $(FLAGS_ROMWAK_PAD) $(SROM) $(SROM) $(FIXSIZE) $(PADBYTED)
endif

ifeq ($(OUTPUT),cart)
neo: cart
	$(RM) $(OUTPUT)/240ptest.neo
	cd $(OUTPUT) && $(7Z) a $(NBPATH)240ptest.zip $(PROM) $(C1ROM) $(C2ROM) $(SROM) $(MROM) $(VROM)
	#cd $(OUTPUT) && $(7Z) a $(NBPATH)240ptest.zip $(PROM) $(C1ROM) $(C2ROM) $(SROM) $(MROM) $(VROM) $(VROMB)
	cd $(NBPATH) && $(NB) -n 240ptest -m dasutinartemio -y 2022 -g Other -s $(NGH) 240ptest.zip
	$(RM) $(NBPATH)240ptest.zip
	$(CP) $(NBPATH)240ptest.neo $(OUTPUT)/neosd
	$(RM) $(NBPATH)240ptest.neo
else
neo: cdz80 cd/$(CDPRG_OUTFILE)
	$(CP) out/$(CROM) $(OUTPUT)/$(CROM)
	cd $(OUTPUT) && $(CHARSPLIT) $(CROM) -cd $(CD_BASE_NAME)
	$(RM) $(OUTPUT)/$(CROM)
	$(CP) out/$(SROM) cd/$(CDFIX_OUTFILE)
	cd $(OUTPUT) && $(MKISOFS) $(FLAGS_MKISOFS) -o iso/$(NGCD_IMAGENAME).iso -V "$(NGCD_DISCLABEL)" $(NGCD_DISCFILES)
	cd $(OUTPUT)/iso && $(CHDMAN) createcd -f -i 240pTestSuite.cue -o ../chd/$(NGCD_IMAGENAME).chd
endif

ifeq ($(OUTPUT),cart)
copyroms: cart
	$(CP) $(OUTPUT)/$(PROM) $(MAMEDIR)
	$(CP) $(OUTPUT)/$(C1ROM) $(MAMEDIR)
	$(CP) $(OUTPUT)/$(C2ROM) $(MAMEDIR)
	$(CP) $(OUTPUT)/$(SROM) $(MAMEDIR)
	$(CP) $(OUTPUT)/$(MROM) $(MAMEDIR)
	$(CP) $(OUTPUT)/$(VROM) $(MAMEDIR)
	#$(CP) $(OUTPUT)/$(VROMB) $(MAMEDIR)
else
copyroms: neo
	$(CP) $(OUTPUT)/chd/240pTestSuite.chd $(MAMECDDIR)
endif

clean:
	$(RM) *.o src/*.o output.map
	$(RM) $(OUTPUTDIR_MIXED)/*.p1 $(OUTPUTDIR_MIXED)/*.m1 $(OUTPUTDIR_MIXED)/*.v*
	$(RM) cart/*.p1 cart/*.m1 cart/*.v* cart/*.c* cart/*.s1
	$(RM) $(OUTPUTDIR_CD)/*.SPR $(OUTPUTDIR_CD)/*.FIX $(OUTPUTDIR_CD)/*.PRG $(OUTPUTDIR_CD)/*.iso 
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

OUTPUTDIR_MIXED = out
OUTPUTDIR_CD = cd

PCMOUT_CART = $(OUTPUTDIR_MIXED)/$(VROM)
#VROMB_OUT_CART = $(OUTPUTDIR_MIXED)/$(VROMB)
PCMOUT_CART_INC = $(SND)/samples_cart.inc

PCMOUT_CD = $(OUTPUTDIR_CD)/$(CDPCM_OUTFILE)
PCMOUT_CD_INC = $(SND)/samples_cd.inc

# FLAGS_VASMZ80 - Flags for vasm Z80
FLAGS_VASMZ80 = -Fbin -nosym -chklabels

# Flags needed for ASM code between Cart and CD
FLAGS_CART = TARGET_CART
FLAGS_CD = TARGET_CD

##############################################################################

z80: pcm
	$(VASM_Z80) $(FLAGS_VASMZ80) -D$(FLAGS_CART) -o $(OUTPUTDIR_MIXED)/$(MROM) $(Z80_FILE)
	$(ROMWAK) $(FLAGS_ROMWAK_PAD) $(OUTPUTDIR_MIXED)/$(MROM) $(OUTPUTDIR_MIXED)/$(MROM) $(Z80OUT_CART_SIZE)

cdz80: cdpcm
	$(VASM_Z80) $(FLAGS_VASMZ80) -D$(FLAGS_CD) -o $(OUTPUTDIR_CD)/$(CDZ80_OUTFILE) $(Z80_FILE)

pcm:
	$(LUA) $(SND)/pcm/svrom.lua --pcma=$(PCMA_LIST) --pcmb=$(PCMB_LIST) --outname=$(PCMOUT_CART) --samplelist=$(PCMOUT_CART_INC)
	#$(LUA) $(SND)/pcm/svrom.lua --pcma=$(PCMA_LIST) --pcmb=$(PCMB_LIST) --outname=$(PCMOUT_CART) --samplelist=$(PCMOUT_CART_INC) --vrom=split --outnameB=$(VROMB_OUT_CART)
	$(ROMWAK) $(FLAGS_ROMWAK_PAD) $(PCMOUT_CART) $(PCMOUT_CART) $(PCMOUT_CART_SIZE)
	#$(ROMWAK) $(FLAGS_ROMWAK_PAD) $(VROMB_OUT_CART) $(VROMB_OUT_CART) $(PCMOUT_CART_SIZE)

cdpcm:
	$(LUA) $(SND)/pcm/svrom.lua --mode=cd --pcma=$(CDPCM_LIST) --outname=$(PCMOUT_CD) --samplelist=$(PCMOUT_CD_INC)


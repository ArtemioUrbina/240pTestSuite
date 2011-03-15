BIN= $(GDK)/bin
LIB= $(GDK)/lib

LIBSRC= $(GDK)/src
LIBINCLUDE= $(GDK)/include

ROM = romscd
SRC= src
RES= res
INCLUDE= include

TRIMFSIMG = $(SCD_LOADER)/trimfsimage
MKISOFS = $(SCD_LOADER)/mkisofs
ASM68K = $(SCD_LOADER)/ASM68K

SHELL=$(BIN)/sh
AR= $(BIN)/ar
CC= $(BIN)/gcc
OBJCPY= $(BIN)/objcopy
ASMZ80= $(BIN)/sjasm
MACCER= $(BIN)/mac68k
SIZEBND= $(BIN)/sizebnd
BINTOC= $(BIN)/bintoc
BINTOS= $(BIN)/bintos
WAVTORAW= $(BIN)/wavtoraw
GENRES= $(BIN)/genres
TFMCOM= $(BIN)/tfmcom
NM= $(BIN)/nm
RM= $(BIN)/rm
MKDIR= $(BIN)/mkdir
NM2WCH= $(BIN)/nm2wch


OPTION= -DSEGACD

SRC_C= $(wildcard *.c)
SRC_C+= $(wildcard $(SRC)/*.c)
SRC_S= $(wildcard *.s)
SRC_S+= $(wildcard $(SRC)/*.s)
SRC_SZ80= $(wildcard *.s80)
SRC_SZ80+= $(wildcard $(SRC)/*.s80)
RES_S= $(wildcard $(RES)/*.s)
RES_BMP= $(wildcard *.bmp)
RES_BMP+= $(wildcard $(RES)/*.bmp)
RES_RAW= $(wildcard *.raw)
RES_RAW+= $(wildcard $(RES)/*.raw)
RES_PCM= $(wildcard *.pcm)
RES_PCM+= $(wildcard $(RES)/*.pcm)
RES_TFD= $(wildcard *.tfd)
RES_TFD+= $(wildcard $(RES)/*.tfd)
RES_TFC= $(wildcard *.tfc)
RES_TFC+= $(wildcard $(RES)/*.tfc)
RES_WAV= $(wildcard *.wav)
RES_WAV+= $(wildcard $(RES)/*.wav)
RES_RC= $(wildcard *.rc)
RES_RC+= $(wildcard $(RES)/*.rc)
RES_ASM= $(wildcard *.asm)
RES_ASM+= $(wildcard $(RES)/*.asm)

OBJ= $(RES_BMP:.bmp=.o)
OBJ+= $(RES_WAV:.wav=.o)
OBJ+= $(RES_RC:.rc=.o)
OBJ+= $(RES_ASM:.asm=.o)
OBJ+= $(RES_RAW:.raw=.o)
OBJ+= $(RES_PCM:.pcm=.o)
OBJ+= $(RES_TFD:.tfd=.o)
OBJ+= $(RES_TFC:.tfc=.o)
OBJ+= $(RES_S:.s=.o)
OBJ+= $(SRC_SZ80:.s80=.o)
OBJ+= $(SRC_S:.s=.o)
OBJ+= $(SRC_C:.c=.o)

INCS= -I$(LIBINCLUDE) -I$(INCLUDE) -I$(RES)
FLAGS= $(OPTION) -m68000 -Wall -O1 -fomit-frame-pointer -fno-builtin-memset -fno-builtin-memcpy $(INCS)
FLAGSZ80= -i$(LIBSRC) -i$(LIBINCLUDE) -i$(SRC) -i$(INCLUDE) -i$(RES)

all: out/$(ROM).bin

default: all

.PHONY: clean

clean:
	$(RM) -f $(OBJ) out.lst out/$(ROM).nm out/$(ROM).wch out/$(ROM).out out/$(ROM).out

cleanobj:
	$(RM) -f $(OBJ) out.lst out/$(ROM).nm out/$(ROM).out

out/$(ROM).bin: out/$(ROM).out
	$(NM) -n -S -t x out/$(ROM).out >out/$(ROM).nm
	$(NM2WCH) out/$(ROM).nm out/$(ROM).wch
	$(OBJCPY) -O binary out/$(ROM).out out/$(ROM).bin
	$(SIZEBND) out/$(ROM).bin -sizealign 131072   
	$(OBJCPY) -O binary out/$(ROM).out $(SCD_LOADER)/_filesystem/M_INIT.PRG
	$(SIZEBND) $(SCD_LOADER)/_filesystem/M_INIT.PRG -sizealign 131072    
	$(MKISOFS) -iso-level 1 -o $(SCD_LOADER)/filesystem.img -pad $(SCD_LOADER)/_filesystem
	$(TRIMFSIMG) $(SCD_LOADER)/filesystem.img $(SCD_LOADER)/filesystem.bin
	$(RM) -f $(SCD_LOADER)/filesystem.img
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/ip-us.asm, $(SCD_LOADER)/_boot/ip-us.bin
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/sp.asm, $(SCD_LOADER)/_boot/sp.bin
	$(ASM68K) /p /j $(SCD_LOADER)/ $(SCD_LOADER)/main-us.asm, out/240p_SegaCD_USA.iso
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/ip-eu.asm, $(SCD_LOADER)/_boot/ip-eu.bin
	$(ASM68K) /p /j $(SCD_LOADER)/ $(SCD_LOADER)/main-eu.asm, out/240p_SegaCD_EU.iso
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/ip-jp.asm, $(SCD_LOADER)/_boot/ip-jp.bin
	$(ASM68K) /p /j $(SCD_LOADER)/ $(SCD_LOADER)/main-jp.asm, out/240p_SegaCD_JP.iso
	$(RM) -f $(SCD_LOADER)/filesystem.bin

out/$(ROM).out: $(LIB)/segacd.o $(OBJ) $(LIB)/libgendev.a
	$(MKDIR) -p out
	$(CC) -T $(GDK)/mdcd.ld -nostdlib $(LIB)/segacd.o $(OBJ) $(LIB)/libgendev.a $(LIB)/libgcc.a -o out/$(ROM).out    

%.asm: %.rc
	$(GENRES) $< $@

%.s: %.asm
	$(MACCER) -o $@ $<

%.s: %.bmp
	$(BINTOS) -bmp $<

%.raw: %.wav
	$(WAVTORAW) $<

%.tfc: %.tfd
	$(TFMCOM) $<

%.s: %.tfc
	$(BINTOS) -align 32768 $<

%.s: %.raw
	$(BINTOS) -align 256 -sizealign 256 $<

%.s: %.pcm
	$(BINTOS) -align 256 -sizealign 256 $<

%.o80: %.s80
	$(ASMZ80) $(FLAGSZ80) $< $@ out.lst

%.c: %.o80
	$(BINTOC) $<

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(FLAGS) -c $< -o $@

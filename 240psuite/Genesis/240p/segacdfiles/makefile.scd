BIN= $(GDK)/bin
LIB= $(GDK)/lib

LIBSRC= $(GDK)/src
LIBRES= $(GDK)/res
LIBINCLUDE= $(GDK)/inc

ROM = rom
SRC= src
RES= res
INCLUDE= inc

TRIMFSIMG = $(SCD_LOADER)/trimfsimage
MKISOFS = $(SCD_LOADER)/mkisofs
ASM68K = $(SCD_LOADER)/ASM68K

SHELL= $(BIN)/sh
RM= $(BIN)/rm
CP= $(BIN)/cp
AR= $(BIN)/ar
CC= $(BIN)/gcc
LD= $(BIN)/ld
NM= $(BIN)/nm
ECHO= echo
OBJCPY= $(BIN)/objcopy
ASMZ80= $(BIN)/sjasm
MACCER= $(BIN)/mac68k
SIZEBND= $(BIN)/sizebnd
BINTOS= $(BIN)/bintos
GENRES= $(BIN)/genres
RESCOMP= $(BIN)/rescomp
MKDIR= $(BIN)/mkdir

SRC_C= $(wildcard *.c)
SRC_C+= $(wildcard $(SRC)/*.c)
SRC_S= $(wildcard *.s)
SRC_S+= $(wildcard $(SRC)/*.s)
SRC_ASM= $(wildcard *.asm)
SRC_ASM+= $(wildcard $(SRC)/*.asm)
SRC_S80= $(wildcard *.s80)
SRC_S80+= $(wildcard $(SRC)/*.s80)

RES_C= $(wildcard $(RES)/*.c)
RES_S= $(wildcard $(RES)/*.s)
RES_RC= $(wildcard *.rc)
RES_RC+= $(wildcard $(RES)/*.rc)
RES_RES= $(wildcard *.res)
RES_RES+= $(wildcard $(RES)/*.res)

OBJ= $(RES_RES:.res=.o)
OBJ+= $(RES_RC:.rc=.o)
OBJ+= $(RES_S:.s=.o)
OBJ+= $(RES_C:.c=.o)
OBJ+= $(SRC_S80:.s80=.o)
OBJ+= $(SRC_ASM:.asm=.o)
OBJ+= $(SRC_S:.s=.o)
OBJ+= $(SRC_C:.c=.o)

OBJS = $(addprefix outscd/, $(OBJ))

INCS= -I$(INCLUDE) -I$(SRC) -I$(RES) -I$(LIBINCLUDE) -I$(LIBRES)
DEFAULT_FLAGS= -DSEGACD -m68000 -Wall -fno-builtin $(INCS) -B$(BIN)
FLAGSZ80= -i$(SRC) -i$(INCLUDE) -i$(RES) -i$(LIBSRC) -i$(LIBINCLUDE)


#release: FLAGS= $(DEFAULT_FLAGS) -O3 -fno-web -fno-gcse -fno-unit-at-a-time -fomit-frame-pointer
release: FLAGS= $(DEFAULT_FLAGS) -O1 -fomit-frame-pointer
release: outscd/rom.bin

debug: FLAGS= $(DEFAULT_FLAGS) -O1 -ggdb -DDEBUG=1
debug: outscd/rom.bin outscd/rom.out outscd/symbol.txt


all: release
default: release

.PHONY: clean


clean:
	$(RM) -f $(OBJS) out.lst outscd/cmd_ outscd/segacd.o outscd/rom_head.bin outscd/rom_head.o outscd/rom.nm outscd/rom.wch outscd/rom.out outscd/rom.bin

cleanobj:
	$(RM) -f $(OBJS) outscd/segacd.o outscd/rom_head.bin outscd/rom_head.o outscd/rom.out

outscd/rom.bin: outscd/rom.out
	$(NM) outscd/$(ROM).out > outscd/symbol.txt
	$(OBJCPY) -O binary outscd/$(ROM).out outscd/$(ROM).bin
	$(SIZEBND) outscd/$(ROM).bin -sizealign 131072
	$(OBJCPY) -O binary outscd/$(ROM).out $(SCD_LOADER)/_filesystem/M_INIT.PRG
	#$(SIZEBND) $(SCD_LOADER)/_filesystem/M_INIT.PRG -sizealign 131072 -nullfill 0xAA
	$(MKISOFS) -iso-level 1 -o $(SCD_LOADER)/filesystem.img -pad $(SCD_LOADER)/_filesystem
	$(TRIMFSIMG) $(SCD_LOADER)/filesystem.img $(SCD_LOADER)/filesystem.bin
	$(RM) -f $(SCD_LOADER)/filesystem.img
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/ip-us.asm, $(SCD_LOADER)/_boot/ip-us.bin
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/sp.asm, $(SCD_LOADER)/_boot/sp.bin
	$(ASM68K) /p /j $(SCD_LOADER)/ $(SCD_LOADER)/main-us.asm, outscd/240p_SegaCD_USA.iso
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/ip-eu.asm, $(SCD_LOADER)/_boot/ip-eu.bin
	$(ASM68K) /p /j $(SCD_LOADER)/ $(SCD_LOADER)/main-eu.asm, outscd/240p_MegaCD_EU.iso
	$(ASM68K) /p /j $(SCD_LOADER)/_boot $(SCD_LOADER)/_boot/ip-jp.asm, $(SCD_LOADER)/_boot/ip-jp.bin
	$(ASM68K) /p /j $(SCD_LOADER)/ $(SCD_LOADER)/main-jp.asm, outscd/240p_MegaCD_JP.iso
	$(RM) -f $(SCD_LOADER)/filesystem.bin

outscd/symbol.txt: outscd/rom.out
	$(NM) outscd/$(ROM).out > outscd/symbol.txt

outscd/rom.out: outscd/segacd.o outscd/cmd_ $(LIB)/libmd.a
	$(CC) -B$(BIN) -n -T $(GDK)/mdcd.ld -nostdlib outscd/segacd.o @outscd/cmd_ $(LIB)/libmd.a $(LIB)/libgcc.a -o outscd/rom.out
	$(RM) outscd/cmd_

outscd/cmd_: $(OBJS)
	$(ECHO) "$(OBJS)" > outscd/cmd_

outscd/segacd.o: segacdfiles/segacd.s outscd/rom_head.bin
	$(MKDIR) -p outscd
	$(CC) $(FLAGS) -c segacdfiles/segacd.s -o $@

outscd/rom_head.bin: outscd/rom_head.o
	$(LD) -T $(GDK)/mdcd.ld -nostdlib --oformat binary -o $@ $<

outscd/rom_head.o: $(LIBSRC)/boot/rom_head.c
	$(MKDIR) -p outscd
	$(CC) $(FLAGS) -c $< -o $@


outscd/%.o: %.c
	$(MKDIR) -p outscd
	$(MKDIR) -p outscd/src
	$(MKDIR) -p outscd/res
	$(CC) $(FLAGS) -c $< -o $@

outscd/%.o: %.s
	$(MKDIR) -p outscd
	$(MKDIR) -p outscd/src
	$(MKDIR) -p outscd/res
	$(CC) $(FLAGS) -c $< -o $@

%.s: %.res
	$(RESCOMP) $< $@

%.asm: %.rc
	$(GENRES) $< $@

%.s: %.asm
	$(MACCER) -o $@ $<

%.o80: %.s80
	$(ASMZ80) $(FLAGSZ80) $< $@ out.lst

%.s: %.o80
	$(BINTOS) $<

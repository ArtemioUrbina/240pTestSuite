ROOTDIR = $(N64_INST)
GCCN64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CHKSUM64PATH = $(ROOTDIR)/bin/chksum64
MKDFSPATH = $(ROOTDIR)/bin/mkdfs
HEADERPATH = $(ROOTDIR)/mips64-elf/lib
N64TOOL = $(ROOTDIR)/bin/n64tool
MKSPRITEPATH = $(ROOTDIR)/bin/mksprite
UCON64 = $(ROOTDIR)/bin/ucon64
LOADER = $(ROOTDIR)/bin/loader64
HEADERNAME = header
LINK_FLAGS = -L$(ROOTDIR)/mips64-elf/lib -ldragon -lc -lm -ldragonsys -ln64memory -Tn64ld.x 
PROG_NAME = 240pSuite
CFLAGS = -DUSE_N64MEM -std=gnu99 -march=vr4300 -mtune=vr4300 -O2 -Wall -Werror -I$(ROOTDIR)/mips64-elf/include
ASFLAGS = -mtune=vr4300 -march=vr4300
CC = $(GCCN64PREFIX)gcc
AS = $(GCCN64PREFIX)as
LD = $(GCCN64PREFIX)ld
OBJCOPY = $(GCCN64PREFIX)objcopy
BITDEPTH = 16

$(PROG_NAME).v64: bitmaps $(PROG_NAME).elf drive.dfs
	$(OBJCOPY) $(PROG_NAME).elf $(PROG_NAME).bin -O binary
	rm -f $(PROG_NAME).v64
	$(N64TOOL) -b -l 2M -t "240p Test Suite" -h $(HEADERPATH)/$(HEADERNAME) -o $(PROG_NAME).v64 $(PROG_NAME).bin -s 1M drive.dfs
	$(CHKSUM64PATH) $(PROG_NAME).v64
	$(UCON64) --nbak --dint $(PROG_NAME).v64

$(PROG_NAME).elf : $(PROG_NAME).o utils.o image.o video.o tests.o
	$(LD) -o $(PROG_NAME).elf $(PROG_NAME).o utils.o image.o video.o tests.o $(LINK_FLAGS)

drive.dfs:
	$(MKDFSPATH) drive.dfs ./filesystem/

all: $(PROG_NAME).v64

clean:
	rm -f *.v64 *.elf *.o *.bin *.dfs filesystem/*.bin

.PHONY: bitmaps all

bitmaps : filesystem/back.bin filesystem/sd.bin filesystem/motoko.bin filesystem/shadow.bin\
    filesystem/buzzbomber.bin

filesystem/back.bin: graphics/back.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITEPATH) $(BITDEPTH) $< $@

filesystem/sd.bin: graphics/sd.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITEPATH) $(BITDEPTH) $< $@

filesystem/motoko.bin: graphics/motoko.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITEPATH) $(BITDEPTH) 10 15 $< $@

filesystem/shadow.bin: graphics/shadow.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITEPATH) $(BITDEPTH) $< $@

filesystem/buzzbomber.bin: graphics/buzzbomber.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITEPATH) $(BITDEPTH) $< $@

run:
	$(LOADER)  $(PROG_NAME).v64
ROOTDIR = $(N64_INST)

HEADERDIR = $(ROOTDIR)/headers
HEADER = $(HEADERDIR)/header

TOOLDIR = $(ROOTDIR)/bin
CHKSUM64 = $(TOOLDIR)/chksum64
MKDFS = $(TOOLDIR)/mkdfs
N64TOOL = $(TOOLDIR)/n64tool
MKSPRITE =  $(TOOLDIR)/mksprite
UCON64 = $(TOOLDIR)/ucon64
LOADER = $(TOOLDIR)/loader64

LDSCRIPTSDIR = $(ROOTDIR)/ldscripts
LINK_FLAGS = -T$(LDSCRIPTSDIR)/n64.ld -Map map.txt -nostdlib

LIBPATH = -L$(ROOTDIR)/lib -L$(ROOTDIR)/lib/gcc/mips64-elf/4.4.0 -L$(ROOTDIR)/mips64-elf/lib
INCPATH = -I$(CURDIR)/include -I$(ROOTDIR)/include -I$(ROOTDIR)/mips64-elf/include
CCFLAGS = -DUSE_N64MEM -std=gnu99 -march=vr4300 -mtune=vr4300 -G 0 -O2 -Wall -Werror -c $(INCPATH)
ASFLAGS = -mtune=vr4300 -march=vr4300

N64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CC = $(N64PREFIX)gcc
AS = $(N64PREFIX)as
LD = $(N64PREFIX)ld
OBJCOPY = $(N64PREFIX)objcopy

PROG_NAME = 240pSuite
LIBS = -ldragon -lc -lm -ldragonsys -ln64memory 
BITDEPTH = 16

OBJS = $(PROG_NAME).o utils.o image.o video.o tests.o patterns.o controller.o

$(PROG_NAME).v64: bitmaps $(PROG_NAME).elf drive.dfs
	$(OBJCOPY) $(PROG_NAME).elf $(PROG_NAME).bin -O binary
	rm -f $(PROG_NAME).v64
	$(N64TOOL) -b -l 2M -t "240p Test Suite" -h $(HEADER) -o $(PROG_NAME).v64 $(PROG_NAME).bin -s 1M drive.dfs
	$(CHKSUM64) $(PROG_NAME).v64
	$(UCON64) --nbak --dint $(PROG_NAME).v64

$(PROG_NAME).elf : $(OBJS)
	$(LD) -o $(PROG_NAME).elf $(OBJS) $(LINK_FLAGS) $(LIBPATH) $(LIBS)

drive.dfs: bitmaps
	$(MKDFS) drive.dfs ./filesystem/

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@ 

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

all: $(PROG_NAME).v64

clean:
	rm -f *.v64 *.elf *.o *.bin *.dfs filesystem/*.bin map.txt

.PHONY: bitmaps all

bitmaps : filesystem/back.bin filesystem/sd.bin filesystem/motoko.bin filesystem/shadow.bin\
    filesystem/buzzbomber.bin filesystem/checkpos.bin filesystem/checkneg.bin filesystem/pluge.bin\
	filesystem/color.bin filesystem/color_grid.bin filesystem/colorlow.bin filesystem/colorhigh.bin\
    filesystem/grid.bin filesystem/grid480.bin 

filesystem/back.bin: graphics/back.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/sd.bin: graphics/sd.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/motoko.bin: graphics/motoko.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@
	
filesystem/pluge.bin: graphics/pluge.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@
	
filesystem/color.bin: graphics/color.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) 32 $< $@

filesystem/grid.bin: graphics/grid.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/grid480.bin: graphics/grid480.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/color_grid.bin: graphics/color_grid.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) 32 $< $@

filesystem/colorlow.bin: graphics/colorlow.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) 32 $< $@
	
filesystem/colorhigh.bin: graphics/colorhigh.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) 32 $< $@

filesystem/shadow.bin: graphics/shadow.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/buzzbomber.bin: graphics/buzzbomber.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/checkpos.bin: graphics/checkpos.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

filesystem/checkneg.bin: graphics/checkneg.png
	@echo convert PNG ... $(notdir $@)
	$(MKSPRITE) $(BITDEPTH) $< $@

run:
	$(LOADER)  $(PROG_NAME).v64
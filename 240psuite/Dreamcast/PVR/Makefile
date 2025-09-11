TARGET = main
OPT = -O3 -Wfatal-errors -Wall -Wextra
KOS_ROMDISK_DIR = romdisk

#Full release compile
all: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST
all: LDFLAGS = -lfftw3 -ljpeg
all: $(TARGET).bin

# ===========================================================================

#DCLOAD debug compile (exit to dcloader)
dcload: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DDCLOAD -DBENCHMARK -DDEBUG_FFT -DTEST_VIDEO
dcload: LDFLAGS = -lfftw3 -ljpeg
dcload: $(TARGET).bin

#DCLOAD debug compile (release flags)
dcloadrel: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST
dcloadrel: LDFLAGS = -lfftw3 -ljpeg
dcloadrel: $(TARGET).bin

#DCLOAD debug compile (exit to dcloader) without FFTW
nofftserial: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_FFTW -DDCLOAD -DBENCHMARK  -DTEST_VIDEO
nofftserial: LDFLAGS = -ljpeg
nofftserial: $(TARGET).bin

#DCLOAD debug compile (exit to dcloader) without DE
nodeserial: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_DREAMEYE_DISP -DDCLOAD -DBENCHMARK  -DTEST_VIDEO
nodeserial: $(TARGET).bin

#DCLOAD debug compile (exit to dcloader) without DE or FFTW
nofftordeserial: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_FFTW -DNO_DREAMEYE_DISP -DDCLOAD -DBENCHMARK  -DTEST_VIDEO
nofftordeserial: $(TARGET).bin

# ===========================================================================

#Release without FFTW
nofft: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_FFTW
nofft: LDFLAGS = -ljpeg
nofft: $(TARGET).bin

#Release without DreamEye display
node: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_DREAMEYE_DISP
node: LDFLAGS = -lfftw3
node: $(TARGET).bin

#Release without FFTW or DreamEye display
nofftorde: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_FFTW -DNO_DREAMEYE_DISP
nofftorde: $(TARGET).bin

KOS_CFLAGS+= $(CFLAGS) 

include $(KOS_BASE)/Makefile.rules

.SRCS	=\
            main.c \
            image.c \
            font.c \
            help.c \
            tests.c \
            patterns.c \
            vmodes.c \
            vmu_print.c \
            controller.c \
            hardware.c \
			sound.c \
            vmufs.c \
            vmu.c \
            menu.c \

HEADERS	=\
            image.h \
            font.h \
            help.h \
            tests.h \
            patterns.h \
            vmodes.h \
            vmu_print.h \
            controller.h \
            hardware.h \
			sound.h \
            vmufs.h \
            vmu.h \
            menu.h \

OBJS = $(.SRCS:.c=.o)

kmg:
	rm -f romdisk/*.kmg.gz romdisk/480/*.kmg.gz
	kmgenc -a1 pngs/*.png
	kmgenc -a1 pngs/480/*.png
	mv pngs/*.kmg romdisk
	mv pngs/480/*.kmg romdisk/480
	gzip romdisk/*.kmg
	gzip romdisk/480/*.kmg
	rm -f romdisk.o romdisk.img

ip:
	makeip -l ../IP/logo.png ../IP/ip.txt ../IP/IP.BIN -f

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin romdisk.o romdisk.img
	rm -f 240pSuite.mdf 240pSuite.mds 240pSuite.cdi 240p-DC-PVR*.zip 
	rm -f 240p.iso 1ST_READ.BIN

$(OBJS): $(HEADERS) romdisk.o

$(TARGET).elf: $(OBJS) romdisk.o 
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) romdisk.o -o $(TARGET).elf $(KOS_START) \
	$(OBJS) -lz -lkmg -lkosutils $(LDFLAGS) -lm $(OBJEXTRA) $(KOS_LIBS) 

$(TARGET).bin: $(TARGET).elf
	$(KOS_OBJCOPY) -R .stack -O binary $(TARGET).elf $(TARGET).bin
	$(KOS_STRIP) $(TARGET).elf

run: clean dcload
	$(KOS_LOADER) $(TARGET).bin 
	
runrel: clean dcloadrel
	$(KOS_LOADER) $(TARGET).bin -n

cdi-int: all ip
	scramble $(TARGET).bin 1ST_READ.BIN
	mkisofs -C 0,11702 -V 240pSuiteDC -G ../IP/IP.BIN -J -r -l -o 240p.iso 0GDTEX.pvr 1ST_READ.BIN mdf
	cdi4dc 240p.iso 240pSuite.cdi

mdf-int: all ip
	scramble $(TARGET).bin 1ST_READ.BIN
	mkisofs -C 0,11702 -V 240pSuiteDC -G ../IP/IP.BIN -J -r -l -o 240p.iso 0GDTEX.pvr 1ST_READ.BIN mdf
	mds4dc -a 240pSuite.mdf 240p.iso

cdi: clean cdi-int
	rm 240p.iso 1ST_READ.BIN 

mdf: clean mdf-int
	rm 240p.iso 1ST_READ.BIN 

cds: clean cdi-int mdf-int
	rm 240p.iso 1ST_READ.BIN 

zip: cdi
	zip -9 240p-DC-PVR-2.x.zip README.TXT Changelog.txt 240pSuite.cdi
	rm 240pSuite.cdi


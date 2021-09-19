TARGET = main
OPT = -O3

#Full release compile
all: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST
all: LDFLAGS = -lfftw3
all: $(TARGET).bin

#Serial debug compile (exit to dcloader)
serial: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DSERIAL -DBENCHMARK -DDEBUG_FFT
serial: LDFLAGS = -lfftw3
serial: $(TARGET).bin

#Release without FFTW
nofft: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_FFTW
nofft: $(TARGET).bin

#Serial debug compile (exit to dcloader) without FFTW
nofftserial: CFLAGS = $(OPT) -fomit-frame-pointer -DDREAMCAST -DNO_FFTW -DSERIAL -DBENCHMARK -DDEBUG_FFT
nofftserial: $(TARGET).bin

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

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin romdisk.o romdisk.img 240pPVR.cdi 240p-DC-PVR*.zip

$(OBJS): $(HEADERS) romdisk.o

$(TARGET).elf: $(OBJS) romdisk.o 
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) romdisk.o -o $(TARGET).elf $(KOS_START) \
	$(OBJS) -lz -lkmg -lkosutils $(LDFLAGS) -lm $(OBJEXTRA) $(KOS_LIBS) 

$(TARGET).bin: $(TARGET).elf
	$(KOS_OBJCOPY) -R .stack -O binary $(TARGET).elf $(TARGET).bin
#	strip $(TARGET).elf

run: $(TARGET).bin
	$(KOS_LOADER) $(TARGET).bin 

cdi: $(TARGET).bin
	scramble $(TARGET).bin 1ST_READ.BIN
	mkisofs -C 0,11702 -V 240pSuite -G ../IP/IP.BIN -joliet -rock -l -o 240p.iso 0GDTEX.pvr 1ST_READ.BIN
	cdi4dc 240p.iso 240pSuite.cdi
	rm 240p.iso 1ST_READ.BIN

zip: $(TARGET).bin
	scramble $(TARGET).bin 1ST_READ.BIN
	mkisofs -C 0,11702 -V 240pSuite -G ../IP/IP.BIN -joliet -rock -l -o 240p.iso 0GDTEX.pvr 1ST_READ.BIN
	cdi4dc 240p.iso 240pSuite.cdi 
	zip 240p-DC-PVR-1.x.zip  README.TXT Changelog.txt 240pSuite.cdi
	rm 240p.iso 1ST_READ.BIN 240pSuite.cdi


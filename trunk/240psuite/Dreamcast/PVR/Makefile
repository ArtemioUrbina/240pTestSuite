TARGET = main

OPTFLAGS=-O3 -fomit-frame-pointer -DDREAMCAST -DUSE_FFTW -DSERIAL

KOS_CFLAGS+= -I$(KOS_BASE)/../kos-ports/include/SDL-1.2.9  $(OPTFLAGS) 

all: $(TARGET).bin

include $(KOS_BASE)/Makefile.rules

.SRCS	=	main.c \
            image.c \
            font.c \
            help.c \
            tests.c \
            patterns.c \
            vmodes.c \
            vmu_print.c \
            controller.c \
            vmu.c \
            settings.c \

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
	mv romdisk/.svn svn
	mv romdisk/480/.svn 480svn
	mv romdisk/help/.svn helpsvn
	$(KOS_GENROMFS) -f romdisk.img -d romdisk -v
	mv svn romdisk/.svn 
	mv 480svn romdisk/480/.svn 
	mv helpsvn romdisk/help/.svn 

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin romdisk.o romdisk.img 240pPVR.cdi 240p-DC-PVR*.zip

$(OBJS): $(HEADERS) romdisk.o

$(TARGET).elf: $(OBJS) romdisk.o 
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) romdisk.o -o $(TARGET).elf $(KOS_START) \
	$(OBJS) -lz -lfftw3 -lm -lkmg -lkosutils  $(OBJEXTRA) $(KOS_LIBS) 

$(TARGET).bin: $(TARGET).elf
	$(KOS_OBJCOPY) -R .stack -O binary $(TARGET).elf $(TARGET).bin
#	strip $(TARGET).elf

run: $(TARGET).bin
	$(KOS_LOADER) $(TARGET).bin

cdi: $(TARGET).bin
	scramble $(TARGET).bin 1ST_READ.BIN
	mkisofs -C 0,11702 -V 240pPVR -G ../IP/IP.BIN -joliet -rock -l -o 240p.iso 1ST_READ.BIN
	cdi4dc 240p.iso 240pPVR.cdi
	rm 240p.iso 1ST_READ.BIN

zip: $(TARGET).bin
	scramble $(TARGET).bin 1ST_READ.BIN
	mkisofs -C 0,11702 -V 240pPVR -G ../IP/IP.BIN -joliet -rock -l -o 240p.iso 1ST_READ.BIN
	cdi4dc 240p.iso 240pPVR.cdi
	zip 240p-DC-PVR-1.x.zip  README.TXT 240pPVR.cdi
	rm 240p.iso 1ST_READ.BIN 240pPVR.cdi


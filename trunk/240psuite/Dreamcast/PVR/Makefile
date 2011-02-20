TARGET = main

OPTFLAGS=-O3 -fomit-frame-pointer -DDREAMCAST -DSERIAL

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
            vmu.c \

OBJS = $(.SRCS:.c=.o)

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
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin romdisk.o romdisk.img 240pPVR.cdi

$(OBJS): $(HEADERS) romdisk.o

$(TARGET).elf: $(OBJS) romdisk.o 
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) romdisk.o -o $(TARGET).elf $(KOS_START) \
	$(OBJS) -lpng  -lz -lm -lkosutils  $(OBJEXTRA) $(KOS_LIBS)

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



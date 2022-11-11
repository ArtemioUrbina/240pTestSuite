# Names of the compiler and friends
APP_BASE = /opt/human68k/bin/human68k
AS 		= $(APP_BASE)-as
CC 		= $(APP_BASE)-gcc
LD 		= $(APP_BASE)-ld
OBJCOPY	= $(APP_BASE)-objcopy
STRIP 	= $(APP_BASE)-strip

# Where to copy the output exe files to
DEST	= release

# libraries and paths
LIBS	 	= -ldos -lm
GFXLIBS		= $(LIBS)
TEXTLIBS	= $(LIBS) 
INCLUDES 	= -I.

# Compiler flags
ASM_FLAGS 	= -m68000 -mtune=68000 --register-prefx-optional
LDFLAGS 	=
CFLAGS 		= -m68000 -mtune=68000 -std=c99 -fomit-frame-pointer -Wall -Wno-unused-function -Wno-unused-variable -O3
LDSCRIPT 	=
OCFLAGS     = -O xfile

# What our application is named
TARGET			= mdf
EXE				= $(TARGET).X

all: $(EXE)

# The main application
OBJFILES = build/main.o build/ym2151.o build/mdfourier.o build/MSM6258.o build/key.o build/crc.o

$(EXE):  $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) $(LIBS) -o bin/$(TARGET)
	$(OBJCOPY) $(OCFLAGS) bin/$(TARGET) bin/$(EXE)
	cp -v bin/$(EXE) $(DEST) 
	
################################
#
# Assembly stuff
#
################################

################################
#
# Main code
#
################################
build/main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o build/main.o

build/ym2151.o: ym2151.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o build/ym2151.o

build/mdfourier.o: mdfourier.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o build/mdfourier.o
	
build/MSM6258.o: MSM6258.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o build/MSM6258.o
	
build/key.o: key.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o build/key.o

build/crc.o: crc.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o build/crc.o

###############################
#
# Clean up
#
###############################
clean:
	rm -f build/*.o bin/$(EXE) bin/$(TARGET)

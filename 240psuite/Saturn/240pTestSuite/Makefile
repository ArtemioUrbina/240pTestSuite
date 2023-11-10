ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/build.pre.mk

BUILTIN_ASSETS+= \
	assets/BOOTLOGO.BG;asset_bootlogo_bg \
	assets/BACK2.BG;asset_back2_bg

SH_PROGRAM:= 240pSuite
SH_SRCS:= \
    background.c \
    fs.c \
    font.c \
    image.c \
    video.c \
    control.c \
    ire.c \
    pattern_colorbars.c \
    pattern_colorbleed.c \
    pattern_checkerboard.c \
    pattern_grid.c \
    pattern_smpte.c \
	libsvin/svin_background.c \
	libsvin/svin_debug.c \
	libsvin/svin.c \
	240pSuite.c
	
SH_LIBRARIES:= tga bcl
SH_CFLAGS+= -O2 -I. -Ilibsvin -Ilibsvin/mcufont -save-temps

IP_VERSION:= V0.001
IP_RELEASE_DATE:= 20180214
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= 240pSuite
IP_MASTER_STACK_ADDR:= 0x060C4000
IP_SLAVE_STACK_ADDR:= 0x06001000
IP_1ST_READ_ADDR:= 0x06004000
IP_1ST_READ_SIZE:= 0

M68K_PROGRAM:=
M68K_OBJECTS:=

include $(YAUL_INSTALL_ROOT)/share/build.post.iso-cue.mk
#include $(YAUL_INSTALL_ROOT)/share/build.post.ss.mk

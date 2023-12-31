ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/build.pre.mk

# Required for library usage
include $(YAUL_INSTALL_ROOT)/share/build.tga.mk
include $(YAUL_INSTALL_ROOT)/share/build.bcl.mk

# Each asset follows the format: <path>;<symbol>. Duplicates are removed
BUILTIN_ASSETS+= \
	assets/BOOTLOGO.BG;asset_bootlogo_bg \
	assets/BACK2.BG;asset_back2_bg \
	assets/MASCOT.BG;asset_mascot_bg

SH_PROGRAM:= 240pSuite
SH_SRCS:= \
    background.c \
    fs.c \
    font.c \
    image.c \
    video.c \
    control.c \
    ire.c \
    pattern_100ire.c \
    pattern_colorbars.c \
    pattern_colorbars_grayscale.c \
    pattern_colorbleed.c \
    pattern_ebu.c \
    pattern_grayramp.c \
    pattern_grid.c \
    pattern_monoscope.c \
    pattern_pluge.c \
    pattern_smpte.c \
    pattern_whitescreen.c \
    videotest_checkerboard.c \
    window_credits.c \
	libsvin/svin_background.c \
	libsvin/svin.c \
	240pSuite.c

SH_CFLAGS+= -Os -I. $(TGA_CFLAGS) $(BCL_CFLAGS) -Ilibsvin -save-temps
SH_LDFLAGS+= $(TGA_LDFLAGS) $(BCL_LDFLAGS)

IP_VERSION:= V0.001
IP_RELEASE_DATE:= 20180214
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= 240pSuite
IP_MASTER_STACK_ADDR:= 0x060C4000
IP_SLAVE_STACK_ADDR:= 0x06001000
IP_1ST_READ_ADDR:= 0x06004000
IP_1ST_READ_SIZE:= 0

include $(YAUL_INSTALL_ROOT)/share/build.post.iso-cue.mk
#include $(YAUL_INSTALL_ROOT)/share/build.post.ss.mk

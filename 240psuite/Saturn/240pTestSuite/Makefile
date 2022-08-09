ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/pre.common.mk

SH_PROGRAM:= 240pSuite
SH_SRCS:= \
    fs.c \
    font.c \
    image.c \
    video.c \
    control.c \
	libsvin/mcufont/mf_encoding.c \
    libsvin/mcufont/mf_font.c \
    libsvin/mcufont/mf_justify.c \
    libsvin/mcufont/mf_kerning.c \
    libsvin/mcufont/mf_rlefont.c \
    libsvin/mcufont/mf_bwfont.c \
    libsvin/mcufont/mf_scaledfont.c \
    libsvin/mcufont/mf_wordwrap.c \
	libsvin/svin_cd_access.c \
	libsvin/svin_background.c \
	libsvin/svin_filelist.c \
	libsvin/svin_menu.c \
	libsvin/svin_text.c \
	libsvin/svin_textbox.c \
	libsvin/svin_sprite.c \
	libsvin/svin_script.c \
	libsvin/svin_debug.c \
	libsvin/svin_alloc.c \
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

M68K_PROGRAM:=
M68K_OBJECTS:=

include $(YAUL_INSTALL_ROOT)/share/post.common.mk

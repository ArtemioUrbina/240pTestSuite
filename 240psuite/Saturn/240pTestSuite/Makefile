ifeq ($(strip $(INSTALL_ROOT)),)
  $(error Undefined INSTALL_ROOT (install root directory))
endif

include $(INSTALL_ROOT)/share/pre.common.mk

SH_PROGRAM:= 240pSuite
SH_OBJECTS:= \
    fs.o \
    font.o \
    image.o \
    video.o \
    control.o \
    root.romdisk.o \
	240pSuite.o
SH_LIBRARIES:= tga

IP_VERSION:= V0.001
IP_RELEASE_DATE:= 20180214
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= 240pSuite
IP_MASTER_STACK_ADDR:= 0x06100000
IP_SLAVE_STACK_ADDR:= 0x00000000
IP_1ST_READ_ADDR:= 0x06004000

include $(INSTALL_ROOT)/share/post.common.mk

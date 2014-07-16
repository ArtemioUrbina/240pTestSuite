# path to snesdev root directory (for emulators, devkitsnes, libsnes)
export DEVKITSNES := /c/snesdev/

# path to devkitsnes root directory for compiler
export DEVKIT65XX := /c/snesdev/devkitsnes

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PVSNESLIB)
LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
 
#---------------------------------------------------------------------------------

export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

GTITLE 		:= -ht"$(TARGET)"
 
.PHONY: bitmaps all

#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc

clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).sfc $(TARGET).sym *.pic *.map *.pal

#---------------------------------------------------------------------------------
font.pic: font.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -pc16 -pe0 -mR! -m! -p! $<

fontback.pic: fontback.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -pc16 -pe0 -mR! -m! -p! $<

#----------------------------------SPRITES-----------------------------------------

gillian.pic: gillian.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs16 -pc16 -po16 -n $<		

sprites.pic: sprites.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs32 -pc16 -po16 -n $<		
	
numbers.pic: numbers.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs32 -pc16 -po16 -n $<		
	
#----------------------------------END SPRITES-------------------------------------

back.pic: back.bmp
	@echo convert bitmap to palette 1... $(notdir $@)
	$(GFXCONV) -pR! -pr -pc16 -po16 -n -gs8 -pe1 -fbmp -m $<
	
motoko.pic: motoko.bmp
	@echo convert bitmap in 256 colors... $(notdir $@)
	$(GFXCONV) -pR! -pc256 -n -gs8 -pe0 -fbmp -m $<
	
color.pic: color.bmp
	@echo convert bitmap in 256 colors wihout palette rounding... $(notdir $@)
	$(GFXCONV) -pR! -pc128 -po128 -n -gs8 -pe0 -fbmp -m $<
	
grayramp.pic: grayramp.bmp
	@echo convert bitmap in 256 colors wihout palette rounding... $(notdir $@)
	$(GFXCONV) -pR! -pc128 -po128 -n -gs8 -pe0 -fbmp -m $<
	
fullscreen256.pic: fullscreen256.bmp
	@echo convert font 256 with no color reduction ... $(notdir $@)
	$(GFXCONV) -pc256 -po16 -n -gs8 -pe0 -fbmp -m $<	

100IRE.pic: 100IRE.bmp
	@echo convert font 256 with no color reduction ... $(notdir $@)
	$(GFXCONV) -pc256 -n -gs8 -pe0 -fbmp -m $<
	
sonicback.pic: sonicback.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc256 -n -gs8 -pe0 -fbmp -m $<

sonicfloor.pic: sonicfloor.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe2 -fbmp -mp -m $<

SMPTECB75.pic: SMPTECB75.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
SMPTECB100.pic: SMPTECB100.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<

pluge.pic: pluge.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
601701cb.pic: 601701cb.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
sharpness.pic: sharpness.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
bitmaps : grid.pic font.pic back.pic gillian.pic grid240.pic motoko.pic SMPTECB75.pic\
	SMPTECB100.pic color.pic circles.pic pluge.pic grayramp.pic colorbleed.pic\
	colorbleedchk.pic 601701cb.pic sharpness.pic fullscreen.pic fullscreen256.pic\
	hstripes.pic vstripes.pic fontback.pic check.pic sprites.pic 100IRE.pic\
	numbers.pic lagtest.pic sonicback.pic sonicfloor.pic
	
#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)

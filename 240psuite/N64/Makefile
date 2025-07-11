PROJECT_NAME = 240pSuite

.DEFAULT_GOAL := all

ASSETS_DIR = assets
FILESYSTEM_DIR = filesystem
BUILD_DIR = build
OUTPUT_DIR = output

BUILD_TIMESTAMP = "$(shell TZ='UTC' date "+%Y-%m-%d")"

include $(N64_INST)/include/n64.mk

# Add version information
N64_CFLAGS += -DVERSION_DATE=\"$(BUILD_TIMESTAMP)\"

# Remove the -fdiagnostics-color=always flag
N64_C_AND_CXX_FLAGS := $(filter-out -fdiagnostics-color=always, $(N64_C_AND_CXX_FLAGS))

T3D_INST ?= tiny3d
include $(T3D_INST)/t3d.mk

src = $(wildcard *.c)
ASSETS_PNG = $(wildcard $(ASSETS_DIR)/*.png)
ASSETS_PNG_CONV = $(addprefix $(FILESYSTEM_DIR)/,$(notdir $(ASSETS_PNG:%.png=%.sprite)))

ASSETS_WAV = $(wildcard $(ASSETS_DIR)/*.wav)
ASSETS_WAV_CONV = $(addprefix $(FILESYSTEM_DIR)/,$(notdir $(ASSETS_WAV:%.wav=%.wav64)))

ASSETS_GLTF = $(wildcard $(ASSETS_DIR)/*.glb)
ASSETS_GLTF_CONV = $(addprefix $(FILESYSTEM_DIR)/,$(notdir $(ASSETS_GLTF:%.glb=%.t3dm)))

ASSETS_TTF = $(wildcard $(ASSETS_DIR)/*.ttf)
ASSETS_TTF_CONV = $(addprefix $(FILESYSTEM_DIR)/,$(notdir $(ASSETS_TTF:%.ttf=%.font64)))

ASSETS_HELP = $(wildcard $(FILESYSTEM_DIR)/help/*.txt)

#AUDIOCONV_FLAGS ?= --wav-compress 0
#MKSPRITE_EXTRA_FLAGS ?=--verbose
MKSPRITE_EXTRA_FLAGS ?=

$(FILESYSTEM_DIR)/%.sprite: $(ASSETS_DIR)/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o $(FILESYSTEM_DIR) "$<"
	
$(FILESYSTEM_DIR)/%.wav64: $(ASSETS_DIR)/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -v -o $(FILESYSTEM_DIR) $<
	
$(FILESYSTEM_DIR)/%.t3dm: $(ASSETS_DIR)/%.glb
	@mkdir -p $(dir $@)
	@echo "    [T3D-MODEL] $@"
	@$(T3D_GLTF_TO_3D) "$<" $@
	@$(N64_BINDIR)/mkasset -o $(FILESYSTEM_DIR) $@
	
$(FILESYSTEM_DIR)/%.font64: $(ASSETS_DIR)/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) -o $(FILESYSTEM_DIR) "$<"

#AUDIO

#lossless
$(FILESYSTEM_DIR)/beep.wav64: AUDIOCONV_FLAGS=--wav-compress 0
$(FILESYSTEM_DIR)/mdfourier-dac-44100.wav64: AUDIOCONV_FLAGS=--wav-compress 0

#compression level 1
$(FILESYSTEM_DIR)/neko_loop.wav64: AUDIOCONV_FLAGS=--wav-compress 1
$(FILESYSTEM_DIR)/sound_test_left.wav64: AUDIOCONV_FLAGS=--wav-compress 1
$(FILESYSTEM_DIR)/sound_test_right.wav64: AUDIOCONV_FLAGS=--wav-compress 1
$(FILESYSTEM_DIR)/sound_test_center.wav64: AUDIOCONV_FLAGS=--wav-compress 1
$(FILESYSTEM_DIR)/sound_test_leftsurround.wav64: AUDIOCONV_FLAGS=--wav-compress 1
$(FILESYSTEM_DIR)/sound_test_rightsurround.wav64: AUDIOCONV_FLAGS=--wav-compress 1

#GRAPHICS

$(FILESYSTEM_DIR)/240pSuite-font.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8
$(FILESYSTEM_DIR)/mainbg.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/sd.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/sd_b1.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/sd_b2.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/monoscopeFBX.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/monoscope.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/pluge.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/plugePAL.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/PLUGEBorder.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/color.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA32
$(FILESYSTEM_DIR)/colorlow.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA32
$(FILESYSTEM_DIR)/colorhigh.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA32
$(FILESYSTEM_DIR)/color_grid.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA32
$(FILESYSTEM_DIR)/EBU75.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/SMPTE75.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/601701cb.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/colorbleed.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/colorbleedchk.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/grayramp.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
$(FILESYSTEM_DIR)/sharpness.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/grid.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/grid-480.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/grid-256.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/grid-512.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/donna.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA16
$(FILESYSTEM_DIR)/donna-480.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA16
$(FILESYSTEM_DIR)/sonicTop.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/sonicWater.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/sonicFall.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/sonicFloor.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
$(FILESYSTEM_DIR)/kiki.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/shadow.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/buzzbomber.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/buzzbomberShadow.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/libdragon.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/tiny3d.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/menu.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/menularge.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/message.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
$(FILESYSTEM_DIR)/qr.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/nish.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA16
$(FILESYSTEM_DIR)/check.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
$(FILESYSTEM_DIR)/stripes.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
$(FILESYSTEM_DIR)/stripes_v.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
$(FILESYSTEM_DIR)/small_grid.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
$(FILESYSTEM_DIR)/diagonal.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
$(FILESYSTEM_DIR)/circle.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/numbers.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 24,40 --format CI4
$(FILESYSTEM_DIR)/phase.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/convergence-01-grid.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/convergence-02-cross.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/convergence-03-dots.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/convergence-04-colors.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/convergence-05-colorsbl.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/rectangle_short.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/rectangle_long.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
$(FILESYSTEM_DIR)/controller.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8

$(FILESYSTEM_DIR)/BebasNeue-Regular.font64: MKFONT_FLAGS+=--size 41
$(FILESYSTEM_DIR)/BebasNeue-Small.font64: MKFONT_FLAGS+=--size 15

$(BUILD_DIR)/$(PROJECT_NAME).dfs: $(ASSETS_PNG_CONV) $(ASSETS_WAV_CONV) $(ASSETS_HELP) $(ASSETS_GLTF_CONV) $(ASSETS_TTF_CONV)
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(src:%.c=$(BUILD_DIR)/%.o)

$(PROJECT_NAME).z64: N64_ROM_TITLE = "240p Test Suite"
$(PROJECT_NAME).z64: N64_ROM_SAVETYPE = none 			# Supported savetypes: none eeprom4k eeprom16 sram256k sram768k sram1m flashram
$(PROJECT_NAME).z64: N64_ROM_REGIONFREE = true			# Set to true to allow booting on any console region
$(PROJECT_NAME).z64: N64_ROM_REGION = 'A'				# Set to a region code 'P' for PAL. 'A' all, 'E' NA
$(PROJECT_NAME).z64: $(BUILD_DIR)/$(PROJECT_NAME).dfs

$(@info $(shell mkdir -p ./$(OUTPUT_DIR) &> /dev/null))

$(OUTPUT_DIR)/$(PROJECT_NAME).z64: $(PROJECT_NAME).z64
	@mv $< $@

all: $(PROJECT_NAME).z64
	@mv $(PROJECT_NAME).z64 $(OUTPUT_DIR)/$(PROJECT_NAME).z64
#	 @$(N64_CHKSUM) $(OUTPUT_DIR)/$(PROJECT_NAME).z64 >/dev/null
.PHONY: all


clean: # NOTE: DO NOT CLEAN $(ASSETS_HELP), they are stored in the filesystem.
	@rm -rf $(ASSETS_PNG_CONV) $(ASSETS_WAV_CONV) $(ASSETS_GLTF_CONV) $(ASSETS_TTF_CONV)
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR) $(PROJECT_NAME).z64
.PHONY: clean

fsclean:
	@rm -rf $(BUILD_DIR)/*.dfs
	
bench: CFLAGS += -DDEBUG_BENCHMARK
bench: all

debug: CFLAGS += -g -ggdb
debug: all

build_lib:
	@rm -rf $(BUILD_DIR) *.z64
	make -C $(T3D_INST)
	make all

.PHONY: all clean

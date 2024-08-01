BUILD_DIR = build
include include/n64.mk

src = $(wildcard *.c)
ASSETS_PNG = $(wildcard assets/*.png)
ASSETS_PNG_CONV = $(addprefix filesystem/,$(notdir $(ASSETS_PNG:%.png=%.sprite)))

#commented to iterate on builds and testing via USB upload
#ASSETS_WAV = $(wildcard assets/*.wav)
#ASSETS_WAV_CONV = $(addprefix filesystem/,$(notdir $(ASSETS_WAV:%.wav=%.wav64)))

AUDIOCONV_FLAGS ?= --wav-compress 0
#MKSPRITE_EXTRA_FLAGS ?=--verbose
MKSPRITE_EXTRA_FLAGS ?=

all: 240pSuite.z64

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"
	
filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -v -o filesystem $<

filesystem/240pSuite-font.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8
filesystem/mainbg.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/sd.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/monoscopeFBX.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/monoscope.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/pluge.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/plugePAL.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/PLUGEBorder.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/color.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
filesystem/color_grid.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
filesystem/EBU75.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/SMPTE75.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/601701cb.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/colorbleed.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/colorbleedchk.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/grayramp.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
filesystem/sharpness.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/grid.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/grid-480.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/donna.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA16
filesystem/donna-480.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA16
filesystem/sonicTop.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/sonicWater.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/sonicFall.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/sonicFloor.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
filesystem/kiki.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/shadow.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/buzzbomber.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/buzzbomberShadow.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/libdragon.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/menu.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/message.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI8
filesystem/qr.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/nish.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format RGBA16
filesystem/check.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
filesystem/stripes.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
filesystem/stripes_v.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 8,8 --format CI4
filesystem/circle.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --format CI4
filesystem/numbers.sprite: MKSPRITE_FLAGS=$(MKSPRITE_EXTRA_FLAGS) --tiles 24,40 --format CI4

$(BUILD_DIR)/240pSuite.dfs: $(ASSETS_PNG_CONV) $(ASSETS_WAV_CONV)
$(BUILD_DIR)/240pSuite.elf: $(src:%.c=$(BUILD_DIR)/%.o)

240pSuite.z64: N64_ROM_TITLE="240pSuite"
240pSuite.z64: $(BUILD_DIR)/240pSuite.dfs 

clean:
	rm -rf $(BUILD_DIR) 240pSuite.z64 $(ASSETS_PNG_CONV) $(ASSETS_WAV_CONV)
	
fsclean:
	rm -rf $(BUILD_DIR)/*.dfs
	
bench: CFLAGS += -DDEBUG_BENCHMARK
bench: all

debug: CFLAGS += -g -ggdb
debug: all

.PHONY: all clean

BUILD_DIR = build
include include/n64.mk

src = $(wildcard *.c)
ASSETS_PNG = $(wildcard assets/*.png)
ASSETS_CONV = $(addprefix filesystem/,$(notdir $(ASSETS_PNG:%.png=%.sprite)))

AUDIOCONV_FLAGS ?=
MKSPRITE_FLAGS ?=

all: 240pSuite.z64

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

filesystem/240pSuite-font.sprite: MKSPRITE_FLAGS=--tiles 8,8
filesystem/240pSuite-font480.sprite: MKSPRITE_FLAGS=--tiles 16,16
filesystem/mainbg.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/sd.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/monoscopeFBX.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/monoscope.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/pluge.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/plugePAL.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/PLUGEBorder.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/color.sprite: MKSPRITE_FLAGS=--format CI8
filesystem/color_grid.sprite: MKSPRITE_FLAGS=--format CI8
filesystem/EBU75.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/SMPTE75.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/601701cb.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/colorbleed.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/colorbleedchk.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/grayramp.sprite: MKSPRITE_FLAGS=--format CI8
filesystem/sharpness.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/grid.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/grid-480.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/donna.sprite: MKSPRITE_FLAGS=--format RGBA16
filesystem/donna-480.sprite: MKSPRITE_FLAGS=--format RGBA16
filesystem/sonicTop.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/sonicWater.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/sonicFall.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/sonicFloor.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/shadow.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/libdragon.sprite: MKSPRITE_FLAGS=--format CI4
filesystem/menu.sprite: MKSPRITE_FLAGS=--format CI4

$(BUILD_DIR)/240pSuite.dfs: $(ASSETS_CONV)
$(BUILD_DIR)/240pSuite.elf: $(src:%.c=$(BUILD_DIR)/%.o)

240pSuite.z64: N64_ROM_TITLE="240pSuite"
240pSuite.z64: $(BUILD_DIR)/240pSuite.dfs 

clean:
	rm -rf $(BUILD_DIR) 240pSuite.z64 filesystem/*.sprite
	
fsclean:
	rm -rf $(BUILD_DIR)/*.dfs
	
bench: CFLAGS += -DDEBUG_BENCHMARK
bench: all

debug: CFLAGS += -g -ggdb
debug: all

.PHONY: all clean

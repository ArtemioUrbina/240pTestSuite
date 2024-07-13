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

filesystem/240pSuite-font.sprite: MKSPRITE_FLAGS=
filesystem/mainbg.sprite: MKSPRITE_FLAGS=
filesystem/grid.sprite: MKSPRITE_FLAGS=
filesystem/donna.sprite: MKSPRITE_FLAGS=

$(BUILD_DIR)/240pSuite.dfs: $(ASSETS_CONV)
$(BUILD_DIR)/240pSuite.elf: $(src:%.c=$(BUILD_DIR)/%.o)

240pSuite.z64: N64_ROM_TITLE="240pSuite"
240pSuite.z64: $(BUILD_DIR)/240pSuite.dfs 

clean:
	rm -rf $(BUILD_DIR) 240pSuite.z64 filesystem/*.sprite
	
debug: CFLAGS += -DDEBUG_BENCHMARK
debug: all

.PHONY: all clean

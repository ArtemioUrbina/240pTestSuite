#include "utils.h"

int fh = 8; // font height

void init_n64()
{
    init_interrupts();
	
	init_video();
	set_video();
	
    dfs_init(DFS_DEFAULT_LOCATION);
    controller_init();
    timer_init();
}

void DrawString(int x, int y, int r, int g, int b, char *str)
{
	uint32_t color = 0;

	color = graphics_make_color(r, g, b, 0xff);
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(disp, x, y, str);
}

void DrawStringS(int x, int y, int r, int g, int b, char *str)
{
	uint32_t color = 0;

	color = graphics_make_color(r, g, b, 0xff);
    graphics_set_color(0x00000000, 0x00000000);
	graphics_draw_text(disp, x+2, y+2, str);
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(disp, x, y, str);
}

int DetectExpansionPack()
{
	int available_memory_size = *(int *)(0x80000318);

    if(available_memory_size != 0x800000)
		return 0;
	return 1;
}
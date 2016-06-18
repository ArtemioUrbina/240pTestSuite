#include "utils.h"

void DrawStringS(display_context_t disp, int x, int y, int r, int g, int b, char *str)
{
		uint32_t color = graphics_make_color(r, g, b, 0xff);
        graphics_set_color( 0x00000000, 0x00000000 );
		graphics_draw_text( disp, x+2, y+2, str);
		graphics_set_color( color, 0x00000000 );
		graphics_draw_text( disp, x, y, str);
}

int isNTSC()
{
	int tv; 
	
	tv = *(int *) 0x80000300;
	return tv;
}
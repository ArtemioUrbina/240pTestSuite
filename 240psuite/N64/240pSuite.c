#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "image.h"
#include "utils.h"

int main(void)
{
	int sel = 1, redraw = 1;
	sprite_t *back = NULL, *sd = NULL;
	struct controller_data keys;

	init_n64();
	
    back = LoadImage("/back.bin");
    sd = LoadImage("/sd.bin");
	
    while(1) 
    {
		int x = 38, y = 62, r = 0xff, g = 0xff, b = 0xff, c = 1;
		
		GetDisplay();
		
		if(redraw)
		{
			SoftDrawImage(0, 0, back);
			SoftDrawImage(221, 86, sd);
		
			redraw++;
			if(redraw == current_buffers+1)
				redraw = 0;
		}
		
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Test Patterns >"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Manual Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Diagonal Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Sound Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Options"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); 
		
		if(isNTSC())
			DrawStringS(255, 216, 0xfa, 0xfa, 0xfa, "N64 NTSC");
		else
			DrawStringS(255, 216, 0xfa, 0xfa, 0xfa, "N64 PAL"); 

        display_show(disp);

        controller_scan();
        keys = get_keys_down();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
		
		if(sel > c)
			sel = 0;
		if(sel < 1)
			sel = 1;
			
		if(keys.c[0].A)
		{
			current_resolution++;
			if(current_resolution > RESOLUTION_512x480)
				current_resolution = RESOLUTION_320x240;
			display_close();
			set_video();
			redraw = 1;
		}
	}
}


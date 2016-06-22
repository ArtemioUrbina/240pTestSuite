#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "image.h"
#include "utils.h"
#include "tests.h"

int main(void)
{
	int sel = 1, redraw = 1;
	sprite_t *back = NULL, *sd = NULL;
	struct controller_data keys;
	char str[20];

	init_n64();
	
	ClearScreen();
	
    while(1) 
    {
		int x = 38, y = 62, r = 0xff, g = 0xff, b = 0xff, c = 1;
		
		GetDisplay();
		
		if(redraw)
		{
			if(!back)
				back = LoadImage("/back.bin");
			if(!sd)
				sd = LoadImage("/sd.bin");
	
			drawBackground(back);
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
			DrawStringS(254, 216, 0xfa, 0xfa, 0xfa, "N64 NTSC");
		else
			DrawStringS(254, 216, 0xfa, 0xfa, 0xfa, "N64 PAL"); 
		
		sprintf(str, "RAM %d MB", DetectRamSize());
		DrawStringS(254, 224, 0xfa, 0xfa, 0xfa, str);

        WaitVsync();

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
			FreeImage(&back);
			FreeImage(&sd);
			
			ClearScreen();
			
			switch(sel)
			{
				case 2:
					DropShadowTest();
					break;
				case 9:
					DrawCheckerboard();
					break;
				case 14:
					current_resolution++;
					if(current_resolution > RESOLUTION_512x480)
						current_resolution = RESOLUTION_320x240;
					set_video();
					break;
				default:
					break;
			}
			
			ClearScreen();
			redraw = 1;
		}
	}
	
	FreeImage(&back);
	FreeImage(&sd);
}


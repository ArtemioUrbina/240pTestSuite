#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "utils.h"

int main(void)
{
	int fp = 0, sel = 1, fh = 8;
	sprite_t *back = NULL, *sd = NULL;
	struct controller_data keys;

    init_interrupts();
	
    display_init( RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_OFF );
    dfs_init( DFS_DEFAULT_LOCATION );
    rdp_init();
    controller_init();
    timer_init();

    fp = dfs_open("/back.bin");
    back = malloc( dfs_size( fp ) );
    dfs_read( back, 1, dfs_size( fp ), fp );
    dfs_close( fp );
	
	fp = dfs_open("/sd.bin");
    sd = malloc( dfs_size( fp ) );
    dfs_read( sd, 1, dfs_size( fp ), fp );
    dfs_close( fp );
    
    while(1) 
    {
        static display_context_t disp = 0;
		int x = 38, y = 62, r = 0xff, g = 0xff, b = 0xff, c = 1;

        while( !(disp = display_lock()) );
    
		graphics_draw_sprite_trans( disp, 0, 0, back );
		
		graphics_draw_sprite_trans( disp, 221, 86, sd );
   
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Test Patterns >"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Lag Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Manual Lag Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Diagonal Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Sound Test"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Options"); y += fh; c++;
		DrawStringS(disp, x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); 
		
		if(isNTSC())
			DrawStringS(disp, 255, 216, 0xfa, 0xfa, 0xfa, "N64 NTSC");
		else
			DrawStringS(disp, 255, 216, 0xfa, 0xfa, 0xfa, "N64 PAL"); 

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
		}
	}
}


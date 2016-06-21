#include "tests.h"

void DropShadowTest()
{
	int redraw = 1, end = 0, draw = 1;
	int x = dW/2, y = dH/2;
	sprite_t *back = NULL, *shadow = NULL;
	struct controller_data keys;

	shadow = LoadImage("/buzzbomber.bin");
    while(!end)
    {	
		GetDisplay();
		
		if(redraw)
		{
			if(!back)
				back = LoadImage("/motoko.bin");
				
			if(!shadow)
				shadow = LoadImage("/buzzbomber.bin");
		
			SoftDrawImageSolid(0, 0, back);
			
			redraw++;
			if(redraw == current_buffers+1)
				redraw = 0;
		}	
		
		if(draw)
			SoftDrawImageSolid(0, 0, back);
		
		//rdp_start();
		SoftDrawImage(x, y, shadow);
		//rdp_end();
		
		WaitVsync();

        controller_scan();
		keys = get_keys_held();

		if(keys.c[0].up)
			y--;

		if(keys.c[0].down)
			y++;
		
		if(y > dH)
			y = 0;
		if(y < 0)
			y = 1;
			
		if(keys.c[0].left)
			x--;

		if(keys.c[0].right)
			x++;
			
		if(x > dW)
			x = 0;
		if(x < 0)
			x = 1;
		
		keys = get_keys_down();
		if(keys.c[0].B)
			end = 1;
		if(keys.c[0].A)
			draw = !draw;
	}
	FreeImage(&back);
	FreeImage(&shadow);
}

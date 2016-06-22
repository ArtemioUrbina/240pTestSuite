#include "tests.h"

void DropShadowTest()
{
	int redraw = 1, end = 0, show = 1;
	int x = 0, y = 0, xlast = 0, ylast = 0;
	sprite_t *back = NULL, *shadow = NULL;
	struct controller_data keys;
	
    while(!end)
    {	
		GetDisplay();
		
		if(redraw)
		{
			if(!back)
				back = LoadImage("/motoko.bin");
			if(!shadow)
				shadow = LoadImage("/shadow.bin");
	
			drawBackground(back);
			
			redraw++;
			if(redraw == current_buffers+1)
				redraw = 0;
		}
		
		drawPatchBackground(xlast, ylast, shadow, back);
			
		if(show)
		{
			rdp_start();
			HardDrawImage(x, y, shadow);
			rdp_end();
		}
		
		show = !show;
		
		WaitVsync();
		
		xlast = x;
		ylast = y;

        controller_scan();
		keys = get_keys_held();

		if(keys.c[0].up)
			y--;

		if(keys.c[0].down)
			y++;
		
		if(y > dH - shadow->height)
			y = dH - shadow->height;
		if(y < 0)
			y = 0;
			
		if(keys.c[0].left)
			x--;

		if(keys.c[0].right)
			x++;
			
		if(x > dW - shadow->width)
			x = dW - shadow->width;
		if(x < 0)
			x = 0;
		
		keys = get_keys_down();
		if(keys.c[0].B)
			end = 1;
	}
	FreeImage(&back);
	FreeImage(&shadow);
}

void DrawCheckerboard()
{
	int end = 0, type = 0;
	sprite_t *pos = NULL, *neg = NULL;
	struct controller_data keys;
	
	if(!pos)
		pos = LoadImage("/checkpos.bin");
	if(!neg)
		neg = LoadImage("/checkneg.bin");
			
    while(!end)
    {	
		GetDisplay();

		rdp_start();
		if(type)
			FillScreenWithTexture(pos);
		else
			FillScreenWithTexture(neg);
		rdp_end();
		
		WaitVsync();
		
		controller_scan();
		keys = get_keys_down();
		
		if(keys.c[0].A)
			type = !type;
			
		if(keys.c[0].B)
			end = 1;
	}
	
	FreeImage(&pos);
	FreeImage(&neg);
}



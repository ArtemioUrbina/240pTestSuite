/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include "tests.h"
#include "utils.h"
#include "menu.h"

void DropShadowTest()
{
	int end = 0, show = 1;
	int x = 0, y = 0;
	sprite_t *back = NULL, *shadow = NULL;
	struct controller_data keys;
	
	back = LoadImage("/motoko.bin");
	shadow = LoadImage("/shadow.bin");
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		if(show)
		{
			rdp_texture_start();	
			rdp_DrawImage(x, y, shadow);
			rdp_end();
		}
		
		show = !show;
		
		CheckMenu(DROPSHADOW);
		WaitVsync();
		
        controller_scan();
		keys = Controller_ButtonsHeld();

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
		
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
		if(keys.c[0].A)
			show = !show;
		CheckStart(keys);
	}
	FreeImage(&back);
	FreeImage(&shadow);
}

void DrawCheckerboard()
{
	int end = 0, type = 0, alternate = 0;
	int frame = 0, dframe = 0;
	sprite_t *pos = NULL, *neg = NULL;
	struct controller_data keys;
	
	if(!pos)
		pos = LoadImage("/checkpos.bin");
	if(!neg)
		neg = LoadImage("/checkneg.bin");
			
    while(!end)
    {	
		GetDisplay();

		rdp_texture_start();
		if(type)
			rdp_FillScreenWithTexture(pos);
		else
			rdp_FillScreenWithTexture(neg);
		rdp_end();
		
		if(dframe)
		{
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			DrawStringB(20, 210, 0xff, 0xff, 0xff, msg);
			frame ++;
			if(!useNTSC)
			{
				if(frame > 49)
					frame = 0;
			}
			else
			{
				if(frame > 59)
					frame = 0;
			}
		}
		
		CheckMenu(CHECKHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].A)
			alternate = !alternate;
			
		if(keys.c[0].C_left)
		{
			dframe = !dframe;
			frame = 0;
		}
		
		if(keys.c[0].C_right && !alternate)
			type = !type;
			
		if(keys.c[0].B)
			end = 1;
		CheckStart(keys);
		
		if(alternate)
			type = !type;
	}
	
	FreeImage(&pos);
	FreeImage(&neg);
}

void DrawStripes()
{
	int end = 0, type = 0, alternate = 0;
	int frame = 0, dframe = 0, vertical = 0;
	sprite_t *pos = NULL, *neg = NULL;
	sprite_t *vstripespos = NULL, *vstripesneg = NULL;
	struct controller_data keys;
	
	if(!pos)
		pos = LoadImage("/stripespos.bin");
	if(!neg)
		neg = LoadImage("/stripesneg.bin");
	if(!vstripespos)
		vstripespos = LoadImage("/vertstripespos.bin");
	if(!vstripesneg)
		vstripesneg = LoadImage("/vertstripesneg.bin");
			
    while(!end)
    {	
		GetDisplay();

		rdp_texture_start();
		if(!vertical)
		{
			if(type)
				rdp_FillScreenWithTexture(pos);
			else
				rdp_FillScreenWithTexture(neg);
		}
		else
		{
			if(type)
				rdp_FillScreenWithTexture(vstripespos);
			else
				rdp_FillScreenWithTexture(vstripesneg);
		}
		rdp_end();
		
		if(dframe)
		{
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			DrawStringB(20, 210, 0xff, 0xff, 0xff, msg);
			frame ++;
			if(!useNTSC)
			{
				if(frame > 49)
					frame = 0;
			}
			else
			{
				if(frame > 59)
					frame = 0;
			}
		}
		
		CheckMenu(STRIPESHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].A)
			alternate = !alternate;
		
		if(keys.c[0].R)
			vertical = !vertical;
	
		if(keys.c[0].C_left)
		{
			dframe = !dframe;
			frame = 0;
		}
		
		if(keys.c[0].C_right && !alternate)
			type = !type;
			
		if(keys.c[0].B)
			end = 1;
		CheckStart(keys);
		
		if(alternate)
			type = !type;
	}
	
	FreeImage(&pos);
	FreeImage(&neg);
	FreeImage(&vstripespos);
	FreeImage(&vstripesneg);
}

void DrawGridScroll()
{
	int end = 0, x = -8, y = -8;
	int speed = 1, acc = 1, pause = 0, direction = 0;
	sprite_t *grid = NULL;
	struct controller_data keys;
	
	if(!grid)
		grid = LoadImage("/circles_grid.bin");
	
    while(!end)
    {	
		GetDisplay();

		rdp_texture_start();
		rdp_FillScreenWithTextureXY(x-7, y-7, grid);
		rdp_end();
		
		CheckMenu(GRIDSCROLL);
		WaitVsync();
		
		if(!pause)
		{
			if(direction)
				x += speed * acc;
			else
				y += speed * acc;
		}	
		
		controller_scan();
		keys = Controller_ButtonsDown();
			
		if(keys.c[0].up)
			speed ++;

		if(keys.c[0].down)
			speed --;
			
		if(keys.c[0].B)
			end = 1;
		
		if(keys.c[0].A)
			pause = !pause;
			
		if(keys.c[0].C_left)
			acc *= -1;

		if(keys.c[0].C_right)
			direction = !direction;
		
		if(x < -7)
			x = 0;
		if(x > 7)
			x = 0;
			
		if(y < -7)
			y = 0;
		if(y > 7)
			y = 0;
			
		if(speed > 5)
			speed = 5;

		if(speed < 1)
			speed = 1;
			
		CheckStart(keys);
	}
	
	FreeImage(&grid);
}

void DrawScroll()
{
	int end = 0, x = 0, y = 0, currentsonic = 0, currentframe = 0;
	int speed = 1, acc = -1, pause = 0, vertical = 0;
	sprite_t *sonicback[4], *overlay;	
	struct controller_data keys;
	
	sonicback[0] = LoadImage("/sonicback1.bin");
	sonicback[1] = LoadImage("/sonicback2.bin");
	sonicback[2] = LoadImage("/sonicback3.bin");
	sonicback[3] = LoadImage("/sonicback4.bin");
	
	overlay = LoadImage("/sonicfloor.bin");
	
    while(!end)
    {	
		GetDisplay();

		rdp_texture_start();
		rdp_updatecache(0);
		if(x > 0)
			rdp_DrawImageClipped(x-256, 0, sonicback[currentsonic], 320, 240);
		rdp_DrawImageClipped(x, 0, sonicback[currentsonic], 320, 240);
		if(x < 64)
			rdp_DrawImageClipped(x+256, 0, sonicback[currentsonic], 320, 420);
		rdp_updatecache(1);

		if(x > 0)
			rdp_DrawImageClipped(2*x-256, 48, overlay, 320, 240);
		rdp_DrawImageClipped(2*x, 48, overlay, 320, 240);
		if(x < 64)
			rdp_DrawImageClipped(2*x+256, 48, overlay, 320, 240);
		// Extra gap
		if(x < -96)
			rdp_DrawImageClipped(2*x+512, 48, overlay, 320, 240);
		
		rdp_end();
		
		CheckMenu(SCROLL);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
			
		if(keys.c[0].up)
			speed += 1;

		if(keys.c[0].down)
			speed -= 1;
			
		if(keys.c[0].B)
			end = 1;
		
		if(keys.c[0].A)
			pause = !pause;
			
		if(keys.c[0].C_left)
			acc *= -1;

		if(keys.c[0].C_right)
			vertical = !vertical;
		
		if(speed > 16)
			speed = 16;

		if(speed < 1)
			speed = 1;

		if(!pause)
		{
			if(!vertical)
				x += speed * acc;
			else
				y -= speed * acc;
		}
		
		if(x < -128)
			x = 128 - speed;
		if(x > 128)
			x = -128 + speed;
			
		if(y < -7)
			y = 0;
		if(y > 7)
			y = 0;
			
		if(!vertical)
		{
			currentframe ++;
			if(currentframe > 10)
			{
				currentsonic++;
				if(currentsonic > 3)
					currentsonic = 0;
				currentframe = 0;
			}
		}
			
		CheckStart(keys);
	}
	
	for(x = 0; x < 4; x++)
		FreeImage(&sonicback[x]);
	FreeImage(&overlay);
}





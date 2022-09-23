/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
 * Copyright (C)2011-2022 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <DATlib.h>
#include <input.h>
#include "types.h"
#include "ng.h"
#include "string_ng.h"
#include "externs.h"
#include "tests.h"
#include "help.h"

BYTE p1,p2,ps,p1e,p2e, p1b,p2b;

void vt_drop_shadow_test()
{
	int done = 0, x = 30, y = 30;
	picture image;
	picture buzz_sprite;

	initGfx();

	clearFixLayer();
	clearSprites(1, 1);
	clearSprites(1, 100);

	//pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);
	//pictureInit(&buzz_sprite, &buzz, 22, 17, 32, 32, FLIP_NONE);

	while (!done)
	{
	
		pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);
		pictureInit(&buzz_sprite, &buzz, 22, 17, x, y, FLIP_NONE);
		palJobPut(16,donna.palInfo->count,donna.palInfo->data);
		palJobPut(17,buzz.palInfo->count,&buzz.palInfo->data);
		
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1 & JOY_UP)
		{
			y--;
			if(y < 0)
				y = 0;
		}

		if (p1 & JOY_DOWN)
		{
			y++;
			if(y > 192)
				y = 192;
		}

		if (p1 & JOY_LEFT)
		{
			x--;
			if (x < 0)
				x = 0;
		}

		if (p1 & JOY_RIGHT)
		{
			x++;
			if (x > 288)
				x = 288;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_SHADOW);
		}
	}
}

void vt_striped_sprite_test()
{
	int done = 0, x = 30, y = 30;
	picture image;
	picture image2;

	clearFixLayer();
	clearSprites(1, 22);
	clearSprites(1, 100);

	pictureInit(&image, &donna, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,donna.palInfo->count,donna.palInfo->data);

	pictureInit(&image2, &marker_striped, 22, 17, 32, 32, FLIP_NONE);
	palJobPut(17,marker_striped.palInfo->count,&marker_striped.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);

		pictureInit(&image2, &marker_striped, 22, 17, x, y, FLIP_NONE);

		if (p1 & JOY_UP)
		{
			y--;
			if(y < 0)
				y = 0;
		}

		if (p1 & JOY_DOWN)
		{
			y++;
			if(y > 192)
				y = 192;
		}

		if (p1 & JOY_LEFT)
		{
			x--;
			if (x < 0)
				x = 0;
		}

		if (p1 & JOY_RIGHT)
		{
			x++;
			if (x > 288)
				x = 288;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_STRIPED);
		}
	}

}

void vt_lag_test()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_LAG);
		}
	}

}

void vt_reflex_test()
{
	picture marker1;
	picture marker2;
	picture marker3;
	picture background;
	char str[10];
	short speed = 1, vary = 0, clicks[10];
	u16 pal = 0x0000, change = 1, loadvram = 1, psgoff = 0, usersound = 0;
	u16 x = 0, y = 0, x2 = 0, y2 = 0, done = 0, variation = 1, draw = 1;
	u16 pos = 0, view = 0, audio = 1, drawoffset = 0;

	x = 144, y = 60, x2 = 108, y2 = 96;

	backgroundColor(0x0000);
	clearFixLayer();
	clearSprites(1, 1);
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (loadvram)
		{
			fixPrint(2, 23, fontColorGreen, 3, "Press the \"A\" button when the sprite");
			fixPrint(2, 24, fontColorGreen, 3, "is aligned. A negative value means");
			fixPrint(2, 25, fontColorGreen, 3, "you pressed \"A\" before they intersect.");
			fixPrint(2, 26, fontColorGreen, 3, "\"B\" button toggles horz/vert");
			fixPrint(2, 27, fontColorGreen, 3, "\"C\" button toggles audio");
			fixPrint(2, 28, fontColorGreen, 3, "DOWN toggles random/rhythmic");

			loadvram = 0;
			draw = 1;

			if (pos)
			{
				int i = 0;
				unsigned short ppos = 0;

				for(i = 0; i < pos; i++)
				{
					pal = fontColorWhite;

					intToStr(clicks[i], str, 1);

					if (clicks[i] == 0)
						pal = fontColorGreen;
					if (clicks[i] < 0)
						pal = fontColorRed;

					ppos = i + 1;
					intToStr(ppos, str, 1);
					fixPrint(2, i+2, fontColorWhite, 3, "Offset");
					fixPrint(9, i+2, fontColorWhite, 3, str);
					fixPrint(i == 9 ? 11 : 10, i+2, fontColorWhite, 3, ":");
					intToStr(clicks[i], str, 1);
					fixPrint(i == 9 ? 13 : 12, i+2, pal, 3, str);
					ppos = strlen(str);
					if (clicks[i] == 1)
						fixPrint(pos == 9 ? 13 : 12 + ppos, i+2, pal, 3, " frame    ");
					else
						fixPrint(pos == 9 ? 13 : 12 + ppos, i+2, pal, 3, " frames   ");
				}
			}
		}
		
		if (y == 96)	//  Screen Flash    
		{
			// Todo: Add beep sound here
				
			// Todo: Flash screen here
		}
		
		if (usersound)
		{
			// Todo: turn sound off here
			usersound = 0;
		}

		if(isMVS)
		{
			if (p1e & JOY_D)
			{
				DrawHelp(HELP_MANUALLAG);
				clearSprites(1, 1);
				loadvram = 1;
				draw = 1;
			}
		} else { 
			if (ps & P1_SELECT)
			{
				DrawHelp(HELP_MANUALLAG);
				clearSprites(1, 1);
				loadvram = 1;
				draw = 1;
			}
		}

		if (p1e & JOY_A)
		{
			if (change)
			{
				clicks[pos] = (y - 96) * speed;
				drawoffset = 1;
				if (clicks[pos] >= 0)
					change = 0;
					
				if (audio)
				{
					if (clicks[pos] == 0)
					{
						// Todo: Add beep sound here
					}
					else {
						// Todo: Add beep sound here
					}
					usersound = 1;
				}
			}
		}

		if (p1e & JOY_B)
		{
			view++;
			if (view > 2)
				view = 0;
		}

		if (p1e & JOY_C)
		{
			audio = !audio;
			draw = 1;
		}

		if (p1e & JOY_DOWN)
		{
			variation = !variation;
			if (!variation)
				vary = 0;
			draw = 1;
		}

		if (ps & P1_START)
		{
			done = 1;
		}

		if (drawoffset)
		{
			unsigned short ppos = 0;

			pal = fontColorWhite;
			intToStr(clicks[pos], str, 1);

			if (clicks[pos] == 0)
				pal = fontColorGreen;
			if (clicks[pos] < 0)
				pal = fontColorRed;

			ppos = pos + 1;
			intToStr(ppos, str, 1);
			fixPrint(2, pos+2, fontColorWhite, 3, "Offset");
			fixPrint(9, pos+2, fontColorWhite, 3, str);
			fixPrint( pos == 9 ? 11 : 10, pos+2, fontColorWhite, 3, ":");
			intToStr(clicks[pos], str, 1);
			fixPrint(pos == 9 ? 13 : 12, pos+2, pal, 3, str);
			ppos = strlen(str);

			if (clicks[pos] == 1)
				fixPrint(pos == 9 ? 13 : 12 + ppos, pos+2, pal, 3, " frame    ");
			else
				fixPrint(pos == 9 ? 13 : 12 + ppos, pos+2, pal, 3, " frames   ");

			if (clicks[pos] >= 0)
				pos++;

			if (pos > 9)
				done = 1;

			drawoffset = 0;
		}

		if (draw)
		{
			fixPrint(24, 2, fontColorWhite, 3, "Audio:");
			if (audio)
				fixPrint(31, 2, fontColorWhite, 3, "on ");
			else
				fixPrint(31, 2, fontColorWhite, 3, "off");

			fixPrint(24, 3, fontColorWhite, 3, "Timing:");
			if (variation)
				fixPrint(32, 3, fontColorWhite, 3, "random  ");
			else
				fixPrint(32, 3, fontColorWhite, 3, "rhythmic");
			draw = 0;
		}

		if (y > 132 + vary)
		{
			speed = -1;
			change = 1;
			if (variation)
			{
				if (random() % 2)
					vary = random() % 7;
				else
					vary = -1 * random() % 7;
			}
		}

		if (y < 60 + vary)
		{
			speed = 1;
			change = 1;

			if (variation)
			{
				if (random() % 2)
					vary = random() % 7;
				else
					vary = -1 * random() % 7;
			}
		}

		y += speed;
		x2 += speed;

		pictureInit(&marker1, &marker, 1, 16, x, 96, FLIP_NONE);
		palJobPut(16,marker.palInfo->count,marker.palInfo->data);

		pictureInit(&marker3, &marker, 5, 16, x, y, FLIP_NONE);
		palJobPut(16,marker.palInfo->count,marker.palInfo->data);

		if (y == 96)								// Red on the spot
		{
			// Todo: Palette Red Here
		}

		if (y == 95 || y == 97)						// Green one pixel before or after
		{
			// Todo: Palette Green Here
		}

		if (y == 98 || y == 94)						// Back to white two pixels before or after
		{
			// Todo: Palette White Here
		}

		if (view == 0 || view == 2)
		{
			pictureInit(&marker3, &marker, 5, 16, x, y, FLIP_NONE);
			palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		} else {
			pictureInit(&marker3, &marker, 5, 16, 320, 224, FLIP_NONE);
			palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		}
		if (view == 1 || view == 2)
		{
			pictureInit(&marker2, &marker, 9, 16, x2, y2, FLIP_NONE);
			palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		} else {
			pictureInit(&marker2, &marker, 9, 16, 320, 224, FLIP_NONE);
			palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		}
		if (y == 96)		// Half the screen?
		{
			if (audio)
			{
				// Todo: Beep here
				if (psgoff == 0)
					psgoff = 2;
			}

			// Todo: Screen flash here
		}

		//if (psgoff)
		//{
		//	psgoff--;
		//	if (psgoff == 0)
				// Todo: Stop sound
		//}
	}

	if (pos > 9)
	{
		int totald = 0, cnt, tot;
		u16 total = 0, count = 0, c = 0;

		pictureInit(&background, &back,1, 16, 0, 0,FLIP_NONE);
		palJobPut(16,back.palInfo->count,back.palInfo->data);
		clearFixLayer();
		SCClose();

		for(c = 0; c < 10; c++)
		{
			if (clicks[c] != 0xFF)
			{
				intToStr(clicks[c], str, 1);

				pal = fontColorWhite;
				if (clicks[c] == 0)
					pal = fontColorRed;

				if (clicks[c] < 0)
					pal = fontColorGreen;

				if (clicks[c] >= 0)
				{
					total += clicks[c];
					count++;
				}
			}
			fixPrint(10, c + 9, pal, 3, str);
		}

		fixPrint(8, 13, fontColorGreen, 3, "+");

		if (count > 0)
		{
			u16 h = 10, v = 20;
			//fix32 framerate = MARS_VDP_DISPMODE & MARS_NTSC_FORMAT ? FIX32(16.6884) : FIX32(20.1196);
			fix32 framerate = FIX32(16.6884);
			fixPrint(h - 2, v++, fontColorWhite, 3, "----");

			cnt = intToFix32(count);
			tot = intToFix32(total);

			intToStr(total, str, 1);
			fixPrint(h, v, fontColorWhite, 3, str);
			h += strlen(str);
			fixPrint(h++, v, fontColorGreen, 3, "/");

			intToStr(count, str, 1);
			fixPrint(h, v, fontColorWhite, 3, str);
			h += strlen(str);
			fixPrint(h++, v, fontColorGreen, 3, "=");

			totald = fix32Div(tot, cnt);
			fix32ToStr(totald, str, 4);
			fixPrint(h, v, fontColorWhite, 3, str);
			h += strlen(str);
			h -= 2;
			fixPrint(h, v, fontColorWhite, 3, "frames");
			fixPrint(5, (++v)+1, fontColorWhite, 3, "These are your reflexes, not a");
			fixPrint(5, (++v)+1, fontColorWhite, 3, "lag test. A frame is");
			fix32ToStr(framerate, str, 6);
			fixPrint(26, v+1, fontColorWhite, 3, str);
			fixPrint(32, v+1, fontColorWhite, 3, "ms.");

			h = 14, v = 14;
			cnt = fix32Mul(totald, framerate);
			fix32ToStr(cnt, str, 2);
			fixPrint(h, v, fontColorRed, 3, str);
			h += strlen(str);
			fixPrint(h, v, fontColorWhite, 3, " milliseconds");

			if (total < 5)
				fixPrint(14, 17, fontColorGreen, 3, "EXCELLENT REFLEXES!");

			if (total == 0)
				fixPrint(14, 17, fontColorGreen, 3, "INCREDIBLE REFLEXES!!");
		}

		done = 0;

		while (!done)
		{
			waitVBlank();

			p1e = volMEMBYTE(P1_EDGE);
			ps  = volMEMBYTE(PS_CURRENT);

			if (p1e & JOY_B || ps & P1_START)
			{
				done = 1;
			}
		}
	}
}

void vt_scroll_test()
{
	int done = 0;

	int x1 = 0, y1 = -96;
	int x2 = 0, y2 = -152;
	int x3 = 0, y3 = 0;

	scroller backScroll, waterScroll, frontScroll;

	clearFixLayer();
	clearSprites(1, 22);

	scrollerInit(&backScroll, &sonic_back, 1, 16, x3, y3);
	palJobPut(16, sonic_back.palInfo->count, sonic_back.palInfo->data);

	scrollerInit(&waterScroll, &sonic_water, 22, 17, x2, y2);
	palJobPut(17, sonic_water.palInfo->count, sonic_water.palInfo->data);

	scrollerInit(&frontScroll, &sonic_floor, 43, 18, x1, y1);
	palJobPut(18, sonic_floor.palInfo->count, sonic_floor.palInfo->data);

	SCClose();

	while (!done)
	{
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		x1++;
		x2++;
		x3++;

		if(x1 > 256)
		{
			x1 = 0;
		}

		if(x2 > 256)
		{
			x2 = 0;
		}

		if(x3 > 256)
		{
			x3 = 0;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_HSCROLL);
		}

		scrollerSetPos(&frontScroll, x1, y1);
		scrollerSetPos(&waterScroll, x2, y2);
		scrollerSetPos(&backScroll, x3, y3);
		SCClose();
	}
}

void vt_vert_scroll_test()
{
	int done = 0;
	int x = 0, y = 0;
	scroller vertScroll;

	clearFixLayer();
	backgroundColor(0x0000);
	clearSprites(1, 100);

	palJobPut(16, kiki.palInfo->count, kiki.palInfo->data);

	while (!done)
	{
		scrollerInit(&vertScroll, &kiki, 1, 16, 1, y);
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		y++;

		if(y > 512)
		{
			y = 0;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_HSCROLL);
		}

		//scrollerSetPos(&vertScroll, 1, y);
		//SCClose();
	}
}

void vt_gridscroll_test()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_VSCROLL);
		}
	}

}

void vt_horizontal_stripes()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &horzstripe, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,horzstripe.palInfo->count,horzstripe.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_STRIPES);
		}
	}

}

void vt_vertical_stripes()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &vertstripe, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,vertstripe.palInfo->count,vertstripe.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_STRIPES);
		}
	}

}

void vt_checkerboard()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &check, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,check.palInfo->count,check.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_CHECK);
		}
	}

}

void vt_backlitzone_test()
{
	int done = 0, block = 2, x = 160, y = 112, draw = 1;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);
	backgroundColor(0x0000);

	while (!done)
	{
		if (draw)
		{
			switch (block)
			{
				case 1:
					pictureInit(&image, &led_1x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 2:
					pictureInit(&image, &led_2x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 3:
					pictureInit(&image, &led_4x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 4:
					pictureInit(&image, &led_6x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 5:
					pictureInit(&image, &led_8x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;
			}
			draw = 0;
		}

		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			block++;
			if(block > 5)
			block = 1;
			draw = 1;
		}

		if (p1 & JOY_UP)
		{
			y--;
			draw = 1;
		}

		if (p1 & JOY_DOWN)
		{
			y++;
			draw = 1;
		}

		if (p1 & JOY_RIGHT)
		{
			x++;
			draw = 1;
		}

		if (p1 & JOY_LEFT)
		{
			x--;
			draw = 1;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if(isMVS)
		{
			if (p1e & JOY_D)
			{
				DrawHelp(HELP_LED);
				draw = 1;
			}
		} else { 
			if (ps & P1_SELECT)
			{
				DrawHelp(HELP_LED);
				draw = 1;
			}
		}
	}
}

void at_sound_test()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_SOUND);
		}
	}

}

void at_audiosync_test()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_AUDIOSYNC);
		}
	}

}

void ht_controller_test()
{
	int done = 0;
	picture image;
	BYTE mvssel = 0, mvscredit = 0;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	setup4P();

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);    // Controller 1
		p2 = volMEMBYTE(P2_CURRENT);    // Controller 2
		p1b = volMEMBYTE(P1B_CURRENT);  // Controller 3
		p2b = volMEMBYTE(P2B_CURRENT);  // Controller 4
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		p2e = volMEMBYTE(P2_EDGE);

		if(isMVS)
		{
			mvscredit = volMEMBYTE(REG_STATUS_A); 
			mvssel = volMEMBYTE(REG_STATUS_B); 
		}

		if (ps & P1_START && p1e & JOY_LEFT)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if(isMVS)
		{
			fixPrint(29, 8, !(mvscredit & MVS_SERV_B) ? fontColorRed : fontColorWhite, 3, "Service");
			if(is4S || is6S)
			{
				fixPrint(31, 23, !(mvssel & MVS_SEL1) ? fontColorRed : fontColorWhite, 3, "Sel 1");
				fixPrint(31, 24, !(mvssel & MVS_SEL2) ? fontColorRed : fontColorWhite, 3, "Sel 2");
			}
		}

		// Controller 1
		fixPrint(9, 9, p1 & JOY_UP ? fontColorRed : fontColorWhite, 3, "Up");
		fixPrint(5, 10, p1 & JOY_LEFT ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(11,10, p1 & JOY_RIGHT ? fontColorRed : fontColorWhite, 3, "Right");
		fixPrint(8, 11, p1 & JOY_DOWN ? fontColorRed : fontColorWhite, 3, "Down");

		fixPrint(18, 9, ps & P1_START ? fontColorRed : fontColorWhite, 3, "Start");
		if(isMVS)
			fixPrint(18, 11, !(mvscredit & P1_CREDIT) ? fontColorRed : fontColorWhite, 3, "Credit");
		else
			fixPrint(18, 11, ps & P1_SELECT ? fontColorRed : fontColorWhite, 3, "Select");

		fixPrint(27, 10, p1 & JOY_A ? fontColorRed : fontColorWhite, 3, "A");
		fixPrint(28, 10, p1 & JOY_B ? fontColorRed : fontColorWhite, 3, "B");
		fixPrint(29, 10, p1 & JOY_C ? fontColorRed : fontColorWhite, 3, "C");
		fixPrint(30, 10, p1 & JOY_D ? fontColorRed : fontColorWhite, 3, "D");

		// Controller 2
		fixPrint(9, 13, p2 & JOY_UP ? fontColorRed : fontColorWhite, 3, "Up");
		fixPrint(5, 14, p2 & JOY_LEFT ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(11, 14, p2 & JOY_RIGHT ? fontColorRed : fontColorWhite, 3, "Right");
		fixPrint(8, 15, p2 & JOY_DOWN ? fontColorRed : fontColorWhite, 3, "Down");

		fixPrint(18, 13, ps & P2_START ? fontColorRed : fontColorWhite, 3, "Start");
		if(isMVS)
			fixPrint(18, 15, !(mvscredit & P2_CREDIT) ? fontColorRed : fontColorWhite, 3, "Credit");
		else
			fixPrint(18, 15, ps & P2_SELECT ? fontColorRed : fontColorWhite, 3, "Select");

		fixPrint(27, 14, p2 & JOY_A ? fontColorRed : fontColorWhite, 3, "A");
		fixPrint(28, 14, p2 & JOY_B ? fontColorRed : fontColorWhite, 3, "B");
		fixPrint(29, 14, p2 & JOY_C ? fontColorRed : fontColorWhite, 3, "C");
		fixPrint(30, 14, p2 & JOY_D ? fontColorRed : fontColorWhite, 3, "D");

		// Controller 3
		fixPrint(9, 17, p1b & JOY_UP ? fontColorRed : fontColorWhite, 3, "Up");
		fixPrint(5, 18, p1b & JOY_LEFT ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(11, 18, p1b & JOY_RIGHT ? fontColorRed : fontColorWhite, 3, "Right");
		fixPrint(8, 19, p1b & JOY_DOWN ? fontColorRed : fontColorWhite, 3, "Down");

		fixPrint(18, 17, ps & P1B_START ? fontColorRed : fontColorWhite, 3, "Start");
		if(isMVS)
			fixPrint(18, 19, !(mvscredit & P3_CREDIT) ? fontColorRed : fontColorWhite, 3, "Credit");
		else
			fixPrint(18, 19, ps & P1B_SELECT ? fontColorRed : fontColorWhite, 3, "Select");

		fixPrint(27, 18, p1b & JOY_A ? fontColorRed : fontColorWhite, 3, "A");
		fixPrint(28, 18, p1b & JOY_B ? fontColorRed : fontColorWhite, 3, "B");
		fixPrint(29, 18, p1b & JOY_C ? fontColorRed : fontColorWhite, 3, "C");
		fixPrint(30, 18, p1b & JOY_D ? fontColorRed : fontColorWhite, 3, "D");

		// Controller 4
		fixPrint(9, 21, p2b & JOY_UP ? fontColorRed : fontColorWhite, 3, "Up");
		fixPrint(5, 22, p2b & JOY_LEFT ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(11, 22, p2b & JOY_RIGHT ? fontColorRed : fontColorWhite, 3, "Right");
		fixPrint(8, 23, p2b & JOY_DOWN ? fontColorRed : fontColorWhite, 3, "Down");

		fixPrint(18, 21, ps & P2B_START ? fontColorRed : fontColorWhite, 3, "Start");
		if(isMVS)
			fixPrint(18, 23, !(mvscredit & P4_CREDIT) ? fontColorRed : fontColorWhite, 3, "Credit");
		else
			fixPrint(18, 23, ps & P2B_SELECT ? fontColorRed : fontColorWhite, 3, "Select");

		fixPrint(27, 22, p2b & JOY_A ? fontColorRed : fontColorWhite, 3, "A");
		fixPrint(28, 22, p2b & JOY_B ? fontColorRed : fontColorWhite, 3, "B");
		fixPrint(29, 22, p2b & JOY_C ? fontColorRed : fontColorWhite, 3, "C");
		fixPrint(30, 22, p2b & JOY_D ? fontColorRed : fontColorWhite, 3, "D");

		fixPrint(9, 26, fontColorGreen, 3, "Use START+LEFT to exit");
	}
}

#define MAX_LOCATIONS 9

void ht_memory_viewer(u32 address)
{
	int done = 0, frameDelay = 0, redraw = 1, docrc = 0, locpos = 1, i = 0;
	u32 crc = 0, locations[MAX_LOCATIONS] = { 0, 0x100000, 0x10F300, 0x110000, 0x200000, 0x300000, 0x400000, 0x402000, 0xC00000 };

	backgroundColor(0x0000);
	clearFixLayer();
	clearSprites(1, 1);
	clearSprites(1, 22);

	for (i = 0; i < MAX_LOCATIONS; i++)
	{
		if (locations[i] == address)
		{
			locpos = i;
			break;
		}
	}

	while (!done)
	{
		if (redraw)
		{
			int i = 0, j = 0;
			u8 *mem = NULL;
			char buffer[10];

			mem = (u8*)address;

			//if (docrc)
			//	crc = CalculateCRC(address, 0x1C0);

			intToHex(address, buffer, 8);
			fixPrint(32, 2, fontColorRed, 3, buffer);
			intToHex(address+448, buffer, 8);
			fixPrint(32, 29, fontColorRed, 3, buffer);

			//if (docrc)
			//{
			//	intToHex(crc, buffer, 8);
			//	fixPrint(32, 14, fontColorGreen, 3, buffer);
			//}

			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < 16; j++)
				{
					intToHex(mem[i*16+j], buffer, 2);
					fixPrint(j*2, i, fontColorWhite, 3, buffer);
				}
			}
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (p1e & JOY_A)
		{
			docrc = !docrc;
			redraw = 1;
		}

		if (p1e & JOY_B)
		{
			locpos ++;
			if (locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
			redraw = 1;
		}

		if(isMVS)
		{
			if (p1e & JOY_D)
			{
				DrawHelp(HELP_MEMVIEW);
				clearSprites(1, 1);
				redraw = 1;
			}
		} else
		{
			if (ps & P1_SELECT)
			{
				DrawHelp(HELP_MEMVIEW);
				clearSprites(1, 1);
				redraw = 1;
			}
		}

		if (p1e & JOY_LEFT)
		{
			if (address >= 0x1C0)
				address -= 0x1C0;
			else
				address = 0;

			redraw = 1;
		}

		if (p1e & JOY_RIGHT)
		{
			address += 0x1C0;	
			redraw = 1;
		}

		if (p1e & JOY_UP)
		{
			if (address >= 0x10000)
				address -= 0x10000;
			else
				address = 0;

			redraw = 1;
		}

		if (p1e & JOY_DOWN)
		{
			address += 0x10000;
			redraw = 1;
		}
	}
	return;
}

void ht_test_ng_ram()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}
	}
}
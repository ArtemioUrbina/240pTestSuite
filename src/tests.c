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
#include "string.h"
#include "externs.h"
#include "tests.h"
#include "help.h"

BYTE p1,p2,ps,p1e,p2e, p1b,p2b;

void vt_drop_shadow_test()
{
	int done = 0, x = 30, y = 30;
	picture image;
	picture buzz_sprite;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);
	palJobPut(16,donna.palInfo->count,donna.palInfo->data);

	pictureInit(&buzz_sprite, &buzz, 20, 17, 32, 32, FLIP_NONE);
	palJobPut(17,buzz.palInfo->count,&buzz.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);

		pictureInit(&buzz_sprite, &buzz, 22, 17, x, y, FLIP_NONE);

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

	pictureInit(&image, &donna, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,donna.palInfo->count,donna.palInfo->data);

	pictureInit(&image2, &marker_striped, 20, 17, 32, 32, FLIP_NONE);
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
	picture marker3;
	char str[10];
	short speed = 1, vary = 0, clicks[10];
	unsigned short pal = 0x0000, change = 1, loadvram = 1, psgoff = 0, usersound = 0;
	unsigned short x = 0, y = 0, x2 = 0, y2 = 0, done = 0, variation = 1, draw = 1;
	unsigned short pos = 0, view = 0, audio = 1, drawoffset = 0;

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
					fixPrint(2, i, fontColorWhite, 3, "Offset");
					fixPrint(9, i, fontColorWhite, 3, str);
					fixPrint(i == 9 ? 11 : 10, i, fontColorWhite, 3, ":");
					intToStr(clicks[i], str, 1);
					fixPrint(i == 9 ? 13 : 12, i, pal, 3, str);
					ppos = strlen(str);
					if (clicks[i] == 1)
						fixPrint(pos == 9 ? 13 : 12 + ppos, i, pal, 3, " frame    ");
					else
						fixPrint(pos == 9 ? 13 : 12 + ppos, i, pal, 3, " frames   ");
				}
			}
		}
		
		if (y == 96)	//  Screen Flash    
		{
			//if (audio && !usersound)
				//HwMdPSGSetEnvelope(0, PSG_ENVELOPE_MIN);
				
			//Hw32xSetBGColor(0, 5, 5, 5);
		}
		
		if (usersound)
		{
			//HwMdPSGSetEnvelope(0, PSG_ENVELOPE_MIN);
			usersound = 0;
		}

		if(isMVS)
		{
			if (p1e & JOY_D)
			{
				DrawHelp(HELP_MANUALLAG);
				clearSprites(1, 1);
				draw = 1;
			}
		} else { 
			if (ps & P1_SELECT)
			{
				DrawHelp(HELP_MANUALLAG);
				clearSprites(1, 1);
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
						//HwMdPSGSetChandVol(0, 0);
						//HwMdPSGSetFrequency(0, 1000);
						//if (psgoff == 0)
						//	psgoff = 2;
					}
					else {
						//HwMdPSGSetChandVol(0, 0);
						//HwMdPSGSetFrequency(0, 500);
						//if (psgoff == 0)
						//	psgoff = 2;
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
			fixPrint(2, pos, fontColorWhite, 3, "Offset");
			fixPrint(9, pos, fontColorWhite, 3, str);
			fixPrint( pos == 9 ? 11 : 10, pos, fontColorWhite, 3, ":");
			intToStr(clicks[pos], str, 1);
			fixPrint(pos == 9 ? 13 : 12, pos, pal, 3, str);
			ppos = strlen(str);

			if (clicks[pos] == 1)
				fixPrint(pos == 9 ? 13 : 12 + ppos, pos, pal, 3, " frame    ");
			else
				fixPrint(pos == 9 ? 13 : 12 + ppos, pos, pal, 3, " frames   ");

			if (clicks[pos] >= 0)
				pos++;

			if (pos > 9)
				done = 1;

			drawoffset = 0;
		}

		if (draw)
		{
			fixPrint(24, 0, fontColorWhite, 3, "Audio:");

			if (audio)
				fixPrint(31, 0, fontColorWhite, 3, "on ");
			else
				fixPrint(31, 0, fontColorWhite, 3, "off");

			fixPrint(24, 1, fontColorWhite, 3, "Timing:");

			if (variation)
				fixPrint(32, 1, fontColorWhite, 3, "random  ");
			else
				fixPrint(32, 1, fontColorWhite, 3, "rhythmic");

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

		pictureInit(&marker1, &marker, 1, 16, x, 10, FLIP_NONE);
		palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		//drawSprite(MARKER_TILE3, x, 96, 32, 32, 0, 0);
		//drawSprite(MARKER_TILE1, x, y, 32, 32, 0, 0);
		pictureInit(&marker3, &marker, 1, 16, x, y, FLIP_NONE);
		palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		SCClose();

		if (y == 96)								// Red on the spot
			//cram16[2] = COLOR(31, 0, 0);

		if (y == 95 || y == 97)						// Green one pixel before or after
			//cram16[2] = COLOR(0, 31, 0);

		if (y == 98 || y == 94)						// Back to white two pixels before or after
			//cram16[2] = COLOR(31, 31, 31);

		if (view == 0 || view == 2)
			//drawSprite(MARKER_TILE1, x, y, 32, 32, 0, 0);
		//else
			//drawSprite(MARKER_TILE1, 320, 224, 32, 32, 0, 0);

		if (view == 1 || view == 2)
			//drawSprite(MARKER_TILE2, x2, y2, 32, 32, 0, 0);
		///else
			//drawSprite(MARKER_TILE2, 320, 224, 32, 32, 0, 0);

		if (y == 96)								// Half the screen?
		{
			if (audio)
			{
				//HwMdPSGSetChandVol(0, 0);
				//HwMdPSGSetFrequency(0, 1000);
				//if (psgoff == 0)
				//	psgoff = 2;
			}

			//Hw32xSetBGColor(0, 5, 5, 5);
		}

		//if (psgoff)
		//{
			//psgoff--;
			//if (psgoff == 0)
				//MDPSG_stop();
		//}
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
	clearSprites(1, 22);

	scrollerInit(&vertScroll, &kiki, 1, 16, x, y);
	palJobPut(16, kiki.palInfo->count, kiki.palInfo->data);

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

		y++;

		if(y > 256)
		{
			y = 0;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_HSCROLL);
		}

		scrollerSetPos(&vertScroll, x, y);
		SCClose();
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
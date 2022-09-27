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
	int done = 0, x = 30, y = 30, draw = 1, changeSprite = 0, evenFrames = 0;
	char flip;
	unsigned int fc;
	picture image;
	picture slug_sprite;
	picture slug_shadow_sprite;
	picture shape_sprite;

	flip = FLIP_NONE;

	while (!done)
	{
		fc = DAT_frameCounter;

		if (draw)
		{
			backgroundColor(0xFAAF);
			gfxClear();

			palJobPut(16,donna.palInfo->count,donna.palInfo->data);
			palJobPut(29,slug.palInfo->count,slug.palInfo->data);
			palJobPut(30,slug_shadow.palInfo->count,slug_shadow.palInfo->data);
			palJobPut(31,shape_shadow.palInfo->count,shape_shadow.palInfo->data);
			draw = 0;
		}

		pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);
		
		if (changeSprite == 0)
		{
			pictureHide(&shape_sprite);
			if (fc % 2 == evenFrames)
			{
				pictureInit(&slug_shadow_sprite, &slug_shadow, 22, 30, x, y, flip);
			} else {
				pictureHide(&slug_shadow_sprite);
			}
			pictureInit(&slug_sprite, &slug, 44, 29, x-20, y-20, flip);
		} else {
			pictureHide(&slug_shadow_sprite);
			pictureHide(&slug_sprite);
			if (fc % 2 == evenFrames)
			{
				pictureInit(&shape_sprite, &shape_shadow, 22, 31, x, y, FLIP_NONE);
			} else {
				pictureHide(&shape_sprite);
			}
		}

		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_C)
		{
			changeSprite = ~changeSprite;
		}

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
			flip = FLIP_X;
			if (x < 0)
				x = 0;
		}

		if (p1 & JOY_RIGHT)
		{
			x++;
			flip = FLIP_NONE;
			if (x > 288)
				x = 288;
		}

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_SHADOW))
			draw = 1;
	}
}

void vt_striped_sprite_test()
{
	int done = 0, x = 30, y = 30, draw = 1;
	picture image;
	picture image2;

	while (!done)
	{
		if (draw)
		{
			backgroundColor(0xFAAF);
			gfxClear();

			palJobPut(16,donna.palInfo->count,donna.palInfo->data);
			palJobPut(29,marker_striped.palInfo->count,marker_striped.palInfo->data);
			draw = 0;
		}

		pictureInit(&image, &donna, 1, 16, 0, 0, FLIP_NONE);
		pictureInit(&image2, &marker_striped, 22, 29, x, y, FLIP_NONE);

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
		}

		if(checkHelp(HELP_STRIPED))
			draw = 1;
	}
}

void vt_lag_test()
{
	u16 lsd, msd;
	int frames = 0, seconds = 0, minutes = 0, hours = 0, framecnt = 1;
	u16 done = 0, draw = 1;
	u16 cc_1 = 17, cc_2 = 17, cc_3 = 17, cc_4 = 17, cc_5 = 17, cc_6 = 17, cc_7 = 17, cc_8 = 17, barC = 20;
	const pictureInfo *numbers[10] = {&num_0, &num_1, &num_2, &num_3, &num_4, &num_5, &num_6, &num_7, &num_8, &num_9};
	u16 pause = 0, cposx = 32, cposy = 17;
	
	picture image;
	picture circle;
	picture c_numbers;
	picture bar;
	picture h1;
	picture h2;
	picture m1;
	picture m2;
	picture s1;
	picture s2;
	picture f1;
	picture f2;

	backgroundColor(0x5fff);
	gfxClear();

	palJobPut(16, num_0.palInfo->count, num_0.palInfo->data);
	palJobPut(17, circle_blue.palInfo->count, circle_blue.palInfo->data);
	palJobPut(18, num_0_w.palInfo->count, num_0_w.palInfo->data);
	palJobPut(19, circle_red.palInfo->count, circle_red.palInfo->data);
	palJobPut(20, bar_l.palInfo->count, bar_l.palInfo->data);	

	while (!done)
	{
		pictureInit(&circle, &circle_blue, 65, cc_1, 12, 68, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 81, cc_2, 92, 68, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 97, cc_3, 172, 68, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 113, cc_4, 248, 68, FLIP_NONE);

		pictureInit(&circle, &circle_blue, 129, cc_5, 12, 132, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 145, cc_6, 92, 132, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 161, cc_7, 172, 132, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 177, cc_8, 248, 132, FLIP_NONE);

		pictureInit(&c_numbers, &num_1_w, 181, 18, 32, 84, FLIP_NONE);
		pictureInit(&c_numbers, &num_2_w, 185, 18, 112, 84, FLIP_NONE);
		pictureInit(&c_numbers, &num_3_w, 189, 18, 192, 84, FLIP_NONE);
		pictureInit(&c_numbers, &num_4_w, 193, 18, 268, 84, FLIP_NONE);

		pictureInit(&c_numbers, &num_5_w, 197, 18, 32, 148, FLIP_NONE);
		pictureInit(&c_numbers, &num_6_w, 201, 18, 112, 148, FLIP_NONE);
		pictureInit(&c_numbers, &num_7_w, 205, 18, 192, 148, FLIP_NONE);
		pictureInit(&c_numbers, &num_8_w, 209, 18, 268, 148, FLIP_NONE);

		pictureInit(&image, &separator, 1, 16, 80, 19, FLIP_NONE);
		pictureInit(&image, &separator, 5, 16, 152, 19, FLIP_NONE);
		pictureInit(&image, &separator, 9, 16, 224, 19, FLIP_NONE);

		pictureInit(&bar, &bar_l, 223, barC, 0, 0, FLIP_NONE);
		pictureInit(&bar, &bar_r, 237, barC, 304, 0, FLIP_NONE);

		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		pictureInit(&h1, numbers[msd], 14, 16, 32, 19, FLIP_NONE);
		pictureInit(&h2, numbers[lsd], 19, 16, 56, 19, FLIP_NONE);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		pictureInit(&m1, numbers[msd], 24, 16, 104, 19, FLIP_NONE);
		pictureInit(&m2, numbers[lsd], 29, 16, 128, 19, FLIP_NONE);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		pictureInit(&s1, numbers[msd], 34, 16, 176, 19, FLIP_NONE);
		pictureInit(&s2, numbers[lsd], 39, 16, 200, 19, FLIP_NONE);

		// Draw frames
		lsd = frames % 10;
		msd = frames / 10;
		pictureInit(&f1, numbers[msd], 44, 16, 248, 19, FLIP_NONE);
		pictureInit(&f2, numbers[lsd], 49, 16, 272, 19, FLIP_NONE);

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		fixPrint(4, 3, fontColorBlack, 3, "hours");
		fixPrint(13, 3, fontColorBlack, 3, "minutes");
		fixPrint(22, 3, fontColorBlack, 3, "seconds");
		fixPrint(31, 3, fontColorBlack, 3, "frames");

		if(framecnt > 8)
			framecnt = 1;

		if (framecnt == 1)
		{
			cc_1 = 19;
		}
		else {
			cc_1 = 17;
		}

		if (framecnt == 2)
		{
			cc_2 = 19;
		}
		else {
			cc_2 = 17;
		}

		if (framecnt == 3)
		{
			cc_3 = 19;
		}
		else {
			cc_3 = 17;
		}

		if (framecnt == 4)
		{
			cc_4 = 19;
		}
		else {
			cc_4 = 17;
		}

		if (framecnt == 5)
		{
			cc_5 = 19;
		}
		else {
			cc_5 = 17;
		}

		if (framecnt == 6)
		{
			cc_6 = 19;
		}
		else {
			cc_6 = 17;
		}

		if (framecnt == 7)
		{
			cc_7 = 19;
		}
		else {
			cc_7 = 17;
		}

		if (framecnt == 8)
		{
			cc_8 = 19;
		}
		else {
			cc_8 = 17;
		}

		if (framecnt % 2 == 0)
		{
			barC = 18;
		}
		else {
			barC = 20;
		}

		if (framecnt > 4)
		{
			cposx = framecnt - 4;
			cposy = 17;
		}
		else {
			cposx = framecnt;
			cposy = 9;
		}
		cposx = (cposx - 1) * 10 + 2;

		if (!pause)
		{
			frames ++;
			framecnt ++;
			if(framecnt > 8)
				framecnt = 1;
		}

		if (frames > 59)
		{
			frames = 0;
			seconds ++;
		}
		
		if (seconds > 59)
		{
			seconds = 0;
			minutes ++;
		}

		if (minutes > 59)
		{
			minutes = 0;
			hours ++;
		}

		if (hours > 99)
			hours = 0;

		if (p1e & JOY_A)
		{
			pause = !pause;
		}

		if (p1e & JOY_B && pause)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 1;
		}

		if (ps & P1_START)
		{
			clearSprites(1,260);
			done = 1;
		}

		if(checkHelp(HELP_LAG))
			clearSprites(1,260);
			draw = 1;
	}
}

void vt_reflex_test()
{
	char str[10];
	short speed = 1, vary = 0, clicks[10], resetbg = 0;
	u16 pal = 0x0000, change = 1, loadvram = 1, psgoff = 0, usersound = 0;
	u16 x = 0, y = 0, x2 = 0, y2 = 0, done = 0, variation = 1, draw = 1;
	u16 pos = 0, view = 0, audio = 1, drawoffset = 0;

	picture marker1;
	picture marker2;
	picture marker3;
	picture background;

	x = 144, y = 60, x2 = 108, y2 = 96;

	while (!done)
	{
		palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (loadvram)
		{
			backgroundColor(0x0000);
			gfxClear();

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
		
		if(resetbg)
		{
			backgroundColor(0x0000);
			resetbg = 0;
		}

		if (y == 96)	//  Screen Flash
		{
			backgroundColor(0x0555);
			resetbg = 1;
		}
		
		if (usersound)
		{
			// Todo: turn sound off here
			usersound = 0;
		}

		if(checkHelp(HELP_MANUALLAG))
			draw = 1;

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
		pictureInit(&marker3, &marker, 5, 16, x, y, FLIP_NONE);

		if (y == 96)								// Red on the spot
			volMEMWORD(0x402202) = 0x0f00;

		if (y == 95 || y == 97)						// Green one pixel before or after
			volMEMWORD(0x402202) = 0x00f0;

		if (y == 98 || y == 94)						// Back to white two pixels before or after
			volMEMWORD(0x402202) = 0x8000;

		if (view == 0 || view == 2)
		{
			pictureInit(&marker3, &marker, 5, 16, x, y, FLIP_NONE);
		} else {
			pictureInit(&marker3, &marker, 5, 16, 320, 224, FLIP_NONE);
		}
		if (view == 1 || view == 2)
		{
			pictureInit(&marker2, &marker, 9, 16, x2, y2, FLIP_NONE);
		} else {
			pictureInit(&marker2, &marker, 9, 16, 320, 224, FLIP_NONE);
		}
		if (y == 96)		// Half the screen?
		{
			if (audio)
			{
				// Todo: Beep here
				if (psgoff == 0)
					psgoff = 2;
			}
			backgroundColor(0x0555);
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
	int done = 0, draw = 1;
	int x1 = 0, y1 = -96;
	int x2 = 0, y2 = -152;
	int x3 = 0, y3 = 0;
	unsigned int fc;

	scroller backScroll, waterScroll, frontScroll;

	while (!done)
	{
		fc = DAT_frameCounter;

		if (draw)
		{
			gfxClear();

			scrollerInit(&backScroll, &sonic_back, 1, 16, x3, y3);
			palJobPut(16, sonic_back.palInfo->count, sonic_back.palInfo->data);

			scrollerInit(&waterScroll, &sonic_water, 22, 18, x2, y2);
			palJobPut(18, sonic_water.palInfo->count, sonic_water.palInfo->data);

			scrollerInit(&frontScroll, &sonic_floor, 43, 19, x1, y1);
			palJobPut(19, sonic_floor.palInfo->count, sonic_floor.palInfo->data);
			draw = 0;
		}
		
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (ps & P1_START)
		{
			done = 1;
		}

		x1++;
		x2++;
		x3++;

		if(x1 > 256)
		{
			x1 = 0;
		}

		if(x2 > 512)
		{
			x2 = 0;
		}

		if(x3 > 512)
		{
			x3 = 0;
		}

		if(checkHelp(HELP_HSCROLL))
			draw = 1;
		
		scrollerSetPos(&frontScroll, x1, y1);
		scrollerSetPos(&waterScroll, x2/2, y2);
		scrollerSetPos(&backScroll, x3/2, y3);
	}
}

void vt_vert_scroll_test()
{
	int done = 0, draw = 1;
	int x = 0, y = 0;
	scroller vertScroll;

	while (!done)
	{
		if (draw)
		{
			backgroundColor(0x8000);
			gfxClear();

			scrollerInit(&vertScroll, &kiki, 1, 16, 0, 0);
			palJobPut(16, kiki.palInfo->count, kiki.palInfo->data);
			draw = 0;
		}
		
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (ps & P1_START)
		{
			done = 1;
		}

		y++;

		if(y > 512)
		{
			y = 0;
		}

		if(checkHelp(HELP_HSCROLL))
			draw = 1;

		scrollerSetPos(&vertScroll, 0, y);
	}
}

void vt_gridscroll_test()
{
	int done = 0, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_STRIPES))
			draw = 1;
	}
}

void vt_horizontal_stripes()
{
	char cntstr[4];
	int done = 0, draw = 1, alternate = 0, field = 1, count = 0, docounter = 0;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			pictureInit(&image, &horzstripe, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,horzstripe.palInfo->count,horzstripe.palInfo->data);
			draw = 0;
		}

		if (alternate)
		{
			if (field == 0)
			{
				palJobPut(16,horzstripe_alt.palInfo->count,horzstripe_alt.palInfo->data);
				field = 1;
			}
			else
			{
				palJobPut(16,horzstripe.palInfo->count,horzstripe.palInfo->data);
				field = 0;
			}
		}

		if(docounter)
		{
			count++;

			if(count > 59)
				count = 0;

			intToStr(count, cntstr, 2);
			fixPrint(2, 25, 5, 4, "Frame:"); // Use font1 in fix bank 4 for solid background font - Fix palette 4
			fixPrint(8, 25, 5, 4, cntstr);   // Use font1 in fix bank 4 for solid background font - Fix palette 4
		}

		if (!alternate && (p1e & JOY_UP || p1e & JOY_DOWN))
		{
			if (field == 0)
			{
				palJobPut(16,horzstripe_alt.palInfo->count,horzstripe_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,horzstripe.palInfo->count,horzstripe.palInfo->data);
				field = 0;
			}
		}

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
			alternate = ~alternate;

		if (p1e & JOY_B)
			docounter = ~docounter;

		if (ps & P1_START)
			done = 1;

		if(checkHelp(HELP_STRIPES))
			draw = 1;
	}
}

void vt_vertical_stripes()
{
	char cntstr[4];
	int done = 0, draw = 1, alternate = 0, field = 1, count = 0, docounter = 0;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			pictureInit(&image, &vertstripe, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,vertstripe.palInfo->count,vertstripe.palInfo->data);
			draw = 0;
		}

		if (alternate)
		{
			if (field == 0)
			{
				palJobPut(16,vertstripe_alt.palInfo->count,vertstripe_alt.palInfo->data);
				field = 1;
			}
			else
			{
				palJobPut(16,vertstripe.palInfo->count,vertstripe.palInfo->data);
				field = 0;
			}
		}

		if(docounter)
		{
			count++;

			if(count > 59)
				count = 0;

			intToStr(count, cntstr, 2);
			fixPrint(2, 25, 5, 4, "Frame:"); // Use font1 in fix bank 4 for solid background font - Fix palette 4
			fixPrint(8, 25, 5, 4, cntstr); // Use font1 in fix bank 4 for solid background font - Fix palette 4
		}

		if (!alternate && (p1e & JOY_UP || p1e & JOY_DOWN))
		{
			if (field == 0)
			{
				palJobPut(16,vertstripe_alt.palInfo->count,vertstripe_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,vertstripe.palInfo->count,vertstripe.palInfo->data);
				field = 0;
			}
		}

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
			alternate = ~alternate;

		if (p1e & JOY_B)
			docounter = ~docounter;

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_STRIPES))
			draw = 1;
	}
}

void vt_checkerboard()
{
	char cntstr[4];
	int done = 0, draw = 1, alternate = 0, field = 1, count = 0, docounter = 0;
	picture image;

	while (!done)
	{
		SCClose();
		waitVBlank();
		if (draw)
		{
			gfxClear();
			pictureInit(&image, &check, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,check.palInfo->count,check.palInfo->data);
			draw = 0;
		}

		if (alternate)
		{
			if (field == 0)
			{
				palJobPut(16,check_alt.palInfo->count,check_alt.palInfo->data);
				field = 1;
			}
			else
			{
				palJobPut(16,check.palInfo->count,check.palInfo->data);
				field = 0;
			}
		}

		if(docounter)
		{
			count++;

			if(count > 59)
				count = 0;

			intToStr(count, cntstr, 2);
			fixPrint(2, 25, 5, 4, "Frame:"); // Use font1 in fix bank 4 for solid background font - Fix palette 4
			fixPrint(8, 25, 5, 4, cntstr); // Use font1 in fix bank 4 for solid background font - Fix palette 4
		}

		if (!alternate && (p1e & JOY_UP || p1e & JOY_DOWN))
		{
			if (field == 0)
			{
				palJobPut(16,check_alt.palInfo->count,check_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,check.palInfo->count,check.palInfo->data);
				field = 0;
			}
		}

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
			alternate = ~alternate;

		if (p1e & JOY_B)
			docounter = ~docounter;

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if(checkHelp(HELP_CHECK))
			draw = 1;
	}
}

void vt_backlitzone_test()
{
	int done = 0, block = 2, x = 160, y = 112, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			backgroundColor(0x0000);
			gfxClear();

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

		if(checkHelp(HELP_LED))
			draw = 1;
	}
}

void at_sound_test()
{
	int done = 0, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,back.palInfo->count,back.palInfo->data);
			draw = 0;
		}

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if(checkHelp(HELP_SOUND))
			draw = 1;
	}

}

void at_audiosync_test()
{
	int done = 0, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);
			draw = 0;
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

		if(checkHelp(HELP_AUDIOSYNC))
			draw = 1;
	}
}

void ht_controller_test()
{
	int done = 0;
	picture image;
	BYTE mvssel = 0, mvscredit = 0;

	gfxClear();

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

/*
CRC 32 based on work by Christopher Baker <https://christopherbaker.net>
*/

u32 _state = ~0L;

static const u32 crc32_table[] = {
	0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
	0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
	0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
	0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

void CRC32_reset()
{
	_state = ~0L;
}

void CRC32_update(u8 data)
{
	u8 tbl_idx = 0;

	tbl_idx = _state ^ (data >> (0 * 4));
	_state = (*(u32*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
	tbl_idx = _state ^ (data >> (1 * 4));
	_state = (*(u32*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
}

u32 CRC32_finalize()
{
	return ~_state;
}

/**********************************/

u32 CalculateCRC(u32 startAddress, u32 size)
{
	u8 *start = NULL;
	u32 address = 0, checksum = 0;
	CRC32_reset();

	start = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		u8 data;
		
		data = start[address];

		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

#define MAX_LOCATIONS 9

void ht_memory_viewer(u32 address)
{
	int done = 0, redraw = 1, docrc = 0, locpos = 1, i = 0, ascii = 0;
	u32 crc = 0, locations[MAX_LOCATIONS] = { 0, 0x100000, 0x10F300, 0x110000, 0x200000, 0x300000, 0x400000, 0x402000, 0xC00000 };

	backgroundColor(0x0000);
	gfxClear();

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

			clearFixLayer();

			if (docrc)
				crc = CalculateCRC(address, 0x1C0);

			intToHex(address, buffer, 8);
			fixPrint(32, 2, fontColorRed, 3, buffer);
			intToHex(address+448, buffer, 8);
			fixPrint(32, 29, fontColorRed, 3, buffer);

			if (docrc)
			{
				intToHex(crc, buffer, 8);
				fixPrint(32, 14, fontColorGreen, 3, buffer);
			}

			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < 16; j++)
				{
					if(!ascii)
					{
						intToHex(mem[i*16+j], buffer, 2);
						fixPrint(j*2, i, fontColorWhite, 3, buffer);
					}
					else
					{
						u8 c;
					
						memset(buffer, 0, sizeof(char)*10);
						c = mem[i*16+j];
						if(c >= 32 && c <= 126)	
						{
							buffer[0] = (char)c;			// ASCII range
							fixPrint(j*2, i, fontColorWhite, 3, buffer);
						}
					}
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

		if (p1e & JOY_C)
		{
			ascii = !ascii;
			redraw = 1;
		}

		if(checkHelp(HELP_MEMVIEW))
			redraw = 1;

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

	gfxClear();

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
		}
	}
}

typedef struct bios_data {
	u8		type;
	u32		crc;
	char	*name;
	char	*text;
} BIOSID;

#define BIOS_SNK_MVS	1
#define BIOS_SNK_AES	2
#define BIOS_HACK		3

const BIOSID bioslist[] = {
{	BIOS_HACK,
	0x465F5764,
	"uni-bios_4_0.rom",
	"Universe Bios (Hack, Ver. 4.0)" },
{	BIOS_HACK,
	0x2BB7B46A,
	"uni-bios_3_3.rom",
	"Universe Bios (Hack, Ver. 3.3)" },
{	BIOS_HACK,
	0xEAD2EFFE,
	"uni-bios_3_2.rom",
	"Universe Bios (Hack, Ver. 3.2)" },
{	BIOS_HACK,
	0xBA8F4B1E,
	"uni-bios_3_1.rom",
	"Universe Bios (Hack, Ver. 3.1)" },
{	BIOS_HACK,
	0xCB54AAD7,
	"uni-bios_3_0.rom",
	"Universe Bios (Hack, Ver. 3.0)" },
{	BIOS_HACK,
	0xE5224EBD,
	"uni-bios_2_3.rom",
	"Universe Bios (Hack, Ver. 2.3)" },
{	BIOS_HACK,
	0x3775739E,
	"uni-bios_2_3o.rom",
	"Universe Bios (Hack, Ver. 2.3 - Alt)" },
{	BIOS_HACK,
	0xC6F8AC92,
	"uni-bios_2_2.rom",
	"Universe Bios (Hack, Ver. 2.2)" },
{	BIOS_HACK,
	0xF341E486,
	"uni-bios_2_1.rom",
	"Universe Bios (Hack, Ver. 2.1)" },
{	BIOS_HACK,
	0x406F79B2,
	"uni-bios_2_0.rom",
	"Universe Bios (Hack, Ver. 2.0)" },
{	BIOS_HACK,
	0xD8A97133,
	"uni-bios_1_3.rom",
	"Universe Bios (Hack, Ver. 1.3)" },
{	BIOS_HACK,
	0x6C4BACD6,
	"uni-bios_1_2.rom",
	"Universe Bios (Hack, Ver. 1.2)" },
{	BIOS_HACK,
	0xED81E4EB,
	"uni-bios_1_2o.rom",
	"Universe Bios (Hack, Ver. 1.2 - Alt)" },
{	BIOS_HACK,
	0xAFBC316A,
	"uni-bios_1_1.rom",
	"Universe Bios (Hack, Ver. 1.1)" },
{	BIOS_HACK,
	0x9DE9D5F1,
	"uni-bios_1_0.rom",
	"Universe Bios (Hack, Ver. 1.0)" },
{	BIOS_HACK,
	0xEA57D3AD,
	"neopen.sp1",
	"NeopenBios v0.1" },
{	BIOS_SNK_MVS,
	0xEE4E56EF,
	"vs-bios.rom",
	"Japan MVS (Ver. 3)" },
{	BIOS_SNK_MVS,
	0x66BC1D26,
	"sp-j2.sp1",
	"Japan MVS (Ver. 2)" },
{	BIOS_SNK_MVS,
	0xC00A0476,
	"sp1.jipan.1024",
	"Japan MVS (Ver. 1)" },
{	BIOS_SNK_MVS,
	0x6893A277,
	"japan-j3.bin",
	"Japan MV1B" },
{	BIOS_SNK_MVS,
	0xF1E44B08,
	"sp1-j3.bin",
	"Japan MVS (J3, alt)" },
{	BIOS_SNK_MVS,
	0x4C747A4D,
	"sp-j3.sp1",
	"Japan NEO-MVH MV1C" },
{	BIOS_SNK_MVS,
	0x15192F9F,
	"sp-s2.sp1",
	"Eurpoe MVS (Ver. 2)" },
{	BIOS_SNK_MVS,
	0x7E65EA24,
	"sp-s.sp1",
	"Eurpoe MVS (Ver. 1)" },
{	BIOS_SNK_MVS,
	0xEFD21CD4,
	"sp-45.sp1",
	"Asia NEO-MVH MV1C" },
{	BIOS_SNK_MVS,
	0xCD0F00E7,
	"sp-s3.sp1",
	"Asia MV1B" },
{	BIOS_SNK_MVS,
	0xCAB95DE9,
	"sp-u2.sp1",
	"US MVS (Ver. 2?)" },
{	BIOS_SNK_MVS,
	0xB907061C,
	"sp-e.sp1",
	"US MVS (Ver. 1)" },
{	BIOS_SNK_MVS,
	0xE86773D2,
	"sp1-u2",
	"US MVS (4 slot, Ver 2)" },
{	BIOS_SNK_MVS,
	0xCB2E44A4,
	"sp1-u4.bin",
	"US MVS (U4)" },
{	BIOS_SNK_MVS,
	0x8F5EBA5E,
	"sp1-u3.bin",
	"US MVS (U3)" },
{	BIOS_SNK_MVS,
	0xF7C94873,
	"sp-1v1_3db8c.bin",
	"Custom Japanese Hotel" },
{	BIOS_SNK_AES,
	0xF481E11,
	"neo-po.bin",
	"Japan AES" },
{	BIOS_SNK_AES,
	0x2C50CBCA,
	"neo-epo.bin",
	"Export AES" },
{	BIOS_SNK_AES,
	0x7A0D4410,
	"neodebug.rom",
	"Development System ROM" },
{	0, 0, NULL, NULL } };

// search known BIOS
const BIOSID *GetBIOSbyCRC(u32 checksum)
{
	u8 i = 0;
	
	while(bioslist[i].crc != 0)
	{		
		if(checksum == bioslist[i].crc)
			return &bioslist[i];
		i++;
	}
	return NULL;
}

void cleanBIOSStr(char *str, u8 size)
{
	u8 i = 0;

	for(i = 0; i < size; i++) {
		u8 c;

		c = str[i];
		if(c >= 32 && c <= 126)	{ // ASCII range
			str[i] = (char)c;
		} else {
			str[i] = ' ';
		}
	}
}

void byteSwap(u8 *data, u8 size)
{
	int pos = 0;

	while(pos < size)	
	{
		u8 t;

		t = data[pos];
		data[pos] = data[pos+1];
		data[pos+1] = t;
		pos += 2;
	}
}

void displayBIOS(u32 address, u8 swap)
{
	int  line = 0;
	char buffer[34];

	for(line = 0; line < 4; line++)
	{
		memcpy(buffer, (void*)(address+0x82+line*32), 32);
		if(swap)
			byteSwap(buffer, 32);
		buffer[32] = '\0';
		cleanBIOSStr(buffer, 32);
		fixPrintf(4, 10+line, 2, 3, buffer);
	}
}

void ht_check_ng_bios_crc(u32 address)
{
	int				done = 0, swap = 0;
	u32				crc = 0;
	picture			image;
	char			buffer[34];
	const BIOSID 	*bios = NULL;

	gfxClear();

	// Print BIOS lines
	displayBIOS(address, swap);

	fixPrintf(12, 16, 2, 3, "Please Wait...");
	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	SCClose();
	waitVBlank();

	crc = CalculateCRC(address, BIOS_SIZE);
	intToHex(crc, buffer, 8);
	fixPrintf(12, 16, 2, 3, "CRC:  ");
	fixPrintf(17, 16, 0, 3, "0x%s ", buffer);

	bios = GetBIOSbyCRC(crc);
	if(bios)
	{
		fixPrintf(6, 18, 0, 3, bios->name);
		fixPrintf(6, 19, 0, 3, bios->text);
	}
	else
	{
		fixPrintf(14, 18, 0, 3, "Unknown BIOS");
		fixPrintf(14, 19, 0, 3, "Please report it");
	}

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
		}

		if (p1e & JOY_C)
		{
			swap = !swap;
			displayBIOS(address, swap);
		}
	}
}
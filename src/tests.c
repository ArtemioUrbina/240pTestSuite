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
#include "tools.h"
#include "sound.h"

static const ushort wFallPal_1[] = {
	0x8000, 0x0200, 0x6840, 0x8720, 0x879b, 0x179f, 0x3bdf, 0x58bf, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};

static const ushort wFallPal_2[] = {
	0x8000, 0x0200, 0x6840, 0x8720, 0x179f, 0x879b, 0x3bdf, 0x58bf, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};

// unused
static const ushort wFallPal_3[] = {
	0x8000, 0x0200, 0x6840, 0x8720, 0x879b, 0x3bdf, 0x58bf, 0x179f, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};

static const ushort wFallPal_4[] = {
	0x8000, 0x0200, 0x6840, 0x8720, 0x3bdf, 0x58bf, 0x179f, 0x879b, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};

//unused
static const ushort waterPal_1[] = {
	0x8000, 0x879b, 0x3bdf, 0x58bf, 0x929d, 0x179f, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};
static const ushort waterPal_2[] = {
	0x8000, 0x179f, 0x879b, 0x3bdf, 0x929d, 0x58bf, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};
static const ushort waterPal_3[] = {
	0x8000, 0x58bf, 0x179f, 0x879b, 0x929d, 0x3bdf, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};
static const ushort waterPal_4[] = {
	0x8000, 0x3bdf, 0x58bf, 0x179f, 0x929d, 0x879b, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	};

void vt_drop_shadow_test()
{
	int done = 0, x = 30, y = 30, draw = 1, spr_type = 0, changeSprite = 1, changeBack = 1, back = 0;
	int pal_water = 0, pal_wfall = 0, text = 0, evenframes = 0;
	char flip = FLIP_NONE;
	int drawshadow = 0, frame = 1;
	short y1 = -96;
	short y2 = -152;
	short y3 = 0;
	picture donna_back;
	picture slug_sprite;
	picture slug_shadow_sprite;
	picture shape_sprite;
	scroller backScroll, waterScroll, frontScroll;

	while (!done)
	{	
		if (draw)
		{
			int palindex = 16, sprindex = 1;
			int pali_donna, pali_slug, pali_slug_shadow, pali_shape;
			int spri_donna, spri_slug, spri_slug_shadow, spri_shape;

			gfxClear();

			// load scrollers, all sprite priority based
			scrollerInit(&backScroll, &sonic_back, sprindex, palindex, 0, y1);
			palJobPut(palindex, sonic_back.palInfo->count, sonic_back.palInfo->data);
			pal_wfall = palindex + 1;
			pal_water = pal_wfall + 1;
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			scrollerInit(&waterScroll, &sonic_water, sprindex, palindex, 0, y2);
			palJobPut(palindex, sonic_water.palInfo->count, sonic_water.palInfo->data);
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			scrollerInit(&frontScroll, &sonic_floor, sprindex, palindex, 0, y3);
			palJobPut(palindex, sonic_floor.palInfo->count, sonic_floor.palInfo->data);
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			// load palettes
			pali_donna = palindex;
			palJobPut(pali_donna,donna.palInfo->count,donna.palInfo->data);
			palindex += donna.palInfo->count;

			pali_slug = palindex;
			palJobPut(pali_slug,slug.palInfo->count,slug.palInfo->data);
			palindex += slug.palInfo->count;

			pali_slug_shadow = palindex;
			palJobPut(pali_slug_shadow,slug_shadow.palInfo->count,slug_shadow.palInfo->data);
			palindex += slug_shadow.palInfo->count;

			pali_shape = palindex;
			palJobPut(pali_shape,shape_shadow.palInfo->count,shape_shadow.palInfo->data);

			// load sprites
			spri_donna = sprindex;
			pictureInit(&donna_back, &donna, spri_donna, pali_donna, 0, isPAL || isPALinMVS ? -16 : -32, FLIP_NONE);
			sprindex += getPicSprites(donna_back.info);

			spri_slug_shadow = sprindex;
			pictureInit(&slug_shadow_sprite, &slug_shadow, spri_slug_shadow, pali_slug_shadow, x, y, flip);
			sprindex += getPicSprites(slug_shadow_sprite.info);

			spri_slug = sprindex;
			pictureInit(&slug_sprite, &slug, spri_slug, pali_slug, x-20, y-20, flip);
			sprindex += getPicSprites(slug_sprite.info);

			spri_shape = sprindex;
			pictureInit(&shape_sprite, &shape_shadow, spri_shape, pali_shape, x, y, FLIP_NONE);

			changeSprite = 1;
			changeBack = 1;
			draw = 0;
		}

		if (changeBack)
		{
			switch(back)
			{
				case 0:
					pictureShow(&donna_back);
					break;
				case 1:
					pictureHide(&donna_back);
					break;
			}
			changeBack = 0;
		}

		if (changeSprite)
		{
			if (!spr_type) {
				pictureShow(&slug_sprite);
				pictureShow(&slug_shadow_sprite);
				pictureHide(&shape_sprite);
			} else {
				pictureHide(&slug_shadow_sprite);
				pictureHide(&slug_sprite);
				pictureShow(&shape_sprite);
			}
			changeSprite = 0;
		}

		if (!spr_type)
		{
			if (drawshadow) {
				pictureSetPos(&slug_shadow_sprite, x, y);
				pictureShow(&slug_shadow_sprite);
			} else {
				pictureHide(&slug_shadow_sprite);
			}
			pictureSetPos(&slug_sprite, x-20, y-20);
		} else {
			if (drawshadow) {
				pictureSetPos(&shape_sprite, x-20, y-20);
				pictureShow(&shape_sprite);
			} else {
				pictureHide(&shape_sprite);
			}
		}

		if (back == 1)
		{
			switch (frame)
			{
			case 30:
				palJobPut(pal_water, 1, waterPal_2);
				palJobPut(pal_wfall, 1, wFallPal_1);
			break;
			case 60:
				palJobPut(pal_water, 1, waterPal_3);
				palJobPut(pal_wfall, 1, wFallPal_2);
			break;
			case 90:
				palJobPut(pal_water, 1, waterPal_4);
				palJobPut(pal_wfall, 1, wFallPal_4);
			break;
			}
			frame++;
			if (frame > 90)
				frame = 1;

			scrollerSetPos(&frontScroll, x, y1);
			scrollerSetPos(&waterScroll, x/2, y2);
			scrollerSetPos(&backScroll, x/2, y3);
		}

		if (text)
		{
			text --;
			if(!text)
				suiteClearFixLayer();
		}

		// Only display vestigial info if debug dip 1 is ON
		if (bkp_data.debug_dip1 & DP_DEBUG1)
		{
			fixPrintf(10, 16, fontColorWhite, 3, "X:    %04d", x);
			fixPrintf(10, 17, fontColorWhite, 3, "Y:    %04d", y);
			fixPrintf(10, 18, fontColorWhite, 3, "Spr:  %04d", spr_type);
			fixPrintf(10, 19, fontColorWhite, 3, "Back: %04d", back);
		}

		SCClose();
		waitVBlank();

		readController();

		drawshadow = !drawshadow;

		if (PRESSED_A)
		{
			back ++;
			if(back > 1)
				back = 0;
			changeBack = 1;
		}

		// change between even and odd frames
		if (PRESSED_B)
		{
			drawshadow = !drawshadow;
			evenframes = !evenframes;
			fixPrintf(16, 3, fontColorGreen, 3, "Shadow in %s frames ", evenframes ? "even" : "odd" );
			text = 120;
		}

		if (PRESSED_C)
		{
			spr_type = !spr_type;
			changeSprite = 1;
		}

		if (HELD_UP)
		{
			y--;
			if(y < 0)
				y = 0;
		}

		if (HELD_DOWN)
		{
			y++;
			if(y > 192)
				y = 192;
		}

		if (HELD_LEFT)
		{
			x--;
			if (flip != FLIP_X && !spr_type)
			{
				flip = FLIP_X;
				pictureSetFlip(&slug_sprite, flip);
				pictureSetFlip(&slug_shadow_sprite, flip);
			}
			if (x < 0)
				x = 0;
		}

		if (HELD_RIGHT)
		{
			x++;
			if (flip != FLIP_NONE && !spr_type)
			{
				flip = FLIP_NONE;
				pictureSetFlip(&slug_sprite, flip);
				pictureSetFlip(&slug_shadow_sprite, flip);
			}
			if (x > 288)
				x = 288;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SHADOW))
			draw = 1;
	}
}

void vt_striped_sprite_test()
{
	int done = 0, x = 30, y = 30, draw = 1, changeBack = 1, back = 0, frame = 1;
	int pal_water = 0, pal_wfall = 0;
	short y1 = -96;
	short y2 = -152;
	short y3 = 0;
	picture donna_back;
	picture sprite;
	scroller backScroll, waterScroll, frontScroll;

	while (!done)
	{
		if (draw)
		{
			int palindex = 16, sprindex = 1;
			int pali_donna;

			gfxClear();

			// load scrollers, all sprite priority based
			scrollerInit(&backScroll, &sonic_back, sprindex, palindex, 0, y1);
			palJobPut(palindex, sonic_back.palInfo->count, sonic_back.palInfo->data);
			pal_wfall = palindex + 1;
			pal_water = pal_wfall + 1;
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			scrollerInit(&waterScroll, &sonic_water, sprindex, palindex, 0, y2);
			palJobPut(palindex, sonic_water.palInfo->count, sonic_water.palInfo->data);
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			scrollerInit(&frontScroll, &sonic_floor, sprindex, palindex, 0, y3);
			palJobPut(palindex, sonic_floor.palInfo->count, sonic_floor.palInfo->data);
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			// Palettes
			pali_donna = palindex;
			palJobPut(palindex,donna.palInfo->count,donna.palInfo->data);
			palindex += donna.palInfo->count;
			palJobPut(palindex,marker_striped.palInfo->count,marker_striped.palInfo->data);

			// Tiles
			pictureInit(&donna_back, &donna, sprindex, pali_donna, 0, isPAL || isPALinMVS ? -16 : -32, FLIP_NONE);
			sprindex += getPicSprites(donna_back.info);
			pictureInit(&sprite, &marker_striped, sprindex, palindex, x, y, FLIP_NONE);

			changeBack = 1;
			draw = 0;
		}

		if(changeBack)
		{
			switch(back)
			{
				case 0:
					pictureShow(&donna_back);
					break;
				case 1:
					pictureHide(&donna_back);
					break;
			}
			changeBack = 0;
		}

		if (back == 1)
		{
			switch (frame)
			{
			case 30:
				palJobPut(pal_water, 1, waterPal_2);
				palJobPut(pal_wfall, 1, wFallPal_1);
			break;
			case 60:
				palJobPut(pal_water, 1, waterPal_3);
				palJobPut(pal_wfall, 1, wFallPal_2);
			break;
			case 90:
				palJobPut(pal_water, 1, waterPal_4);
				palJobPut(pal_wfall, 1, wFallPal_4);
			break;
			}
			frame++;
			if (frame > 90)
				frame = 1;

			scrollerSetPos(&frontScroll, x, y1);
			scrollerSetPos(&waterScroll, x/2, y2);
			scrollerSetPos(&backScroll, x/2, y3);
		}

		// Only display vestigial info if debug dip 1 is ON
		if (bkp_data.debug_dip1 & DP_DEBUG1)
		{
			fixPrintf(10, 16, fontColorWhite, 3, "X:    %04d", x);
			fixPrintf(10, 17, fontColorWhite, 3, "Y:    %04d", y);
			fixPrintf(10, 18, fontColorWhite, 3, "Back: %04d", back);
		}

		pictureSetPos(&sprite, x, y);

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			back ++;
			if(back > 1)
				back = 0;
			changeBack = 1;
		}

		if (HELD_UP)
		{
			y--;
			if(y < 0)
				y = 0;
		}

		if (HELD_DOWN)
		{
			y++;
			if(y > 192)
				y = 192;
		}

		if (HELD_LEFT)
		{
			x--;
			if (x < 0)
				x = 0;
		}

		if (HELD_RIGHT)
		{
			x++;
			if (x > 288)
				x = 288;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_STRIPED))
			draw = 1;
	}
}

void vt_lag_test()
{
	u16 done = 0, draw = 1, pause = 0, cposx = 32, cposy = 17, lsd, msd;
	int frames = 0, seconds = 0, minutes = 0, hours = 0, framecnt = 1;
	const pictureInfo *numbers[10] = {&num_0, &num_1, &num_2, &num_3, &num_4, &num_5, &num_6, &num_7, &num_8, &num_9};
	picture image, circle, c_numbers, bar, h1, h2, m1, m2, s1, s2, f1, f2;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			backgroundColor(0xDfff);

			palJobPut(16, num_0.palInfo->count, num_0.palInfo->data);
			palJobPut(17, circle_blue.palInfo->count, circle_blue.palInfo->data);
			palJobPut(18, num_0_w.palInfo->count, num_0_w.palInfo->data);
			palJobPut(19, circle_red.palInfo->count, circle_red.palInfo->data);
			palJobPut(20, bar_l.palInfo->count, bar_l.palInfo->data);
			draw = 0;
		}

		pictureInit(&circle, &circle_blue, 65, framecnt == 1 ? 19 : 17, 12, 68, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 81, framecnt == 2 ? 19 : 17, 92, 68, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 97, framecnt == 3 ? 19 : 17, 172, 68, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 113, framecnt == 4 ? 19 : 17, 248, 68, FLIP_NONE);

		pictureInit(&circle, &circle_blue, 129, framecnt == 5 ? 19 : 17, 12, 132, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 145, framecnt == 6 ? 19 : 17, 92, 132, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 161, framecnt == 7 ? 19 : 17, 172, 132, FLIP_NONE);
		pictureInit(&circle, &circle_blue, 177, framecnt == 8 ? 19 : 17, 248, 132, FLIP_NONE);

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

		pictureInit(&bar, &bar_l, 223, framecnt % 2 == 0 ? 18 : 20, 0, 0, FLIP_NONE);
		pictureInit(&bar, &bar_l, 237, framecnt % 2 == 0 ? 18 : 20, 304, 0, FLIP_X);

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

		fixPrint(4, 3, fontColorBlack, 3, "hours");
		fixPrint(13, 3, fontColorBlack, 3, "minutes");
		fixPrint(22, 3, fontColorBlack, 3, "seconds");
		fixPrint(31, 3, fontColorBlack, 3, "frames");

		readController();

		if (framecnt > 8)
			framecnt = 1;

		if (framecnt > 4)
		{
			cposx = framecnt - 4;
			cposy = 17;
		} else {
			cposx = framecnt;
			cposy = 9;
		}
		cposx = (cposx - 1) * 10 + 2;

		if (!pause)
		{
			frames ++;
			framecnt ++;
			if (framecnt > 8)
				framecnt = 1;
		}

		if (isPAL)
		{
			if (frames > 49)
			{
				frames = 0;
				seconds ++;
			}
		} else {
			if (frames > 59)
			{
				frames = 0;
				seconds ++;
			}
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

		if (PRESSED_A)
			pause = !pause;

		if (PRESSED_B && pause)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 1;
		}

		if (PRESSED_START)
		{
			clearSprites(1,260);
			done = 1;
		}

		if (checkHelp(HELP_LAG))
		{
			clearSprites(1,MAX_SPRITES);
			draw = 1;
		}
	}
}

void vt_reflex_test()
{
	char str[10];
	short speed = 1, vary = 0, clicks[10], resetbg = 0;
	u16 pal = 0x0000, change = 1, loadvram = 1, psgoff = 0, usersound = 0;
	u16 x = 0, y = 0, x2 = 0, y2 = 0, done = 0, variation = 1, draw = 1;
	u16 pos = 0, view = 0, audio = 1, drawoffset = 0;
	picture marker1, marker2, marker3, background;

	x = 144, y = 60, x2 = 108, y2 = 96;

	while (!done)
	{
		palJobPut(16,marker.palInfo->count,marker.palInfo->data);
		SCClose();
		waitVBlank();

		readController();

		if (loadvram)
		{
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
		
		if (resetbg)
		{
			backgroundColor(0x8000);
			resetbg = 0;
		}

		if (y == 96)	//  Screen Flash
		{
			backgroundColor(0x8555);
			resetbg = 1;
		}
		
		if (usersound)
		{
			// Todo: turn sound off here
			usersound = 0;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_MANUALLAG))
			loadvram = 1;

		if (PRESSED_A)
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

		if (PRESSED_B)
		{
			view++;
			if (view > 2)
				view = 0;
		}

		if (PRESSED_C)
		{
			audio = !audio;
			draw = 1;
		}

		if (PRESSED_DOWN)
		{
			variation = !variation;
			if (!variation)
				vary = 0;
			draw = 1;
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
			backgroundColor(0x8555);
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
		suiteClearFixLayer();
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
			fix32 framerate = isPAL ? FIX32(20.1196) : FIX32(16.6884);
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

			readController();

			if (PRESSED_B || PRESSED_START)
				done = 1;
		}
	}
}

void vt_scroll_test()
{
	int done = 0, changed = 1, reload = 1, frame = 1, vertical = 0, pause = 0, acc = 1;
	short x1 = 0, y1 = -96;
	short x2 = 0, y2 = -152;
	short x3 = 0, y3 = 0;
	short xvert[3] = { 96, 64, 32 }, currxvert = 1, y = 0;
	int pal_water = 0, pal_wfall = 0;
	scroller backScroll, waterScroll, frontScroll, vertScroll;

	while (!done)
	{
		if (reload)
		{
			int palindex = 16, sprindex = 1;

			gfxClear();

			scrollerInit(&backScroll, &sonic_back, sprindex, palindex, x3, y3);
			palJobPut(palindex, sonic_back.palInfo->count, sonic_back.palInfo->data);
			pal_wfall = palindex + 1;
			pal_water = pal_wfall + 1;
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			scrollerInit(&waterScroll, &sonic_water, sprindex, palindex, x2, y2);
			palJobPut(palindex, sonic_water.palInfo->count, sonic_water.palInfo->data);
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;

			scrollerInit(&frontScroll, &sonic_floor, sprindex, palindex, x1, y1);
			palJobPut(palindex, sonic_floor.palInfo->count, sonic_floor.palInfo->data);
			sprindex += SCROLLER_SIZE;
			palindex += backScroll.info->palInfo->count;
			scrollerInit(&vertScroll, &kiki, sprindex, palindex, xvert[currxvert], y3);
			palJobPut(palindex, kiki.palInfo->count, kiki.palInfo->data);

			changed = 1;
			reload = 0;
		}
		
		if (changed)
		{
			gfxClear();
			changed = 0;
		}

		if (!vertical)
		{
			switch (frame)
			{
			case 30:
				palJobPut(pal_water, 1, waterPal_2);
				palJobPut(pal_wfall, 1, wFallPal_1);
			break;
			case 60:
				palJobPut(pal_water, 1, waterPal_3);
				palJobPut(pal_wfall, 1, wFallPal_2);
			break;
			case 90:
				palJobPut(pal_water, 1, waterPal_4);
				palJobPut(pal_wfall, 1, wFallPal_4);
			break;
			}

			if (x1 >=  0)
				scrollerSetPos(&frontScroll, x1, y1);
			else
				scrollerSetPos(&frontScroll, 256+x1, y1);
			if (x2 >= 0)
				scrollerSetPos(&waterScroll, x2/2, y2);
			else
				scrollerSetPos(&waterScroll, 256+x2/2, y2);
			if (x3 >= 0)
				scrollerSetPos(&backScroll, x3/2, y3);
			else
				scrollerSetPos(&backScroll, 256+x3/2, y3);
			if (!pause)
			{
				x1+=acc;
				x2+=acc;
				x3+=acc;
			}
			if (x1 >= 256) x1 -= 256;
			if (x2 >= 512) x2 -= 512;
			if (x3 >= 512) x3 -= 512;
			if (x1 <= -256) x1 += 256;
			if (x2 <= -512) x2 += 512;
			if (x3 <= -512) x3 += 512;
		} else {
			scrollerSetPos(&vertScroll, xvert[currxvert], y);
			if (!pause) 	y+=acc;
			if (y > 512) y = 0;
			if (y < -512) y += 512;
		}

		// Only display vestigial info if debug dip 1 is ON
		if (bkp_data.debug_dip1 & DP_DEBUG1)
		{
			fixPrintf(10, 16, fontColorWhite, 3, "X1:   %04d", x1);
			fixPrintf(10, 17, fontColorWhite, 3, "X2:   %04d", x2);
			fixPrintf(10, 18, fontColorWhite, 3, "X3:   %04d", x3);
			fixPrintf(10, 19, fontColorWhite, 3, "XV:   %04d", xvert[currxvert]);
			fixPrintf(10, 20, fontColorWhite, 3, "Y:    %04d", y);
			fixPrintf(10, 21, fontColorWhite, 3, "acc:  %04d", acc);
		}

		SCClose();
		waitVBlank();

		frame++;
		if (frame > 90)
			frame = 1;

		readController();

		if (PRESSED_A)
			pause = !pause;

		if (PRESSED_B)
		{
			vertical = !vertical;
			pause = 0;
			changed = 1;
		}

		if (PRESSED_C)
			acc *= -1;

		if (PRESSED_UP)
		{
			acc++;
			if (acc >= 20)
				acc = 20;
			if(acc == 0)
				acc = 1;
		}

		if (PRESSED_DOWN)
		{
			acc--;
			if (acc <= -20)
				acc = -20;
			if(acc == 0)
				acc = -1;
		}

		if (vertical)
		{
			if (PRESSED_LEFT)
				currxvert --;

			if (PRESSED_RIGHT)
				currxvert ++;

			if (currxvert > 2)
				currxvert = 2;
			if (currxvert < 0)
				currxvert = 0;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_HSCROLL))
			reload = 1;
	}
}

void vt_gridscroll_test()
{
	int done = 0, draw = 1, x = 0, y = 0;
	int acc = 1, pause = 0, direction = 0, horizontal = 0;
	scroller grid;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			scrollerInit(&grid, &scroll_grid, 1, 16, 0, 0);
			palJobPut(16, scroll_grid.palInfo->count, scroll_grid.palInfo->data);
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (!horizontal)
		{
			if (!pause) {
				if (!direction) {
					y += (y * acc);
					scrollerSetPos(&grid, x, y);
				} else {
					y -= (y * acc);
					scrollerSetPos(&grid, x, y);
				}
			}
		} else {
			if (!pause) {
				if (!direction) {
					x += (x * acc);
					scrollerSetPos(&grid, x, y);
				} else {
					x -= (x * acc);
					scrollerSetPos(&grid, x, y);
				}
			}
		}

		if (PRESSED_A)
			pause = !pause;

		if (PRESSED_B)
			horizontal = !horizontal;

		if (PRESSED_LEFT)
			direction = !direction;

		if (PRESSED_UP)
		{
			acc++;
			if (acc == 10)
				acc = 10;
		}

		if (PRESSED_DOWN)
		{
			acc--;
			if (acc == 1)
				acc = 1;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_STRIPES))
			draw = 1;

		if(x > 31) x = 0;
		//if(y > 31) y = 0;
		x++;
		//x += (x * acc);
		scrollerSetPos(&grid, x, y);
	}
}

void vt_horizontal_stripes()
{
	int done = 0, draw = 1, alternate = 0, field = 1, frames = 0, docounter = 0;
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
			if (field == 0) {
				palJobPut(16,horzstripe_alt.palInfo->count,horzstripe_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,horzstripe.palInfo->count,horzstripe.palInfo->data);
				field = 0;
			}
		}

		if (docounter)
		{
			frames++;
			if (isPAL)
			{
				if (frames > 49)
					frames = 0;
			} else {
				if (frames > 59)
					frames = 0;
			}

			fixPrintf(2, 25, fontColorSolid, 4, "Frame: %02d", frames); // Use font1 in fix bank 4 for solid background font - Fix palette 4
		}

		if (!alternate && (PRESSED_UP || PRESSED_DOWN))
		{
			if (field == 0) {
				palJobPut(16,horzstripe_alt.palInfo->count,horzstripe_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,horzstripe.palInfo->count,horzstripe.palInfo->data);
				field = 0;
			}
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
			alternate = ~alternate;

		if (PRESSED_B)
		{
			docounter = ~docounter;
			if (!docounter)
				suiteClearFixLayer();
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_STRIPES))
			draw = 1;
	}
}

void vt_vertical_stripes()
{
	int done = 0, draw = 1, alternate = 0, field = 1, frames = 0, docounter = 0;
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
			if (field == 0) {
				palJobPut(16,vertstripe_alt.palInfo->count,vertstripe_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,vertstripe.palInfo->count,vertstripe.palInfo->data);
				field = 0;
			}
		}

		if (docounter)
		{
			frames++;
			if (isPAL)
			{
				if (frames > 49)
					frames = 0;
			} else {
				if (frames > 59)
					frames = 0;
			}

			fixPrintf(2, 25, fontColorSolid, 4, "Frame: %02d", frames); // Use font1 in fix bank 4 for solid background font - Fix palette 4
		}

		if (!alternate && (PRESSED_UP || PRESSED_DOWN))
		{
			if (field == 0) {
				palJobPut(16,vertstripe_alt.palInfo->count,vertstripe_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,vertstripe.palInfo->count,vertstripe.palInfo->data);
				field = 0;
			}
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
			alternate = ~alternate;

		if (PRESSED_B)
		{
			docounter = ~docounter;
			if (!docounter)
				suiteClearFixLayer();
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_STRIPES))
			draw = 1;
	}
}

void vt_checkerboard()
{
	int done = 0, draw = 1, alternate = 0, field = 1, frames = 0, docounter = 0;
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
			if (field == 0) {
				palJobPut(16,check_alt.palInfo->count,check_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,check.palInfo->count,check.palInfo->data);
				field = 0;
			}
		}

		if (docounter)
		{
			frames++;
			if (isPAL)
			{
				if (frames > 49)
					frames = 0;
			} else {
				if (frames > 59)
					frames = 0;
			}

			fixPrintf(2, 25, fontColorSolid, 4, "Frame: %02d", frames); // Use font1 in fix bank 4 for solid background font - Fix palette 4
		}

		if (!alternate && (PRESSED_UP || PRESSED_DOWN))
		{
			if (field == 0) {
				palJobPut(16,check_alt.palInfo->count,check_alt.palInfo->data);
				field = 1;
			} else {
				palJobPut(16,check.palInfo->count,check.palInfo->data);
				field = 0;
			}
		}

		readController();

		if (PRESSED_A)
			alternate = ~alternate;

		if (PRESSED_B)
		{
			docounter = ~docounter;
			if(!docounter)
				suiteClearFixLayer();
		}

		if (PRESSED_START)
		{
			done = 1;
			suiteClearFixLayer();
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

		readController();

		if (PRESSED_A)
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

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_LED))
			draw = 1;
	}
}

void at_sound_test()
{
	int done = 0, draw = 1, sel = 0, adpcmb_sel = 2;
	int option = 1, change = 0, changeoption = 0;
	char *adpcmb_rates[] = { "11025", "16538", "22050", "27563", "33075", "38588", "44100", "55125"  };
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

		fixPrint(15, 8, fontColorGreen, 3, "Sound Test");

		fixPrint(16, 11, fontColorGreen, 3, "ADPCM-A");
		fixPrint(12, 12, sel == 0 && option == 0 ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(17, 12, sel == 0 && option == 1 ? fontColorRed : fontColorWhite, 3, "Center");
		fixPrint(24, 12, sel == 0 && option == 2 ? fontColorRed : fontColorWhite, 3, "Right");

		fixPrint(16, 14, fontColorGreen, 3, "ADPCM-B");
		fixPrintf(16, 15, fontColorWhite, 3, "C:%s", adpcmb_rates[adpcmb_sel]);
		fixPrint(12, 16, sel == 1 && option == 0 ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(17, 16, sel == 1 && option == 1 ? fontColorRed : fontColorWhite, 3, "Center");
		fixPrint(24, 16, sel == 1 && option == 2 ? fontColorRed : fontColorWhite, 3, "Right");

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_LEFT)
		{
			option --;
			changeoption = 1;
		}

		if (PRESSED_RIGHT)
		{
			option ++;
			changeoption = 1;
		}

		if (PRESSED_UP)
		{
			sel --;
			change = 1;
		}

		if (PRESSED_DOWN)
		{
			sel++;
			change = 1;
		}

		if(change)
		{
			if(sel < 0)
				sel = 1;
			if (sel > 1)
				sel = 0;
			switch(sel)
			{
			case 0:
				option = 1;
				break;
			case 1:
				option = 1;
				break;
			}
			change = 0;
		}

		if(changeoption)
		{
			if(sel == 0)
			{
				if(option < 0)
					option = 2;
				if(option > 2)
					option = 0;
			}
			if(sel == 1)
			{
				if(option < 0)
					option = 3;
				if(option > 3)
					option = 0;
			}
			changeoption = 0;
		}

		if (PRESSED_A)
		{
			if(sel == 0)
			{
				switch(option)
				{
				case 0:
					playSound(SOUNDCMD_PlayLeft);
					break;
				case 1:
					playSound(SOUNDCMD_PlayCenter);
					break;
				case 2:
					playSound(SOUNDCMD_PlayRight);
					break;
				}
			}

			if (sel == 1)
			{
				switch(option)
				{
				case 0:
					playSound(SOUNDCMD_ADPCMB_Left);
					break;
				case 1:
					playSound(SOUNDCMD_ADPCMB_Center);
					break;
				case 2:
					playSound(SOUNDCMD_ADPCMB_Right);
					break;
				}
			}
		}

		if(PRESSED_C)
		{
			if(sel == 1)
			{
				adpcmb_sel ++;
				if(adpcmb_sel > 7)
					adpcmb_sel = 0;
				playSound(SOUNDCMD_RateB_0+adpcmb_sel);
			}
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SOUND))
			draw = 1;
	}
}

void executePulseTrain()
{
	int frame = 0;

	for(frame = 0; frame < 10; frame++)
	{
		playSound(SOUNDCMD_SSGPulseStart);
		waitVBlank();
		playSound(SOUNDCMD_SSGPulseStop);
		waitVBlank();
	}
}

void executeSilence()
{
	int frame = 0;

	for(frame = 0; frame < 20; frame++)
		waitVBlank();
}

void at_sound_mdfourier()
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

		readController();

		if (PRESSED_A)
		{
			int frame = 0;

			playSound(SOUNDCMD_SSGRampinit);
			waitVBlank();

			executePulseTrain();
			executeSilence();

			for(frame = 0; frame < 4096; frame++)
			{
				playSoundatVideoStart(SOUNDCMD_SSGRampcycle);
				waitVBlank();
			}
			playSound(SOUNDCMD_SSGRampinit);

			executeSilence();

			executePulseTrain();
		}

		if (PRESSED_B)
		{
			int stop = 0;

			gfxClear();
			SCClose();
			waitVBlank();
			do
			{
				backgroundColor(WH_107);
				playSoundatLine(0x010C, SOUNDCMD_SSGPulseStart);
				readController();
				if(PRESSED_A)
					stop = 1;
				playSoundatLine(0x1ED, SOUNDCMD_SSGPulseStop);
				waitVBlank();
				
				backgroundColor(_BLACK);
				readController();
				if(PRESSED_A)
					stop = 1;
				waitVBlank();
				
			}while(!stop);
			draw = 1;
		}

		if (PRESSED_C)
		{
			int stop = 0;

			gfxClear();
			SCClose();
			waitVBlank();
			do
			{
				backgroundColor(WH_107);
				playSound(SOUNDCMD_SSGPulseStart);
				readController();
				if(PRESSED_A)
					stop = 1;
				waitVBlank();

				backgroundColor(_BLACK);
				playSound(SOUNDCMD_SSGPulseStop);
				readController();
				if(PRESSED_A)
					stop = 1;
				waitVBlank();
				
			}while(!stop);
			draw = 1;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SOUND))
			draw = 1;
	}
}

void at_audiosync_test()
{
	int done = 0, draw = 1, cycle = 0, x = 160, y = 160;
	s16 acc = 1, status = -1;
	picture syncbar, syncbar2, syncfloor, block;

	backgroundColor(0x8000);

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			pictureInit(&syncbar, &audiosync_bar,1, 16, 0, 48,FLIP_NONE);
			palJobPut(16,audiosync_bar.palInfo->count,audiosync_bar.palInfo->data);

			pictureInit(&syncbar2, &audiosync_bar,8, 16, 288, 48,FLIP_X);

			pictureInit(&syncfloor, &audiosync_floor,16, 17, 0, 160,FLIP_NONE);
			palJobPut(17,audiosync_floor.palInfo->count,audiosync_floor.palInfo->data);

			pictureInit(&block, &led_2x,36, 18, x, y,FLIP_NONE);
			palJobPut(18,led_2x.palInfo->count,led_2x.palInfo->data);
			draw = 0;
		}

		SCClose();
		waitVBlank();

		if (PRESSED_A)
		{
			cycle = !cycle;
			if (!cycle)
				status = 121;
			else
				y = 160;
		}

		if (cycle == 1 && status == -1)
		{
			status = 0;
			acc = -1;
		}

		if (status > -1)
		{
			status++;
			if (status <= 120)
			{
				y += acc;
				pictureMove(&block, x, y);
			}
		}

		readController();

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_AUDIOSYNC))
			draw = 1;
	}
}

inline void DrawController(u16 x, u16 y, BYTE input, BYTE start, BYTE ps, BYTE select, BYTE mvscredit, BYTE credit)
{
	fixPrint(x+4, y,   input & JOY_UP ? fontColorRed : fontColorWhite, 3, "Up");
	fixPrint(x,   y+1, input & JOY_LEFT ? fontColorRed : fontColorWhite, 3, "Left");
	fixPrint(x+6, y+1, input & JOY_RIGHT ? fontColorRed : fontColorWhite, 3, "Right");
	fixPrint(x+3, y+2, input & JOY_DOWN ? fontColorRed : fontColorWhite, 3, "Down");

	fixPrint(x+13, y, ps & start ? fontColorRed : fontColorWhite, 3, "Start");
	if (isMVS)
	{
		// Alow drawing select with MVS1F which have AES ports and UNIBIOS in AES mode
		fixPrint(x+13, y+1, ps & select ? fontColorRed : fontColorWhite, 3, ps & select ? "Select" : "      ");
		fixPrint(x+13, y+2, !(mvscredit & credit) ? fontColorRed : fontColorWhite, 3, "Credit");
	}
	else
		fixPrint(x+13, y+2, ps & select ? fontColorRed : fontColorWhite, 3, "Select");

	fixPrint(x+22, y+1, input & JOY_A ? fontColorRed : fontColorWhite, 3, "A");
	fixPrint(x+23, y+1, input & JOY_B ? fontColorRed : fontColorWhite, 3, "B");
	fixPrint(x+24, y+1, input & JOY_C ? fontColorRed : fontColorWhite, 3, "C");
	fixPrint(x+25, y+1, input & JOY_D ? fontColorRed : fontColorWhite, 3, "D");
}

#define DC_X	7

void ht_controller_test()
{
	int done = 0, hardDip2 = 0, lastDip2 = 0, y = 0, enable4p = 0;
	picture image;
	BYTE mvssel = 0, mvscredit = 0, detected4p = 0;

	gfxClear();

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	detected4p = setup4P();

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (isMVS)
		{
			mvscredit = volMEMBYTE(REG_STATUS_A); 
			mvssel    = volMEMBYTE(REG_STATUS_B); 
			hardDip2  = getHardDipValue(DP_CHUTES);
			if (hardDip2 != lastDip2)
			{
				suiteClearFixLayer();
				lastDip2 = hardDip2;
			}
		}

		if (isMVS && (detected4p || hardDip2      || // hard dip 2 status
			volMEMBYTE(BIOS_4P_MODE)    == 0xFF   || // Main 4P flag, is set when hard dip 2 is on and 4P board is found. 
			volMEMBYTE(BIOS_4P_PLUGGED) == 0xFF   || // 4P compatible bios will check for 4P board regardless of dip2 switch status. 
			volMEMBYTE(SOFT_DIP_3)))				 // Soft dip failback, so the user can force it in MVS mode
		{
			y = 9;
			enable4p = 1;
		} else {
			y = 13;
			enable4p = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);    // Controller 1
		p2 = volMEMBYTE(P2_CURRENT);    // Controller 2
		if (enable4p)
		{
			p1b = volMEMBYTE(P1B_CURRENT);  // Controller 3
			p2b = volMEMBYTE(P2B_CURRENT);  // Controller 4
		}
		ps  = volMEMBYTE(PS_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		p2e = volMEMBYTE(P2_EDGE);

		if (isMVS)
		{
			if(!AES_AS_MVS)
				fixPrint(29, 8, !(mvscredit & MVS_SERV_B) ? fontColorRed : fontColorWhite, 3, "Service");
			if (is4S || is6S)
			{
				fixPrint(4,  24, !(mvssel & MVS_SEL1) ? fontColorRed : fontColorWhite, 3, "Sel 1");
				fixPrint(31, 24, !(mvssel & MVS_SEL2) ? fontColorRed : fontColorWhite, 3, "Sel 2");
			}
		}

		// Controller 1
		DrawController(DC_X, y, p1, P1_START, ps, P1_SELECT, AES_AS_MVS ? mvssel : mvscredit, AES_AS_MVS ? MVS_SEL1 : P1_CREDIT);
		y += 4;

		// Controller 2
		DrawController(DC_X, y, p2, P2_START, ps, P2_SELECT, AES_AS_MVS ? mvssel : mvscredit, AES_AS_MVS ? MVS_SEL2 : P2_CREDIT);
		if(enable4p)
		{
			y += 4;
			// Controller 3
			DrawController(DC_X, y, p1b, P1B_START, ps, P1B_SELECT, mvscredit, P3_CREDIT);
			y += 4;

			// Controller 4
			DrawController(DC_X, y, p2b, P2B_START, ps, P2B_SELECT, mvscredit, P4_CREDIT);
		}

		fixPrint(9, 26, fontColorGreen, 3, "Use START+LEFT to exit");

		if (HELD_START && HELD_LEFT)
			done = 1;
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

#define MAX_LOCATIONS 11

void ht_memory_viewer(u32 address)
{
	int done = 0, redraw = 1, docrc = 0, locpos = 1, pos = 0, ascii = 0;
	u32 crc = 0, locations[MAX_LOCATIONS] = { 0, 0x100000, 0x10F300, 0x110000, 0x200000, 0x300000, 
											0x400000, 0x402000, 0x800000, 0xC00000, 0xD00000 };

	gfxClear();

	for (pos = 0; pos < MAX_LOCATIONS; pos++)
	{
		if (locations[pos] == address)
		{
			locpos = pos;
			break;
		}
	}

	while (!done)
	{
		if (redraw)
		{
			int		i = 0, j = 0;
			u8		*mem = NULL;
			char	buffer[10];

			memset(buffer, 0, sizeof(char)*10);
			mem = (u8*)address;

			suiteClearFixLayer();

			if (docrc)
				crc = CalculateCRC(address, 0x1C0);

			intToHex(address, buffer, 8);
			fixPrint(32, 2, fontColorRed, 3, buffer);
			intToHex(address+448, buffer, 8);
			fixPrint(32, 29, fontColorRed, 3, buffer);

			if (docrc)
			{
				intToHex(crc, buffer, 8);
				fixPrint(32, 16, fontColorGreen, 3, buffer);
			}

			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < 16; j++)
				{
					if (!ascii)
					{
						intToHex(mem[i*16+j], buffer, 2);
						fixPrint(j*2, i+2, fontColorWhite, 3, buffer);
					} else {
						u8 c;
					
						c = mem[i*16+j];
						// ASCII range
						if (c >= 32 && c <= 126)	
							fixPrintf(j*2, i+2, fontColorWhite, 3, "%c", (char)c);
					}
				}
			}
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_START)
			done = 1;

		if (PRESSED_A)
		{
			docrc = !docrc;
			redraw = 1;
		}

		if (PRESSED_B)
		{
			locpos ++;
			if (locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
			redraw = 1;
		}

		if (PRESSED_C)
		{
			ascii = !ascii;
			redraw = 1;
		}

		if (PRESSED_LEFT)
		{
			if (address >= 0x1C0)
				address -= 0x1C0;
			else
				address = 0;

			redraw = 1;
		}

		if (PRESSED_RIGHT)
		{
			address += 0x1C0;	
			redraw = 1;
		}

		if (PRESSED_UP)
		{
			if (address >= 0x10000)
				address -= 0x10000;
			else
				address = 0;

			redraw = 1;
		}

		if (PRESSED_DOWN)
		{
			address += 0x10000;
			redraw = 1;
		}

		if(checkHelp(HELP_MEMVIEW))
			redraw = 1;
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

		readController();

		if (PRESSED_START)
			done = 1;
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
	"US/Europe MVS (Ver. 2)" },
{	BIOS_SNK_MVS,
	0x7E65EA24,
	"sp-s.sp1",
	"US/Europe MVS (Ver. 1)" },
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

	for (i = 0; i < size; i++) {
		u8 c;

		c = str[i];
		if (c >= 32 && c <= 126) { // ASCII range
			str[i] = (char)c;
		} else {
			str[i] = ' ';
		}
	}
}

void byteSwap(u8 *data, u8 size)
{
	int pos = 0;

	while (pos < size)
	{
		u8 t;

		t = data[pos];
		data[pos] = data[pos+1];
		data[pos+1] = t;
		pos += 2;
	}
}

u8 detectUNIBIOSfast(u32 address)
{
	u16 *bios = (u16*)address;

	if (bios[0x58] == 0x4E55)
		return 1;
	return 0;
}

u8 getBIOS_region(u32 address)
{
	u8 *bios = (u8*)address;

	return(bios[0x000401]);
}

u8 getBIOS_HW(u32 address)
{
	u8 *bios = (u8*)address;

	return(bios[0x000400]);
}

void displayBIOS(u32 address, u8 swap)
{
	int  line = 0, len = 32, start = 0x82, x = 4;
	char buffer[34];

	if (detectUNIBIOSfast(address))
	{
		len = 16;
		start = 0xA0;
		x = 12;
	}

	for (line = 0; line < 4; line++)
	{
		memcpy(buffer, (void*)(address+start+line*len), len);
		if (swap)
			byteSwap(buffer, len);
		buffer[len] = '\0';
		cleanBIOSStr(buffer, len);
		fixPrintf(x, 10+line, fontColorGreen, 3, buffer);
	}
}

void drawBIOSHeader(u32 address, short x, short y)
{
	u8				bios_HW, bios_region;

	fixPrint(x, y, fontColorGreen, 3, "Header:");
	bios_HW = getBIOS_HW(address);
	switch(bios_HW)
	{
		case SYSTEM_AES:
			fixPrint(x+8, y, fontColorWhite, 3, "AES");
		break;

		case SYSTEM_MVS:
			fixPrint(x+8, y, fontColorWhite, 3, "MVS");
		break;

		default:
			fixPrint(x+8, y, fontColorWhite, 3, "???");
		break;
	}

	bios_region = getBIOS_region(address);
	switch(bios_region)
	{
		case SYSTEM_EUROPE:
			fixPrint(x+8, y+1, fontColorWhite, 3, "World");
		break;

		case SYSTEM_JAPAN:
			fixPrint(x+8, y+1, fontColorWhite, 3, "Japan");
		break;

		case SYSTEM_USA:
			fixPrint(x+8, y+1, fontColorWhite, 3, "USA");
		break;

		default:
			fixPrint(x+8, y+1, fontColorWhite, 3, "???");
		break;
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

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	// Print BIOS lines
	if (detectUNIBIOSfast(address))
		swap = 1;
	displayBIOS(address, swap);

	drawBIOSHeader(address, 13, 23);

	menu_footer();

	fixPrintf(12, 16, fontColorGreen, 3, "Please Wait...");

	SCClose();
	waitVBlank();

	crc = CalculateCRC(address, BIOS_SIZE);
	intToHex(crc, buffer, 8);
	fixPrintf(12, 16, fontColorGreen, 3, "CRC:  ");
	fixPrintf(17, 16, fontColorWhite, 3, "0x%s ", buffer);

	bios = GetBIOSbyCRC(crc);
	if (bios)
	{
		fixPrintf(6, 18, fontColorWhite, 3, bios->name);
		fixPrintf(6, 19, fontColorWhite, 3, bios->text);
	} else {
		fixPrintf(13, 18, fontColorWhite, 3, "Unknown BIOS");
		fixPrintf(13, 19, fontColorWhite, 3, "Please report it");
		if (detectUNIBIOSfast(address))
		{
			fixPrintf(7, 21, fontColorGreen, 3, "Non-free versions of UNIBIOS");
			fixPrintf(7, 22, fontColorGreen, 3, "can't be recognized by CRC");
		}
	}

	while (!done)
	{
		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_START)
			done = 1;

		if (PRESSED_A)
		{
			swap = !swap;
			displayBIOS(address, swap);
		}
	}
}

void ht_displayregs()
{
	int done = 0, redraw = 1;

	while (!done)
	{
		int y = 12;

		if (redraw)
		{
			gfxClear();
			draw_background();
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		fixPrintf(14, 6, fontColorGreen, 3, "HW Registers");

		displayRegWord(4, y++, "REG_LSPCMODE", REG_LSPCMODE);
		y++;
		displayRegByte(4, y++, "REG_STATUS_A", REG_STATUS_A);
		displayRegByte(4, y++, "REG_STATUS_B", REG_STATUS_B);
		displayRegByte(4, y++, "BIOS_MVS_FLAG", BIOS_MVS_FLAG);
		displayRegByte(4, y++, "REG_SYSTYPE", REG_SYSTYPE);
		displayRegByte(4, y++, "REG_DIPSW", REG_DIPSW);

		if (bkp_data.debug_dip1 & DP_DEBUG1)
		{
			fixPrintf(4, ++y, fontColorGreen, 3, "Soft Dips:");
			fixPrintf(26, y, fontColorWhite, 3, "%01d%01d%01d%01d%01d",
				getSoftDipvalue(SOFT_DIP_1),
				getSoftDipvalue(SOFT_DIP_2),
				getSoftDipvalue(SOFT_DIP_3),
				getSoftDipvalue(SOFT_DIP_4),
				getSoftDipvalue(SOFT_DIP_5));
		}

		menu_footer();

		if (PRESSED_START)
			done = 1;
	}
	return;
}
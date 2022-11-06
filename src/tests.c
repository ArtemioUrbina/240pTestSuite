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
	int limitX = 0, limitY = 0;
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

	getScreenLimits(&limitX, &limitY);
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
				pictureSetPos(&slug_shadow_sprite, x-12, y-12);
				pictureShow(&slug_shadow_sprite);
			} else {
				pictureHide(&slug_shadow_sprite);
			}
			pictureSetPos(&slug_sprite, x-32, y-32);
		} else {
			if (drawshadow) {
				pictureSetPos(&shape_sprite, x-16, y-16);
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

		if (BTTN_MAIN)
		{
			back ++;
			if(back > 1)
				back = 0;
			changeBack = 1;
		}

		// change between even and odd frames
		if (BTTN_OPTION_1)
		{
			drawshadow = !drawshadow;
			evenframes = !evenframes;
			fixPrintf(16, 3, fontColorGreen, 3, "Shadow in %s frames ", evenframes ? "even" : "odd" );
			text = 120;
		}

		if (BTTN_OPTION_2)
		{
			spr_type = !spr_type;
			changeSprite = 1;
		}

		if (HELD_UP)
			y--;

		if (HELD_DOWN)
			y++;

		if (HELD_LEFT)
		{
			x--;
			if (flip != FLIP_X && !spr_type)
			{
				flip = FLIP_X;
				pictureSetFlip(&slug_sprite, flip);
				pictureSetFlip(&slug_shadow_sprite, flip);
			}
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
		}

		if(x < 1)
			x = 1;
		if(x > limitX)
			x = limitX;
		if(y < 1)
			y = 1;
		if(y > limitY)
			y = limitY;

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_SHADOW))
			draw = 1;
	}
}

void vt_striped_sprite_test()
{
	int done = 0, x = 30, y = 30, draw = 1, changeBack = 1, back = 0, frame = 1;
	int pal_water = 0, pal_wfall = 0, limitX = 0, limitY = 0;
	short y1 = -96;
	short y2 = -152;
	short y3 = 0;
	picture donna_back;
	picture sprite;
	scroller backScroll, waterScroll, frontScroll;

	getScreenLimits(&limitX, &limitY);
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

		pictureSetPos(&sprite, x-16, y-16);

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_MAIN)
		{
			back ++;
			if(back > 1)
				back = 0;
			changeBack = 1;
		}

		if (HELD_UP)

			y--;

		if (HELD_DOWN)
			y++;

		if (HELD_LEFT)
			x--;

		if (HELD_RIGHT)
			x++;

		if(x < 1)
			x = 1;
		if(x > limitX)
			x = limitX;
		if(y < 1)
			y = 1;
		if(y > limitY)
			y = limitY;

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
			pause = !pause;

		if (BTTN_OPTION_1 && pause)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 1;
		}

		if (BTTN_EXIT)
			done = 1;

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
			fixPrint(2, 26, fontColorGreen, 3, "\"C\" button toggles horz/vert");
			fixPrint(2, 27, fontColorGreen, 3, "\"D\" button toggles audio");
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

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_MANUALLAG))
			loadvram = 1;

		if (BTTN_MAIN)
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

		if (BTTN_OPTION_1)
		{
			view++;
			if (view > 2)
				view = 0;
		}

		if (BTTN_OPTION_2)
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

			if (BTTN_EXIT)
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

	if(vmode_snk)
	{
		xvert[0] -= 8;
		xvert[2] += 8;
	}

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

		if (BTTN_MAIN)
			pause = !pause;

		if (BTTN_OPTION_1)
		{
			vertical = !vertical;
			pause = 0;
			changed = 1;
		}

		if (BTTN_OPTION_2)
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

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
			pause = !pause;

		if (BTTN_OPTION_1)
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

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
			alternate = ~alternate;

		if (BTTN_OPTION_1)
		{
			docounter = ~docounter;
			if (!docounter)
				suiteClearFixLayer();
		}

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
			alternate = ~alternate;

		if (BTTN_OPTION_1)
		{
			docounter = ~docounter;
			if (!docounter)
				suiteClearFixLayer();
		}

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
			alternate = ~alternate;

		if (BTTN_OPTION_1)
		{
			docounter = ~docounter;
			if(!docounter)
				suiteClearFixLayer();
		}

		if (BTTN_EXIT)
			done = 1;

		if(checkHelp(HELP_CHECK))
			draw = 1;
	}
}

void vt_backlitzone_test()
{
	int done = 0, block = 2, x = 160, y = 112, draw = 1, shown = 1;
	int limitX = 0, limitY = 0, sprSize[] = { 1, 1, 2, 3, 4 };
	picture image;

	getScreenLimits(&limitX, &limitY);

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			switch (block)
			{
				case 0:
					pictureInit(&image, &led_1x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 1:
					pictureInit(&image, &led_2x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 2:
					pictureInit(&image, &led_4x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 3:
					pictureInit(&image, &led_6x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;

				case 4:
					pictureInit(&image, &led_8x, 1, 16, x, y,FLIP_NONE);
					palJobPut(16,led_1x.palInfo->count,led_1x.palInfo->data);
				break;
			}
			draw = 0;
		}

		pictureSetPos(&image, x - sprSize[block], y - sprSize[block]);
		SCClose();
		waitVBlank();

		readController();

		if (BTTN_MAIN)
		{
			block++;
			if(block > 4)
				block = 0;
			draw = 1;
		}

		if (BTTN_OPTION_1)
		{
			shown = !shown;
			if(shown)
				pictureShow(&image);
			else
				pictureHide(&image);
		}

		if (p1 & JOY_UP)
			y--;

		if (p1 & JOY_DOWN)
			y++;

		if (p1 & JOY_RIGHT)
			x++;

		if (p1 & JOY_LEFT)
			x--;

		if(x < 1)
			x = 1;
		if(x > limitX)
			x = limitX;
		if(y < 1)
			y = 1;
		if(y > limitY)
			y = limitY;

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_LED))
			draw = 1;
	}
}


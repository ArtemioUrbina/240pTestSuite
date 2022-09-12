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
#include "externs.h"
#include "tests.h"
#include "help.h"

BYTE p1,p2,ps,p1e,p2e;

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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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



		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

}

void vt_reflex_test()
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

}

void vt_scroll_test()
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

}

void vt_vert_scroll_test()
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

}

void vt_backlitzone_test()
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

}

void ht_controller_test()
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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

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

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

	}

}
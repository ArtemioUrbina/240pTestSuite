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
#include "ng.h"
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
			DrawHelp(HELP_MANUALLAG);
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
			DrawHelp(HELP_HSCROLL);
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
			DrawHelp(HELP_HSCROLL);
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
			DrawHelp(HELP_LED);
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
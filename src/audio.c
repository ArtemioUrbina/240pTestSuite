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

#ifndef __cd__
#define SND_SEL_SSG		0
#define SND_SEL_ADPCMA	1
#define SND_SEL_ADPCMB	2

#define SND_SEL_MIN		SND_SEL_SSG
#define SND_SEL_MAX		SND_SEL_ADPCMB
#else
#define SND_SEL_SSG		0
#define SND_SEL_ADPCMA	1
#define SND_SEL_CDDA	2

#define SND_SEL_MIN		SND_SEL_SSG
#define SND_SEL_MAX		SND_SEL_CDDA
#endif

#define SSG_TIMOUT_FRAMES 120

void at_sound_test()
{
	int done = 0, draw = 1, sel = 0, adpcmb_sel = 2, ssgval = 0;
	int option = 0, change = 0, changeoption = 0, timer = 0;
#ifndef __cd__
	int loopB = 0, adpcmb_rates[] = { 11025, 16538, 22050, 27563, 33075, 38588, 44100, 55125  };
#endif
	picture image;

	// Set initial State
	sendZ80command(SOUNDCMD_RateB_0+adpcmb_sel);
	sendZ80command(SOUNDCMD_NoLoopB);
	sendZ80command(SOUNDCMD_SSGPulseStop);

	while (!done)
	{
		int y = 11;
		char buffer[4];

		if (draw)
		{
			gfxClear();
			pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,back.palInfo->count,back.palInfo->data);
			draw = 0;
		}

		fixPrint(15, 8, fontColorGreen, 3, "Sound Test");

		intToHex(ssgval, buffer, 4);
		fixPrintf(18, y++, fontColorGreen, 3, "SSG 0x%s", buffer);
		fixPrint(12, y, sel == SND_SEL_SSG && option == 0 ? fontColorRed : fontColorWhite, 3, "Init");
		fixPrint(17, y, sel == SND_SEL_SSG && option == 1 ? fontColorRed : fontColorWhite, 3, "Cycle");
		fixPrint(24, y++, sel == SND_SEL_SSG && option == 2 ? fontColorRed : fontColorWhite, 3, "Stop");
		y++;

		fixPrint(16, y++, fontColorGreen, 3, "ADPCM-A");
		fixPrint(12, y, sel == SND_SEL_ADPCMA && option == 0 ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(17, y, sel == SND_SEL_ADPCMA && option == 1 ? fontColorRed : fontColorWhite, 3, "Center");
		fixPrint(24, y++, sel == SND_SEL_ADPCMA && option == 2 ? fontColorRed : fontColorWhite, 3, "Right");
		y++;

		// ADPCM-B not present in Neo Geo CD
#ifndef __cd__
		fixPrint(16, y++, fontColorGreen, 3, "ADPCM-B");
		fixPrint(10, y, sel == SND_SEL_ADPCMB ? fontColorGreen : fontColorWhite, 3, "C:");
		fixPrintf(13, y, fontColorWhite, 3, "%dhz", adpcmb_rates[adpcmb_sel]);
		fixPrint(21, y, sel == SND_SEL_ADPCMB ? fontColorGreen : fontColorWhite, 3, "D:");
		fixPrintf(24, y++, fontColorWhite, 3, "loop %s", loopB ? "on ": "off");
		fixPrint(12, y, sel == SND_SEL_ADPCMB && option == 0 ? fontColorRed : fontColorWhite, 3, "Left");
		fixPrint(17, y, sel == SND_SEL_ADPCMB && option == 1 ? fontColorRed : fontColorWhite, 3, "Center");
		fixPrint(24, y, sel == SND_SEL_ADPCMB && option == 2 ? fontColorRed : fontColorWhite, 3, "Right");
#else
		fixPrint(18, y++, fontColorGreen, 3, "CDDA");
		y++;
		fixPrint(18, y, sel == SND_SEL_CDDA && option == 1 ? fontColorRed : fontColorWhite, 3, "Play");
#endif

		SCClose();
		waitVBlank();

		readController();

		if(timer)
		{
			timer--;
			if(!timer)
				sendZ80command(SOUNDCMD_SSGPulseStop);
		}

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
			if(sel < SND_SEL_MIN)
				sel = SND_SEL_MAX;
			if (sel > SND_SEL_MAX)
				sel = SND_SEL_MIN;
			switch(sel)
			{
			case SND_SEL_SSG:
				option = 0;
				break;
			case SND_SEL_ADPCMA:
				option = 1;
				break;
#ifndef __cd__
			case SND_SEL_ADPCMB:
				option = 1;
				break;
#else
			case SND_SEL_CDDA:
				option = 1;
				break;
#endif
			}
			change = 0;
		}

		if(changeoption)
		{
			if(sel == SND_SEL_SSG)
			{
				if(option < 0)
					option = 2;
				if(option > 2)
					option = 0;
			}

			if(sel == SND_SEL_ADPCMA)
			{
				if(option < 0)
					option = 2;
				if(option > 2)
					option = 0;
			}

#ifndef __cd__
			if(sel == SND_SEL_ADPCMB)
			{
				if(option < 0)
					option = 3;
				if(option > 3)
					option = 0;
			}
#else
			if(sel == SND_SEL_CDDA)
				option = 1;
#endif
			changeoption = 0;
		}

		if (BTTN_MAIN)
		{
			if(sel == SND_SEL_SSG)
			{
				switch(option)
				{
				case 0:
					sendZ80command(SOUNDCMD_SSGRampinit);
					ssgval = 0;
					break;
				case 1:
					sendZ80command(SOUNDCMD_SSGRampcycle);
					ssgval ++;
					break;
				case 2:
					sendZ80command(SOUNDCMD_SSGPulseStop);
					break;
				}
				//sendZ80command(SOUNDCMD_SSG1KHZStart);
				//timer = SSG_TIMOUT_FRAMES;
			}

			if(sel == SND_SEL_ADPCMA)
			{
				switch(option)
				{
				case 0:
					sendZ80command(SOUNDCMD_PlayLeft);
					break;
				case 1:
					sendZ80command(SOUNDCMD_PlayCenter);
					break;
				case 2:
					sendZ80command(SOUNDCMD_PlayRight);
					break;
				}
			}

#ifndef __cd__
			if (sel == SND_SEL_ADPCMB)
			{
				switch(option)
				{
				case 0:
					sendZ80command(SOUNDCMD_ADPCMB_Left);
					break;
				case 1:
					sendZ80command(SOUNDCMD_ADPCMB_Center);
					break;
				case 2:
					sendZ80command(SOUNDCMD_ADPCMB_Right);
					break;
				}
			}
#else
			if (sel == SND_SEL_CDDA)
				playCDDA(CDDA_SNDTEST, 0);
#endif
		}

		if (BTTN_OPTION_1)
		{
			if(sel == SND_SEL_ADPCMA)
				sendZ80command(SOUNDCMD_PlayLeft);
#ifndef __cd__
			if(sel == SND_SEL_ADPCMB)
			{
				adpcmb_sel ++;
				if(adpcmb_sel > 7)
					adpcmb_sel = 0;
				sendZ80command(SOUNDCMD_RateB_0+adpcmb_sel);
			}
#endif
		}

		if (BTTN_OPTION_2)
		{
			if(sel == SND_SEL_ADPCMA)
				sendZ80command(SOUNDCMD_PlayRight);
#ifndef __cd__
			if(sel == SND_SEL_ADPCMB)
			{
				loopB = !loopB;
				if(loopB)
					sendZ80command(SOUNDCMD_LoopB);
				else
					sendZ80command(SOUNDCMD_NoLoopB);
				sendZ80command(SOUNDCMD_StopADPCMB);
			}
#endif
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_SOUND))
			draw = 1;
	}

	sendZ80command(SOUNDCMD_StopADPCMA);
	sendZ80command(SOUNDCMD_StopADPCMB);
	sendZ80command(SOUNDCMD_SSGPulseStop);

#ifdef __cd__
	pauseCDDA(0);
#endif
}

void executePulseTrain()
{
	int frame = 0;

	for(frame = 0; frame < 10; frame++)
	{
		sendZ80command(SOUNDCMD_SSGPulseStart);
		waitVBlank();
		sendZ80command(SOUNDCMD_SSGPulseStop);
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

		if (BTTN_MAIN)
		{
			int frame = 0;

			sendZ80command(SOUNDCMD_SSGRampinit);
			waitVBlank();

			executePulseTrain();
			executeSilence();

			for(frame = 0; frame < 4096; frame++)
			{
				sendZ80commandAtVideoStart(SOUNDCMD_SSGRampcycle);
				waitVBlank();
			}
			sendZ80command(SOUNDCMD_SSGRampinit);

			executeSilence();

			executePulseTrain();
		}

		if (BTTN_OPTION_1)
		{
			int stop = 0;

			gfxClear();
			SCClose();
			waitVBlank();
			do
			{
				backgroundColor(WH_107);
				sendZ80commandAtLine(0x010C, SOUNDCMD_SSGPulseStart);
				readController();
				if (BTTN_MAIN)
					stop = 1;
				sendZ80commandAtLine(0x1ED, SOUNDCMD_SSGPulseStop);
				waitVBlank();
				
				backgroundColor(_BLACK);
				readController();
				if (BTTN_MAIN)
					stop = 1;
				waitVBlank();
				
			}while(!stop);
			draw = 1;
		}

		if (BTTN_OPTION_2)
		{
			int stop = 0;

			gfxClear();
			SCClose();
			waitVBlank();
			do
			{
				backgroundColor(WH_107);
				sendZ80command(SOUNDCMD_SSGPulseStart);
				readController();
				if (BTTN_MAIN)
					stop = 1;
				waitVBlank();

				backgroundColor(_BLACK);
				sendZ80command(SOUNDCMD_SSGPulseStop);
				readController();
				if (BTTN_MAIN)
					stop = 1;
				waitVBlank();
				
			}while(!stop);
			draw = 1;
		}

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
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

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_AUDIOSYNC))
			draw = 1;
	}
}


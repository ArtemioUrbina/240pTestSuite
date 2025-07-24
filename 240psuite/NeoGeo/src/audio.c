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
#define SND_SEL_FM_NOTE		0
#define SND_SEL_FM_OCTAVE	1
#define SND_SEL_FM_PANNING	2
#define SND_SEL_SSG			3
#define SND_SEL_ADPCMA		4
#define SND_SEL_ADPCMB		5

#define SND_SEL_MIN		SND_SEL_FM_NOTE
#define SND_SEL_MAX		SND_SEL_ADPCMB
#else
#define SND_SEL_FM_NOTE		0
#define SND_SEL_FM_OCTAVE	1
#define SND_SEL_FM_PANNING	2
#define SND_SEL_SSG			3
#define SND_SEL_ADPCMA		4
#define SND_SEL_CDDA		5

#define SND_SEL_MIN		SND_SEL_FM_NOTE
#define SND_SEL_MAX		SND_SEL_CDDA
#endif

#define SSG_TIMOUT_FRAMES 60

void at_sound_test()
{
	int done = 0, draw = 1, sel = 0;
	int fmnote = 6, fmoctave = 3, fmpan = 1, refresh = 1;
	int option = fmnote, change = 0, changeoption = 0;
	int	timer0 = 0, timer1 = 0, timer2= 0, version = 0;
#ifndef __cd__
	int adpcmb_sel = 6;
	int loopB = 0, adpcmb_rates[] = { 11025, 16538, 22050, 27563, 33075, 38588, 44100, 55125 }, adpcm_b_swap = 0;
#endif
	picture image;

	// Set initial State
	sendZ80command(SOUNDCMD_StopAll);

	sendZ80command(SOUNDCMD_FMInitSndTest);
	sendZ80command(SOUNDCMD_FMOctave0+fmoctave);
	sendZ80command(SOUNDCMD_FMNote0+fmnote);

#ifndef __cd__
	sendZ80command(SOUNDCMD_ADPCMB_Sample0);
	sendZ80command(SOUNDCMD_NoLoopB);
	sendZ80command(SOUNDCMD_RateB_0+adpcmb_sel);

	adpcm_b_swap = getSoftDipvalue(SD_MV1C_STEREO);
#endif

	version = verifyZ80Version();

	while (!done)
	{
		int x = 7, y = 9;

		if (draw)
		{
			gfxClear();
			pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,back.palInfo->count,back.palInfo->data);
			draw = 0;
			refresh = 1;
		}

		if(refresh)
		{
#ifndef __cd__
			fixPrintC(6, fontColorGreen, fbase, "YM 2610 Sound Test");
#else
			fixPrintC(6, fontColorGreen, fbase, "YM 2610 & CDDA Sound Test");
#endif

			fixPrintC(y++, fontColorGreen, fbase, "FM");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 0 ? fontColorRed : fontColorWhite, fbase, "1");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 1 ? fontColorRed : fontColorWhite, fbase, "2");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 2 ? fontColorRed : fontColorWhite, fbase, "3");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 3 ? fontColorRed : fontColorWhite, fbase, "4");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 4 ? fontColorRed : fontColorWhite, fbase, "5");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 5 ? fontColorRed : fontColorWhite, fbase, "6");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 6 ? fontColorRed : fontColorWhite, fbase, "7");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 7 ? fontColorRed : fontColorWhite, fbase, "8");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 8 ? fontColorRed : fontColorWhite, fbase, "9");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 9 ? fontColorRed : fontColorWhite, fbase, "A");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 10 ? fontColorRed : fontColorWhite, fbase, "B");
			fixPrint(x+=2, y, sel == SND_SEL_FM_NOTE && option == 11 ? fontColorRed : fontColorWhite, fbase, "C");
			y++;
			x = 9;
			fixPrint(x, y, sel == SND_SEL_FM_OCTAVE ? fontColorRed : fontColorGreen, fbase, "Octave:");
			fixPrint(x+=8, y, sel == SND_SEL_FM_OCTAVE && option == 0 ? fontColorRed : fmoctave == 0 ? fontColorGrayDark : fontColorWhite, fbase, "1");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 1 ? fontColorRed : fmoctave == 1 ? fontColorGrayDark : fontColorWhite, fbase, "2");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 2 ? fontColorRed : fmoctave == 2 ? fontColorGrayDark : fontColorWhite, fbase, "3");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 3 ? fontColorRed : fmoctave == 3 ? fontColorGrayDark : fontColorWhite, fbase, "4");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 4 ? fontColorRed : fmoctave == 4 ? fontColorGrayDark : fontColorWhite, fbase, "5");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 5 ? fontColorRed : fmoctave == 5 ? fontColorGrayDark : fontColorWhite, fbase, "6");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 6 ? fontColorRed : fmoctave == 6 ? fontColorGrayDark : fontColorWhite, fbase, "7");
			fixPrint(x+=2, y, sel == SND_SEL_FM_OCTAVE && option == 7 ? fontColorRed : fmoctave == 7 ? fontColorGrayDark : fontColorWhite, fbase, "8");
			y++;
			fixPrint(12, y, sel == SND_SEL_FM_PANNING && option == 0 ? fontColorRed : fmpan == 0 ? fontColorGrayDark : fontColorWhite, fbase, "Left");
			fixPrint(17, y, sel == SND_SEL_FM_PANNING && option == 1 ? fontColorRed : fmpan == 1 ? fontColorGrayDark : fontColorWhite, fbase, "Center");
			fixPrint(24, y, sel == SND_SEL_FM_PANNING && option == 2 ? fontColorRed : fmpan == 2 ? fontColorGrayDark : fontColorWhite, fbase, "Right");

			x = 15;
			y+=2;
			fixPrintC(y++, fontColorGreen, fbase, "SSG");
			fixPrint(x+=2, y, sel == SND_SEL_SSG && option == 0 ? fontColorRed : fontColorWhite, fbase, "1");
			fixPrint(x+=2, y, sel == SND_SEL_SSG && option == 1 ? fontColorRed : fontColorWhite, fbase, "2");
			fixPrint(x+=2, y, sel == SND_SEL_SSG && option == 2 ? fontColorRed : fontColorWhite, fbase, "3");
			y+=2;

			fixPrintC(y++, fontColorGreen, fbase, "ADPCM-A");
			fixPrint(12, y, sel == SND_SEL_ADPCMA && option == 0 ? fontColorRed : fontColorWhite, fbase, "Left");
			fixPrint(17, y, sel == SND_SEL_ADPCMA && option == 1 ? fontColorRed : fontColorWhite, fbase, "Center");
			fixPrint(24, y++, sel == SND_SEL_ADPCMA && option == 2 ? fontColorRed : fontColorWhite, fbase, "Right");
			y++;

			// ADPCM-B not present in Neo Geo CD
#ifndef __cd__
			if(adpcm_b_swap)
				fixPrintC(y++, fontColorGreen, fbase, "ADPCM-B MV1C REVERSED");
			else
				fixPrintC(y++, fontColorGreen, fbase, "ADPCM-B");
			fixPrint(10, y, sel == SND_SEL_ADPCMB ? fontColorGreen : fontColorWhite, fbase, "C:");
			fixPrintf(13, y, fontColorWhite, fbase, "%dhz", adpcmb_rates[adpcmb_sel]);
			fixPrint(21, y, sel == SND_SEL_ADPCMB ? fontColorGreen : fontColorWhite, fbase, "D:");
			fixPrintf(24, y++, fontColorWhite, fbase, "loop %s", loopB ? "on ": "off");
			fixPrint(12, y, sel == SND_SEL_ADPCMB && option == 0 ? fontColorRed : fontColorWhite, fbase, "Left");
			fixPrint(17, y, sel == SND_SEL_ADPCMB && option == 1 ? fontColorRed : fontColorWhite, fbase, "Center");
			fixPrint(24, y, sel == SND_SEL_ADPCMB && option == 2 ? fontColorRed : fontColorWhite, fbase, "Right");
#else
			fixPrintC(y++, fontColorGreen, fbase, "CDDA");
			y++;
			fixPrint(18, y, sel == SND_SEL_CDDA && option == 1 ? fontColorRed : fontColorWhite, fbase, "Play");
#endif
			if(!version)
				fixPrintC(++y, fontColorRed, fbase, max_z80_timout == Z80_TIMEOUT ? "Z80 Driver Timed Out" : "Incorrect Audio Driver");
			else {
				if(disable_z80_check)
					fixPrintC(++y, fontColorRed, fbase, "Z80 Command check disabled");
			}
				
			refresh = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if(timer0) {
			timer0--;
			if(!timer0)
				sendZ80command(SOUNDCMD_SSG260HZStop);
		}

		if(timer1) {
			timer1--;
			if(!timer1)
				sendZ80command(SOUNDCMD_SSG1KHZStop);
		}

		if(timer2) {
			timer2--;
			if(!timer2)
				sendZ80command(SOUNDCMD_SSGNoiseStop);
		}

		if (PRESSED_LEFT) {
			option --;
			changeoption = 1;
		}

		if (PRESSED_RIGHT) {
			option ++;
			changeoption = 1;
		}

		if (PRESSED_UP)	{
			sel --;
			change = 1;
		}

		if (PRESSED_DOWN) {
			sel++;
			change = 1;
		}

		if(change) {
			if (sel < SND_SEL_MIN) sel = SND_SEL_MAX;
			if (sel > SND_SEL_MAX) sel = SND_SEL_MIN;
			switch(sel)
			{
			case SND_SEL_FM_NOTE:
				option = fmnote;
				break;
			case SND_SEL_FM_OCTAVE:
				option = fmoctave;
				break;
			case SND_SEL_FM_PANNING:
				option = fmpan;
				break;
			case SND_SEL_SSG:
				option = 1;
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
			refresh = 1;
		}

		if(changeoption) {
			if (sel == SND_SEL_FM_NOTE) {
				if(option < 0) option = 11;
				if(option > 11) option = 0;
			}

			if (sel == SND_SEL_FM_OCTAVE) {
				if(option < 0) option = 7;
				if(option > 7) option = 0;
			}

			if (sel == SND_SEL_FM_PANNING) {
				if(option < 0) option = 2;
				if(option > 2) option = 0;
			}

			if (sel == SND_SEL_SSG) {
				if(option < 0) option = 2;
				if(option > 2) option = 0;
			}

			if (sel == SND_SEL_ADPCMA) {
				if(option < 0)
					option = 2;
				if(option > 2)
					option = 0;
			}

#ifndef __cd__
			if (sel == SND_SEL_ADPCMB) {
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
			refresh = 1;
		}

		if (BTTN_MAIN) {
			if (sel == SND_SEL_FM_NOTE) {
				fmnote = option;
				sendZ80command(SOUNDCMD_FMNote0+fmnote);
				sendZ80command(SOUNDCMD_FMPlay);
			}

			if (sel == SND_SEL_FM_OCTAVE) {
				fmoctave = option;
				sendZ80command(SOUNDCMD_FMOctave0+fmoctave);
			}

			if (sel == SND_SEL_FM_PANNING) {
				fmpan = option;
				sendZ80command(SOUNDCMD_FMUseLeft+fmpan);
			}

			if (sel == SND_SEL_SSG) {
				switch(option)
				{
				case 0:
					sendZ80command(SOUNDCMD_SSG260HZStart);
					timer0 = SSG_TIMOUT_FRAMES;
					break;
				case 1:
					sendZ80command(SOUNDCMD_SSG1KHZStart);
					timer1 = SSG_TIMOUT_FRAMES;
					break;
				case 2:
					sendZ80command(SOUNDCMD_SSGNoiseStart);
					timer2 = SSG_TIMOUT_FRAMES;
					break;
				}
			}

			if (sel == SND_SEL_ADPCMA) {
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
			if (sel == SND_SEL_ADPCMB) {
				switch(option)
				{
				case 0:
					sendZ80command(adpcm_b_swap ? SOUNDCMD_ADPCMB_Right : SOUNDCMD_ADPCMB_Left);
					break;
				case 1:
					sendZ80command(SOUNDCMD_ADPCMB_Center);
					break;
				case 2:
					sendZ80command(adpcm_b_swap ? SOUNDCMD_ADPCMB_Left : SOUNDCMD_ADPCMB_Right);
					break;
				}
			}
#else
			if (sel == SND_SEL_CDDA)
				playCDDA(CDDA_SNDTEST, 0);
#endif
		}

		if (BTTN_OPTION_1) {
			if (sel == SND_SEL_ADPCMA)
				sendZ80command(SOUNDCMD_PlayLeft);
#ifndef __cd__
			if (sel == SND_SEL_ADPCMB)
			{
				adpcmb_sel ++;
				if(adpcmb_sel > 7)
					adpcmb_sel = 0;
				sendZ80command(SOUNDCMD_RateB_0+adpcmb_sel);
				refresh = 1;
			}
#endif
		}

		if (BTTN_OPTION_2) {
			if (sel == SND_SEL_ADPCMA)
				sendZ80command(SOUNDCMD_PlayRight);
#ifndef __cd__
			if (sel == SND_SEL_ADPCMB)
			{
				loopB = !loopB;
				if(loopB)
					sendZ80command(SOUNDCMD_LoopB);
				else
					sendZ80command(SOUNDCMD_NoLoopB);
				refresh = 1;
			}
#endif
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_SOUND))
			draw = 1;
	}

	sendZ80command(SOUNDCMD_StopAll);

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

	for(frame = 0; frame < 40; frame++)
		waitVBlank();
}

void waitSound(int frames)
{
	int frame = 0;

	for(frame = 0; frame < frames; frame++)
		waitVBlank();
}

void ExecuteFM(int framelen)
{
	int octave, frame;
	
	// FM Test
	
	for(octave = 0; octave < 8; octave ++)
	{
		int note;
			
		sendZ80command(SOUNDCMD_FMOctave0+octave);
		for(note = 0; note < 12; note++)
		{
			sendZ80command(SOUNDCMD_FMNote0+note);
			sendZ80command(SOUNDCMD_FMNextMDF);
				
			for(frame = 0; frame < framelen; frame++)
			{					
				if(frame == framelen - framelen/5)
					sendZ80command(SOUNDCMD_FMStopAll);
				waitVBlank();
			}
		}
	}
	sendZ80command(SOUNDCMD_FMStopAll);
}

#define	MDF_DEFAULT		0
#define	MDF_COMPATIBLE	1
#define MDF_TYPES		2

void at_sound_mdfourier()
{
	picture 	image;
	int 		done = 0, draw = 1, version = 0, msgpos = 0;
#ifndef __cd__
	int 		mdf_type = MDF_DEFAULT;
	fmenudata	fmenu[MDF_TYPES] = { 
			{ MDF_DEFAULT,		"Full Test" },
			{ MDF_COMPATIBLE,	"NGCD compatible" },
	};

	mdf_type = select_menu("Select Test Type", fmenu, MDF_TYPES, mdf_type);
	if(mdf_type == SEL_MENU_CANCEL)
		return;
#endif

	version = verifyZ80Version();
	if(!version)
		checkZ80Version();
	else {
		if(disable_z80_check)
			draw_warning("Z80 Command check disabled\nResults might be off", 1, 16, 1);
	}

	while (!done)
	{
		if (draw)
		{
			int y = 12;

			gfxClear();
			pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,back.palInfo->count,back.palInfo->data);

			fixPrintC(6, fontColorGreen, fbase, "MDFourier");
			y+= 2;
			fixPrintC(y++, fontColorGreen, fbase, "Auto sequence");
			msgpos = y;
			fixPrintC(y++, fontColorWhite, fbase, "Start recording and press A");
			if(!version)
				fixPrintC(++y, fontColorRed, fbase, "Faulty Audio Driver");
			if(bkp_data.debug_dip1&DP_DEBUG2)
				fixPrintC(3+y++, fontColorYellow, fbase, "SPSG Noise enabled");
#ifndef __cd__
			if(mdf_type == MDF_DEFAULT && bkp_data.debug_dip1&DP_DEBUG1)
				fixPrintC(3+y++, fontColorYellow, fbase, "Full sample rate enabled");
			if(mdf_type == MDF_COMPATIBLE)
				fixPrintC(3+y, fontColorYellow, fbase, "NGCD Compatible test");
#endif
			fixPrintC(26, fontColorWhite, fbase, "Press START for help");
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_MDFOURIER))
			draw = 1;

		if (BTTN_MAIN)
		{
			int frame = 0, close = 0;

            // Turn Off credits sound
            soundOn = 0;

			fixPrintC(msgpos, fontColorWhite, fbase, "Please wait while recording");
			SCClose();
			waitVBlank();

			sendZ80command(SOUNDCMD_StopAll);

			sendZ80command(SOUNDCMD_FMInitMDF);
			sendZ80command(SOUNDCMD_SSGRampinit);

			sendZ80command(SOUNDCMD_NoLoopB);
			sendZ80command(SOUNDCMD_ADPCMB_LdSweep);

			waitVBlank();

			executePulseTrain();
			executeSilence();

			ExecuteFM(20);

			// First detailed SSG ramp
			for(frame = 0; frame < 256; frame++)
			{
				sendZ80command(SOUNDCMD_SSGRampcycle);
				waitVBlank();
			}
			// then low SSG tones ramp, at 0x10 steps (they would be 3840 frame otherwise)
			for(frame = 0; frame < 240; frame++)
			{
				sendZ80command(SOUNDCMD_SSGRampStep);
				waitVBlank();
			}
			sendZ80command(SOUNDCMD_SSGStop);

			// Only play th spart of the  test if debug dip 2 is enabled
			if(bkp_data.debug_dip1&DP_DEBUG2)
			{
				// SSG Noise is too random for a peroid
				for(frame = 0; frame < 32; frame++)
				{
					sendZ80command(SOUNDCMD_SSGNoiseRamp);
					waitSound(5);
				}
			}
			
			sendZ80command(SOUNDCMD_SSGStop);
			waitVBlank();								// extra frame

			// ADPCM-A takes 236.46 frames at 16.77724
			// 3967.312 mmilliseconds in an AES NTSC System
			sendZ80command(SOUNDCMD_PlaySweep);
			waitSound(240);

#ifndef __cd__
			// ADPCM-B not present in Neo Geo CD
			if(mdf_type == MDF_DEFAULT)
			{
				// Only play full test if debug dip is enabled
				if(bkp_data.debug_dip1&DP_DEBUG1)
				{
					sendZ80command(SOUNDCMD_RateB_0_Play);			// 11025hz
					waitSound(610);

					sendZ80command(SOUNDCMD_RateB_2_Play);			// 22050hz
					waitSound(305);
				}

				// Common playback
				sendZ80command(SOUNDCMD_RateB_6_Play);			// 44100hz
				waitSound(152);

				// Only play full test if debug dip is enabled
				if(bkp_data.debug_dip1&DP_DEBUG1)
				{
					// 2.01968 seconds or 121 frames
					sendZ80command(SOUNDCMD_RateB_7_Play);			// 55125hz
					waitSound(122);
				}
			}
#endif
			executeSilence();

			executePulseTrain();

			sendZ80command(SOUNDCMD_StopAll);

#ifdef __cd__
			// We play CDDA outside from the main tones
			// 10.8 seconds or 643.73 frames in AES NTSC
			playCDDA(CDDA_MDFOURIER, 0);
			waitSound(644);
			pauseCDDA(0);
			waitVBlank();								// extra frame
#endif

			fixPrintC(msgpos, fontColorWhite, fbase, "You can now stop recording.");
			fixPrintC(msgpos+1, fontColorWhite, fbase, "Press any button to continue");

            // Turn On credits sound
            soundOn = 1;

			while(!close)
			{
				SCClose();
				waitVBlank();

				readController();

				if (BTTN_ANY)
					close = 1;
			}
			draw = 1;
		}

#ifndef __cd__
		if (BTTN_OPTION_1)
		{
			int tmp = 0;

			tmp = select_menu("Select Test Type", fmenu, MDF_TYPES, mdf_type);
			if(mdf_type != SEL_MENU_CANCEL)
				mdf_type = tmp;	
			draw = 1;
		}
#endif
	}

	sendZ80command(SOUNDCMD_StopAll);
#ifdef __cd__
	pauseCDDA(0);
#endif
}

void at_audiosync_test()
{
	int done = 0, draw = 1, x = 160, y = 180;
	int paused = 0, speed = -1;
	int x1 = 0 , x2 = 160, hstep = 1, state = 0;
	picture syncbar, syncbar2, syncfloor, block;

	while (!done)
	{
		if (draw)
		{
			int palindex = 16, sprindex = 1;

			gfxClear();

			pictureInit(&syncbar, &audiosync_bar,sprindex, palindex, x1, 64,FLIP_NONE);
			palJobPut(palindex,audiosync_bar.palInfo->count,audiosync_bar.palInfo->data);
			sprindex += getPicSprites(syncbar.info);
			palindex += audiosync_bar.palInfo->count;

			pictureInit(&syncbar2, &audiosync_bar,sprindex, palindex, x2, 64, FLIP_NONE);
			sprindex += getPicSprites(syncbar.info);

			pictureInit(&syncfloor, &audiosync_floor,sprindex, palindex, 0, 180, FLIP_NONE);
			palJobPut(palindex,audiosync_floor.palInfo->count,audiosync_floor.palInfo->data);
			sprindex += getPicSprites(syncfloor.info);
			palindex += audiosync_floor.palInfo->count;

			pictureInit(&block, &led_2x,sprindex, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,led_2x.palInfo->count,led_2x.palInfo->data);

			draw = 0;
		}

		if(bkp_data.debug_dip1&DP_DEBUG1)
		{
			fixPrintf(22, 14, fontColorSolid, fbase+1, "X:      %04d", x);
			fixPrintf(22, 15, fontColorSolid, fbase+1, "Y:      %04d", y);
			fixPrintf(22, 16, fontColorSolid, fbase+1, "speed:  %04d", speed);
			fixPrintf(22, 17, fontColorSolid, fbase+1, "X1:     %04d", x1);
			fixPrintf(22, 18, fontColorSolid, fbase+1, "X2:     %04d", x2);
			fixPrintf(22, 19, fontColorSolid, fbase+1, "hstep:  %04d", hstep);
			fixPrintf(22, 20, fontColorSolid, fbase+1, "paused: %04d", paused);
		}

		SCClose();
		waitVBlank();

		if(!paused)
		{
			y += speed;

			if(y == 180)
			{
				switch(state) {
					case 0:
						sendZ80commandAtVideoStart(SOUNDCMD_SSG1KHZStart);
						backgroundColor(WH_100);

						sendZ80commandAtVideoEnd(SOUNDCMD_SSG1KHZStop);
						backgroundColor(_BLACK);
						break;
					case 1:
						sendZ80commandAtVideoStart(SOUNDCMD_SSG260HZStart);
						backgroundColor(WH_100);

						sendZ80commandAtVideoEnd(SOUNDCMD_SSG260HZStop);
						backgroundColor(_BLACK);
						break;
				}

				state = !state;
			}

			if(y == 180 || y == 120)
			{
				speed *= -1;
				hstep *= -1;
			}

			x1 -= hstep;
			x2 += hstep;

			pictureSetPos(&syncbar, x1, 64);
			pictureSetPos(&syncbar2, x2, 64);
			pictureSetPos(&block, x, y);
		}

		readController();

		if (BTTN_MAIN)
			paused = !paused;

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_AUDIOSYNC))
			draw = 1;
	}
}


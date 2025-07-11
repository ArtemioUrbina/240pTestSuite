/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 
#include "system.h"
#include "audio.h"
#include "menu.h"
#include "font.h"

// Wrap the results for scenarios when the wav is not compiled into the ROM (testing/etc)
int openWAV(wav64_t *wav64, char *fileName) {
	int fh = 0;
	
	fh = dfs_open(fileName+5);   // remove "rom:/"
	if(fh < DFS_ESUCCESS)
		return 0;
	dfs_close(fh);
	
	wav64_open(wav64, fileName);
	return 1;
}

#define MDF_BUFFERS		20
#define MDF_CHANNELS	2
#define MDF_CHANNEL		0
 
void drawMDFourier() {
	int end = 0, refresh = SUITE_NUM_BUFFERS, changePlaybackState = 0, isPlaying = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	wav64_t mdfSamples;
	
	if(!openWAV(&mdfSamples, "rom:/mdfourier-dac-44100.wav64")) {
		drawMessageBox("MDFourier samples not present");
		return;
	}
		
	back = loadImage("rom:/mainbg.sprite");
	if(!back)
		return;

	audio_init(44100, MDF_BUFFERS);
	mixer_init(MDF_CHANNELS);
	
	while(!end) {
		if(changePlaybackState) {
			if(!mixer_ch_playing(MDF_CHANNEL)) {
				wav64_play(&mdfSamples, MDF_CHANNEL);
				isPlaying = 1;
			}
			else {
				mixer_ch_stop(MDF_CHANNEL);
				isPlaying = 0;
			}
			changePlaybackState = 0;
			refresh = 1;
		}
		
		if(refresh) {
			getDisplay();

			rdpqStart();
			
			rdpqDrawImage(back);
			rdpqEnd();
			
			drawStringC(42, 0x00, 0xff, 0x00, "MDFourier");
			if(isPlaying)
				drawStringC(100, 0xff, 0xff, 0xff, "Playing back ");
			else
				drawStringC(100, 0xff, 0xff, 0xff, "Press #GA#G to play");
			
			drawStringC(201, 0xff, 0xff, 0x00, "#Yhttp://junkerhq.net/MDFourier#Y");
			refresh --;
		}
		
		if(!mixer_ch_playing(MDF_CHANNEL))
			checkMenu(MDFOURIERHELP, &refresh);
		
		drawNoVsyncWithAudio();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(!mixer_ch_playing(MDF_CHANNEL))
			checkStart(keys);
		
		if(keys.b) {
			if(!mixer_ch_playing(MDF_CHANNEL))
				end = 1;
			else
				keys.a = 1;
		}
		
		if(keys.a)
			changePlaybackState = 1;
	}
	
	if(mixer_ch_playing(MDF_CHANNEL)) {
		int purge = 60;
		
		mixer_ch_stop(MDF_CHANNEL);
		while(purge) {
			waitVsyncWithAudio();
			purge --;
		}
	}
	
	wav64_close(&mdfSamples);
	freeImage(&back);
	
	mixer_close();
	audio_close();
}
 
#define REGULAR_TEST		0
#define SCOPE_TEST			1
#define SCOPE_FRAMES		60
#define AUDIO_SYNC_FRAMES	-8

#define SYNC_CHANNELS	1
#define SYNC_BUFFERS	1

typedef struct rectangle_type {
	int x, y, w, h;
} rect_t;

void drawAudioSyncTest() {
	int 		done = 0, paused = 0, playtone = 0;
	int			y = 0, speed = -1, W = 320;
	int			testMode = REGULAR_TEST, countScope = 0;
	rect_t		rectL, rectR, sprite;
	float		hstep = 0;
	wav64_t 	beepSamples;
	joypad_buttons_t keys;
	
	y = 180;
	hstep = -1 * W/120; // 60 steps times half screen
	
	sprite.x = W / 2 - 4;
	sprite.y = y;
	sprite.w = 8;
	sprite.h = 8;
	
	rectL.w = W / 2;
	rectL.h = 16;
	rectL.x = 0;
	rectL.y = 80;
	
	rectR.w = W / 2;
	rectR.h = 16;
	rectR.x = W / 2;
	rectR.y = 80;
	
	if(!openWAV(&beepSamples, "rom:/beep.wav64")) {
		drawMessageBox("Audio samples not present");
		return;
	}
	
	audio_init(44100, SYNC_BUFFERS);
	mixer_init(SYNC_CHANNELS);
	
	while(!done) {
		if(testMode == REGULAR_TEST) {
			if(!paused)	{
				y += speed;
				
				sprite.y = y;
				rectL.w += hstep;
				
				rectR.x -= hstep;
				rectR.w += hstep;
				if(y == 180 || y == 120) {
					speed *= -1;
					hstep *= -1;
				}
					
				// need to calibrate when to playback
				if(y == (180 + AUDIO_SYNC_FRAMES) && speed == 1)
					playtone = 1;
			}
			
			getDisplay();

			rdpqStart();
			if(y == 180)
				rdpqClearScreenWhite();
			else
				rdpqClearScreen();
			rdpqEnd();

			graphics_draw_box(__disp, rectL.x, rectL.y, rectL.w, rectL.h, COLOR_W);
			graphics_draw_box(__disp, rectR.x, rectR.y, rectR.w, rectR.h, COLOR_W);
			graphics_draw_box(__disp, 0, 188, W, 8, COLOR_W);
			graphics_draw_box(__disp, sprite.x, sprite.y, sprite.w, sprite.h, COLOR_W);

			checkMenu(NULL, NULL);
			waitVsyncWithAudio();
		}
		else {
			getDisplay();

			rdpqStart();
			if(countScope == SCOPE_FRAMES) {
				rdpqClearScreenWhite();
				countScope = 0;
			}
			else
				rdpqClearScreen();
			if(countScope == (SCOPE_FRAMES + AUDIO_SYNC_FRAMES)) 
				playtone = 1;
			rdpqEnd();
			
			checkMenu(NULL, NULL);
			waitVsyncWithAudio();
			countScope++;
		}
		
		if(playtone == 1) {
			wav64_play(&beepSamples, 0);
			playtone = 0;
		}
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.b)
			done =	1;
			
		if(keys.a)
			paused = !paused;

		if(keys.c_left)	{
			if(testMode == REGULAR_TEST && 
				drawAskQuestion("This will enable the\n#COscilloscope#C version which\nflashes white every 60 frames.\n#YAre you sure?#Y"))
				testMode = SCOPE_TEST;
			else
				testMode = REGULAR_TEST;
		}
		
		if(!mixer_ch_playing(0))
			checkStart(keys);
	}
	
	wav64_close(&beepSamples);
	
	mixer_close();
	audio_close();
}

#define ST_BUFFERS		1
#define ST_CHANNELS		3

void drawAudioTest() {
	int end = 0, sel = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	wav64_t left, right, center;

	if(!openWAV(&left, "rom:/sound_test_left.wav64")) {
		drawMessageBox("Sound test Left samples not present");
		return;
	}
	if(!openWAV(&center, "rom:/sound_test_center.wav64")) {
		drawMessageBox("Sound test Right samples not present");
		return;
	}
	if(!openWAV(&right, "rom:/sound_test_right.wav64")) {
		drawMessageBox("Sound test Right samples not present");
		return;
	}
	back = loadImage("rom:/mainbg.sprite");
	if(!back)
		return;
		
	audio_init(44100, ST_BUFFERS);
	mixer_init(ST_CHANNELS);
	
	mixer_ch_set_vol_dolby(0, 1, 0, 0, 0, 0);
	mixer_ch_set_vol_dolby(1, 1, 1, 0, 0, 0);
	mixer_ch_set_vol_dolby(2, 0, 1, 0, 0, 0);
	
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqClearScreen();
		rdpqDrawImage(back);
		rdpqEnd();
		
		drawStringC(40, 0x00, 0xff, 0x00, "Sound Test");
		drawStringS(90, 120, 0xff, sel == 0 ? 0 : 0xff,	sel == 0 ? 0 : 0xff, "Left");
		drawStringC(130, 0xff, sel == 1 ? 0 : 0xff, sel == 1 ? 0 : 0xff, "Center");
		drawStringS(190, 120, 0xff, sel == 2 ? 0 : 0xff, sel == 2 ? 0 : 0xff, "Right");
		
		drawNoVsyncWithAudio();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.d_left)
			sel --;
		if(keys.d_right)
			sel ++;
		
		if(sel < 0)
			sel = 2;

		if(sel > 2)
			sel = 0;
		
		if(keys.a) {
			switch(sel) {
				case 0:
					wav64_play(&left, 0);
					break;
				case 1:
					wav64_play(&center, 1);
					break;
				case 2:
					wav64_play(&right, 2);
					break;
				default:
					break;
			}
		}
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
	
	wav64_close(&left);
	wav64_close(&center);
	wav64_close(&right);
	
	mixer_close();
	audio_close();
}

#define SPLII_CHANNELS 5

void drawAudioTestPLII() {
	int end = 0, sel = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	wav64_t left, right, center, sleft, sright;

	if(!openWAV(&left, "rom:/sound_test_left.wav64")) {
		drawMessageBox("Sound test Left samples not present");
		return;
	}
	if(!openWAV(&center, "rom:/sound_test_center.wav64")) {
		drawMessageBox("Sound test Center samples not present");
		return;
	}
	if(!openWAV(&right, "rom:/sound_test_right.wav64")) {
		drawMessageBox("Sound test Right samples not present");
		return;
	}
	if(!openWAV(&sleft, "rom:/sound_test_leftsurround.wav64")) {
		drawMessageBox("Sound test Surr. Left samples not present");
		return;
	}
	if(!openWAV(&sright, "rom:/sound_test_rightsurround.wav64")) {
		drawMessageBox("Sound test Surr. Right samples not present");
		return;
	}
	back = loadImage("rom:/mainbg.sprite");
	if(!back)
		return;
		
	audio_init(44100, ST_BUFFERS);
	mixer_init(SPLII_CHANNELS);
	
	mixer_ch_set_vol_dolby(0, 1, 0, 0, 0, 0);
	mixer_ch_set_vol_dolby(1, 0, 0, 1, 0, 0);
	mixer_ch_set_vol_dolby(2, 0, 1, 0, 0, 0);
	mixer_ch_set_vol_dolby(3, 0, 0, 0, 1, 0);
	mixer_ch_set_vol_dolby(4, 0, 0, 0, 0, 1);
	
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqClearScreen();
		rdpqDrawImage(back);
		rdpqEnd();
		
		drawStringC(40, 0x00, 0xff, 0x00, "Sound Test Prologic II");
		drawStringS(90, 120, 0xff, sel == 0 ? 0 : 0xff,	sel == 0 ? 0 : 0xff, "Left");
		drawStringC(130, 0xff, sel == 1 ? 0 : 0xff, sel == 1 ? 0 : 0xff, "Center");
		drawStringS(190, 120, 0xff, sel == 2 ? 0 : 0xff, sel == 2 ? 0 : 0xff, "Right");
		drawStringS(70, 140, 0xff, sel == 3 ? 0 : 0xff,	sel == 3 ? 0 : 0xff, "Surr. Left");
		drawStringS(170, 140, 0xff, sel == 4 ? 0 : 0xff, sel == 4 ? 0 : 0xff, "Surr. Right");
		
		drawNoVsyncWithAudio();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.d_left)
			sel --;
		if(keys.d_right)
			sel ++;
		
		if(sel < 0)
			sel = 4;

		if(sel > 4)
			sel = 0;
		
		if(keys.a) {
			switch(sel) {
				case 0:
					wav64_play(&left, 0);
					break;
				case 1:
					wav64_play(&center, 1);
					break;
				case 2:
					wav64_play(&right, 2);
					break;
				case 3:
					wav64_play(&sleft, 3);
					break;
				case 4:
					wav64_play(&sright, 4);
					break;
				default:
					break;
			}
		}
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
	
	wav64_close(&left);
	wav64_close(&center);
	wav64_close(&right);
	wav64_close(&sleft);
	wav64_close(&sright);
	
	mixer_close();
	audio_close();
}

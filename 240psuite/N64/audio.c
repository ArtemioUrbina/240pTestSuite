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
				drawStringC(100, 0xff, 0xff, 0xff, "Press #Ga#G to play");
			
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
 

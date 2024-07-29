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
 
#define CHANNEL_MUSIC   0

// Wrap the results for scenarios when the wav is not compiled into the ROM (testing/etc)
bool openWAV(wav64_t *wav64, char *fileName) {
	int fh = 0;
	
	fh = dfs_open(fileName);
	if(fh != DFS_ESUCCESS)
		return false;
	dfs_close(fh);
	
	wav64_open(wav64, fileName);
	return true;
}
 
void drawMDFourier() {
	int end = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	wav64_t mdfSamples;
	
	if(!openWAV(&mdfSamples, "rom:/mdfourier-dac-44100.wav64"))
		return;
		
	back = loadImage("rom:/mainbg.sprite");
	if(!back)
		return;

	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(back);
		rdpqEnd();
		
		if(mixer_ch_playing(CHANNEL_MUSIC))
			drawStringS(40, 60, 0xff, 0xff, 0xff, "Playing back ");
		else
			drawStringS(30, 60, 0xff, 0xff, 0xff, "Press #Ga#G to play");
		
		if(!mixer_ch_playing(CHANNEL_MUSIC))
			checkMenu(NULL, NULL);
		drawNoVsync();
		
		mixer_try_play();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(!mixer_ch_playing(CHANNEL_MUSIC))
			checkStart(keys);
		if(keys.a) {
			if(!mixer_ch_playing(CHANNEL_MUSIC))
				wav64_play(&mdfSamples, CHANNEL_MUSIC);
			else
				mixer_ch_stop(CHANNEL_MUSIC);
		}
		
		if(keys.b)
			end = 1;
	}
	
	if(mixer_ch_playing(CHANNEL_MUSIC))
		mixer_ch_stop(CHANNEL_MUSIC);
	
	wav64_close(&mdfSamples);
	freeImage(&back);
}
 

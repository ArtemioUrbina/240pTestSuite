/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
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
#include "video.h"
#include "image.h"
#include "font.h"

resolution_t		current_resolution;
unsigned int		current_bitdepth = 0;
unsigned int		current_buffers = 0;
unsigned int		current_gamma = 0;
filter_options_t	current_antialias = 0;
unsigned int		enablePAL = 0;
unsigned int		vMode = SUITE_NONE;


#define TICKS_SINCE_MS(time) TICKS_SINCE(time) / (TICKS_PER_SECOND / 1000.0f)
unsigned int videoSet = 0;

surface_t* __disp = NULL;
volatile uint64_t __frames = 0;

uint64_t __lastVbl = 0;
float __vblLen = 0;

resolution_t 	__newInternalResChange;
int 			__changeInternalRes = 0;

float getFrameLen() {
	return __vblLen;
}

void vblCallback(void) {
	uint64_t now = get_ticks_us();
	
	__vblLen = (now - __lastVbl)/1000.0f;
	__lastVbl = now;
    __frames++;
}

uint64_t __frameStart = 0;
uint64_t __idleStart = 0;
float __frameIdle = 20, __minIdle = 20;
float __frameLen = 0, __maxFrameLen = 0;

#ifdef DEBUG_BENCHMARK
void resetIdle() {
	__minIdle = 20.0f;
	__maxFrameLen = 0.0f;
}

void drawFrameLens() {
	int warn = 0;
	char str[100];
	
	if(__frameIdle < __minIdle)
		__minIdle = __frameIdle;
	if(__frameLen > __maxFrameLen)
		__maxFrameLen = __frameLen;
	
	if(__frameIdle > IDLE_WARN_MS || __frameLen > N64_NTSC_FRAME_LEN)
		warn = 1;
	
	sprintf(str, "ALL:%0.1f/%0.1f IDL:%0.1f/%0.1f VBL:%0.1f", 
		__frameLen, __maxFrameLen, __frameIdle, __minIdle, __vblLen);
	drawStringB(10, 4, !warn ? 0xff : 0x00, warn ? 0xff : 0x00, 0x00, str);
	
	sprintf(str, "RAM: %dKB/%dKB", (getUsedRAM() / 1024), get_memory_size() / 1024);
	drawStringB(10, 15, 0xff, 0xff, 0xff, str);
}
#endif

int getHardWidth() {
	if(!videoSet || vMode == SUITE_NONE)
		return 0;
	if(__changeInternalRes)
		return __newInternalResChange.width;
	return(current_resolution.width);
}

int getHardHeight() {
	if(!videoSet || vMode == SUITE_NONE)
		return 0;
	if(__changeInternalRes)
		return __newInternalResChange.height;
	return(current_resolution.height);
}

void checkInternalResChange() {
	if(!__changeInternalRes)
		return;
	
	setVideoInternal(__newInternalResChange);
	setClearScreen();
	
	__changeInternalRes = 0;
}

void getDisplay() {
	checkInternalResChange();
	__frameStart = get_ticks_us();
	
	__disp = display_get();

	rdpqUpscalePrepareFB();
}

void _internalFlipBuffer() {
	if(__disp) {
#ifdef DEBUG_BENCHMARK
		drawFrameLens();
#endif
		executeUpscaleFB();
		display_show(__disp);
		__disp = NULL;
	}
}

void _internalWaitVsync() {
	uint64_t nextFrame = __frames + 1;
	
	assertf(__disp == NULL, "_internalWaitVsync(): disp is not NULL");
	
	__idleStart = get_ticks_us();
	while (nextFrame > __frames);
	uint64_t now = get_ticks_us();
	
	__frameLen = (now - __frameStart)/1000.0f;
	__frameIdle = (now - __idleStart)/1000.0f;
}

#define MAX_AUDIO_WAIT	1.0f

void _internalWaitVsyncWithAudio() {
	float maxWait = 0, totalWait = 0, idleRest = 0;
	uint64_t 	nextFrame = __frames + 1;
	
	assertf(__disp == NULL, "_internalWaitVsync(): disp is not NULL");
	
	__idleStart = get_ticks_us();
	idleRest = N64_NTSC_FRAME_LEN - (__idleStart - __frameStart)/1000.0f;
	
	while (nextFrame > __frames) {
		if(idleRest > MAX_AUDIO_WAIT) {
			uint64_t mixerStart = 0;
			
			mixerStart = get_ticks();
			
			mixer_try_play();
			
			float wait = TICKS_SINCE_MS(mixerStart);
			totalWait += wait;
			if(wait > maxWait)
				maxWait = wait;
			idleRest -= wait;
		}
	}
	
	//assertf(idleRest > 0.2, "mixer_try_play() took longer than expected. Idle: %f", idleRest);
	//debugf("Total Wait: %f MaxWait: %f Rest: %f\n", totalWait, maxWait, idleRest);

	uint64_t now = get_ticks_us();
	
	__frameLen = (now - __frameStart)/1000.0f;
	__frameIdle = idleRest;
}

void drawNoVsyncWithAudio() {
	_internalFlipBuffer();
	
	// Fill times since we have no data
	__frameLen = (get_ticks_us() - __frameStart)/1000.0f;
	__frameIdle = __frameLen;

	mixer_try_play();
}

void waitVsyncWithAudio() {
	_internalFlipBuffer();
	_internalWaitVsyncWithAudio();
}

void waitVsync() {
	_internalFlipBuffer();
	_internalWaitVsync();
}

/* Resolutions */

void initVideo() {
	__disp = NULL;
	__changeInternalRes = 0;

	current_resolution = RESOLUTION_320x240;
	current_bitdepth = DEPTH_16_BPP;
	current_buffers = SUITE_NUM_BUFFERS;
	current_gamma = GAMMA_NONE;
	current_antialias = FILTERS_RESAMPLE;
	enablePAL = isPAL;
	vMode = SUITE_NONE;
	
	videoSet = 0;
}

void setVideoInternal(resolution_t newRes) {
	if(vMode != SUITE_NONE && isSameRes(&newRes, &current_resolution))
		return;
	
	if(videoSet) {
		disable_interrupts();
		//freeMenuFB();				// comment this one out to get "improper" resolution backgrounds for menu
		freeUpscaleFB();
		enable_interrupts();
		
		_internalWaitVsync();
		display_close();
		unregister_VI_handler(vblCallback);
		videoSet = 0;
	}
	
	current_resolution = newRes;
	display_init(current_resolution, current_bitdepth, current_buffers, current_gamma, current_antialias);
	
	vMode = videoModeToInt(&current_resolution);
	rdpq_init();
	register_VI_handler(vblCallback);
	setClearScreen();
	
	videoSet = 1;

#ifdef DEBUG_BENCHMARK
	resetIdle();
#endif
}

void changeToH256onVBlank() {
	if(__changeInternalRes)
		return;
	if(current_resolution.height == 240)
		if(current_resolution.width == 320) {
			changeVMode(RESOLUTION_256x240);
	}
	
	if(current_resolution.height == 480) {
		if(current_resolution.width == 640)
			changeVMode(RESOLUTION_512x480);
	}
}

void changeToH320onVBlank() {
	if(__changeInternalRes)
		return;
	if(current_resolution.height == 240) {
		if(current_resolution.width == 256)
			changeVMode(RESOLUTION_320x240);
	}
	
	if(current_resolution.height == 480) {
		if(current_resolution.width == 512)
			changeVMode(RESOLUTION_640x480);
	}
}

void changeVMode(resolution_t newRes) {
	if(!__changeInternalRes && isSameRes(&newRes, &current_resolution))
		return;
	__newInternalResChange = newRes;
	__changeInternalRes = 1;
}

int isVMode256() {
	if(vMode == SUITE_256x240)
		return 1;
	if(vMode == SUITE_512x480)
		return 1;
	return 0;
}

int isVMode480() {
	if(vMode == SUITE_640x480)
		return 1;
	if(vMode == SUITE_512x480)
		return 1;
	return 0;
}

int isSameRes(resolution_t *res1, const resolution_t *res2) {
	if(res1->width != res2->width)
		return 0;
	if(res1->height != res2->height)
		return 0;
	if(res1->interlaced != res2->interlaced)
		return 0;
	if(res1->pal60 != res2->pal60)
		return 0;
	return 1;
}

int videoModeToInt(resolution_t *res) {
	if(isSameRes(res, &RESOLUTION_320x240))
		return SUITE_320x240;
	if(isSameRes(res, &RESOLUTION_640x480))
		return SUITE_640x480;
	if(isSameRes(res, &RESOLUTION_640x240))
		return SUITE_512x240;
	if(isSameRes(res, &RESOLUTION_512x240))
		return SUITE_512x240;
	if(isSameRes(res, &RESOLUTION_512x480))
		return SUITE_512x480;
	if(isSameRes(res, &RESOLUTION_256x240))
		return SUITE_256x240;
	return SUITE_NONE;
}

void getVideoModeStr(char *res, int shortdesc) {
	int vmode = videoModeToInt(&current_resolution);
	if(!shortdesc) {
		switch(vmode) {
			case SUITE_320x240:
				sprintf(res, "320x240p");
				break;		
			case SUITE_640x480:
				sprintf(res, "640x480i");
				break;
			case SUITE_256x240:
				sprintf(res, "256x240");
				break;
			case SUITE_512x480:
				sprintf(res, "512x480");
				break;
		}
	}
	else {
		switch(vmode) {
			case SUITE_320x240:
				sprintf(res, "[240p]");
				break;
			case SUITE_640x480:
				sprintf(res, "[480i]");
				break;
			case SUITE_256x240:
				sprintf(res, "[256]");
				break;
			case SUITE_512x480:
				sprintf(res, "[512]");
				break;
		}
	}
}

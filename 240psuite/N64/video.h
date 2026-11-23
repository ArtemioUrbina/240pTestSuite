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
 
#ifndef VIDEO_H
#define VIDEO_H

/*
	Reference values for progressive modes, the real frame length is taken
	from libdragon after every video mode change (getFrameLenMS), since
	interlaced modes have one less half-line per field:
	NTSC 240p 16.715 ms (59.826 Hz), measured with scope and get_ticks_us() 
	NTSC 480i 16.683 ms (59.940 Hz)
	PAL  288p 20.032 ms (49.920 Hz), 313 lines x 794.5 px @ 12.414 MHz
	PAL  576i 20.000 ms (50.000 Hz), confirmed on hardware
*/
#define N64_NTSC_FRAME_LEN 16.715f
#define N64_PAL_FRAME_LEN 20.032f

#define SUITE_320x240	0
#define SUITE_640x480	1
#define SUITE_256x240	3
#define SUITE_512x480	4
#define SUITE_512x240	5
#define SUITE_320x288	6
#define SUITE_640x576	7
#define SUITE_256x288	8
#define SUITE_512x576	9

#define SUITE_NONE		-1

#define PAL_BG_DEFAULT	0xC2

extern surface_t* __disp;

extern resolution_t		current_resolution;
extern bitdepth_t		current_bitdepth;
extern unsigned int		current_buffers;
extern unsigned int		current_gamma;
extern filter_options_t	current_antialias;
extern rdpq_antialias_t current_rdp_aa_filter;
extern unsigned int		enablePAL60;
extern unsigned int		enablePAL288;
extern unsigned int		enablePALBG;
extern int				palBackR;
extern int				palBackG;
extern int				palBackB;
extern unsigned int		vMode;

int is50Hz();
float getFrameLenMS();

void getDisplay();
void waitVsync();
void waitVsyncWithAudio();
void drawNoVsyncWithAudio();

int getHardWidth();
int getHardHeight();

void initVideo();
void changeVMode(resolution_t newRes);
void togglePAL60();
void togglePAL288();
resolution_t adjustResForPAL(resolution_t res, int pal288);

int isSameRes(resolution_t *res1, const resolution_t *res2, int palCheck);
int videoModeToInt(resolution_t *res);
void getVideoModeStr(char *res, int shortdesc);
void changeToH256onVBlank();
void changeToH320onVBlank();
void changeTo16BitDepthOnVBlank();
void changeTo32BitDepthOnVBlank();
int isVMode256();
int isVMode480();
int isResolution480Class(resolution_t *res);

void setVideoInternal(resolution_t newRes);

#ifdef DEBUG_BENCHMARK
#define	IDLE_WARN_MS	2.0f
void resetIdle();
void resetHeapMin();
void drawVIRegisters(int x, int y);
#endif

#endif
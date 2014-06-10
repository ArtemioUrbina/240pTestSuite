/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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

#include <gccore.h>

#define TOTAL_VMODES 10

#define VIDEO_240P      	0   // 240p
#define VIDEO_480I_A240 	1   // 480i with assets in 240p
#define VIDEO_480I     		2	// 480i with 480p assets
#define VIDEO_288P 		 	3	// 288p
#define VIDEO_576I_A264	 	4	// 576i with assets in 240p
#define VIDEO_576I 		 	5	// 576i with 480p assets
#define VIDEO_480P_SL  		6	// 480p with software scanlines
#define VIDEO_480P 		 	7	// 480p with 480p assets

#define INVALID_VIDEO		8

#define PAL_OFFSET			3   // With this offset, PAL signal starts at line 23
								// verified with the scope. NTSC starts at 22 
								// after VBI. We can start drawing at 22 in PAL,
								// but wouldn't respect the standard.
#define PAL_LINE23HALF		0
#define PAL_LINE24			1
#define PAL_CENTERED		2

#define MODE_NTSC			0
#define MODE_PAL			1

extern u32 	ActiveFB;
extern u8	IsPAL;
extern void *frameBuffer[2][2];
extern GXRModeObj *rmode;
extern u32 vmode;
extern u8 offsetY;

extern int W;
extern int H;
extern int dW;
extern int dH;

extern	s8	AspectRatio;
extern	s8	OffsetH;

void InitVideo();
void RestoreVideo();
void InitFrameBuffers();
void DeleteFrameBuffers();
void CleanFB();
void SetVideoMode(u32 mode);
void Set576iLine23Option(s8 set);
char *GetPalStartText();

/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#ifndef V_MODES_H
#define V_MODES_H

// 240 1:1 modes
#define VIDEO_240P              0   // 240p
#define VIDEO_288P				1   // 288p
// 480 scaled modes
#define VIDEO_480I_A240         2   // 480i with assets in 240p
#define VIDEO_576I_A264			3   // 576i with assets in 240p
#define VIDEO_480P_SL           4   // 480p with software scanlines
// 480 1:1 modes
#define VIDEO_480I              5   // 480i with 480p assets
#define VIDEO_576I				6   // 576i with 480p assets
#define VIDEO_480P              7   // 480p with 480p assets
#define	HIGH_RES				VIDEO_480I			

#define INVALID_VIDEO			8

#define	PAL_LINE23			0
#define	PAL_CENTERED		1
#define	PAL_BOTTOM			2

#define	NTSC_FRAME_LEN	16.6833
#define NTSC_FRAME_RATE	59.94

#define	PAL_FRAME_LEN	20.0
#define PAL_FRAME_RATE	50.0

// Average voltage difference to match The DC 808 mV white to IRE standard 714.3 mV white
#define	MATCH_NTSC_IRE	0.8898

#include "image.h"

extern int vmode;
extern int vcable;
extern int ovmode;
extern int ovcable;

extern int W;
extern int H;
extern int dW;
extern int dH;
extern int offsetY;
extern int IsPAL;
extern int IsPALDC;

extern ImagePtr scanlines;
extern vid_mode_t custom_240;
extern vid_mode_t custom_288;
extern vid_mode_t custom_576;

void GetVideoModeStr(char *res, int shortdesc);
void AdjustVideoModes();
void LoadScanlines();
inline void ReleaseScanlines();
inline void DrawScanlines();
float GetScanlineIntensity();
void SetScanlineIntensity(float value);
int ScanlinesEven();
void LowerScanlineIntensity();
void RaiseScanlineIntensity();
void SetScanlineEven();
void SetScanlineOdd();
void ChangeResolution(int nvmode);
char *GetPalStartText();
void Set576iLine23Option(uint8 set);
void PVRStats(char *msg);
void Toggle240p480i(int mode);
#ifdef DCLOAD
void TestVideoMode(int mode);
#endif

#endif 


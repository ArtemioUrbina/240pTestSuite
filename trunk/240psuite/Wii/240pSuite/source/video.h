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

#define TOTAL_VMODES 3

#define NATIVE_320      0   // 240p
#define FAKE_640        1	// 480i with assets in 240p
//#define NATIVE_640      2	// 480i with 480p assets
#define NATIVE_640_FS   2	// 480p
#define FAKE_640_SL     3	// 480p with software scanlines

extern void *frameBuffer[TOTAL_VMODES];
extern GXRModeObj *rmode;
extern u32 vmode;

extern int W;
extern int H;
extern int dW;
extern int dH;

void InitFrameBuffers();
void SetVideoMode(u32 mode);
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

#define NATIVE_320      0
#define FAKE_640        1
#define NATIVE_640      2
#define NATIVE_640_FS   3
#define FAKE_640_SL     4

#include "image.h"

extern int vmode;
extern int vcable;


extern int W;
extern int H;
extern int dW;
extern int dH;

extern ImagePtr scanlines;

void LoadScanlines();
inline void ReleaseScanlines();
inline void DrawScanlines();
double GetScanlineIntensity();
int ScanlinesEven();
void LowerScanlineIntensity();
void RaiseScanlineIntensity();
void ToggleScanlineEvenOdd();
void ChangeResolution();
void PVRStats(char *msg);
void Toggle240p480i(int mode);

#endif 


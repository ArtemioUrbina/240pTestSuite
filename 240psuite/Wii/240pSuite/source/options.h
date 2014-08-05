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
 
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_OPTIONS { 0, 0,	0, 0, 0xC2, 0xC2, 0xC2, PAL_CENTERED, 0, 0, GX_FALSE, GX_NEAR }

struct options_st {
	int Activate480p;
	int SFCClassicController;
	int EnablePAL;
	int EnablePALBG;
	int PalBackR;
	int PalBackG;
	int PalBackB;
	int PALline23;
	int PALScale576;
	int TrapFilter;
	int FlickerFilter;
	int BilinearFiler;
};

extern struct options_st Options;
extern struct options_st DefaultOptions;

u8 LoadOptions();
u8 SaveOptions();
u8 InitFS();
void CloseFS();
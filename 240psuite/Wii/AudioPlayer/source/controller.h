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

#ifdef WII_VERSION
#include <wiiuse/wpad.h>
#endif

enum ControlTypes{	
	ControllerGC,
	ControllerWiimote,
	ControllerWiiClassic	
};

enum ControlNames{	
	ControlButtonStart,
	ControlButtonA,
	ControlButtonB,
	ControlButtonX,
	ControlButtonY,
	ControlButtonL,
	ControlButtonR
};

extern u8 ControllerType;
extern char *ControlNamesGC[];
#ifdef WII_VERSION
extern char *ControlNamesWii[];
extern char	*ControlNamesWiiCC[];
extern char	*ControlNamesWiiSFC_CC[];
#endif

s32 ControllerInit();
s32 ControllerScan();
u32 Controller_ButtonsDown(int chan);
u32 Controller_ButtonsHeld(int chan);
u8 ControllerBattery(int chan);
void ControllerRumble(int chan, int rumble);
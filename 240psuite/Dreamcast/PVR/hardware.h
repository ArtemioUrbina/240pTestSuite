/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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

#ifndef HARDWARE_H
#define HARDWARE_H

void ControllerTest();
void ListMapleDevices();
void MemoryViewer(uint32 address);
void VMUControllerTest();
int isLightGunPresent();
void LightGunTest();
void MicrophoneTest();

int flashrom_get_region_country();
int flashrom_get_region_language();
int flashrom_get_region_broadcast();

void Show_ISP_Data();

int check_for_bad_lcd();

#define FLASHROM_CACHE_SIZE			6

#define FLASHROM_BROADCAST_NTSC		1
#define FLASHROM_BROADCAST_PAL		2
#define FLASHROM_BROADCAST_PALM		3
#define FLASHROM_BROADCAST_PALN		4
#define FLASHROM_BROADCAST_UNKNOWN	0

#define FLASHROM_LANGUAGE_JAPAN		1
#define FLASHROM_LANGUAGE_ENGLISH	2
#define FLASHROM_LANGUAGE_GERMAN	3
#define FLASHROM_LANGUAGE_FRENCH	4
#define FLASHROM_LANGUAGE_SPANISH	5
#define FLASHROM_LANGUAGE_ITALIAN	6
#define FLASHROM_LANGUAGE_UNKNOWN	0

// Redefined from KOS, seems those are wrong or I don't know how to use them

// 0-7 empty?

#define 	CONT_FIXED_CAPABILITY_RTRIG   		(1<<8)
#define 	CONT_FIXED_CAPABILITY_LTRIG   		(1<<9)
#define 	CONT_FIXED_CAPABILITY_ANALOG_X   	(1<<10)
#define 	CONT_FIXED_CAPABILITY_ANALOG_Y   	(1<<11)

#define 	CONT_FIXED_CAPABILITY_ANALOG2_X   	(1<<12)
#define 	CONT_FIXED_CAPABILITY_ANALOG2_Y   	(1<<13)

// 14 & 15? Might be two other triggers?

#define 	CONT_FIXED_CAPABILITY_Z   			(1<<16)
#define 	CONT_FIXED_CAPABILITY_Y   			(1<<17)
#define 	CONT_FIXED_CAPABILITY_X   			(1<<18)
#define 	CONT_FIXED_CAPABILITY_D   			(1<<19)

#define 	CONT_FIXED_CAPABILITY_DPAD2_UP   	(1<<20)
#define 	CONT_FIXED_CAPABILITY_DPAD2_DOWN   	(1<<21)
#define 	CONT_FIXED_CAPABILITY_DPAD2_LEFT   	(1<<22)
#define 	CONT_FIXED_CAPABILITY_DPAD2_RIGHT   (1<<23)


#define 	CONT_FIXED_CAPABILITY_C   			(1<<24)
#define 	CONT_FIXED_CAPABILITY_B   			(1<<25)
#define 	CONT_FIXED_CAPABILITY_A   			(1<<26)
#define 	CONT_FIXED_CAPABILITY_START   		(1<<27)

#define 	CONT_FIXED_CAPABILITY_DPAD_UP   	(1<<28)
#define 	CONT_FIXED_CAPABILITY_DPAD_DOWN   	(1<<29)
#define 	CONT_FIXED_CAPABILITY_DPAD_LEFT   	(1<<30)
#define 	CONT_FIXED_CAPABILITY_DPAD_RIGHT   	(1<<31)

#endif


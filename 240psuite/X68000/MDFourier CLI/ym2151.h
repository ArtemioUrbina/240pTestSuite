/* 
 * 240p Test Suite
 * Copyright (C)2021 Artemio Urbina
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
 
#ifndef _YM2151_H_
#define _YM2151_H_

#include "types.h"

#define YM2151_REG	0xE90001
#define YM2151_DATA	0xE90003

#define YM2151_REG_PAN	0x20
#define YM2151_REG_VOL	0x60

#define YM2151_MAX_VOL	0
#define YM2151_MIN_VOL	127

#define YM2151_LEFT		0x80
#define YM2151_RIGHT	0x40
#define YM2151_STEREO	0xC0

void YM2151_reset();
void YM2151_setvolume(u8 channel, u8 slot, u8 volume);

void YM2151_writeReg(const u8 reg, const u8 data);

#endif // _YM2151_H_

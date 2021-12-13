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
 
#include "ym2151.h"

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


void YM2151_writeReg(const u8 reg, const u8 data)
{
	vu8 *regPTR, *dataPTR;

	regPTR = (u8*) YM2151_REG;
	dataPTR = (u8*) YM2151_DATA;

	// set reg and write data

	// wait while YM2151 busy
	while (*dataPTR & 0x80);
	*regPTR = reg;
	// wait while YM2151 busy
	while (*dataPTR & 0x80);
	*dataPTR = data;

	//printf("%02X->%c%c%c%c%c%c%c%c\n", reg, BYTE_TO_BINARY(data));
}

void YM2151_setvolume(u8 channel, u8 slot, u8 volume)
{
	u8 reg = 0;

	reg = YM2151_REG_VOL + 8*slot + channel;
	YM2151_writeReg(reg, volume);
}

void YM2151_reset()
{

}
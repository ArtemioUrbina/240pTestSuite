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

#ifndef GBA_H
#define GBA_H

#include "image.h"

#define GBA_TRANSFER_OK		0
#define GBA_INVALID_PORT 	-1
#define GBA_ROM_TOO_BIG 	-2
#define GBA_NOTCONNECTED 	-3
#define GBA_BIOS_TIMEOUT	-4
#define GBA_INVALID_SESSION	-5
#define GBA_TRANSFER_ERROR	-6
#define GBA_FINISH_ERROR	-7

void InitGBARAM();
void ReleaseGBARAM();
u32 checkGBAConnected(unsigned int port);
int detectLinkCable(ImagePtr title, unsigned int port);
int GBASendROM(ImagePtr title, unsigned int port);

#endif


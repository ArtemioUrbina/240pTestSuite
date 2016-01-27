/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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

#include "genesis.h"
#include "myvdp.h"

u16 pal_240;

void VDP_setSpriteAttr(u16 index, u16 tile_attr)
{
	SpriteDef *sprite;

	sprite = &vdpSpriteCache[index];

	sprite->tile_attr = tile_attr;
}

u16 Detect_VDP_PAL()
{
	return (GET_VDPSTATUS(IS_PALSYSTEM));
}

void VDP_setMyTileMapRect(u16 plan, const u16 * data, u16 basetile, u16 x, u16 y, u16 w, u16 h)
{
	vu32 *plctrl;
	vu16 *pwdata;
	const u16 *src;
	u32 addr;
	u32 planwidth;
	u16 i, j;

	VDP_setAutoInc(2);

	/* point to vdp port */
	plctrl = (u32 *) GFX_CTRL_PORT;
	pwdata = (u16 *) GFX_DATA_PORT;

	planwidth = VDP_getPlanWidth();
	addr = plan + (2 * (x + (planwidth * y)));
	src = data;

	i = h;
	while(i--)
	{
		*plctrl = GFX_WRITE_VRAM_ADDR(addr);

		j = w;
		while(j--)
			*pwdata = basetile + *src++;

		addr += planwidth * 2;
	}
}

u8 VDP_Detect_Interlace()
{
	if(VDP_getReg(0x0C) & 0x02)
		return 1;
	return 0;
}

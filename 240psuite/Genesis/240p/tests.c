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
#include "res.h"
#include "tests.h"
#include "help.h"
#include "main.h"
#include "mdfourier.h"

typedef struct timecode
{
	u16 hours;
	u16 minutes;
	u16 seconds;
	u16 frames;
	u16 type;
	u16 res;
} timecode;

void DrawCheckBoard()
{
	char cntstr[4];
	u16 ind = 0, size = 0, count = 0, docounter = 0, loadvram = 1;
	u16 field = 1, alternate = 0, exit = 0, type = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;

	type = DrawFloatMenuRes(RES_320);
	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			
			if(type == RES_256)
				VDP_setScreenWidth256();
			else
				VDP_setScreenWidth320();
				
			VDP_setPalette(PAL1, bw_pal);

			ind = TILE_USERINDEX;
			size = sizeof(check_tile) / 32;
			VDP_loadTileData(check_tile, ind, size, USE_DMA);
			ind += size;
			size = sizeof(check_tile_inv) / 32;
			VDP_loadTileData(check_tile_inv, ind, size, USE_DMA);

			VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
			VDP_End();
			loadvram = 0;
		}

		if(alternate)
		{
			if(field == 0)
			{
				VDP_Start();
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				VDP_End();
				field = 1;
			}
			else
			{
				VDP_Start();
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				VDP_End();
				field = 0;
			}
		}

		if(docounter)
		{
			count++;

			if(IsPALVDP)
			{
				if(count > 49)
					count = 0;
			}
			else
			{
				if(count > 59)
					count = 0;
			}

			intToStr(count, cntstr, 2);
			VDP_Start();
			VDP_drawText("Frame:", 2, 25);
			VDP_drawText(cntstr, 8, 25);
			VDP_End();
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_CHECK))
			loadvram = 1;

		if(pressedButtons & BUTTON_A)
			alternate = ~alternate;

		if(!alternate && (pressedButtons & BUTTON_UP || pressedButtons & BUTTON_DOWN))
		{
			if(field == 0)
			{
				VDP_Start();
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				VDP_End();
				field = 1;
			}
			else
			{
				VDP_Start();
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				VDP_End();
				field = 0;
			}
		}

		if(pressedButtons & BUTTON_B)
			docounter = ~docounter;

		if(pressedButtons & BUTTON_START)
			exit = 1;
			
		if(pressedButtons & BUTTON_C)
		{
			type = DrawFloatMenuRes(type);
			oldButtons |= BUTTON_A;
			loadvram = 1;
		}
	
		VDP_waitVSync();
	}
}

void DrawStripes()
{
	char cntstr[4];
	u16 hor1 = 0, hor2 = 0, ver1 = 0, ver2 = 0, size = 0, count = 0, docounter = 0, loadvram = 1;
	u16 field = 1, alternate = 0, exit = 0, vertical = 0, redraw = 0;
	u16 buttons = 0, oldButtons = 0xffff, pressedButtons = 0, type = 0;	

	type = DrawFloatMenuRes(RES_320);
	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			if(type == RES_256)
				VDP_setScreenWidth256();
			else
				VDP_setScreenWidth320();
				
			VDP_setPalette(PAL1, bw_pal);

			hor1 = TILE_USERINDEX;
			size = sizeof(bw_tile) / 32;
			VDP_loadTileData(bw_tile, hor1, size, USE_DMA);
			hor2 = hor1 + size;
			size = sizeof(wb_tile) / 32;
			VDP_loadTileData(wb_tile, hor2, size, USE_DMA);
			ver1 = hor2 + size;
			size = sizeof(vstripes_tiles) / 32;
			VDP_loadTileData(vstripes_tiles, ver1, size, USE_DMA);
			ver2 = ver1 + size;
			size = sizeof(vstripesneg_tiles) / 32;
			VDP_loadTileData(vstripesneg_tiles, ver2, size, USE_DMA);
			VDP_End();

			redraw = 1;
			loadvram = 0;
		}

		if(alternate || redraw)
		{
			VDP_Start();
			if(field == 0)
			{
				if(vertical)
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver1, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				else
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor1, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				field = 1;
			}
			else
			{
				if(vertical)
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver2, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				else
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor2, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				field = 0;
			}
			VDP_End();
			redraw = 0;
		}

		if(docounter)
		{
			count++;
			if(Detect_VDP_PAL())
			{
				if(count > 49)
					count = 0;
			}
			else
			{
				if(count > 59)
					count = 0;
			}

			intToStr(count, cntstr, 2);
			VDP_Start();
			VDP_drawText("Frame:", 2, 25);
			VDP_drawText(cntstr, 8, 25);
			VDP_End();
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_STRIPES))
			loadvram = 1;

		if(pressedButtons & BUTTON_A)
			alternate = ~alternate;

		if(!alternate && (pressedButtons & BUTTON_UP || pressedButtons & BUTTON_DOWN))
		{
			VDP_Start();
			if(field == 0)
			{
				if(vertical)
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver1, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				else
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor1, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				field = 1;
			}
			else
			{
				if(vertical)
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver2, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				else
					VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor2, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
				field = 0;
			}
			VDP_End();
		}

		if(pressedButtons & BUTTON_LEFT || pressedButtons & BUTTON_RIGHT)
		{
			vertical = ~vertical;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_B)
			docounter = ~docounter;

		if(pressedButtons & BUTTON_START)
			exit = 1;
			
		if(pressedButtons & BUTTON_C)
		{
			type = DrawFloatMenuRes(type);
			oldButtons |= BUTTON_A;
			loadvram = 1;
		}

		VDP_waitVSync();
	}
}

void DropShadowTest()
{
	u16 sonic_water = 0, sonic_floor = 0, frame = 0, direction = 0, loadvram = 1;
	u16 size = 0, ind = 0, back = 0, changeback = 1, invert = 0, sprite = 0, redraw = 0;
	u16 field = 1, x = 0, y = 0, exit = 0, text = 0, shadowpos = 0, buzzpos = 0, buzzshadowpos = 0, waterfall = 0;
	u16 buttons = 0, pressedButtons = 0, oldButtons = 0xffff;

	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			
			VDP_setPalette(PAL0, palette_green);
			VDP_setPalette(PAL1, motoko_pal);
			VDP_setPalette(PAL2, wb_pal);
			VDP_setPalette(PAL3, buzz_pal);

			shadowpos = TILE_USERINDEX;
			size = sizeof(shadow_tiles) / 32;
			VDP_loadTileData(shadow_tiles, shadowpos, size, USE_DMA);

			buzzpos = shadowpos + size;
			size = sizeof(buzzShadow_tiles) / 32;
			VDP_loadTileData(buzz_tiles, buzzpos, size, USE_DMA);

			buzzshadowpos = buzzpos + size;
			size = sizeof(buzzShadow_tiles) / 32;
			VDP_loadTileData(buzzShadow_tiles, buzzshadowpos, size, USE_DMA);

			waterfall = buzzshadowpos + size;
			size = sizeof(waterfall_tiles) / 32;
			VDP_loadTileData(waterfall_tiles, waterfall, size, USE_DMA);

			ind = waterfall + size;

			sprite = random() % 2;

			VDP_setSprite(0, 12, 12, SPRITE_SIZE(4, 4), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos, 1);
			VDP_setSprite(1, 32, 32, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos, 2);
			VDP_setSprite(2, 0, 0, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos, 3);
			VDP_setSprite(3, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 4);
			VDP_setSprite(4, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 0);

			if(sprite == 1)
			{
				x = 32;
				y = 32;

				VDP_setSpritePosition(2, 320, 224);
			}
			else
			{
				VDP_setSpritePosition(0, 320, 224);
				VDP_setSpritePosition(1, 320, 224);
			}
			VDP_End();
			loadvram = 0;
			changeback = 1;
		}

		if(changeback || redraw)
		{
			VDP_Start();
			
			if(redraw)
				VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);

			if(back != 1)
				VDP_setPalette(PAL0, palette_green);

			if(back == 2)
			{
				VDP_setHorizontalScroll(PLAN_B, 0);
				VDP_setHorizontalScroll(PLAN_A, 0);
				VDP_setSpritePosition(3, 320, 224);
				VDP_setSpritePosition(4, 320, 224);
				VDP_updateSprites();
			}

			switch (back)
			{
			case 0:
				if(!redraw)
				{
					size = sizeof(motoko_tiles) / 32;
					VDP_loadTileData(motoko_tiles, ind, size, USE_DMA);
					VDP_setPalette(PAL1, motoko_pal);
				}
				VDP_setMyTileMapRect(BPLAN, motoko_map, ind, 0, 0, 320 / 8, 224 / 8);
				break;
			case 1:
				if(!redraw)
				{
					VDP_setPalette(PAL0, sonicback_pal);
					VDP_setPalette(PAL1, sonicwater_pal);

					size = sizeof(sonicback_tiles) / 32;
					VDP_loadTileData(sonicback_tiles, ind, size, USE_DMA);

					sonic_water = ind + size;
					size = sizeof(sonicwater_tiles) / 32;
					VDP_loadTileData(sonicwater_tiles, sonic_water, size, USE_DMA);

					sonic_floor = sonic_water + size;
					size = sizeof(sonicfloor_tiles) / 32;
					VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, USE_DMA);
				}

				if(!redraw)
				{
					VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 0, 0, 256 / 8, 152 / 8);
					VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 256 / 8, 0, 256 / 8, 152 / 8);
					VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 0, 152 / 8, 256 / 8, 48 / 8);
					VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 256 / 8, 152 / 8, 256 / 8, 48 / 8);
				}
				VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 0, 96 / 8, 256 / 8, 128 / 8);
				VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 256 / 8, 96 / 8, 256 / 8, 128 / 8);
				break;
			case 2:
				if(!redraw)
				{
					size = sizeof(check_tile) / 32;
					VDP_loadTileData(check_tile, ind, size, USE_DMA);
					VDP_setPalette(PAL1, bw_pal);
				}
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320 / 8, 224 / 8);
				break;
			case 3:
				if(!redraw)
				{
					size = sizeof(bw_tile) / 32;
					VDP_loadTileData(bw_tile, ind, size, USE_DMA);
					VDP_setPalette(PAL1, bw_pal);
				}
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320 / 8, 224 / 8);
				break;
			}
			VDP_End();
			changeback = 0;
			redraw = 0;
		}

		if(text)
		{
			text--;
			if(text == 0)
				redraw = 1;
		}

		if(sprite == 1)
		{
			VDP_Start();
			VDP_setSpritePosition(0, x - 20, y - 20);
			VDP_End();
		}

		if(field == invert)
		{
			VDP_Start();
			VDP_setSpritePosition(sprite == 1 ? 1 : 2, x, y);
			VDP_End();
			field = !field;
		}
		else
		{
			VDP_Start();
			VDP_setSpritePosition(sprite == 1 ? 1 : 2, 320, 224);
			VDP_End();

			field = !field;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;
		
		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_SHADOW))
			loadvram = 1;

		if(buttons & BUTTON_UP)
		{
			if(y > 0)
				y--;
		}

		if(buttons & BUTTON_DOWN)
		{
			if(y < 192)
				y++;
		}

		if(buttons & BUTTON_LEFT)
		{
			if(x > 0)
				x--;
			if(direction == 1)
			{
				direction = 0;
				VDP_Start();
				VDP_setSpriteAttr(0, TILE_ATTR(PAL3, 0, 0, direction) + buzzpos);
				VDP_setSpriteAttr(1, TILE_ATTR(PAL2, 0, 0, direction) + buzzshadowpos);
				VDP_End();
			}
		}

		if(buttons & BUTTON_RIGHT)
		{
			if(x < 288)
				x++;
			if(direction == 0)
			{
				direction = 1;
				VDP_Start();
				VDP_setSpriteAttr(0, TILE_ATTR(PAL3, 0, 0, direction) + buzzpos);
				VDP_setSpriteAttr(1, TILE_ATTR(PAL2, 0, 0, direction) + buzzshadowpos);
				VDP_End();
			}
		}

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_A)
		{
			invert = !invert;

			VDP_Start();
			if(invert)
				VDP_drawText("Shadow on odd frames ", 19, 0);
			else
				VDP_drawText("Shadow on even frames", 19, 0);
			VDP_End();

			text = 60;
		}

		if(back == 1)
		{
			VDP_Start();
			VDP_setSpritePosition(3, x * -2 + 79, 120);
			VDP_setSpritePosition(4, x * -2 + 335, 120);
			VDP_setHorizontalScroll(PLAN_B, x * -2);
			VDP_setHorizontalScroll(PLAN_A, x * -4);

			switch (frame)
			{
			case 30:
				VDP_setPalette(PAL1, sonicwater3_pal);
				break;
			case 60:
				VDP_setPalette(PAL1, sonicwater2_pal);
				break;
			case 90:
				VDP_setPalette(PAL1, sonicwater_pal);
				break;
			}
			VDP_End();

			frame++;
			if(frame > 90)
				frame = 1;
		}

		if(pressedButtons & BUTTON_B)
		{
			if(back < 3)
				back++;
			else
				back = 0;

			changeback = 1;
			VDP_Start();
			VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_End();
		}

		if(pressedButtons & BUTTON_C)
		{
			VDP_Start();
			if(!sprite)
				VDP_setSpritePosition(2, 320, 224);
			else
			{
				VDP_setSpritePosition(0, 320, 224);
				VDP_setSpritePosition(1, 320, 224);
			}
			VDP_End();

			sprite = !sprite;
		}

		VDP_Start();
		VDP_updateSprites();
		VDP_End();
		
		VDP_waitVSync();
	}
}

void StripedSpriteTest()
{
	u16 sonic_water = 0, sonic_floor = 0, sprite, frame = 0, waterfall = 0;
	u16 size, ind = 0, back = 0, changeback = 1, loadvram = 1;
	u16 x = 0, y = 0, exit = 0;
	u16 buttons, pressedButtons, oldButtons = 0xffff;

	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			VDP_setPalette(PAL1, motoko_pal);
			VDP_setPalette(PAL3, wb_pal);

			sprite = TILE_USERINDEX;
			size = sizeof(striped_tiles) / 32;
			VDP_loadTileData(striped_tiles, sprite, size, USE_DMA);
			VDP_setSprite(0, x, y, SPRITE_SIZE(4, 4), TILE_ATTR(PAL3, 0, 0, 0) + sprite, 1);

			waterfall = sprite + size;
			size = sizeof(waterfall_tiles) / 32;
			VDP_loadTileData(waterfall_tiles, waterfall, size, USE_DMA);

			VDP_setSprite(1, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 2);
			VDP_setSprite(2, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 0);

			ind = waterfall + size;
			
			VDP_End();

			loadvram = 0;
			changeback = 1;
		}

		if(changeback)
		{
			changeback = 0;
			
			VDP_Start();
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
			if(back != 1)
			{
				VDP_setHorizontalScroll(PLAN_B, 0);
				VDP_setHorizontalScroll(PLAN_A, 0);
				VDP_setSpritePosition(1, 320, 224);
				VDP_setSpritePosition(2, 320, 224);
				VDP_updateSprites();
			}
			switch (back)
			{
			case 0:
				size = sizeof(motoko_tiles) / 32;
				VDP_loadTileData(motoko_tiles, ind, size, USE_DMA);
				VDP_setPalette(PAL1, motoko_pal);
				VDP_setMyTileMapRect(BPLAN, motoko_map, ind, 0, 0, 320 / 8, 224 / 8);
				break;
			case 1:
				VDP_setPalette(PAL0, sonicback_pal);
				VDP_setPalette(PAL1, sonicwater_pal);
				size = sizeof(sonicback_tiles) / 32;
				VDP_loadTileData(sonicback_tiles, ind, size, USE_DMA);
				sonic_water = ind + size;
				size = sizeof(sonicwater_tiles) / 32;
				VDP_loadTileData(sonicwater_tiles, sonic_water, size, USE_DMA);
				sonic_floor = sonic_water + size;
				size = sizeof(sonicfloor_tiles) / 32;
				VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, USE_DMA);
				VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 0, 0, 256 / 8, 152 / 8);
				VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 256 / 8, 0, 256 / 8, 152 / 8);
				VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 0, 152 / 8, 256 / 8, 48 / 8);
				VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 256 / 8, 152 / 8, 256 / 8, 48 / 8);
				VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 0, 96 / 8, 256 / 8, 128 / 8);
				VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 256 / 8, 96 / 8, 256 / 8, 128 / 8);
				break;
			case 2:
				size = sizeof(check_tile) / 32;
				VDP_loadTileData(check_tile, ind, size, USE_DMA);
				VDP_setPalette(PAL1, bw_pal);
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320 / 8, 224 / 8);
				break;
			case 3:
				size = sizeof(bw_tile) / 32;
				VDP_loadTileData(bw_tile, ind, size, USE_DMA);
				VDP_setPalette(PAL1, bw_pal);
				VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320 / 8, 224 / 8);
				break;
			}
			VDP_End();
		}

		VDP_Start();
		VDP_setSpritePosition(0, x, y);
		VDP_End();

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_STRIPED))
			loadvram = 1;

		if(buttons & BUTTON_UP)
		{
			if(y > 0)
				y--;
		}

		if(buttons & BUTTON_DOWN)
		{
			if(y < 192)
				y++;
		}

		if(buttons & BUTTON_LEFT)
		{
			if(x > 0)
				x--;
		}

		if(buttons & BUTTON_RIGHT)
		{
			if(x < 288)
				x++;
		}

		if(back == 1)
		{
			VDP_Start();
			
			VDP_setSpritePosition(1, x * -2 + 79, 120);
			VDP_setSpritePosition(2, x * -2 + 335, 120);
			VDP_setHorizontalScroll(PLAN_B, x * -2);
			VDP_setHorizontalScroll(PLAN_A, x * -4);

			switch (frame)
			{
			case 30:
				VDP_setPalette(PAL1, sonicwater3_pal);
				break;
			case 60:
				VDP_setPalette(PAL1, sonicwater2_pal);
				break;
			case 90:
				VDP_setPalette(PAL1, sonicwater_pal);
				break;
			}
			VDP_End();

			frame++;
			if(frame > 90)
				frame = 1;
		}

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_A)
		{
			if(back > 0)
				back--;
			else
				back = 3;

			changeback = 1;
		}

		if(pressedButtons & BUTTON_B)
		{
			if(back < 3)
				back++;
			else
				back = 0;

			changeback = 1;
		}

		VDP_Start();
		VDP_updateSprites();
		VDP_End();
		VDP_waitVSync();
	}
}



void LagTest()
{
	char str[10];
	u16 pal = PAL0, change = 1, loadvram = 1;
	s16 speed = 1, vary = 0;
	u16 size, ind = 0, usersound = 0;
	u16 x = 0, y = 0, x2 = 0, y2 = 0, exit = 0, variation = 1, draw = 1;
	u16 buttons, pressedButtons, oldButtons = 0xffff;
	u16 pos = 0, view = 0, audio = 1, drawoffset = 0;
	u16 first_pal[16], oldColor = 0;
	s16 clicks[10];

	x = 144;
	y = 60;

	x2 = 108;
	y2 = 96;
	
	PSG_init();
	PSG_setFrequency(0, 1000);

	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			
			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_red);
			VDP_setPalette(PAL2, bw_pal);
			VDP_setPalette(PAL3, palette_green);

			ind = TILE_USERINDEX;
			size = sizeof(shadow_tiles) / 32;
			VDP_loadTileData(shadow_tiles, ind, size, USE_DMA);
			ind += size;
			size = sizeof(lag_tiles) / 32;
			VDP_loadTileData(lag_tiles, ind, size, USE_DMA);

			VDP_setSprite(0, x, 96, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + ind, 1);
			VDP_setSprite(1, x, y, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + ind, 2);
			VDP_setSprite(2, x2, y2, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + ind, 0);

			VDP_drawTextBG(APLAN, "Press the \"A\" button when the sprite", TILE_ATTR(PAL3, 0, 0, 0), 2, 21);
			VDP_drawTextBG(APLAN, "is aligned. A negative value means", TILE_ATTR(PAL3, 0, 0, 0), 2, 22);
			VDP_drawTextBG(APLAN, "you pressed \"A\" before they intersect.", TILE_ATTR(PAL3, 0, 0, 0), 2, 23);
			VDP_drawTextBG(APLAN, "\"B\" button toggles horz/vert", TILE_ATTR(PAL3, 0, 0, 0), 2, 24);
			VDP_drawTextBG(APLAN, "\"C\" button toggles audio", TILE_ATTR(PAL3, 0, 0, 0), 2, 25);
			VDP_drawTextBG(APLAN, "DOWN toggles random/rhythmic", TILE_ATTR(PAL3, 0, 0, 0), 2, 26);

			loadvram = 0;
			draw = 1;

			if(pos)
			{
				int i = 0;
				u16 ppos = 0;

				for(i = 0; i < pos; i++)
				{
					pal = PAL0;

					intToStr(clicks[i], str, 1);

					if(clicks[i] == 0)
						pal = PAL3;
					if(clicks[i] < 0)
						pal = PAL1;

					ppos = i + 1;
					intToStr(ppos, str, 1);
					VDP_drawTextBG(APLAN, "Offset", TILE_ATTR(PAL0, 0, 0, 0), 2, i);
					VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), 9, i);
					VDP_drawTextBG(APLAN, ":", TILE_ATTR(PAL0, 0, 0, 0), i == 9 ? 11 : 10, i);
					intToStr(clicks[i], str, 1);
					VDP_drawTextBG(APLAN, str, TILE_ATTR(pal, 0, 0, 0), i == 9 ? 13 : 12, i);
					ppos = strlen(str);
					if(clicks[i] == 1)
						VDP_drawTextBG(APLAN, " frame    ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, i);
					else
						VDP_drawTextBG(APLAN, " frames   ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, i);
				}
			}
			VDP_End();
		}
		
		if(y == 96)	// half the screen?        
		{
			VDP_Start();
			if(audio && !usersound)
				PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
				
			VDP_getPalette(PAL0, first_pal);
			first_pal[0] = oldColor;
			VDP_setPalette(PAL0, first_pal);
			VDP_End();
		}
		
		if(usersound)
		{
			PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
			usersound = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_MANUALLAG))
			loadvram = 1;

		if(pressedButtons & BUTTON_A)
		{
			if(change)
			{
				clicks[pos] = (y - 96) * speed;
				drawoffset = 1;
				if(clicks[pos] >= 0)
					change = 0;
					
				if(audio)
				{
					if(clicks[pos] == 0)
						PSG_setFrequency(0, 1000);
					else
						PSG_setFrequency(0, 500);
						
					PSG_setEnvelope(0, PSG_ENVELOPE_MAX);
					usersound = 1;
				}
			}
		}

		if(pressedButtons & BUTTON_B)
		{
			view++;
			if(view > 2)
				view = 0;
		}

		if(pressedButtons & BUTTON_C)
		{
			audio = !audio;
			draw = 1;
		}

		if(pressedButtons & BUTTON_DOWN)
		{
			variation = !variation;
			if(!variation)
				vary = 0;
			draw = 1;
		}

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(drawoffset)
		{
			u16 ppos = 0;

			pal = PAL0;
			intToStr(clicks[pos], str, 1);

			if(clicks[pos] == 0)
				pal = PAL3;
			if(clicks[pos] < 0)
				pal = PAL1;

			ppos = pos + 1;
			intToStr(ppos, str, 1);
			VDP_Start();
			VDP_drawTextBG(APLAN, "Offset", TILE_ATTR(PAL0, 0, 0, 0), 2, pos);
			VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), 9, pos);
			VDP_drawTextBG(APLAN, ":", TILE_ATTR(PAL0, 0, 0, 0), pos == 9 ? 11 : 10, pos);
			intToStr(clicks[pos], str, 1);
			VDP_drawTextBG(APLAN, str, TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12, pos);
			ppos = strlen(str);
			if(clicks[pos] == 1)
				VDP_drawTextBG(APLAN, " frame    ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, pos);
			else
				VDP_drawTextBG(APLAN, " frames   ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, pos);

			VDP_End();
			if(clicks[pos] >= 0)
				pos++;

			if(pos > 9)
				exit = 1;
			drawoffset = 0;
		}

		if(draw)
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, "Audio:", TILE_ATTR(PAL0, 0, 0, 0), 24, 0);
			if(audio)
				VDP_drawTextBG(APLAN, "on ", TILE_ATTR(PAL0, 0, 0, 0), 31, 0);
			else
				VDP_drawTextBG(APLAN, "off", TILE_ATTR(PAL0, 0, 0, 0), 31, 0);
			VDP_drawTextBG(APLAN, "Timing:", TILE_ATTR(PAL0, 0, 0, 0), 24, 1);
			if(variation)
				VDP_drawTextBG(APLAN, "random  ", TILE_ATTR(PAL0, 0, 0, 0), 32, 1);
			else
				VDP_drawTextBG(APLAN, "rhythmic", TILE_ATTR(PAL0, 0, 0, 0), 32, 1);
			VDP_End();
			draw = 0;
		}

		if(y > 132 + vary)
		{
			speed = -1;
			change = 1;
			if(variation)
			{
				if(random() % 2)
					vary = random() % 7;
				else
					vary = -1 * random() % 7;
			}
		}

		if(y < 60 + vary)
		{
			speed = 1;
			change = 1;
			if(variation)
			{
				if(random() % 2)
					vary = random() % 7;
				else
					vary = -1 * random() % 7;
			}
		}

		y += speed;
		x2 += speed;

		VDP_Start();
		if(y == 96)	// Red on the spot
			VDP_setSpriteAttr(0, TILE_ATTR(PAL1, 0, 0, 0) + ind);
		if(y == 95 || y == 97)	//Green one pixel before or after
			VDP_setSpriteAttr(0, TILE_ATTR(PAL3, 0, 0, 0) + ind);
		if(y == 98 || y == 94)	//Back to white two pixels before or after
			VDP_setSpriteAttr(0, TILE_ATTR(PAL2, 0, 0, 0) + ind);

		if(view == 0 || view == 2)
			VDP_setSpritePosition(1, x, y);
		else
			VDP_setSpritePosition(1, 320, 224);

		if(view == 1 || view == 2)
			VDP_setSpritePosition(2, x2, y2);
		else
			VDP_setSpritePosition(2, 320, 224);

		VDP_updateSprites();
		VDP_End();

		if(y == 96)	// half the screen?        
		{
			VDP_Start();
			if(audio)
			{
				PSG_setFrequency(0, 1000);
				PSG_setEnvelope(0, PSG_ENVELOPE_MAX);
			}

			VDP_getPalette(PAL0, first_pal);
			oldColor = first_pal[0];
			first_pal[0] = 0x0666;
			VDP_setPalette(PAL0, first_pal);
			VDP_End();
		}
		VDP_waitVSync();
	}

	if(pos > 9)
	{
		fix32 totald = 0, cnt, tot;
		u16 total = 0, count = 0, c = 0;
		
		PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
		
		VDP_Start();
		VDP_resetSprites();
		VDP_updateSprites();
		VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
		VDP_End();
		
		DrawMainBG();
		exit = 0;

		for(c = 0; c < 10; c++)
		{
			if(clicks[c] != 0xFF)
			{
				intToStr(clicks[c], str, 1);

				pal = PAL0;
				if(clicks[c] == 0)
					pal = PAL3;
				if(clicks[c] < 0)
					pal = PAL1;
				if(clicks[c] >= 0)
				{
					total += clicks[c];
					count++;
				}

				VDP_Start();
				VDP_drawTextBG(APLAN, str, TILE_ATTR(pal, 0, 0, 0), 10, c + 7);
				VDP_End();
			}
		}

		VDP_Start();
		VDP_drawTextBG(APLAN, "+", TILE_ATTR(PAL1, 0, 0, 0), 8, 11);
		VDP_End();

		if(count > 0)
		{
			u16 h = 10;
			u16 v = 18;

			VDP_Start();
			
			VDP_drawTextBG(APLAN, "----", TILE_ATTR(PAL0, 0, 0, 0), h - 2, v++);

			cnt = intToFix32(count);
			tot = intToFix32(total);

			intToStr(total, str, 1);
			VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), h, v);
			h += strlen(str);
			VDP_drawTextBG(APLAN, "/", TILE_ATTR(PAL1, 0, 0, 0), h++, v);

			intToStr(count, str, 1);
			VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), h, v);
			h += strlen(str);
			VDP_drawTextBG(APLAN, "=", TILE_ATTR(PAL1, 0, 0, 0), h++, v);

			totald = fix32Div(tot, cnt);
			fix32ToStr(totald, str, 4);
			VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL3, 0, 0, 0), h, v);
			h += strlen(str);
			h -= 2;
			VDP_drawTextBG(APLAN, " frames", TILE_ATTR(PAL0, 0, 0, 0), h, v++);
			VDP_drawTextBG(APLAN, "Keep in mind that a frame is", TILE_ATTR(PAL0, 0, 0, 0), 6, ++v);
			VDP_drawTextBG(APLAN, "around 16.68 milliseconds.", TILE_ATTR(PAL0, 0, 0, 0), 6, ++v);

			h = 14;
			v = 12;
			tot = FIX32(16.6834);
			cnt = fix32Mul(totald, tot);
			fix32ToStr(cnt, str, 2);
			VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL3, 0, 0, 0), h, v);
			h += strlen(str);
			//h -= 2;
			VDP_drawTextBG(APLAN, " milliseconds", TILE_ATTR(PAL0, 0, 0, 0), h, v);

			if(total < 5)
				VDP_drawTextBG(APLAN, "EXCELLENT REFLEXES!", TILE_ATTR(PAL1, 0, 0, 0), 14, 15);
			if(total == 0)
				VDP_drawTextBG(APLAN, "INCREDIBLE REFLEXES!!", TILE_ATTR(PAL1, 0, 0, 0), 14, 15);
			VDP_End();
		}

		while(!exit)
		{
			buttons = JOY_readJoypad(JOY_1);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			if(pressedButtons & BUTTON_START)
				exit = 1;
			VDP_waitVSync();
		}
	}
}

void HScrollTest()
{
	u16 size, sonic_floor, sonic_water, waterfall, loadvram = 1;
	u16 exit = 0, frame = 1, vertical = 0, type = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	int x = 0, y = 0, speed = 1, acc = -1, pause = 0, kikipos = 32;

	type = DrawFloatMenuRes(RES_320);
	while(!exit)
	{
		if(loadvram)
		{	
			VDP_Start();
			
			if(!vertical)
			{	
				if(type == RES_256)
					VDP_setScreenWidth256();
				else
					VDP_setScreenWidth320();
				
				VDP_setVerticalScroll(PLAN_A, 0);
				VDP_setHorizontalScroll(PLAN_A, x);
				VDP_setHorizontalScroll(PLAN_B, x / 2);				

				VDP_setPalette(PAL0, sonicback_pal);
				VDP_setPalette(PAL1, sonicwater_pal);
				
				size = sizeof(sonicback_tiles) / 32;
				VDP_loadTileData(sonicback_tiles, TILE_USERINDEX, size, USE_DMA);

				sonic_water = TILE_USERINDEX + size;
				size = sizeof(sonicwater_tiles) / 32;
				VDP_loadTileData(sonicwater_tiles, sonic_water, size, USE_DMA);

				sonic_floor = sonic_water + size;
				size = sizeof(sonicfloor_tiles) / 32;
				VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, USE_DMA);

				waterfall = sonic_floor + size;
				size = sizeof(waterfall_tiles) / 32;
				VDP_loadTileData(waterfall_tiles, waterfall, size, USE_DMA);

				VDP_setMyTileMapRect(BPLAN, sonicback_map, TILE_USERINDEX, 0, 0, 256 / 8, 152 / 8);
				VDP_setMyTileMapRect(BPLAN, sonicback_map, TILE_USERINDEX, 256 / 8, 0, 256 / 8, 152 / 8);

				VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 0, 152 / 8, 256 / 8, 48 / 8);
				VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 256 / 8, 152 / 8, 256 / 8, 48 / 8);

				VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 0, 96 / 8, 256 / 8, 128 / 8);
				VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 256 / 8, 96 / 8, 256 / 8, 128 / 8);

				VDP_setSprite(0, 72, 120, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 1);
				VDP_setSprite(1, 328, 120, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 0);
			}
			else
			{
				VDP_resetSprites();
				VDP_updateSprites();
				VDP_setHorizontalScroll(PLAN_B, 0);
				VDP_setHorizontalScroll(PLAN_A, type != RES_256 ? kikipos : 0);
				
				if(type == RES_256)
					VDP_setScreenWidth256();
				else
				{
					VDP_setScreenWidth320();
					VDP_setHorizontalScroll(PLAN_A, kikipos);
				}

				VDP_setPalette(PAL0, kiki_pal);
				size = sizeof(kiki_tiles) / 32;
				VDP_loadTileData(kiki_tiles, TILE_USERINDEX, size, USE_DMA);

				VDP_setMyTileMapRect(APLAN, kiki_map, TILE_USERINDEX, 0, 0, 256 / 8, 512 / 8);
			}

			VDP_End();
			loadvram = 0;
		}

		if(!vertical)
		{
			VDP_Start();
			switch (frame)
			{
			case 30:
				VDP_setPalette(PAL1, sonicwater3_pal);
				break;
			case 60:
				VDP_setPalette(PAL1, sonicwater2_pal);
				break;
			case 90:
				VDP_setPalette(PAL1, sonicwater_pal);
				break;
			}
			VDP_End();
		}

		frame++;
		if(frame > 90)
			frame = 1;

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_HSCROLL))
			loadvram = 1;
		
		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_UP)
			speed++;

		if(pressedButtons & BUTTON_DOWN)
			speed--;

		if(speed > 20)
			speed = 20;

		if(speed < 0)
			speed = 0;

		if(pressedButtons & BUTTON_A)
			pause = !pause;

		if(pressedButtons & BUTTON_LEFT)
			acc *= -1;
		
		if(pressedButtons & BUTTON_B)
		{
			vertical = !vertical;
			loadvram = 1;
			FadeAndCleanUp();
		}

		if(!vertical)
		{
			if(!pause)
				x += acc * speed;
			
			if(x >= 512)
				x = x % 512;

			if(x <= -512)
				x = x % -512;

			VDP_Start();
			VDP_setSpritePosition(0, x / 2 + 79, 120);
			VDP_setSpritePosition(1, x / 2 + 335, 120);
			VDP_updateSprites();
			VDP_setHorizontalScroll(PLAN_A, x);
			VDP_setHorizontalScroll(PLAN_B, x / 2);
			VDP_End();
		}
		else
		{
			if(!pause)
				y += acc * speed;
			
			if(y >= 512)
				y = y % 512;

			if(x <= -512)
				y = y % -512;

			VDP_Start();
			VDP_setVerticalScroll(PLAN_A, y);
			VDP_End();
		}
		

		if(vertical && type == RES_320 && pressedButtons & BUTTON_RIGHT)
		{
			kikipos += 32;
			if(kikipos > 64)
				kikipos = 0;
			VDP_Start();
			VDP_setHorizontalScroll(PLAN_A, kikipos);
			VDP_End();
		}
		
		if(pressedButtons & BUTTON_C)
		{
			type = DrawFloatMenuRes(type);
			oldButtons |= BUTTON_A;
			loadvram = 1;
		}
		
		VDP_waitVSync();
	}
}

void VScrollTest()
{
	u16 size, loadvram = 1;
	u16 exit = 0, type = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	int pos = 0, speed = 1, acc = -1, pause = 0, direction = 0;

	type = DrawFloatMenuRes(RES_320);
	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			if(type == RES_256)
				VDP_setScreenWidth256();
			else
				VDP_setScreenWidth320();
				
			VDP_setPalette(PAL0, bw_pal);

			size = sizeof(circles_grid_tiles) / 32;
			VDP_loadTileData(circles_grid_tiles, TILE_USERINDEX, size, USE_DMA);

			VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 512 / 8, 512 / 8);
			VDP_End();
			loadvram = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_VSCROLL))
			loadvram = 1;

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_UP)
			speed++;

		if(pressedButtons & BUTTON_DOWN)
			speed--;

		if(speed > 5)
			speed = 5;

		if(speed < 0)
			speed = 0;

		if(pressedButtons & BUTTON_A)
			pause = !pause;

		if(pressedButtons & BUTTON_LEFT)
			acc *= -1;

		if(pressedButtons & BUTTON_B)
			direction = !direction;
			
		if(pressedButtons & BUTTON_C)
		{
			type = DrawFloatMenuRes(type);
			oldButtons |= BUTTON_A;
			loadvram = 1;
		}

		if(!pause)
			pos += acc * speed;

		if(pos >= 512)
			pos = pos % 512;

		if(pos <= -512)
			pos = pos % -512;

		VDP_Start();
		if(direction)
			VDP_setHorizontalScroll(PLAN_A, pos);
		else
			VDP_setVerticalScroll(PLAN_A, pos);
		VDP_End();
		VDP_waitVSync();
	}
}

enum SounTestValue{
	stFM,
	stFMOct,
	stFMPan,
	stPSG,
#ifdef SEGACD
	stCD,
	stPCM
#endif
};

void SoundTest()
{
	int sel = 2, loadvram = 1, psgoff = 0, fmpan = 1, fmoff = 0;
	int type = stFM;
	u16 exit = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 redraw = 0, selmax = 5;
	int octave = 24;
#ifdef SEGACD
	int pcm = 0, loaded = 0;
#endif

	yminit();
	PSG_init();
#ifdef SEGACD
	SendSCDCommand(Op_SetSamplesTest);
#endif
	while(!exit)
	{
		if(loadvram)
		{	
			DrawMainBG();
			
			loadvram = 0;
			redraw = 1;
		}
		
		if(redraw)
		{
			int x = 13;
#ifdef SEGACD
			int y = 8;
#else
			int y = 9;
#endif
			
			char buffer[2];
			
			VDP_Start();
			VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(PAL1, 0, 0, 0), x+1, 6);
			
			VDP_drawTextBG(APLAN, "YM2612 FM", TILE_ATTR(PAL1, 0, 0, 0), x+2, y++);
			VDP_drawTextBG(APLAN, "1", TILE_ATTR((type == stFM && sel == 0) ? PAL3 : PAL0, 0, 0, 0), x+1, y);
			VDP_drawTextBG(APLAN, "2", TILE_ATTR((type == stFM && sel == 1) ? PAL3 : PAL0, 0, 0, 0), x+3, y);
			VDP_drawTextBG(APLAN, "3", TILE_ATTR((type == stFM && sel == 2) ? PAL3 : PAL0, 0, 0, 0), x+5, y);
			VDP_drawTextBG(APLAN, "4", TILE_ATTR((type == stFM && sel == 3) ? PAL3 : PAL0, 0, 0, 0), x+7, y);
			VDP_drawTextBG(APLAN, "5", TILE_ATTR((type == stFM && sel == 4) ? PAL3 : PAL0, 0, 0, 0), x+9, y);
			VDP_drawTextBG(APLAN, "6", TILE_ATTR((type == stFM && sel == 5) ? PAL3 : PAL0, 0, 0, 0), x+11, y);
			y++;
			VDP_drawTextBG(APLAN, "FM Octave:", TILE_ATTR((type == stFMOct && sel == 0) ? PAL3 : PAL0, 0, 0, 0), x, y);
			intToStr(octave, buffer, 2);
			VDP_drawTextBG(APLAN, buffer, TILE_ATTR((type == stFMOct && sel == 0) ? PAL3 : PAL0, 0, 0, 0), x+11, y);
			y++;
			VDP_drawTextBG(APLAN, "Left", TILE_ATTR((type == stFMPan && sel == 0) ? PAL1 : fmpan == 0 ? PAL3 : PAL0, 0, 0, 0), x-2, y);
			VDP_drawTextBG(APLAN, "Center", TILE_ATTR((type == stFMPan && sel == 1) ? PAL1 : fmpan == 1 ? PAL3 : PAL0, 0, 0, 0), x+3, y);
			VDP_drawTextBG(APLAN, "Right", TILE_ATTR((type == stFMPan && sel == 2) ? PAL1 : fmpan == 2 ? PAL3 : PAL0, 0, 0, 0), x+10, y);
			
			y+=2;
			VDP_drawTextBG(APLAN, "PSG CHANNEL", TILE_ATTR(PAL1, 0, 0, 0), x+1, y++);
			VDP_drawTextBG(APLAN, "0", TILE_ATTR((type == stPSG && sel == 0) ? PAL3 : PAL0, 0, 0, 0), x+3, y);
			VDP_drawTextBG(APLAN, "1", TILE_ATTR((type == stPSG && sel == 1) ? PAL3 : PAL0, 0, 0, 0), x+5, y);
			VDP_drawTextBG(APLAN, "2", TILE_ATTR((type == stPSG && sel == 2) ? PAL3 : PAL0, 0, 0, 0), x+7, y);
			VDP_drawTextBG(APLAN, "3", TILE_ATTR((type == stPSG && sel == 3) ? PAL3 : PAL0, 0, 0, 0), x+9, y);
			
#ifdef SEGACD
			y+=2;
			VDP_drawTextBG(APLAN, "CD-DA", TILE_ATTR((type == stCD && sel == 0) ? PAL3 : PAL1, 0, 0, 0), x+4, y);
			y+=2;
			VDP_drawTextBG(APLAN, "PCM", TILE_ATTR(PAL1, 0, 0, 0), x+5, y++);
			VDP_drawTextBG(APLAN, "Left", TILE_ATTR((type == stPCM && sel == 0) ? PAL3 : PAL0, 0, 0, 0), x-2, y);
			VDP_drawTextBG(APLAN, "Center", TILE_ATTR((type == stPCM && sel == 1) ? PAL3 : PAL0, 0, 0, 0), x+3, y);
			VDP_drawTextBG(APLAN, "Right", TILE_ATTR((type == stPCM && sel == 2) ? PAL3 : PAL0, 0, 0, 0), x+10, y);
#endif
			VDP_End();
			
			DrawHelpText();
			redraw = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_SOUND))
			loadvram = 1;

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_UP)
		{
			type--;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_DOWN)
		{
			type++;
			redraw = 1;
		}

#ifndef SEGACD
		if(type > stPSG)
			type = stFM;

		if(type < stFM)
			type = stPSG;
#else
		if(type > stPCM)
			type = stFM;

		if(type < stFM)
			type = stPCM;
#endif
		
		if(redraw)
			switch(type)
			{
				case stFM:
					selmax = 5;
					break;
				case stFMOct:
					selmax = 0;
					break;
				case stFMPan:
					selmax = 2;
					break;
				case stPSG:
					selmax = 3;
					break;
#ifdef SEGACD
				case stCD:
					selmax = 0;
					break;
				case stPCM:
					selmax = 2;
					break;
#endif
				default:
					break;
			}
		
		if(pressedButtons & BUTTON_LEFT)
		{
			sel--;
			redraw = 1;
			
			if(type == stFMOct)
			{
				octave -= 8;
				if(octave < 0)
					octave = 56;
				redraw = 1;
			}
		}

		if(pressedButtons & BUTTON_RIGHT)
		{
			sel++;
			redraw = 1;
			
			if(type == stFMOct)
			{
				octave += 8;
				if(octave > 56)
					octave = 0;
				redraw = 1;
			}
		}

		if(sel > selmax)
			sel = 0;

		if(sel < 0)
			sel = selmax;

		if(pressedButtons & BUTTON_A)
		{
			switch(type)
			{
				case stFM: //YM2612
				{
					ymPlay(3, sel, octave, fmpan == 0 ? STEREO_LEFT: fmpan == 1 ? STEREO_BOTH : STEREO_RIGHT);
					fmoff = 120;
				}
				break;
				case stFMOct: // Octave
				{
					octave += 8;
					if(octave > 56)
						octave = 0;
					redraw = 1;
				}
				break;
				case stFMPan: // FM Pan
				{
					fmpan = sel;
					redraw = 1;
				}
				break;
				case stPSG: // PSG
				{
					if(sel == 0)
					{
						PSG_setFrequency(0, 200);
						PSG_setEnvelope(0, PSG_ENVELOPE_MAX);
					}
					if(sel == 1)
					{
						PSG_setFrequency(1, 2000);
						PSG_setEnvelope(1, PSG_ENVELOPE_MAX);
					}
					if(sel == 2)
					{
						PSG_setFrequency(2, 4095);
						PSG_setEnvelope(2, PSG_ENVELOPE_MAX);
					}
					if(sel == 3)
					{
						PSG_setNoise(PSG_NOISE_TYPE_WHITE, PSG_NOISE_FREQ_TONE3);
						PSG_setFrequency(3, 500);
						PSG_setEnvelope(3, PSG_ENVELOPE_MAX);
					}
					
					psgoff = 60;
				}
				break;
#ifdef SEGACD
				case stCD:  // CD-DA
				{
					SendSCDCommand(Op_PlayCD240);
				}
				break;
				case stPCM: // PCM
				{
					if(!loaded && random() % 40 == 7)
					{
						SendSCDCommand(Op_SetSamplesTest2);
						loaded = 1;
					}
						
					if(pcm)
						SendSCDCommand(Op_StopPCM);

					if(sel == 0)
					{
						SendSCDCommand(Op_SetPCMLeft);
						SendSCDCommand(Op_PlayPCM);
					}
					if(sel == 1)
					{
						SendSCDCommand(Op_SetPCMCenter);
						SendSCDCommand(Op_PlayPCM);
					}
					if(sel == 2)
					{
						SendSCDCommand(Op_SetPCMRight);
						SendSCDCommand(Op_PlayPCM);
					}
					
					pcm = 110;
				}
				break;
#endif
				default:
					break;
			}
		}

#ifdef SEGACD		
		if(pcm)
		{
			pcm --;
			if(pcm == 0)
				SendSCDCommand(Op_StopPCM);
		}
#endif
		
		if(psgoff)
		{
			psgoff --;
			if(psgoff == 0)
				StopPSG();
		}
		
		if(fmoff)
		{
			fmoff --;
			if(fmoff == 0)
				ym2612_keyoffAll();
		}


		VDP_waitVSync();
	}
	
	ym2612_keyoffAll();
	StopPSG();
#ifdef SEGACD
	SendSCDCommand(Op_StopPCM);
	SendSCDCommand(Op_StopCD);
	SendSCDCommand(Op_SetPCMCenter);
#endif
}

void LEDZoneTest()
{
	u16 size = 0, sprite0 = 0, sprite1 = 0, sprite2 = 0, sprite3 = 0, sprite4 = 0, tmp = 0;
	u16 x = 160, y = 112, exit = 0, sprite = 1, change = 0, draw = 1;
	u16 buttons, pressedButtons, oldButtons = 0xffff, loadvram = 1, type = 0;
		
	type = DrawFloatMenuRes(RES_320);	
	if(type == RES_256)
		x = 128;
	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			
			if(type == RES_256)
				VDP_setScreenWidth256();
			else
				VDP_setScreenWidth320();
				
			VDP_setPalette(PAL1, bw_pal);

			size = sizeof(size0led_t) / 32;

			sprite0 = TILE_USERINDEX;
			VDP_loadTileData(size0led_t, sprite0, size, USE_DMA);
			sprite1 = sprite0 + size;
			VDP_loadTileData(size1led_t, sprite1, size, USE_DMA);
			sprite2 = sprite1 + size;
			VDP_loadTileData(size2led_t, sprite2, size, USE_DMA);
			sprite3 = sprite2 + size;
			VDP_loadTileData(size3led_t, sprite3, size, USE_DMA);
			sprite4 = sprite3 + size;
			VDP_loadTileData(size4led_t, sprite4, size, USE_DMA);

			VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite1, 0);

			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
			VDP_End();
			loadvram = 0;
		}

		VDP_Start();
		VDP_setSpritePosition(0, x, y);
		VDP_End();

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;
		
		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_LED))
			loadvram = 1;

		if(draw)
		{
			if(buttons & BUTTON_UP)
			{
				if(y > 0)
					y--;
			}

			if(buttons & BUTTON_DOWN)
			{
				if(y + size < (pal_240 ? 240 : 224))
					y++;
			}

			if(buttons & BUTTON_LEFT)
			{
				if(x > 0)
					x--;
			}

			if(buttons & BUTTON_RIGHT)
			{
				if(x + size < (type == RES_256 ? 256 : 320))
					x++;
			}
			
			if(type == RES_256 && x > (256 - size))
				x = 256 - size;
			
			if(!pal_240 && y > (224 - size))
				y = 224 - size;

		}
		
		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_A)
		{
			if(sprite == 4)
				sprite = 0;
			else
				sprite++;
			change = 1;
		}


		if(pressedButtons & BUTTON_B)
		{
			draw = !draw;
			change = 1;
			if(draw)
				x = tmp;
			else
			{
				tmp = x;
				x = 340;
			}
		}
		
		if(pressedButtons & BUTTON_C)
		{
			type = DrawFloatMenuRes(type);
			oldButtons |= BUTTON_A;
			loadvram = 1;
		}

		if(change)
		{
			VDP_Start();
			
			switch (sprite)
			{
			case 0:
				VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite0, 0);
				size = 1;
				break;
			case 1:
				VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite1, 0);
				size = 2;
				break;
			case 2:
				VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite2, 0);
				size = 4;
				break;
			case 3:
				VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite3, 0);
				size = 6;
				break;
			case 4:
				VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite4, 0);
				size = 8;
				break;
			}
			VDP_End();
		}

		VDP_Start();
		VDP_updateSprites();
		VDP_End();
		
		VDP_waitVSync();
	}
}

void PassiveLagTest()
{
	u16 frames = 0, seconds = 0, minutes = 0, hours = 0, framecnt = 1, bgcol = PAL2;
	u16 exit = 0, color = 1, loadvram = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 numbers[11], size, lsd, msd, pause = 0, circle = 0, cposx = 32, cposy = 17, solid, bars;

	while(!exit)
	{
		if(loadvram)
		{
			int i = 0;
			
			VDP_Start();
			
			VDP_setPalette(PAL1, btw_pal);
			VDP_setPalette(PAL2, bluew_pal);
			VDP_setPalette(PAL3, redw_pal);

			numbers[0] = TILE_USERINDEX;
			size = sizeof(tiles_0) / 32;

			VDP_loadTileData(tiles_0, numbers[0], size, USE_DMA);
			numbers[1] = numbers[0] + size;
			VDP_loadTileData(tiles_1, numbers[1], size, USE_DMA);
			numbers[2] = numbers[1] + size;
			VDP_loadTileData(tiles_2, numbers[2], size, USE_DMA);
			numbers[3] = numbers[2] + size;
			VDP_loadTileData(tiles_3, numbers[3], size, USE_DMA);
			numbers[4] = numbers[3] + size;
			VDP_loadTileData(tiles_4, numbers[4], size, USE_DMA);
			numbers[5] = numbers[4] + size;
			VDP_loadTileData(tiles_5, numbers[5], size, USE_DMA);
			numbers[6] = numbers[5] + size;
			VDP_loadTileData(tiles_6, numbers[6], size, USE_DMA);
			numbers[7] = numbers[6] + size;
			VDP_loadTileData(tiles_7, numbers[7], size, USE_DMA);
			numbers[8] = numbers[7] + size;
			VDP_loadTileData(tiles_8, numbers[8], size, USE_DMA);
			numbers[9] = numbers[8] + size;
			VDP_loadTileData(tiles_9, numbers[9], size, USE_DMA);
			numbers[10] = numbers[9] + size;
			VDP_loadTileData(tiles_c, numbers[10], size, USE_DMA);

			circle = numbers[10] + size;
			size = sizeof(circle56_tiles) / 32;
			VDP_loadTileData(circle56_tiles, circle, size, USE_DMA);

			solid = circle + size;
			size = sizeof(solidw_tiles) / 32;
			VDP_loadTileData(solidw_tiles, solid, size, USE_DMA);

			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);

			VDP_fillTileMapRect(BPLAN, TILE_ATTR(PAL2, 0, 0, 0) + solid, 0, 0, 320 / 8, (pal_240 ? 240 : 224) / 8);
			
			// Load side bars
			bars = solid + size;
			size = sizeof(vbars_tiles) / 32;
			VDP_loadTileData(vbars_tiles, bars, size, USE_DMA);
			
			//Draw Side bars
			for(i = 0; i < 28; i++)
			{
				VDP_fillTileMapInc(BPLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 0) + bars,    i*64, 1);
				VDP_fillTileMapInc(BPLAN, TILE_ATTR_FULL(PAL0, 0, 0, 1, 0) + bars, 39+i*64, 1);
			}

			VDP_drawTextBG(APLAN, "hours", TILE_ATTR(PAL1, 0, 0, 0), 4, 1);
			VDP_drawTextBG(APLAN, "minutes", TILE_ATTR(PAL1, 0, 0, 0), 13, 1);
			VDP_drawTextBG(APLAN, "seconds", TILE_ATTR(PAL1, 0, 0, 0), 22, 1);
			VDP_drawTextBG(APLAN, "frames", TILE_ATTR(PAL1, 0, 0, 0), 31, 1);

			// Draw Frame divisor numbers
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[1], 4, 10, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[2], 14, 10, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[3], 24, 10, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[4], 34, 10, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[5], 4, 18, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[6], 14, 18, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[7], 24, 18, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + numbers[8], 34, 18, 3, 5);

			// Draw counter separators
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[10], 10, 2, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[10], 19, 2, 3, 5);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[10], 28, 2, 3, 5);

			//Draw initial circles
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 2, 9, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 12, 9, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 22, 9, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 32, 9, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 2, 17, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 12, 17, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 22, 17, 7, 7);
			VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, 32, 17, 7, 7);

			VDP_End();
			loadvram = 0;
		}

		if(framecnt > 8)
			framecnt = 1;

		if(!pause && color)
		{
			VDP_Start();
			if(bgcol == PAL2)
			{
				bgcol = PAL3;
				VDP_setPaletteColor(0xE, 0x0);
			}
			else
			{
				bgcol = PAL2;
				VDP_setPaletteColor(0xE, 0x0fff);
			}
			VDP_End();
		}

		if(Detect_VDP_PAL())
		{
			if(frames > 49)
			{
				frames = 0;
				seconds++;
			}
		}
		else
		{
			if(frames > 59)
			{
				frames = 0;
				seconds++;
			}
		}

		if(seconds > 59)
		{
			seconds = 0;
			minutes++;
		}

		if(minutes > 59)
		{
			minutes = 0;
			hours++;
		}

		if(hours > 99)
			hours = 0;

		VDP_Start();
		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[msd], 4, 2, 3, 5);
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[lsd], 7, 2, 3, 5);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[msd], 13, 2, 3, 5);
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[lsd], 16, 2, 3, 5);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[msd], 22, 2, 3, 5);
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + numbers[lsd], 25, 2, 3, 5);

		// Draw frames
		lsd = frames % 10;
		msd = frames / 10;
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(bgcol, 0, 0, 0) + numbers[msd], 31, 2, 3, 5);
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(bgcol, 0, 0, 0) + numbers[lsd], 34, 2, 3, 5);

		// Draw Frame divisor circles

		VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + circle, cposx, cposy, 7, 7);
		VDP_End();
		
		if(framecnt > 4)
		{
			cposx = framecnt - 4;
			cposy = 17;
		}
		else
		{
			cposx = framecnt;
			cposy = 9;
		}
		cposx = (cposx - 1) * 10 + 2;

		VDP_Start();
		VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL2, 0, 0, 0) + circle, cposx, cposy, 7, 7);
		VDP_End();

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_LAG))
			loadvram = 1;

		if(pressedButtons & BUTTON_A)
			pause = !pause;

		if(pressedButtons & BUTTON_B && pause)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 1;
		}

		if(pressedButtons & BUTTON_C)
		{
			VDP_Start();
			color = !color;
			if(!color)
			{
				VDP_setPalette(PAL1, btw_pal);
				VDP_setPalette(PAL2, btw_pal);
				VDP_setPalette(PAL3, bw_pal);
			}
			else
			{
				VDP_setPalette(PAL1, btw_pal);
				VDP_setPalette(PAL2, bluew_pal);
				VDP_setPalette(PAL3, redw_pal);
			}
			VDP_End();

			if(color)
				bgcol = PAL3;
			else
				bgcol = PAL2;
		}

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(!pause)
		{
			frames++;
			framecnt++;
		}
		VDP_waitVSync();
	}
}

u16 ConvertToFrames(timecode * time)
{
	u16 frames = 0;

	if(!time)
		return frames;

	frames = time->frames;
	if(Detect_VDP_PAL())
		frames += time->seconds * 50;
	else
		frames += time->seconds * 60;
	frames += time->minutes * 3600;
	frames += time->hours * 216000;
	return frames;
}

void ConvertFromFrames(timecode * value, u16 Frames)
{
	if(!value)
		return;
	value->hours = Frames / 216000;
	Frames = Frames % 216000;
	value->minutes = Frames / 3600;
	Frames = Frames % 3600;
	if(Detect_VDP_PAL())
	{
		value->seconds = Frames / 50;
		value->frames = Frames % 50;
	}
	else
	{
		value->seconds = Frames / 60;
		value->frames = Frames % 60;
	}
}

void Alternate240p480i()
{
	u16 frames = 0, seconds = 0, minutes = 0, hours = 0;
	u16 exit = 0, loadvram = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 oldinterlaced, res = 0, current = 0, status = 0, changed = 1;
	u16 redraw = 1;
	timecode times[20];
	char str[10];

	oldinterlaced = VDP_Detect_Interlace();

	VDP_Start();
	VDP_setScanMode(INTERLACED_NONE);
	VDP_End();

	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);

			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_green);
			VDP_setPalette(PAL2, palette_red);

			VDP_drawText("Elapsed Timer: 00:00:00:00", 7, 3);
			VDP_End();
			
			loadvram = 0;
			changed = 1;
		}

		VDP_Start();
		intToStr(hours, str, 2);
		VDP_drawText(str, 22, 3);
		intToStr(minutes, str, 2);
		VDP_drawText(str, 25, 3);
		intToStr(seconds, str, 2);
		VDP_drawText(str, 28, 3);
		intToStr(frames, str, 2);
		VDP_drawText(str, 31, 3);
		VDP_End();

		if(changed)
		{
			u16 i = 0;

			if(redraw)
			{
				i = 0;
				redraw = 0;
			}
			else
				i = current - 1;

			VDP_Start();
			VDP_drawTextBG(APLAN, "Current Resolution:", TILE_ATTR(PAL1, 0, 0, 0), 7, 2);
			if(res == 0)
				VDP_drawTextBG(APLAN, "240p", TILE_ATTR(PAL1, 0, 0, 0), 27, 2);
			else
				VDP_drawTextBG(APLAN, "480i", TILE_ATTR(PAL1, 0, 0, 0), 27, 2);
			VDP_End();

			if(current)
			{
				VDP_Start();
				for(i = 0; i < current; i++)
				{
					u8 len = 0;

					if(times[i].type == 0)
					{
						VDP_drawTextBG(APLAN, "Switched     :", TILE_ATTR(PAL1, 0, 0, 0), 1, i + 5);
						VDP_drawTextBG(APLAN, times[i].res == 0 ? "240p" : "480i", TILE_ATTR(PAL1, 0, 0, 0), 10, i + 5);
						len = 14;
					}
					else
					{
						VDP_drawTextBG(APLAN, "Viewed:", TILE_ATTR(PAL1, 0, 0, 0), 1, i + 5);
						len = 8;
					}

					VDP_drawText("00:00:00:00", 3 + len, i + 5);
					intToStr(times[i].hours, str, 2);
					VDP_drawText(str, 3 + len, i + 5);
					intToStr(times[i].minutes, str, 2);
					VDP_drawText(str, 6 + len, i + 5);
					intToStr(times[i].seconds, str, 2);
					VDP_drawText(str, 9 + len, i + 5);
					intToStr(times[i].frames, str, 2);
					VDP_drawText(str, 12 + len, i + 5);

					if(times[i].type != 0 && i >= 1 && i <= 19)
					{
						u16 framesA = 0, framesB = 0, res = 0;
						timecode len;

						framesB = ConvertToFrames(&times[i]);
						framesA = ConvertToFrames(&times[i - 1]);
						res = framesB - framesA;
						ConvertFromFrames(&len, res);

						VDP_drawTextBG(APLAN, "00:00:00:00", TILE_ATTR(PAL2, 0, 0, 0), 24, i + 5);
						intToStr(len.hours, str, 2);
						VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL2, 0, 0, 0), 24, i + 5);
						intToStr(len.minutes, str, 2);
						VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL2, 0, 0, 0), 27, i + 5);
						intToStr(len.seconds, str, 2);
						VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL2, 0, 0, 0), 30, i + 5);
						intToStr(len.frames, str, 2);
						VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL2, 0, 0, 0), 33, i + 5);
					}
				}
				VDP_End();
			}
			changed = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_ALTERNATE))
			loadvram = 1;

		if(pressedButtons & BUTTON_A)
		{
			if(current <= 19)
				current++;
			else
			{
				current = 1;
				loadvram = 1;
			}

			times[current - 1].frames = frames;
			times[current - 1].minutes = minutes;
			times[current - 1].seconds = seconds;
			times[current - 1].hours = hours;

			status++;
			if(status == 1)
			{
				times[current - 1].type = 0;
				res = !res;
				times[current - 1].res = res;
				VDP_Start();
				if(!res)
					VDP_setScanMode(INTERLACED_NONE);
				else
					VDP_setScanMode(INTERLACED_MODE1);
				VDP_End();
			}
			if(status == 2)
			{
				times[current - 1].type = 1;
				times[current - 1].res = res;
				status = 0;
			}
			changed = 1;
		}

		if(pressedButtons & BUTTON_START)
			exit = 1;

		frames++;

		if(Detect_VDP_PAL())
		{
			if(frames > 49)
			{
				frames = 0;
				seconds++;
			}
		}
		else
		{
			if(frames > 59)
			{
				frames = 0;
				seconds++;
			}
		}

		if(seconds > 59)
		{
			seconds = 0;
			minutes++;
		}

		if(minutes > 59)
		{
			minutes = 0;
			hours++;
		}

		if(hours > 99)
			hours = 0;

		VDP_waitVSync();
	}

	VDP_Start();
	if(oldinterlaced)
		VDP_setScanMode(INTERLACED_MODE1);
	else
		VDP_setScanMode(INTERLACED_NONE);
	VDP_End();

	return;
}

void AudioSyncTest()
{
	u16 loadvram = 1, exit = 0, cycle = 0;
	u16 size, tiles, i, sprite = 0, x = 160, y = 160;
	u16 buttons, pressedButtons, oldButtons = 0xffff;
	u16 black_pal[16];
	s16 acc = 1, status = -1;

	PSG_init();
	PSG_setFrequency(0, 1000);

	for(i = 0; i < 16; i++)
		black_pal[i] = 0x0;

	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			
			VDP_setPalette(PAL0, black_pal);
			VDP_setPaletteColor(17, 0x0ddd);
			VDP_setPaletteColor(34, 0x0ddd);

			tiles = TILE_USERINDEX;
			size = sizeof(color_tiles) / 32;
			VDP_loadTileData(color_tiles, tiles, size, USE_DMA);

			sprite = tiles + size;
			size = sizeof(size1led_t) / 32;
			VDP_loadTileData(size1led_t, sprite, size, USE_DMA);

			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 5) + tiles, 0, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 4) + tiles, 4, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 3) + tiles, 8, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 2) + tiles, 12, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1) + tiles, 16, 6, 4, 2);

			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1) + tiles, 20, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 2) + tiles, 24, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 3) + tiles, 28, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 4) + tiles, 32, 6, 4, 2);
			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 5) + tiles, 36, 6, 4, 2);

			VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite, 0);
			VDP_setSpritePosition(0, x, y);
			VDP_updateSprites();	

			VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL2, 1, 0, 0, 1) + tiles, 0, 20, 40, 1);
			
			VDP_End();

			loadvram = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;
		
		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_AUDIOSYNC))
			loadvram = 1;

		if(pressedButtons & BUTTON_A)
		{
			cycle = !cycle;
			if(!cycle)
				status = 121;
			else
				y = 160;
		}

		if(cycle == 1 && status == -1)
		{
			status = 0;
			acc = -1;
		}

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(status > -1)
		{
			status++;
			if(status <= 120)
			{
				y += acc;
				VDP_Start();
				VDP_setSpritePosition(0, x, y);
				VDP_updateSprites();
				VDP_End();
			}
		}

		if(status >= 20 && status <= 120)
		{
			switch (status)
			{
			case 20:
				black_pal[7] = 0x0ddd;
				break;
			case 40:
				black_pal[6] = 0x0ddd;
				break;
			case 60:
				acc = 1;
				black_pal[5] = 0x0ddd;
				break;
			case 80:
				black_pal[4] = 0x0ddd;
				break;
			case 100:
				black_pal[3] = 0x0ddd;
				break;
			case 120:
				black_pal[2] = 0x0ddd;
				break;
			}

			VDP_Start();
			VDP_setPalette(PAL0, black_pal);
			VDP_End();
		}
		
		if(status == 120)
		{
			PSG_setEnvelope(0, PSG_ENVELOPE_MAX);
			black_pal[0] = 0xFFF;
			VDP_Start();
			VDP_setPalette(PAL0, black_pal);
			VDP_End();
		}

		if(status == 122)
		{
			for(i = 0; i < 8; i++)
				black_pal[i] = 0x0;

			PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
			VDP_Start();
			VDP_setPalette(PAL0, black_pal);
			VDP_End();
			status = -1;
		}
		
		VDP_waitVSync();
	}
	VDP_Start();
	VDP_resetSprites();
	VDP_updateSprites();
	VDP_End();
	PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
}


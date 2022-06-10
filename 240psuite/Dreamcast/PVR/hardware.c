/* 
 * r
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"
#include "vmufs.h"

#include "controller.h"
#include "help.h"
#include "menu.h"

#include "hardware.h"

char	flashrom_region_cache[FLASHROM_CACHE_SIZE] = { 0 };
int		flashrom_is_cached = 0;

#define TRUNCATE_LEN	22

void ReduceName(char *target, char *source, int truncate)
{
	int len;
	
	target[0] = '\0';
	len = strlen(source);
	if(!len || len == 1)
		return;

	len--;		
	while(len > 1 && source[len] == ' ')
		len--;
	
	if(truncate && len > TRUNCATE_LEN)
		len = TRUNCATE_LEN+1;
	strncpy(target, source, sizeof(char)*(len+1));
	if(truncate)
		target[TRUNCATE_LEN+1] = 127;
	target[len+1] = '\0';
}

void DiplayController(int num, float x, float y)
{
	int				isPressed = 0;
	int				hasAnalogX = 0, hasAnalogY = 0, hasAnalog2X = 0, hasAnalog2Y = 0;
	cont_state_t	*st;
	maple_device_t	*dev = NULL;
	char			msg[256], name[256];
	float			orig_x = x, orig_y = y;
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER|MAPLE_FUNC_LIGHTGUN);
	if(!dev)
		return;
	
	ReduceName(msg, dev->info.product_name, 1);
	sprintf(name, "#G%c%c:#G %s", 'A'+(dev->port), '0'+(dev->unit), msg);
	DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, name);
	y += fh;
	
	sprintf(msg, "Power[%u-%u]mW", 
			dev->info.standby_power, dev->info.max_power);
	DrawStringS(x, y, 0.0f, 0.7f, 0.6f, msg);
	y += fh;
	
	// Tells us the device type in 
	// isFishingRod/isMaracas/isStockController/isArcade
	DetectControllerType(dev);
	
	st = (cont_state_t*)maple_dev_status(dev);
	if(!st)
		return;
		
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG_X)
		hasAnalogX = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG_Y)
		hasAnalogY = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG2_X)
		hasAnalog2X = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG2_Y)
		hasAnalog2Y = 1;
	
	x += 3*fw;

	if(!isMaracas)
	{
		int hadtriggers = 0;
		
		// Draw Triggers
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_LTRIG)
		{
			sprintf(msg, "%03d", st->ltrig);
			DrawStringS(x+1*fw, y, 1.0f, 1.0f, 1.0f, msg);
			hadtriggers = 1;
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_RTRIG)
		{
			sprintf(msg, "%03d", st->rtrig);
			DrawStringS(x+7*fw, y, 1.0f, 1.0f, 1.0f, msg);
			hadtriggers = 1;
		}
		if(hadtriggers)
			y += fh;

		// Regular DC controller 	0xfe060f00
		// Fishing Rod 				0xfe063f00
		if(!(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C))
		{
			int advance = 0;
			
			// Draw Up and Y
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_UP)
			{
				isPressed = st->buttons & CONT_DPAD_UP;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "U");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Y)
			{
				isPressed = st->buttons & CONT_Y;
				DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Y");
				advance++;
			}
			if(advance)
			{
				y += fh;
				advance = 0;
			}
		
			// Draw Left, Right, Start, X and B
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_LEFT)
			{
				isPressed = st->buttons & CONT_DPAD_LEFT;
				DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "L");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_RIGHT)
			{
				isPressed = st->buttons & CONT_DPAD_RIGHT;
				DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "R");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START)
			{
				isPressed = st->buttons & CONT_START;
				DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "S");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_X)
			{
				isPressed = st->buttons & CONT_X;
				DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "X");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B)
			{
				isPressed = st->buttons & CONT_B;
				DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "B");
				advance++;
			}
			if(advance)
				y += fh;
		
			// Draw Down and A
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_DOWN)
			{
				isPressed = st->buttons & CONT_DPAD_DOWN;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A)
			{
				isPressed = st->buttons & CONT_A;
				DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "A");
			}
		}
		else	// Arcade Stick 	0xff070000
		{
			int advance = 0;
			
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START)
			{
				isPressed = st->buttons & CONT_START;
				DrawStringS(x+ 7*fw+3, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "S");
				advance++;
			}
			
			y+=fh;
			
			// Draw Up and X, Y, Z
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_UP)
			{
				isPressed = st->buttons & CONT_DPAD_UP;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "U");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_X)
			{
				isPressed = st->buttons & CONT_X;
				DrawStringS(x+ 6*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "X");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Y)
			{
				isPressed = st->buttons & CONT_Y;
				DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Y");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Z)
			{
				isPressed = st->buttons & CONT_Z;
				DrawStringS(x+10*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Z");
				advance++;
			}
			if(advance)
			{
				y += fh;
				advance = 0;
			}
		
			// Draw Left, Right
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_LEFT)
			{
				isPressed = st->buttons & CONT_DPAD_LEFT;
				DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "L");
				advance++;
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_RIGHT)
			{
				isPressed = st->buttons & CONT_DPAD_RIGHT;
				DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "R");
				advance++;
			}
			if(advance)
			{
				y += fh;
				advance = 0;
			}
		
			// Draw Down and A. B, C
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_DOWN)
			{
				isPressed = st->buttons & CONT_DPAD_DOWN;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A)
			{
				isPressed = st->buttons & CONT_A;
				DrawStringS(x+ 6*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "A");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B)
			{
				isPressed = st->buttons & CONT_B;
				DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "B");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C)
			{
				isPressed = st->buttons & CONT_C;
				DrawStringS(x+10*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "C");
			}
		}
	}
	
	// Maracas 0x0f093c00
	if(isMaracas)
	{
		// Left Maraca
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C)
		{
			isPressed = st->buttons & CONT_C;
			DrawStringS(x- 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "C");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B)
		{
			isPressed = st->buttons & CONT_B;
			DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "B");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Z)
		{
			isPressed = st->buttons & CONT_Z;
			DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Z");
		}
		// Right Maraca
		
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START)
		{
			isPressed = st->buttons & CONT_START;
			DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "S");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A)
		{
			isPressed = st->buttons & CONT_A;
			DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "A");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_D)
		{
			isPressed = st->buttons & CONT_D;
			DrawStringS(x+11*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
		}
	}
	y += fh;
	
	x = orig_x+fw;
	// Center the Analog if only one is present
	if(!hasAnalog2X && !hasAnalog2Y)
		x += 4*fw;
	
	// Draw Analog Joystick
	if(hasAnalogY)
	{
		// Analog Up
		sprintf(msg, "%03d", st->joyy < 0 ? -1*st->joyy : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	if(hasAnalogX)
	{
		// Analog Right & Left
		sprintf(msg, "%03d %03d", st->joyx < 0 ? -1*st->joyx : 0, st->joyx > 0 ? st->joyx : 0);
		DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	// Analog Down
	if(hasAnalogY)
	{
		sprintf(msg, "%03d", st->joyy > 0 ? st->joyy : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	}
	
	// Rewind 2 lines towards the top
	if(hasAnalog2X || hasAnalog2Y)
	{
		y -= 2*fh;
		x += 8*fw;
	}
	
	// Second Analog controller
	if(hasAnalog2Y)
	{
		// Analog Up
		sprintf(msg, "%03d", st->joy2y < 0 ? -1*st->joy2y : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	// Analog Right & Left
	if(hasAnalog2X)
	{
		sprintf(msg, "%03d %03d", st->joy2x < 0 ? -1*st->joy2x : 0, st->joy2x > 0 ? st->joy2x : 0);
		DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	// Analog Down
	if(hasAnalog2Y)
	{
		sprintf(msg, "%03d", st->joy2y > 0 ? st->joy2y : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	}
	//Do we have the next few ones?
	x = orig_x;
	y += fh;
	
	// Rewind if we have only added empty lines
	if(y == orig_y + 4*fh)
		y = orig_y + 2*fh;
	
	// Adds existing but without known implementations...
	// Second d-pad and D button
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_UP)
	{
		isPressed = st->buttons & CONT_DPAD2_UP;
		DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "U");
	}
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_DOWN)
	{
		isPressed = st->buttons & CONT_DPAD2_DOWN;
		DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
	}
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_LEFT)
	{
		isPressed = st->buttons & CONT_DPAD2_LEFT;
		DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "L");
	}
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_RIGHT)
	{
		isPressed = st->buttons & CONT_DPAD2_RIGHT;
		DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "R");
	}
	if(!isMaracas && dev->info.function_data[0] & CONT_FIXED_CAPABILITY_D)
	{
		isPressed = st->buttons & CONT_D;
		DrawStringS(x+11*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
	}
}

void DiplayControllerIgnore(int num, float x, float y)
{
	int				hasAnalogX = 0, hasAnalogY = 0;
	int				hasAnalog2X = 0, hasAnalog2Y = 0;
	int				isPressed = 0, hasFunc = 0;
	float			orig_x = x;
	cont_state_t	*st;
	maple_device_t	*dev = NULL;
	char			msg[256], name[256];
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER|MAPLE_FUNC_LIGHTGUN);
	if(!dev)
		return;
	
	ReduceName(msg, dev->info.product_name, 1);
	sprintf(name, "#G%c%c:#G %s", 'A'+(dev->port), '0'+(dev->unit), msg);
	DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, name);
	y += fh;
	
	sprintf(msg, "Power[%u-%u]mW", 
			dev->info.standby_power, dev->info.max_power);
	DrawStringS(x, y, 0.0f, 0.7f, 0.6f, msg);
	y += fh;
	
	st = (cont_state_t*)maple_dev_status(dev);
	if(!st)
		return;
		
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG_X)
		hasAnalogX = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG_Y)
		hasAnalogY = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG2_X)
		hasAnalog2X = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG2_Y)
		hasAnalog2Y = 1;
	
	x += 3*fw;

	// Draw Triggers
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_LTRIG ? 1 : 0;
	sprintf(msg, "%03d", st->ltrig);
	DrawStringS(x+1*fw, y, hasFunc ? 1.0f : 0.7f, hasFunc ? 1.0f : 0.7f, hasFunc ? 1.0f : 0.7f, msg);

	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_RTRIG ? 1 : 0;
	sprintf(msg, "%03d", st->rtrig);
	DrawStringS(x+7*fw, y, hasFunc ? 1.0f : 0.7f, hasFunc ? 1.0f : 0.7f, hasFunc ? 1.0f : 0.7f, msg);
	
	y += fh;

	// Draw Up and X, Y, Z
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_UP ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_UP;
	DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "U");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_X ? 1 : 0;
	isPressed = st->buttons & CONT_X;
	DrawStringS(x+ 6*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "X");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Y ? 1 : 0;
	isPressed = st->buttons & CONT_Y;
	DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "Y");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Z ? 1 : 0;
	isPressed = st->buttons & CONT_Z;
	DrawStringS(x+10*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "Z");
		
	y += fh;

	// Draw Left, Right, Start
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_LEFT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_LEFT;
	DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "L");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_RIGHT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_RIGHT;
	DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "R");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START ? 1 : 0;
	isPressed = st->buttons & CONT_START;
	DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "S");
	
	y += fh;
	
	// Draw Down and A. B, C
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_DOWN ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_DOWN;
	DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "D");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A ? 1 : 0;
	isPressed = st->buttons & CONT_A;
	DrawStringS(x+ 6*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "A");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B ? 1 : 0;
	isPressed = st->buttons & CONT_B;
	DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "B");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C ? 1 : 0;
	isPressed = st->buttons & CONT_C;
	DrawStringS(x+10*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "C");
	
	y += fh;
	
	x = orig_x+fw;
	
	// Draw Analog Joystick
	
	// Analog Up
	sprintf(msg, "%03d", st->joyy < 0 ? -1*st->joyy : 0);
	DrawStringS(x+2*fw, y, hasAnalogY ? 1.0f : 0.7f, hasAnalogY ? 1.0f : 0.7f, hasAnalogY ? 1.0f : 0.7f, msg);
	y += fh;
	
	// Analog Right & Left
	sprintf(msg, "%03d %03d", st->joyx < 0 ? -1*st->joyx : 0, st->joyx > 0 ? st->joyx : 0);
	DrawStringS(x, y, hasAnalogX ? 1.0f : 0.7f, hasAnalogX ? 1.0f : 0.7f, hasAnalogX ? 1.0f : 0.7f, msg);
	y += fh;
		
	// Analog Down
	sprintf(msg, "%03d", st->joyy > 0 ? st->joyy : 0);
	DrawStringS(x+2*fw, y, hasAnalogY ? 1.0f : 0.7f, hasAnalogY ? 1.0f : 0.7f, hasAnalogY ? 1.0f : 0.7f, msg);
	
	// Rewind 2 lines towards the top
	y -= 2*fh;
	x += 8*fw;
	
	// Second Analog controller
	
	// Analog Up
	sprintf(msg, "%03d", st->joy2y < 0 ? -1*st->joy2y : 0);
	DrawStringS(x+2*fw, y, hasAnalog2Y ? 1.0f : 0.7f, hasAnalog2Y ? 1.0f : 0.7f, hasAnalog2Y ? 1.0f : 0.7f, msg);
	y += fh;
	
	// Analog Right & Left
	sprintf(msg, "%03d %03d", st->joy2x < 0 ? -1*st->joy2x : 0, st->joy2x > 0 ? st->joy2x : 0);
	DrawStringS(x, y, hasAnalog2X ? 1.0f : 0.7f, hasAnalog2X ? 1.0f : 0.7f, hasAnalog2X ? 1.0f : 0.7f, msg);
	y += fh;
	
	// Analog Down
	sprintf(msg, "%03d", st->joy2y > 0 ? st->joy2y : 0);
	DrawStringS(x+2*fw, y, hasAnalog2Y ? 1.0f : 0.7f, hasAnalog2Y ? 1.0f : 0.7f, hasAnalog2Y ? 1.0f : 0.7f, msg);
	
	//Do we have the next few ones?
	x = orig_x+fw;
	y += fh;
	
	
	// Adds existing but without known implementations...
	// Second d-pad and D button
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_UP ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_UP;
	DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "U");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_DOWN ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_DOWN;
	DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "D");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_LEFT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_LEFT;
	DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "L");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_RIGHT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_RIGHT;
	DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "R");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_D ? 1 : 0;
	isPressed = st->buttons & CONT_D;
	DrawStringS(x+11*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 1.0f : hasFunc ? 1.0f : 0.7f, isPressed ? 0.0f : hasFunc ? 1.0f : 0.7f, "D");
}

void GetControllerPorts(int *ports)
{
	int				ctrl = 0;
	maple_device_t	*dev = NULL;
	
	memset(ports, 0, sizeof(int)*4);
	for(ctrl = 0; ctrl < 4; ctrl++)
	{
		dev = maple_enum_type(ctrl, MAPLE_FUNC_CONTROLLER|MAPLE_FUNC_LIGHTGUN);
		if(dev)
			ports[dev->port] = 1;
	}
}

void ControllerTest()
{
	uint16			pressed = 0;
	int 			done = 0, oldvmode = -1;
	int				ignoreFunctions = 0, timeout = 0;
	int				ports[4];
	ImagePtr		back = NULL, black = NULL;
	maple_device_t	*dev = NULL;
	cont_state_t 	*st = NULL;

	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
		
	back = LoadKMG("/rd/ControlBack.kmg.gz", 0);
	if(!back)
		return;
	
	while(!done && !EndProgram) 
	{				
		int		num_controller = 0;
		float 	x = 35, y = 26, w = 160, h = 10*fh;
		
		if(oldvmode != vmode)
		{
			back->alpha = 0.6f;
			
			black->w = 320;
			black->h = 240;
			
			oldvmode = vmode;
		}
		StartScene();
		DrawImage(black);
		DrawImage(back);
		
		GetControllerPorts(ports);
		
		// IgnoreFunctions lists all inputs regardless
		// of functions reported
		if(!ignoreFunctions)
		{
			DrawStringSCentered(16, 0.0f, 1.0f, 0.0f, "Controller Test"); 
			
			if(ports[0])
				DiplayController(num_controller++, x, y);
			else
				DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, "#GA0:#G #C(Empty)#C");
			if(ports[1])
				DiplayController(num_controller++, x+w, y);
			else
				DrawStringS(x+w-4*fw, y, 1.0f, 1.0f, 0.0f, "#GB0:#G #C(Empty)#C");
			if(ports[2])
				DiplayController(num_controller++, x,y+h);
			else
				DrawStringS(x-4*fw, y+h, 1.0f, 1.0f, 0.0f, "#GC0:#G #C(Empty)#C");
			if(ports[3])
				DiplayController(num_controller  , x+w, y+h);
			else
				DrawStringS(x+w-4*fw, y+h, 1.0f, 1.0f, 0.0f, "#GD0:#G #C(Empty)#C");
				
			DrawStringSCentered(y+2*h+fh, 0.0f, 1.0f, 0.0f, "[Right&Start on Controller 1 to ignore reported functions]"); 
		}
		else
		{
			DrawStringSCentered(16, 0.0f, 1.0f, 0.0f, "Controller Test #Y(non-reported functions in gray)#Y");
			if(ports[0])
				DiplayControllerIgnore(num_controller++, x, y);
			else
				DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, "#GA0:#G #C(Empty)#C");
			if(ports[1])
				DiplayControllerIgnore(num_controller++, x+w, y);
			else
				DrawStringS(x+w-4*fw, y, 1.0f, 1.0f, 0.0f, "#GB0:#G #C(Empty)#C");
			if(ports[2])
				DiplayControllerIgnore(num_controller++, x, y+h);
			else
				DrawStringS(x-4*fw, y+h, 1.0f, 1.0f, 0.0f, "#GC0:#G #C(Empty)#C");
			if(ports[3])
				DiplayControllerIgnore(num_controller  , x+w, y+h);
			else
				DrawStringS(x+w-4*fw, y+h, 1.0f, 1.0f, 0.0f, "#GD0:#G #C(Empty)#C");
			DrawStringSCentered(y+2*h+fh, 0.0f, 1.0f, 0.0f, "[Right&Start on Controller 1 for reported functions only]"); 
		}
		
		DrawStringS(110, y+2*h-fh, 0.0f, 1.0f, 0.0f, "[Up&Start for Help]"); 
		DrawStringS(70, y+2*h, 0.0f, 1.0f, 0.0f, "[Left&Start on Controller 1 to Exit}"); 
		
        EndScene();
		if(timeout)
			timeout--;
			
		VMURefresh("Controller", ignoreFunctions ? "Ignore" : "Normal");
		
		// we do it without helper functions here
		dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
		if(dev)
		{			
			st = (cont_state_t*)maple_dev_status(dev); 
			if (st->buttons & CONT_START  &&
				st->buttons & CONT_DPAD_LEFT)
				done =	1;
			if (st->buttons & CONT_START  &&
				st->buttons & CONT_DPAD_RIGHT)
			{
				if(!timeout)
				{
					ignoreFunctions = !ignoreFunctions;
					timeout = 20;
					refreshVMU = 1;
				}
			}
			if (st->buttons & CONT_START  &&
				st->buttons & CONT_DPAD_UP)
				{
					// we clear controller buffer for menu
					ReadController(0, &pressed);
					ShowMenu(CONTROLHELP);
				}
		}
		
	}
	// we clear controller buffer for exit
	ReadController(0, &pressed);
	FreeImage(&back);
	FreeImage(&black);
	return;
}

int maple_device_scan(float x, float y, int selected)
{
	int     		port = 0, unit = 0, count = 0;
	maple_device_t  *dev = NULL;
	char			msg[512], name[256];

    /* Enumerate everything */
    for(port = 0; port < MAPLE_PORT_COUNT; port++) 
	{	
		float	orig_y = y, max_y = y;
		
        for(unit = 0; unit < MAPLE_UNIT_COUNT; unit++)
		{
            dev = &maple_state.ports[port].units[unit];

            if(dev->valid)
			{
				count ++;
				if(unit == 0)
				{
					y += 2.0f*fh;
					ReduceName(name, dev->info.product_name, 0);
					sprintf(msg, "%s#C[%c%c]#C #Y%s#Y (%s)",
						selected == count ? "#Y>#Y" : " ",
						'A' + port, '0' + unit,
						name,
						maple_pcaps(dev->info.functions));
					DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
					orig_y = y;
				}
				else
				{
					float xpos = x+2.0f*fw;
					
					if(unit == 4)
						y = orig_y;
					if(unit > 3)
						xpos += 28.0f*fw;
					y += fh;
					sprintf(msg, "%s#C[%c%c]#C %s",
						selected == count ? "#Y>#Y" : " ",
						'A' + port, '0' + unit,
						maple_pcaps(dev->info.functions));
						
					DrawStringS(xpos, y, 1.0f, 1.0f, 1.0f, msg);
				}
				if(y > max_y)
					max_y = y;
            }
        }
		y = max_y;
    }
	return count;
}

#define	MAPLE_BUFFER_CHAR	8192
char			*maple_display = NULL;
int				maple_display_pos = 0;
int				maple_locked_device = 0;
int				maple_use_reply_bytes = 0;
int				maple_useCache = 0;
int				maple_command_finish_wait = 0;

#ifdef DCLOAD
void PrintCleanString(char *str) 
{	
	while (*str) 
	{		
		if(*str == '$')
		{
			dbglog(DBG_INFO, " "); 
			str++;
			continue;
		}
		
		if(*str == '#')
		{						
			str++;
			str++;
			continue;
		}
		dbglog(DBG_INFO, "%c", *str++);
	}
}
#endif

void mapleprintf(char *fmt, ... )
{
	int		len = 0;
	char	buffer[512];
	va_list arguments;

	if(!maple_display)
		return;

	va_start(arguments, fmt);
	vsprintf(buffer, fmt, arguments);
	va_end(arguments);
	
	len = strlen(buffer);
	if(maple_display_pos + len < MAPLE_BUFFER_CHAR)
	{
		memcpy(maple_display+maple_display_pos, buffer, sizeof(char)*len);
		maple_display_pos += len;
	}
	else
		dbglog(DBG_ERROR, "Maple display text buffer was too small"); 
		
#ifdef DCLOAD	
	PrintCleanString(buffer);
#endif
}

/*
 * Code below is from upgraded dcutils by Donald Haase.
 * taken from https://dcemulation.org/phpBB/viewtopic.php?t=97047
 * slightly adapted formating for PVR display
 */
 
#define GRABALL 2
/* Local copy of the returned buffer for maple stuff */
unsigned char 	recv_buff[1024 + 32];

static void vbl_allinfo_callback(maple_frame_t * frm) {
	maple_response_t	*resp;

	/* So.. did we get a response? */
	resp = (maple_response_t *)frm->recv_buf;

	if (resp->response == MAPLE_RESPONSE_ALLINFO) {
		/* Copy in the new buff */
		memcpy(recv_buff, resp, 1024 + 32);
	} else {
		uint16 resp_size = 0;
		
		switch(resp->response)
		{
			case MAPLE_RESPONSE_AGAIN:
				mapleprintf("\n Device asked for retry.\n Please try again. Got #YMAPLE_RESPONSE_AGAIN#Y"); 
				break;
			case MAPLE_RESPONSE_BADCMD:
				mapleprintf("\n #CDevice doesn't support command.#C\n Maybe an emulator?\n Got #YMAPLE_RESPONSE_BADCMD#Y"); 
				break;
			case MAPLE_RESPONSE_BADFUNC:
				mapleprintf("\n #CDevice doesn't support command.#C\n Got #YMAPLE_RESPONSE_BADFUNC#Y"); 
				break;
			case MAPLE_RESPONSE_NONE:
				mapleprintf("\n #CDevice didn't respond to query command.#C\n Got #YMAPLE_RESPONSE_NONE#Y"); 
				break;
			case MAPLE_COMMAND_ALLINFO:
				mapleprintf("\n #CDevice doesn't support command.#C\n Maybe an emulator?\n Got #YMAPLE_COMMAND_ALLINFO#Y"); 
				break;
			case MAPLE_RESPONSE_DEVINFO:
				mapleprintf("\n #CDevice doesn't support command.#C\n Got #YMAPLE_RESPONSE_DEVINFO#Y"); 
				break;
			case MAPLE_COMMAND_RESET:
				mapleprintf("\n Device asked for reset.\n Please reconnect. Got #YMAPLE_COMMAND_RESET#Y"); 
				break;
			case MAPLE_RESPONSE_OK:
				mapleprintf("\n Unexpected #YMAPLE_RESPONSE_OK#Y from device", resp->response); 
				break;
			case MAPLE_RESPONSE_DATATRF:
				mapleprintf("\n Unexpected #YMAPLE_RESPONSE_DATATRF#Y from device", resp->response); 
				break;
			default:
				mapleprintf("\n Unexpected response [#Y%d#Y] from device", resp->response); 
				break;
		}
		if(resp->response != MAPLE_RESPONSE_NONE)
		{
			mapleprintf(" [%u bytes response]", resp->data_len*4);
			if(maple_use_reply_bytes)
				resp_size = resp->data_len*4;
			else
				resp_size = 1024 + 32;
				
			if(resp_size)
				memcpy(recv_buff, resp, resp_size);
		}
		
		mapleprintf("\n");
	}

	maple_frame_unlock(frm);
	maple_command_finish_wait = 1;
}

/* Send a ALLINFO command for the given port/unit */
static void vbl_send_allinfo(int p, int u) {
	maple_device_t 		*dev = NULL;
	int					frames_wait_for_lock = 60;

	maple_command_finish_wait = 0;
	maple_locked_device = 0;

	/* Reserve access; if we don't get it, forget about it */
	do
	{	
		dev = &maple_state.ports[p].units[u];
		if(maple_frame_lock(&dev->frame) == 0)
		{
			maple_locked_device = 1;
			break;
		}
		
		pvr_wait_ready();
		frames_wait_for_lock --;
	}while(!maple_locked_device && frames_wait_for_lock != 0);
	
	if(!maple_locked_device)
	{
		mapleprintf("\nmaple_frame_lock() failed");
		if(dev)
			mapleprintf(" for #Y%c%c#Y", 'A'+(dev->port), '0'+(dev->unit));
		mapleprintf("\n\n Please try reconnecting the device\n");
		return;
	}

	/* Setup our autodetect frame to probe at a new device */
	maple_frame_init(&dev->frame);
	dev->frame.cmd = MAPLE_COMMAND_ALLINFO;
	dev->frame.dst_port = p;
	dev->frame.dst_unit = u;
	dev->frame.callback = vbl_allinfo_callback;
	maple_queue_frame(&dev->frame);
}

/* Formatted output for the devinfo struct from command */
void print_devinfo(maple_devinfo_t *info)
{
	/* Print each piece of info in an itemized fashion */	
	mapleprintf("#CFunctions     :#C 0x%.8lx  ", info->functions);
	mapleprintf("#CFunction int 0:#C 0x%.8lx\n", info->function_data[0]);
	mapleprintf("#CFunction int 1:#C 0x%.8lx  ", info->function_data[1]);
	mapleprintf("#CFunction int 2:#C 0x%.8lx\n", info->function_data[2]);
	mapleprintf("#CRegion:#C         0x%.2x        ", info->area_code);
	mapleprintf("#CConnection:#C     0x%.2x\n", info->connector_direction);
	mapleprintf("#CProduct Name & License:#C %.30s\n", info->product_name);
	mapleprintf("  %.60s\n", info->product_license);
	mapleprintf("#CStandby power:#C 0x%.4x (%umW) ", info->standby_power, info->standby_power);
	mapleprintf("#CMax:#C 0x%.4x (%umW)\n", info->max_power, info->max_power);
}

/* Formatted output for the devinfo struct in KOS */
void FillKOSDevInfo(maple_device_t *dev, int skipline)
{
	mapleprintf("%s#YValues from MAPLE_RESPONSE_DEVINFO#Y:\n", skipline ? "\n" : "");
	mapleprintf("#CFunctions     :#C 0x%.8lx  ", dev->info.functions);
	mapleprintf("#CFunction int 0:#C 0x%.8lx\n", dev->info.function_data[0]);
	mapleprintf("#CFunction int 1:#C 0x%.8lx  ", dev->info.function_data[1]);
	mapleprintf("#CFunction int 2:#C 0x%.8lx\n", dev->info.function_data[2]);
	mapleprintf("#CRegion:#C         0x%.2x        ", dev->info.area_code);
	mapleprintf("#CConnection:#C     0x%.2x\n", dev->info.connector_direction);
	mapleprintf("#CProduct Name & License:#C %.30s\n", dev->info.product_name);
	mapleprintf("  %.60s\n", dev->info.product_license);
	mapleprintf("#CStandby power:#C 0x%.4x (%umW) ", dev->info.standby_power, dev->info.standby_power);
	mapleprintf("#CMax:#C 0x%.4x (%umW)\n", dev->info.max_power, dev->info.max_power);
}

#define BPL 16	/* Bytes to print per line */
/* 
	Get and print the data returned by the ALLINFO command, 
	if extra=0, do not print the Free Data
*/
void print_device_allinfo(maple_device_t *dev, int extra)
{
	int				timeout = 0;
	unsigned int	size = 4;
	unsigned int	i = 0;
	unsigned int	j = 0;
#ifdef BENCHMARK
	uint64			start, end;
#endif	

	if(dev == NULL)	
	{
		dbglog(DBG_ERROR, "print_device_allinfo: received NULL device");
		return;
	}
	
	/* Clear the old buffer */
	memset(recv_buff, 0, 1024+32);
	if(!maple_useCache)
		mapleprintf("Requested info for device #Y%c%c#Y ", 'A'+(dev->port), '0'+(dev->unit));	
	else
		mapleprintf("#YValues from MAPLE_RESPONSE_ALLINFO#Y for #Y%c%c#Y ", 'A'+(dev->port), '0'+(dev->unit));	
		
#ifdef BENCHMARK
	start = timer_ms_gettime64();
#endif
	vbl_send_allinfo((dev->port), (dev->unit));
	do
	{
		// the value is updated inside an interrupt
		// can't use semaphores or mutex, but it is safe exactly because of that
		timer_spin_sleep(10);
		timeout ++;
	}while(maple_command_finish_wait == 0 && timeout < 80);
	
#ifdef BENCHMARK
	end = timer_ms_gettime64();
	dbglog(DBG_INFO, "\n>> vbl_send_allinfo took %"PRIu64" ms <<\n", end - start);
#endif
	
	size += (recv_buff[3]*4);
	
	if(!maple_locked_device)
		return;
		
	if(size-4 <= 0)
	{
		if(!maple_useCache)
			FillKOSDevInfo(dev, 1);
		return;
	}
	
	mapleprintf("[%d bytes of data]:\n", size-4);
	print_devinfo((maple_devinfo_t *)&recv_buff[4]);

	if(extra)
	{
#if (GRABALL == 1)
		size = 1024+32;
#elif (GRABALL == 2)
		/* Grab a bit extra, cause some devices are stupid, and give more than they tell */
		size = ((size+64)<(1024+32))?(size+64):(1024+32);
#endif

		mapleprintf("#CExtra data:#C\n");
		/* Print the hex for the data followed by ascii */
		for(i=4+112; i< size; i+=BPL) 
		{	
			mapleprintf("#G%.3x#G$#G|#G$", i-(4+112));
			for(j=0;j<BPL;j++)	mapleprintf("%.2x$", recv_buff[i+j]);		
			mapleprintf("#G|#G$");
			
			for(j=0;j<BPL;j++)	mapleprintf("%c"   , isprint(recv_buff[i+j])?recv_buff[i+j]:' ');
			mapleprintf("\n");
		}	
		mapleprintf("#CEnd of Extra data#C\n");
	}
}

/*
 * End code copy from Donald Haase.
 */
void print_device_kos_cache(maple_device_t *dev)
{
	if(!dev)
		return;
		
	FillKOSDevInfo(dev, 0);
	mapleprintf("#G--------------------------------------------------------------#G\n");
}

void cleanMapleBuffer()
{
	maple_display_pos = 0;
	memset(maple_display, 0, sizeof(char)*MAPLE_BUFFER_CHAR);
}

void ListMapleDevices()
{
	int 			done = 0, sel = 1, oldvmode = -1, c = -1;
	int				v_scroll = 0, joycnt = 0;
	uint16			pressed;		
	ImagePtr		back = NULL, black = NULL;
	controller		*st = NULL;

	if(maple_display)
	{
		dbglog(DBG_ERROR, "Maple display text buffer was not NULL"); 
		return;
	}
	
	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
		
	back = LoadKMG("/rd/maple.kmg.gz", 0);
	if(!back)
		return;
	
	maple_display = (char*)malloc(sizeof(char)*MAPLE_BUFFER_CHAR);
	if(!maple_display)
	{
		dbglog(DBG_ERROR, "Out of memory for maple display text buffer\n"); 
		return;
	}

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->alpha = 0.4f;
			black->w = 320;
			black->h = 240;
			
			oldvmode = vmode;
		}
		StartScene();
		DrawImage(black);
		DrawImage(back);

		DrawStringSCentered(20+v_scroll, 0.0f, 1.0f, 0.0f, "Maple Devices");
		c = maple_device_scan(15, 20+v_scroll, sel);
		if(maple_use_reply_bytes)
			DrawStringSCentered(210+v_scroll, 0.0f, 1.0f, 0.0f, "Use Maple response bytes"); 
		if(maple_useCache)
			DrawStringSCentered(210+v_scroll+fh, 0.0f, 1.0f, 0.0f, "Use short response"); 
		EndScene();

		VMURefresh("Maple", "Devices");

		st = ReadController(0, &pressed);
		JoystickMenuMove(st, &sel, c, &joycnt);
		if(st)
		{
			if ( pressed & CONT_LTRIGGER )
				v_scroll -= fw;
			if ( pressed & CONT_RTRIGGER )
				v_scroll += fw;
			if(v_scroll > 0)
				v_scroll = 0;
			if(v_scroll < -1*c*fh)
				v_scroll = -1*c*fh;

			if ( pressed & CONT_DPAD_UP )
				sel --;
			
			if ( pressed & CONT_DPAD_DOWN )
				sel ++;
		
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				if(sel > 0)
				{
					int 			close_display = 0;
					maple_device_t	*dev = NULL;
					
					dev = maple_enum_type(sel - 1, 0xffffffff);
					if(dev)
					{
						int		sub_v_scroll = 0, hscroll = 0, lines = 0;
						int		joycntx = 0, joycnty = 0;
						char 	vmudata[20];
						
						sprintf(vmudata, "query: %c%c", 'A'+(dev->port), '0'+(dev->unit));	
						updateVMU("Maple", vmudata, 1);	
						cleanMapleBuffer();
						if(maple_useCache)
							print_device_kos_cache(dev);
						print_device_allinfo(dev, 1);
						lines = countLineFeeds(maple_display);
						while(!close_display && !EndProgram) 
						{
							StartScene();
							DrawImage(black);
							DrawImage(back);
							DrawString(5+hscroll, 20+sub_v_scroll, 1.0f, 1.0f, 1.0f, maple_display);
							EndScene();
							
							st = ReadController(0, &pressed);
							JoystickDirectios(st, &pressed, &joycntx, &joycnty);
							if(st)
							{
								if ( pressed & CONT_LTRIGGER || pressed & CONT_DPAD_UP )
									sub_v_scroll -= fw;
								if ( pressed & CONT_RTRIGGER || pressed & CONT_DPAD_DOWN )
									sub_v_scroll += fw;
								if(sub_v_scroll > 0)
									sub_v_scroll = 0;
								if(sub_v_scroll < -1*lines*fh)
									sub_v_scroll = -1*lines*fh;
								
								if ( pressed & CONT_DPAD_LEFT )
									hscroll -= fw;
								if ( pressed & CONT_DPAD_RIGHT )
									hscroll += fw;
								if(hscroll > 8*fw)
									hscroll = 8*fw;
								if(hscroll < -20*fw)
									hscroll = -20*fw;
									
								if (pressed & CONT_Y)
									hscroll = sub_v_scroll = 0;
								
								if (pressed & CONT_B)
									close_display = 1;

								if (pressed & CONT_START)
									ShowMenu(MAPLEHELP);
							}
						}
					}
					refreshVMU = 1;
				}
			}

			if (pressed & CONT_Y)
				maple_useCache = !maple_useCache;
			if (pressed & CONT_X)
				maple_use_reply_bytes = !maple_use_reply_bytes;
			if (pressed & CONT_START)
				ShowMenu(MAPLEHELP);
		}
		
		if(sel < 1)
			sel = c;
		if(sel > c)
			sel = 1;
	}
	FreeImage(&back);
	FreeImage(&black);
	if(maple_display)
	{
		free(maple_display);
		maple_display = NULL;
		maple_display_pos = 0;
	}
	return;
}

/*
 *
 * We have two methods to detect a VMU that reports as having an LCD 
 * but doesn't. That wouldn't be an issue, but when graphics are sent 
 * to reported the "LCD", it resets the controller, slowing everything
 * down and in some cases, mangling controller input.
 *
 * This method checks the INFO and ALL_INFO commands and compares them
 * for reported inconsistencies. I've found that the Visual Memory
 * packet was lifted verbatim from an official VMU, hence the flags report 
 * clock and LCD. 
 *
 * If this happens, all LCD commands are blocked for the current session.
 *
 * The other method is checked every controller swap, in vmu.c
 *
 */

int check_for_bad_lcd()
{
	int				timeout = 0;
	unsigned int	size = 4;
	maple_device_t	*dev;
	maple_devinfo_t *info;
#ifdef BENCHMARK
	uint64			start, end;

	start = timer_ms_gettime64();
#endif	

	dev = maple_enum_type(0, MAPLE_FUNC_LCD);
	if(!dev)
		return 0;
	
	/* Clear the old buffer */
	memset(recv_buff, 0, 1024+32);
	vbl_send_allinfo((dev->port), (dev->unit));
	do
	{
		// the value is updated inside an interrupt
		// can't use semaphores or mutex, but it is safe exactly because of that
		timer_spin_sleep(10);
		timeout ++;
	}while(maple_command_finish_wait == 0 && timeout < 80);
	
#ifdef BENCHMARK
	end = timer_ms_gettime64();
	dbglog(DBG_INFO, "check_for_bad_lcd() took %"PRIu64" ms\n", end - start);
#endif
	
	size += (recv_buff[3]*4);
	
	if(!maple_locked_device)
		return 1;
		
	// we allow if the device doesn't support the command
	// emulators fail this
	if(size-4 <= 0)
		return 1;
	
	info = (maple_devinfo_t *)&recv_buff[4];
	if(dev->info.functions != info->functions)
		return 1;
	if(dev->info.function_data[0] != info->function_data[0])
		return 1;
	if(dev->info.function_data[1] != info->function_data[1])
		return 1;
	if(dev->info.function_data[2] != info->function_data[2])
		return 1;
	
	return 0;
}

/*
CRC 32 based on work by Christopher Baker <https://christopherbaker.net>
*/

uint32_t _state = ~0L;

static const uint32_t crc32_table[] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


void CRC32_reset()
{
    _state = ~0L;
}


void CRC32_update(uint8_t data)
{
    uint8_t tbl_idx = 0;

    tbl_idx = _state ^ (data >> (0 * 4));
    _state = (*(uint32_t*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
    tbl_idx = _state ^ (data >> (1 * 4));
    _state = (*(uint32_t*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
}


uint32_t CRC32_finalize()
{
    return ~_state;
}

/**********************************/

uint32_t CalculateCRC(uint32_t startAddress, uint32_t size)
{
	uint8_t *start = NULL;
	uint32_t address = 0, checksum = 0;
	CRC32_reset();

	start = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		uint8_t data;
		
		data = start[address];

		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

#define VISIBLE_HORZ	16
#define VISIBLE_VERT	26
#define MAX_LOCATIONS	17

void MemoryViewer(uint32 address)
{
	int 		done = 0, ascii = 0, locpos = 0, docrc = 0, factor = 1;
	int			joycntx = 0, joycnty = 0, oldvmode = -1;
	uint16		pressed;		
	uint32		crc = 0;
	ImagePtr	back = NULL;
	uint32		locations[MAX_LOCATIONS] = { 	0x00000000, 0x00200000,
												0x00240000, 0x04000000,
												0x0C000000, 0x0C000100, // KallistiOS 
												0x0C004000, 0x0C008000, // KallistiOS 
												0x0C008300, 0x0C00b800, // KallistiOS 
												0x0C00e400, 0x0C010000, // KallistiOS 
												0x10000000, 0x10800000,
												0x14000000, 0x1C000000, 0x1FFFFFFF};
	char		*names[MAX_LOCATIONS] = { 	"Boot ROM (2 Mb)", "Flash ROM", 
											"System devices", "Texture (8 Mb)",
											"KOS Sys State", "KOS Syscalls",
											"KOS Reserved", "IP.BIN",
											"KOS Bootstrap 1", "KOS Bootstrap 2",
											"KOS Unused", "KOS Application",
											"System memory", "Tile Accelerator",
											"Texture memory", "G2 devices", "NULL"};
	controller	*st;

	back = LoadKMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
		
	if(address == 0)
		address = locations[0];
	while(!done && !EndProgram) 
	{
		int 	i = 0, j = 0, pos = -1;
		uint8_t *mem = NULL;
		char 	buffer[10];
		
		if(oldvmode != vmode)
		{
			if(vmode >= HIGH_RES)
				factor = 2;
			else
				factor = 1;
				
			back->w = dW;
			back->h = dH;
			
			oldvmode = vmode;
		}

		StartScene();
		DrawImage(back);

		mem = (uint8_t*)address;
		
		if(docrc)
			crc = CalculateCRC(address, VISIBLE_HORZ*factor*VISIBLE_VERT*factor);
	
		sprintf(buffer, "%08" PRIx32, address);
		DrawStringS(VISIBLE_HORZ*factor*3*fw, 0, 0.0f, 1.0f, 0.0f, buffer);
		
		sprintf(buffer, "%08" PRIx32, address+VISIBLE_HORZ*factor*VISIBLE_VERT*factor);
		DrawStringS(VISIBLE_HORZ*factor*3*fw, (VISIBLE_VERT*factor-1)*fh, 0.0f, 1.0f, 0.0f, buffer);

		if(docrc)
		{
			sprintf(buffer, "%08" PRIx32, crc);
			DrawStringS(VISIBLE_HORZ*factor*3*fw, (VISIBLE_VERT*factor/2)*fh, 1.0f, 1.0f, 0.0f, buffer);		
		}
		
		for(i = 0; i < MAX_LOCATIONS; i++)
		{
			if(address < locations[i])
			{
				pos = i - 1;
				break;
			}
		}
		if(pos != -1)
			DrawStringS(VISIBLE_HORZ*factor*3*fw, (VISIBLE_VERT*factor/2)*(fh-1), 1.0f, 1.0f, 0.0f, names[pos]);
			
		for(i = 0; i < VISIBLE_VERT*factor; i++)
		{
			for(j = 0; j < VISIBLE_HORZ*factor; j++)
			{	
				if(!ascii)
					sprintf(buffer, "%02X", mem[i*VISIBLE_HORZ*factor+j]);
				else
				{
					uint8_t c;
					
					memset(buffer, 0, sizeof(char)*10);

					c = mem[i*VISIBLE_HORZ*factor+j];
					if(c >= 32 && c <= 126)	
						buffer[0] = (char)c;			// ASCII range
					else
						buffer[0] = (char)32;			// Space
				}
				DrawString(3*j*fw, i*fh, 1.0f, 1.0f, 1.0f, buffer);
			}
		}
        EndScene();
		
		VMURefresh("MemView", "");
		st = ReadController(0, &pressed);
		
		JoystickDirectios(st, &pressed, &joycntx, &joycnty);
		if (pressed & CONT_DPAD_LEFT)	
		{
			if(address > locations[0])
				address -= VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
			else
				address = locations[0];
				
			if(address >= 0x670000 && address < 0x04000000)
				address = 0x670000;
		}
		
		if (pressed & CONT_DPAD_RIGHT)
		{
			address += VISIBLE_HORZ*factor*VISIBLE_VERT*factor;

			if(address >= 0xFFFFFFFF) 
				address = 0xFFFFFFFF-VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
				
			if(address >= 0x670000 && address < 0x04000000)
				address = 0x04000000;
		}
		
		if (pressed & CONT_DPAD_UP)	
		{
			if(address > locations[0]+0x10000)
				address -= 0x10000;
			else
				address = locations[0];
				
			if(address >= 0x670000 && address < 0x04000000)
				address = 0x670000;
		}
		
		if (pressed & CONT_DPAD_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0xFFFFFFFF) // 0x817fffff
				address = 0xFFFFFFFF-VISIBLE_HORZ*factor*VISIBLE_VERT*factor;

			if(address >= 0x670000 && address < 0x04000000)
				address = 0x04000000;
		}
		
		if (pressed & CONT_B)
			done =	1;
			
		if (pressed & CONT_A)
		{
			locpos ++;
			if(locpos == MAX_LOCATIONS - 1)
				locpos = 0;
			address = locations[locpos];
		}
			
		if (pressed & CONT_Y)
			ascii =	!ascii;
			
		if (pressed & CONT_X)
			docrc =	!docrc;

		if (pressed & CONT_START)
			ShowMenu(MEMVIEWHELP);
	}
	FreeImage(&back);
	return;
}

void VMUControllerTest()
{
	int		saveexists = 0;
	char 	error[256],
			*msg = "VMU data could not be saved\n";
	
	if(!isVMUPresent())
		return;
		
	if(MemcardOtherGameExists(VMU_CTRL_NAME))
	{
		DrawMessage("There is already another game in this #CVMU#C\nPlease insert a #CVMU#C without a game,\nor delete this one first.");
		return;
	}
	
	saveexists = MemcardSaveExists(VMU_CTRL_NAME, NULL);
	if(saveexists == VMU_SAVEEXISTS)
	{
		int			dowrite = 1;
		fmenudata 	resmenudata[] = { {1, "Overwrite"}, {2, "Cancel"} };
			
		dowrite = SelectMenu("Replace VMU Test?", resmenudata, 2, 2);
		if(dowrite == MENU_CANCEL || dowrite == 2)
			return;
	}
	
	if(!saveexists)
	{
		if(!AskQuestion("#YVMU Controller Test#Y\n\nThis will save the application to your #CVMU#C\nProceed with save?"))
			return;
	}
	
	if(check_for_bad_lcd())
	{
		if(!AskQuestion("This #CVMU#C reports an LCD but seems to not have one\nWrite test anyway?"))
			return;
	}
	
	sprintf(error, "%s", msg);
	DrawMessageOnce("Please don't remove VMU while saving...");
	if(WriteMemCardControlTest(error+strlen(msg)) != VMU_OK)
		DrawMessage(error);
	else
		DrawMessage("Controller Test saved to #CVMU#C\nPlease dettach the unit and run it from the #YLCD#Y.");
}

/* Modified from KOS */
int flashrom_get_region_data()
{
    int start, size;

	if(flashrom_is_cached)
		return FLASHROM_ERR_NONE;
		
	memset(flashrom_region_cache, 0, sizeof(char)*FLASHROM_CACHE_SIZE);
	
    /* Find the partition */
    if(flashrom_info(FLASHROM_PT_SYSTEM, &start, &size)) {
        dbglog(DBG_ERROR, "flashrom_get_region: can't find partition 0\n");
        return FLASHROM_ERR_NO_PARTITION;
    }

    /* Read the first 5 characters of that partition */
    if(flashrom_read(start, flashrom_region_cache, 5) < 0) {
        dbglog(DBG_ERROR, "flashrom_get_region: can't read partition 0\n");
        return FLASHROM_ERR_READ_PART;
    }
	
	flashrom_is_cached = 1;
	dbglog(DBG_INFO, "Read Flashrom Data %s\n", flashrom_region_cache);
	return FLASHROM_ERR_NONE;
}
	
int flashrom_get_region_country()
{
	if(!flashrom_is_cached)
	{
		if(flashrom_get_region_data() != FLASHROM_ERR_NONE)
			return FLASHROM_REGION_UNKNOWN;
	}
	
    /* Now compare cache against known codes */
    if(flashrom_region_cache[2] == '0')
        return FLASHROM_REGION_JAPAN;
    else if(flashrom_region_cache[2] == '1')
        return FLASHROM_REGION_US;
    else if(flashrom_region_cache[2] == '2')
        return FLASHROM_REGION_EUROPE;
    else {
        dbglog(DBG_WARNING, "flashrom_get_region_country: unknown code '%s' (pos %d %c)\n",
			flashrom_region_cache, 2, flashrom_region_cache[2]);
        return FLASHROM_REGION_UNKNOWN;
    }
}

int flashrom_get_region_language()
{
    if(!flashrom_is_cached)
	{
		if(flashrom_get_region_data() != FLASHROM_ERR_NONE)
			return FLASHROM_LANGUAGE_UNKNOWN;
	}
	
    /* Now compare against known codes */
    if(flashrom_region_cache[3] == '0')
        return FLASHROM_LANGUAGE_JAPAN;
    else if(flashrom_region_cache[3] == '1')
        return FLASHROM_LANGUAGE_ENGLISH;
    else if(flashrom_region_cache[3] == '2')
        return FLASHROM_LANGUAGE_GERMAN;
	else if(flashrom_region_cache[3] == '3')
        return FLASHROM_LANGUAGE_FRENCH;
	else if(flashrom_region_cache[3] == '4')
        return FLASHROM_LANGUAGE_SPANISH;
	else if(flashrom_region_cache[3] == '5')
        return FLASHROM_LANGUAGE_ITALIAN;
    else {
        dbglog(DBG_WARNING, "flashrom_get_region_language: unknown code '%s' (pos %d %c)\n",
			flashrom_region_cache, 3, flashrom_region_cache[3]);
        return FLASHROM_LANGUAGE_UNKNOWN;
    }
}

int flashrom_get_region_broadcast()
{
    if(!flashrom_is_cached)
	{
		if(flashrom_get_region_data() != FLASHROM_ERR_NONE)
			return FLASHROM_BROADCAST_UNKNOWN;
	}
	
    /* Now compare against known codes */
    if(flashrom_region_cache[4] == '0')
        return FLASHROM_BROADCAST_NTSC;
    else if(flashrom_region_cache[4] == '1')
        return FLASHROM_BROADCAST_PAL;
    else if(flashrom_region_cache[4] == '2')
        return FLASHROM_BROADCAST_PALM;
	else if(flashrom_region_cache[4] == '3')
        return FLASHROM_BROADCAST_PALN;
    else {
        dbglog(DBG_WARNING, "flashrom_get_region_broadcast: unknown code '%s' (pos %d %c)\n",
			flashrom_region_cache, 4, flashrom_region_cache[4]);
        return FLASHROM_BROADCAST_UNKNOWN;
    }
}
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
#include "sound.h"

int				flashrom_is_cached = 0;
flash_data		flash_cache;
flash_data_p	p_flash_cache;

int				flashromp2_is_cached = 0;
cid_record_st	cid_record;

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

#define CONTROLLER_FUNCTIONS MAPLE_FUNC_CONTROLLER|MAPLE_FUNC_LIGHTGUN

void DiplayController(int num, float x, float y)
{
	int				isPressed = 0;
	int				hasAnalogX = 0, hasAnalogY = 0, hasAnalog2X = 0, hasAnalog2Y = 0, miny = 0;
	cont_state_t	*st;
	maple_device_t	*dev = NULL;
	char			msg[256], name[256];
	float			orig_x = x, orig_y = y;
	
	dev = maple_enum_type(num, CONTROLLER_FUNCTIONS);
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
	miny = y;
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
		if(!hasAnalogY)
			y += fh;
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
		if(y <= miny)
			y = miny + fh;
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

#define NON_FUNC	 0.4f
void DiplayControllerIgnore(int num, float x, float y)
{
	int				hasAnalogX = 0, hasAnalogY = 0;
	int				hasAnalog2X = 0, hasAnalog2Y = 0;
	int				isPressed = 0, hasFunc = 0;
	float			orig_x = x;
	cont_state_t	*st;
	maple_device_t	*dev = NULL;
	char			msg[256], name[256];
	
	dev = maple_enum_type(num, CONTROLLER_FUNCTIONS);
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
	DrawStringS(x+1*fw, y, hasFunc ? 1.0f : NON_FUNC, hasFunc ? 1.0f : NON_FUNC, hasFunc ? 1.0f : NON_FUNC, msg);

	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_RTRIG ? 1 : 0;
	sprintf(msg, "%03d", st->rtrig);
	DrawStringS(x+7*fw, y, hasFunc ? 1.0f : NON_FUNC, hasFunc ? 1.0f : NON_FUNC, hasFunc ? 1.0f : NON_FUNC, msg);
	
	y += fh;

	// Draw Up and X, Y, Z
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_UP ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_UP;
	DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "U");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_X ? 1 : 0;
	isPressed = st->buttons & CONT_X;
	DrawStringS(x+ 6*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "X");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Y ? 1 : 0;
	isPressed = st->buttons & CONT_Y;
	DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "Y");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Z ? 1 : 0;
	isPressed = st->buttons & CONT_Z;
	DrawStringS(x+10*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "Z");
		
	y += fh;

	// Draw Left, Right, Start
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_LEFT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_LEFT;
	DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "L");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_RIGHT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_RIGHT;
	DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "R");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START ? 1 : 0;
	isPressed = st->buttons & CONT_START;
	DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "S");
	
	y += fh;
	
	// Draw Down and A. B, C
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_DOWN ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD_DOWN;
	DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "D");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A ? 1 : 0;
	isPressed = st->buttons & CONT_A;
	DrawStringS(x+ 6*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "A");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B ? 1 : 0;
	isPressed = st->buttons & CONT_B;
	DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "B");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C ? 1 : 0;
	isPressed = st->buttons & CONT_C;
	DrawStringS(x+10*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "C");
	
	y += fh;
	
	x = orig_x+fw;
	
	// Draw Analog Joystick
	
	// Analog Up
	sprintf(msg, "%03d", st->joyy < 0 ? -1*st->joyy : 0);
	DrawStringS(x+2*fw, y, hasAnalogY ? 1.0f : NON_FUNC, hasAnalogY ? 1.0f : NON_FUNC, hasAnalogY ? 1.0f : NON_FUNC, msg);
	y += fh;
	
	// Analog Right & Left
	sprintf(msg, "%03d %03d", st->joyx < 0 ? -1*st->joyx : 0, st->joyx > 0 ? st->joyx : 0);
	DrawStringS(x, y, hasAnalogX ? 1.0f : NON_FUNC, hasAnalogX ? 1.0f : NON_FUNC, hasAnalogX ? 1.0f : NON_FUNC, msg);
	y += fh;
		
	// Analog Down
	sprintf(msg, "%03d", st->joyy > 0 ? st->joyy : 0);
	DrawStringS(x+2*fw, y, hasAnalogY ? 1.0f : NON_FUNC, hasAnalogY ? 1.0f : NON_FUNC, hasAnalogY ? 1.0f : NON_FUNC, msg);
	
	// Rewind 2 lines towards the top
	y -= 2*fh;
	x += 8*fw;
	
	// Second Analog controller
	
	// Analog Up
	sprintf(msg, "%03d", st->joy2y < 0 ? -1*st->joy2y : 0);
	DrawStringS(x+2*fw, y, hasAnalog2Y ? 1.0f : NON_FUNC, hasAnalog2Y ? 1.0f : NON_FUNC, hasAnalog2Y ? 1.0f : NON_FUNC, msg);
	y += fh;
	
	// Analog Right & Left
	sprintf(msg, "%03d %03d", st->joy2x < 0 ? -1*st->joy2x : 0, st->joy2x > 0 ? st->joy2x : 0);
	DrawStringS(x, y, hasAnalog2X ? 1.0f : NON_FUNC, hasAnalog2X ? 1.0f : NON_FUNC, hasAnalog2X ? 1.0f : NON_FUNC, msg);
	y += fh;
	
	// Analog Down
	sprintf(msg, "%03d", st->joy2y > 0 ? st->joy2y : 0);
	DrawStringS(x+2*fw, y, hasAnalog2Y ? 1.0f : NON_FUNC, hasAnalog2Y ? 1.0f : NON_FUNC, hasAnalog2Y ? 1.0f : NON_FUNC, msg);
	
	//Do we have the next few ones?
	x = orig_x+fw;
	y += fh;
	
	
	// Adds existing but without known implementations...
	// Second d-pad and D button
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_UP ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_UP;
	DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "U");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_DOWN ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_DOWN;
	DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "D");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_LEFT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_LEFT;
	DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "L");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_RIGHT ? 1 : 0;
	isPressed = st->buttons & CONT_DPAD2_RIGHT;
	DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "R");
	
	hasFunc = dev->info.function_data[0] & CONT_FIXED_CAPABILITY_D ? 1 : 0;
	isPressed = st->buttons & CONT_D;
	DrawStringS(x+11*fw, y, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 1.0f : hasFunc ? 1.0f : NON_FUNC, isPressed ? 0.0f : hasFunc ? 1.0f : NON_FUNC, "D");
}

void GetControllerPorts(int *ports)
{
	int				ctrl = 0;
	maple_device_t	*dev = NULL;
	
	memset(ports, 0, sizeof(int)*4);
	for(ctrl = 0; ctrl < 4; ctrl++)
	{
		dev = maple_enum_type(ctrl, CONTROLLER_FUNCTIONS);
		if(dev)
			ports[dev->port] = 1;
	}
}

#define MAX_KBD_BUFF 81
#define MAX_KBD_SIZE 80
#define MAX_KBD_LAST 79
#define MAX_KBD_LINE 20

char kbd_buffer[4][MAX_KBD_BUFF];
int	 kbd_buffer_pos[4];

int MoveUp(char *str, int pos) 
{	
	int lfeed = 0;
	int curlen = 0, gpos = 0;
	
	while (*str) 
	{		
		if(*str == '\n')
		{
			lfeed ++;
			curlen = 0;
		}
		if(curlen == MAX_KBD_LINE)
			curlen = 0;
		if(gpos == pos)
		{
			if(!lfeed)
				return 0;
			return(gpos - curlen - 1);
				
		}
		str++;
		curlen++;
		gpos ++;
	}
	return 0;
}

int MoveDown(char *str, int pos)
{
	int lfeed = 0;
	int curlen = 0;
	
	str += pos;
	while (*str) 
	{
		if(*str == '\n')
		{
			lfeed ++;
			if(lfeed == 2)
				return(pos + curlen);
			curlen = 0;
		}
		if(curlen == MAX_KBD_LINE)
		{
			if(!lfeed)
				return 0;
			else
				return(pos + curlen);
		}
		str++;
		curlen++;
	}
	if(!lfeed)
		return 0;
	else
		return(pos + curlen);
}

void DiplayMouse(int port_num, float x, float y)
{
	maple_device_t	*mouse = NULL;
    mouse_state_t	*mstate;
    char			msg[256], name[256];
	int				isPressed = 0;
	
	mouse = maple_enum_type(port_num, MAPLE_FUNC_MOUSE);
	if(!mouse)
		return;
		
	mstate = (mouse_state_t *)maple_dev_status(mouse);
	if(!mstate)
		return;
		
	ReduceName(msg, mouse->info.product_name, 1);
	sprintf(name, "#G%c%c:#G %s", 'A'+(mouse->port), '0'+(mouse->unit), msg);
	DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, name);
	y += fh;
	
	sprintf(msg, "Power[%u-%u]mW", 
			mouse->info.standby_power, mouse->info.max_power);
	DrawStringS(x, y, 0.0f, 0.7f, 0.6f, msg);
	y += fh;
	
	// ID analog functions?
		
	x += 2*fw;
	// Move Up
	sprintf(msg, "%03d", mstate->dy < 0 ? -1*mstate->dy : 0);
	DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	y += fh;
	
	// Move Right & Left
	sprintf(msg, "%03d %03d", mstate->dx < 0 ? -1*mstate->dx : 0, mstate->dx > 0 ? mstate->dx : 0);
	DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
	y += fh;
		
	// Move Down
	sprintf(msg, "%03d", mstate->dy > 0 ? mstate->dy : 0);
	DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	
	// Reposition
	y -= 2*fh;
	x += 6*fw;
	// Wheel Up
	sprintf(msg, "%03d", mstate->dz < 0 ? -1*mstate->dz : 0);
	DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	y += 2*fh;
	
	// Wheel Down
	sprintf(msg, "%03d", mstate->dz > 0 ? mstate->dz : 0);
	DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	y += 2*fh;
	
	// mouse Buttons
	x -= 3*fw;
	isPressed = mstate->buttons & MOUSE_SIDEBUTTON ;
	DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, isPressed ? 1.0f : 1.0f , isPressed ? 0.0f : 1.0f, "S");
	
	isPressed = mstate->buttons & MOUSE_LEFTBUTTON ;
	DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, isPressed ? 1.0f : 1.0f , isPressed ? 0.0f : 1.0f, "L");
	
	isPressed = mstate->buttons & MOUSE_RIGHTBUTTON ;
	DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : 1.0f, isPressed ? 1.0f : 1.0f , isPressed ? 0.0f : 1.0f, "R");
}

void DiplayKBorMouse(int kb_num, float x, float y, int *hit_escape)
{
	int				rcv = 0, region = 0;
	maple_device_t	*dev = NULL;
	char			msg[256], name[256];
	static int		frame = 0, showcursor = 1;

	dev = maple_enum_type(kb_num, MAPLE_FUNC_KEYBOARD);
	if(!dev)
	{
		DiplayMouse(kb_num, x, y);
		return;
	}
	
	if(dev->info.area_code	== 0x01)
		region = KBD_REGION_US;
	else
		region = KBD_REGION_JP;
	ReduceName(msg, dev->info.product_name, 1);
	sprintf(name, "#G%c%c:#G %s", 'A'+(dev->port), '0'+(dev->unit), msg);
	DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, name);
	y += fh;
	
	sprintf(msg, "Power[%u-%u]mW", 
			dev->info.standby_power, dev->info.max_power);
	DrawStringS(x, y, 0.0f, 0.7f, 0.6f, msg);
	y += fh;
	
	rcv = kbd_queue_pop(dev, region);
	if(rcv != -1)
	{
		switch(rcv)
		{
			case 0x08:		// Backspace in ASCII
				{
					int del_pos = 0;
					
					if(kbd_buffer_pos[kb_num] > 0)
					{
						kbd_buffer_pos[kb_num]--;
						del_pos = kbd_buffer_pos[kb_num];
					}
					else
					{
						kbd_buffer_pos[kb_num] = MAX_KBD_LAST;
						del_pos = MAX_KBD_LAST;
					}
					kbd_buffer[kb_num][del_pos] = ' ';
				}
				break;
			case 0x0A:		// Enter in ASCII
				if(countLineFeeds(kbd_buffer[kb_num]) < 3)
				{
					kbd_buffer[kb_num][kbd_buffer_pos[kb_num]] = (char)rcv;
					kbd_buffer_pos[kb_num]++;
					if(kbd_buffer_pos[kb_num] == MAX_KBD_SIZE)
						kbd_buffer_pos[kb_num] = 0;
				}
				break;
			case 0x1B:		// ESCAPE
				if(hit_escape)
					*hit_escape = 1;
				break;
			case 0x4A00:	// HOME
				kbd_buffer_pos[kb_num] = 0;
				break;
			case 0x4C00:	// DEL
				memcpy(kbd_buffer[kb_num]+kbd_buffer_pos[kb_num], 
					kbd_buffer[kb_num]+kbd_buffer_pos[kb_num]+1,
					MAX_KBD_LAST - kbd_buffer_pos[kb_num]);
				kbd_buffer[kb_num][MAX_KBD_LAST] = ' ';
				break;
			case 0x4D00:	// END
				kbd_buffer_pos[kb_num] = MAX_KBD_LAST;
				break;
			case 0x4f00:	// RIGHT
				if(kbd_buffer_pos[kb_num] < MAX_KBD_LAST)
					kbd_buffer_pos[kb_num] ++;
				else
					kbd_buffer_pos[kb_num] = 0;
				break;
			case 0x5000:	// LEFT
				if(kbd_buffer_pos[kb_num] > 0)
					kbd_buffer_pos[kb_num] --;
				else
					kbd_buffer_pos[kb_num] = MAX_KBD_LAST;
				break;
			case 0x5100:	// DOWN
				{
					int move_down = 0;
					
					move_down = MoveDown(kbd_buffer[kb_num], kbd_buffer_pos[kb_num]);
					if(move_down)
						kbd_buffer_pos[kb_num] = move_down;
					else
					{
						kbd_buffer_pos[kb_num] += MAX_KBD_LINE;
						kbd_buffer_pos[kb_num] = kbd_buffer_pos[kb_num] % MAX_KBD_SIZE;
					}
				}
				break;
			case 0x5200:	// UP
				{
					int move_up = 0;
					
					move_up = MoveUp(kbd_buffer[kb_num], kbd_buffer_pos[kb_num]);
					if(move_up)
						kbd_buffer_pos[kb_num] = move_up;
					else
					{
						kbd_buffer_pos[kb_num] -= MAX_KBD_LINE;
						if(kbd_buffer_pos[kb_num] < 0)
							kbd_buffer_pos[kb_num] += MAX_KBD_SIZE;
					}
				}
				break;
			default:
				// ASCII
				if(rcv >= 0x20 && rcv <= 0x7E)
				{
					kbd_buffer[kb_num][kbd_buffer_pos[kb_num]] = (char)rcv;
					kbd_buffer_pos[kb_num]++;
					if(kbd_buffer_pos[kb_num] == MAX_KBD_SIZE)
						kbd_buffer_pos[kb_num] = 0;
				}
				break;
		}
		
		showcursor = 1;
	}
	else
	{
		frame++;
		if(frame == 20)
		{
			showcursor = !showcursor;
			frame = 0;
		}
	}
	DrawStringSKB(x, y, 1.0f, 1.0f, 1.0f, kbd_buffer[kb_num], 
				MAX_KBD_LINE, showcursor ? kbd_buffer_pos[kb_num] : -1);
}

void GetKBMPorts(int *ports)
{
	int				kbm = 0;
	maple_device_t	*dev = NULL;
	
	memset(ports, 0, sizeof(int)*4);
	for(kbm = 0; kbm < 4; kbm++)
	{
		dev = maple_enum_type(kbm, MAPLE_FUNC_KEYBOARD);
		if(dev)
			ports[dev->port] = 1;
			
		dev = maple_enum_type(kbm, MAPLE_FUNC_MOUSE);
		if(dev)
			ports[dev->port] = 1;
	}
}

void ControllerTest()
{
	uint16			pressed = 0;
	int 			done = 0, oldvmode = -1, i = 0;
	int				ignoreFunctions = 0, timeout = 0;
	int				ports[4], kbm_ports[4];
	ImagePtr		back = NULL, black = NULL;
	maple_device_t	*dev = NULL;
	cont_state_t 	*st = NULL;

	black = LoadIMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
		
	back = LoadIMG("/rd/ControlBack.kmg.gz", 0);
	if(!back)
		return;
	
	disableSleep();
	
	memset(kbd_buffer, ' ', sizeof(char)*4*MAX_KBD_BUFF);
	for(i = 0; i < 4; i++)
		kbd_buffer[i][MAX_KBD_SIZE] = '\0';
	memset(kbd_buffer_pos, 0, 4*sizeof(int));

	while(!done && !EndProgram) 
	{				
		int		num_controller = 0, num_keyb = 0, hit_escape = 0;
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
		GetKBMPorts(kbm_ports);
		
		// IgnoreFunctions lists all inputs regardless
		// of functions reported
		if(!ignoreFunctions)
		{
			DrawStringSCentered(16, 0.0f, 1.0f, 0.0f, "Controller Test"); 
			
			if(ports[0])
				DiplayController(num_controller++, x, y);
			if(ports[1])
				DiplayController(num_controller++, x+w, y);
			if(ports[2])
				DiplayController(num_controller++, x,y+h);
			if(ports[3])
				DiplayController(num_controller  , x+w, y+h);
				
			DrawStringSCentered(y+2*h+fh, 1.0f, 1.0f, 0.0f, "[Right&Start on Controller 1 to ignore reported functions]"); 
		}
		else
		{
			DrawStringSCentered(16, 0.0f, 1.0f, 0.0f, "Controller Test #Y(non-reported functions in gray)#Y");
			if(ports[0])
				DiplayControllerIgnore(num_controller++, x, y);
			if(ports[1])
				DiplayControllerIgnore(num_controller++, x+w, y);
			if(ports[2])
				DiplayControllerIgnore(num_controller++, x, y+h);
			if(ports[3])
				DiplayControllerIgnore(num_controller  , x+w, y+h);
			DrawStringSCentered(y+2*h+fh, 1.0f, 1.0f, 0.0f, "[Right&Start on Controller 1 for reported functions only]"); 
		}
		
		// Keyboards if available
		if(kbm_ports[0])
			DiplayKBorMouse(num_keyb++, x, y, &hit_escape);
		else if(!ports[0])
			DrawStringS(x-4*fw, y, 1.0f, 1.0f, 0.0f, "#GA0:#G #C(Empty)#C");
		if(kbm_ports[1])
			DiplayKBorMouse(num_keyb++, x+w, y, NULL);
		else if(!ports[1])
			DrawStringS(x+w-4*fw, y, 1.0f, 1.0f, 0.0f, "#GB0:#G #C(Empty)#C");
		if(kbm_ports[2])
			DiplayKBorMouse(num_keyb++, x,y+h, NULL);
		else if(!ports[2])
			DrawStringS(x-4*fw, y+h, 1.0f, 1.0f, 0.0f, "#GC0:#G #C(Empty)#C");
		if(kbm_ports[3])
			DiplayKBorMouse(num_keyb  , x+w, y+h, NULL);
		else if(!ports[3])
			DrawStringS(x+w-4*fw, y+h, 1.0f, 1.0f, 0.0f, "#GD0:#G #C(Empty)#C");
						
		DrawStringS(110, y+2*h-fh, 1.0f, 1.0f, 0.0f, "[Up&Start for Help]"); 
		DrawStringS(70, y+2*h, 1.0f, 1.0f, 0.0f, "[Left&Start on Controller 1 to Exit}"); 
		
        EndScene();
		if(timeout)
			timeout--;
			
		VMURefresh("Controller", ignoreFunctions ? "Ignore" : "Normal");
		
		// we do it without helper functions here
		dev = maple_enum_type(0, CONTROLLER_FUNCTIONS);
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
		
		// Got Escape on Keyboard and it is controller 1
		if(hit_escape)
			done = 1;
	}
	// we clear controller buffer for exit
	ReadController(0, &pressed);
	
	enableSleep();
	
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

#define	DISPLAY_BUFFER_CHAR	8192
#define BUFFER_PRINT_SIZE	4096

char			*display_buffer = NULL;
char			buffer_printf_buffer[BUFFER_PRINT_SIZE];
int				display_buffer_pos = 0;
int				maple_locked_device = 0;
int				maple_use_reply_bytes = 0;
int				maple_useCache = 1;
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

void buffer_printf(char *fmt, ... )
{
	int		len = 0;
	va_list arguments;

	if(!display_buffer)
		return;

	//memset(buffer_printf_buffer, 0, BUFFER_PRINT_SIZE*sizeof(char));
	
	va_start(arguments, fmt);
	vsprintf(buffer_printf_buffer, fmt, arguments);
	va_end(arguments);
	
	len = strlen(buffer_printf_buffer);
	if(display_buffer_pos + len + 1 < DISPLAY_BUFFER_CHAR)
	{
		memcpy(display_buffer+display_buffer_pos, buffer_printf_buffer, sizeof(char)*len);
		display_buffer_pos += len;
		
#ifdef DCLOAD	
		PrintCleanString(buffer_printf_buffer);
#endif
	}
	else
		dbglog(DBG_ERROR, "Display text buffer was too short, needed extra %d for:\n%s\n",
			display_buffer_pos + len - DISPLAY_BUFFER_CHAR, buffer_printf_buffer); 
}

/*
 * Code below is from upgraded dcutils by Donald Haase.
 * taken from https://dcemulation.org/phpBB/viewtopic.php?t=97047
 * slightly adapted formating for PVR display
 */
 
#define GRABALL 2
/* Local copy of the returned buffer for maple stuff */
unsigned char 	recv_buff[1024 + 32];

static void vbl_allinfo_callback(maple_state_t *st, maple_frame_t *frm) {
	maple_response_t	*resp;
	(void)st;

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
				buffer_printf("\n Device asked for retry.\n Please try again. Got #YMAPLE_RESPONSE_AGAIN#Y"); 
				break;
			case MAPLE_RESPONSE_BADCMD:
				buffer_printf("\n #CDevice doesn't support command.#C\n Maybe an emulator?\n Got #YMAPLE_RESPONSE_BADCMD#Y"); 
				break;
			case MAPLE_RESPONSE_BADFUNC:
				buffer_printf("\n #CDevice doesn't support command.#C\n Got #YMAPLE_RESPONSE_BADFUNC#Y"); 
				break;
			case MAPLE_RESPONSE_NONE:
				buffer_printf("\n #CDevice didn't respond to query command.#C\n Got #YMAPLE_RESPONSE_NONE#Y"); 
				break;
			case MAPLE_COMMAND_ALLINFO:
				buffer_printf("\n #CDevice doesn't support command.#C\n Maybe an emulator?\n Got #YMAPLE_COMMAND_ALLINFO#Y"); 
				break;
			case MAPLE_RESPONSE_DEVINFO:
				buffer_printf("\n #CDevice doesn't support command.#C\n Got #YMAPLE_RESPONSE_DEVINFO#Y"); 
				break;
			case MAPLE_COMMAND_RESET:
				buffer_printf("\n Device asked for reset.\n Please reconnect. Got #YMAPLE_COMMAND_RESET#Y"); 
				break;
			case MAPLE_RESPONSE_OK:
				buffer_printf("\n Unexpected #YMAPLE_RESPONSE_OK#Y from device", resp->response); 
				break;
			case MAPLE_RESPONSE_DATATRF:
				buffer_printf("\n Unexpected #YMAPLE_RESPONSE_DATATRF#Y from device", resp->response); 
				break;
			default:
				buffer_printf("\n Unexpected response [#Y%d#Y] from device", resp->response); 
				break;
		}
		if(resp->response != MAPLE_RESPONSE_NONE)
		{
			buffer_printf(" [%u bytes response]", resp->data_len*4);
			if(maple_use_reply_bytes)
				resp_size = resp->data_len*4;
			else
				resp_size = 1024 + 32;
				
			if(resp_size)
				memcpy(recv_buff, resp, resp_size);
		}
		
		buffer_printf("\n");
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
		buffer_printf("\nmaple_frame_lock() failed");
		if(dev)
			buffer_printf(" for #Y%c%c#Y", 'A'+(dev->port), '0'+(dev->unit));
		buffer_printf("\n\n Please try reconnecting the device\n");
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
	buffer_printf("#CFunctions     :#C 0x%.8lx  ", info->functions);
	buffer_printf("#CFunction int 0:#C 0x%.8lx\n", info->function_data[0]);
	buffer_printf("#CFunction int 1:#C 0x%.8lx  ", info->function_data[1]);
	buffer_printf("#CFunction int 2:#C 0x%.8lx\n", info->function_data[2]);
	buffer_printf("#CRegion:#C         0x%.2x        ", info->area_code);
	buffer_printf("#CConnection:#C     0x%.2x\n", info->connector_direction);
	buffer_printf("#CProduct Name & License:#C %.30s\n", info->product_name);
	buffer_printf("  %.60s\n", info->product_license);
	buffer_printf("#CStandby power:#C 0x%.4x (%umW) ", info->standby_power, info->standby_power);
	buffer_printf("#CMax:#C 0x%.4x (%umW)\n", info->max_power, info->max_power);
}

/* Formatted output for the devinfo struct in KOS */
void FillKOSDevInfo(maple_device_t *dev, int skipline)
{
	buffer_printf("%s#YValues from MAPLE_RESPONSE_DEVINFO#Y:\n", skipline ? "\n" : "");
	buffer_printf("#CFunctions     :#C 0x%.8lx  ", dev->info.functions);
	buffer_printf("#CFunction int 0:#C 0x%.8lx\n", dev->info.function_data[0]);
	buffer_printf("#CFunction int 1:#C 0x%.8lx  ", dev->info.function_data[1]);
	buffer_printf("#CFunction int 2:#C 0x%.8lx\n", dev->info.function_data[2]);
	buffer_printf("#CRegion:#C         0x%.2x        ", dev->info.area_code);
	buffer_printf("#CConnection:#C     0x%.2x\n", dev->info.connector_direction);
	buffer_printf("#CProduct Name & License:#C %.30s\n", dev->info.product_name);
	buffer_printf("  %.60s\n", dev->info.product_license);
	buffer_printf("#CStandby power:#C 0x%.4x (%umW) ", dev->info.standby_power, dev->info.standby_power);
	buffer_printf("#CMax:#C 0x%.4x (%umW)\n", dev->info.max_power, dev->info.max_power);
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
		buffer_printf("Requested info for device #Y%c%c#Y ", 'A'+(dev->port), '0'+(dev->unit));	
	else
		buffer_printf("#YValues from MAPLE_RESPONSE_ALLINFO#Y for #Y%c%c#Y ", 'A'+(dev->port), '0'+(dev->unit));	
		
#ifdef BENCHMARK
	start = timer_us_gettime64();
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
	end = timer_us_gettime64();
	dbglog(DBG_INFO, "\n>> vbl_send_allinfo took %g ms <<\n", (double)(end - start)/1000.0);
#endif
	
	size += (recv_buff[3]*4);
	
	if(!maple_locked_device)
		return;
		
	if(size-4 <= 0)
		return;
	
	buffer_printf("[%d bytes of data]:\n", size-4);
	print_devinfo((maple_devinfo_t *)&recv_buff[4]);

	if(extra)
	{
#if (GRABALL == 1)
		size = 1024+32;
#elif (GRABALL == 2)
		/* Grab a bit extra, cause some devices are stupid, and give more than they tell */
		size = ((size+64)<(1024+32))?(size+64):(1024+32);
#endif

		buffer_printf("#CExtra data:#C\n");
		/* Print the hex for the data followed by ascii */
		for(i=4+112; i< size; i+=BPL) 
		{	
			buffer_printf("#G%.3x#G$#G|#G$", i-(4+112));
			for(j=0;j<BPL;j++)	buffer_printf("%.2x$", recv_buff[i+j]);		
			buffer_printf("#G|#G$");
			
			for(j=0;j<BPL;j++)	buffer_printf("%c"   , isprint(recv_buff[i+j])?recv_buff[i+j]:' ');
			buffer_printf("\n");
		}	
		buffer_printf("#CEnd of Extra data#C\n");
	}
}

/*
 * End code copy from Donald Haase.
 */
void print_device_kos_cache(maple_device_t *dev)
{
	if(!dev)
		return;
	//buffer_printf("#G--------------------------------------------------------------#G\n");
	buffer_printf("\n");
	FillKOSDevInfo(dev, 0);
}

void cleanDisplayBuffer()
{
	display_buffer_pos = 0;
	memset(display_buffer, 0, sizeof(char)*DISPLAY_BUFFER_CHAR);
}

void DisplayDreameyeImage(maple_device_t *dev)
{
	char	msg[100];
	
	if(!dev)
		return;
		
	if(!(dev->info.functions & MAPLE_FUNC_CAMERA))
		return;
	
	if(dreameye_get_image_count(dev, 1) == MAPLE_EOK)
	{
		dreameye_state_t 	*state = NULL;
		int 				num = 0;
#ifdef DREAMEYE_DISP
		uint8 				*buf = NULL;
		int 				size = 0;
#endif
	
		state = (dreameye_state_t *)maple_dev_status(dev);
		if(!state->image_count_valid)
		{
			sprintf(msg, "Dreameye at %c-%c has an invalid image count", 
					'A'+(dev->port), '0'+(dev->unit));
			DrawMessage(msg);
			return;
		}

		num = state->image_count;
#ifndef DREAMEYE_DISP
		if(num)
			sprintf(msg, "Dreameye at %c-%c has %d image%c", 
				'A'+(dev->port), '0'+(dev->unit), num, num > 1 ? 's' : ' ');
		else
			sprintf(msg, "Dreameye at %c-%c has no images", 
				'A'+(dev->port), '0'+(dev->unit));
		DrawMessage(msg);
#else
		/* Disabled since libjpeg crashes with the Dreameye images:  */
		/* "Corrupt JPEG data: premature end of data segment"        */
		if(num == 0)
		{
			sprintf(msg, "Dreameye at %c-%c has no images", 
				'A'+(dev->port), '0'+(dev->unit));
			DrawMessage(msg);
			return;
		}

		sprintf(msg, "Accessing Dreameye at %c-%c", 
			'A'+(dev->port), '0'+(dev->unit));
		DrawMessageOnce(msg);
		if(dreameye_get_image(dev, 2, &buf, &size)	== MAPLE_EOK)
		{
			char fn[100];
			
			sprintf(fn, "de-img%03d.jpg", 0);
			if(fs_ramdisk_attach(fn, buf, size) == 0)
			{
				int 		done = 0;
				ImagePtr 	img = NULL;
				controller	*st = NULL;
				uint16		pressed;
				
				sprintf(fn, "/ram/de-img%03d.jpg", 0);
				img = LoadIMG(fn, 0);						
				if(img)
				{
					do
					{
						StartScene();
						DrawImage(img);
						EndScene();
						
						st = ReadController(0, &pressed);
						if(st)
						{
							if (pressed & CONT_B)
								done =	1;
						}
					}while(!done);
					
					FreeImage(&img);
				}
				if(fs_ramdisk_detach(fn, (void**)&buf, (size_t*)&size) != 0)
					dbglog(DBG_ERROR, "Could not dettach %s from /ram", fn);
			}
			else
			{
				sprintf(msg, "Could not attach image from Dreameye at %c-%c", 
					'A'+(dev->port), '0'+(dev->unit));
				DrawMessage(msg);
			}

			free(buf);
			buf = NULL;
			size = 0;
		}
		else
		{
			sprintf(msg, "Could not load the image from Dreameye at %c-%c", 
				'A'+(dev->port), '0'+(dev->unit));
			DrawMessage(msg);
		}
#endif
	}
}

void ListMapleDevices()
{
	int 			done = 0, sel = 1, oldvmode = -1, c = -1;
	int				v_scroll = 0, joycnt = 0;
	uint16			pressed;		
	ImagePtr		back = NULL, black = NULL;
	controller		*st = NULL;

	if(display_buffer)
	{
		dbglog(DBG_ERROR, "Maple display text buffer was not NULL"); 
		return;
	}
	
	black = LoadIMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
		
	back = LoadIMG("/rd/maple.kmg.gz", 0);
	if(!back)
		return;
	
	display_buffer = (char*)malloc(sizeof(char)*DISPLAY_BUFFER_CHAR);
	if(!display_buffer)
	{
		dbglog(DBG_ERROR, "Out of memory for maple display text buffer\n"); 
		return;
	}
	
	disableSleep();

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
		if(!maple_useCache)
			DrawStringSCentered(210+v_scroll+fh, 0.0f, 1.0f, 0.0f, "Omit short response"); 
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
						refreshVMU = 1;
						cleanDisplayBuffer();
						print_device_allinfo(dev, 1);
						if(maple_useCache)
							print_device_kos_cache(dev);
						lines = countLineFeeds(display_buffer);
						while(!close_display && !EndProgram) 
						{
							StartScene();
							DrawImage(black);
							DrawImage(back);
							DrawString(5+hscroll, 20+sub_v_scroll, 1.0f, 1.0f, 1.0f, display_buffer);
							EndScene();
							
							VMURefresh("Maple", vmudata);
							
							st = ReadController(0, &pressed);
							JoystickDirections(st, &pressed, &joycntx, &joycnty);
							if(st)
							{
								if ( pressed & CONT_LTRIGGER || pressed & CONT_DPAD_UP )
									sub_v_scroll += fw;
								if ( pressed & CONT_RTRIGGER || pressed & CONT_DPAD_DOWN )
									sub_v_scroll -= fw;
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
			
			// Bonus device functions
			if (sel > 0 &&
					(pressed & CONT_DPAD_RIGHT ||
					pressed & CONT_DPAD_LEFT))
			{
				maple_device_t	*dev = NULL;

				dev = maple_enum_type(sel - 1, 0xffffffff);
				if(dev)
				{
					char	dev_msg[256];
					
					if(dev->info.functions & MAPLE_FUNC_PURUPURU)
					{
						int		useMotor1 = 0;
						
						useMotor1 = pressed & CONT_DPAD_RIGHT;
						sprintf(dev_msg, "Activating #YJump Pack#Y at %c%c (Motor %d)...",
								'A'+(dev->port), '0'+(dev->unit),
								useMotor1 ? 1 : 2);
						DrawMessageOnce(dev_msg);
						if(rumble_puru(dev, useMotor1))
							timer_spin_sleep(500);
						else
							DrawMessage("Rumble effect failed");
						stop_puru(dev);
					}
					
					if(dev->info.functions & MAPLE_FUNC_CLOCK)
					{
						if(pressed & CONT_DPAD_RIGHT)
						{
							sprintf(dev_msg, "Beeping #YVMU#Y at %c%c...", 
								'A'+(dev->port), '0'+(dev->unit));
							DrawMessageOnce(dev_msg);
							vmu_beep_raw(dev, 0x65f0);
							timer_spin_sleep(800);
							vmu_beep_raw(dev, 0);
						}
						else
						{
							sprintf(dev_msg, "Drawing to #YVMU#Y at %c%c...", 
								'A'+(dev->port), '0'+(dev->unit));
							DrawMessageOnce(dev_msg);
							updateVMU_SD_Dev(dev);
							timer_spin_sleep(1000);
							clearVMUGraphicDev(dev);  // it may not be the only one
							refreshVMU = 1;
						}
					}
					
					if(dev->info.functions & MAPLE_FUNC_CAMERA)
						DisplayDreameyeImage(dev);
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
	
	enableSleep();
	
	FreeImage(&back);
	FreeImage(&black);
	if(display_buffer)
	{
		free(display_buffer);
		display_buffer = NULL;
		display_buffer_pos = 0;
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

	start = timer_us_gettime64();
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
	end = timer_us_gettime64();
	dbglog(DBG_INFO, "check_for_bad_lcd() took %g ms\n", (double)(end - start)/1000.0);
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

typedef struct bios_data {
	int			type;
    uint32_t	crc;
    char		*name;
	char		*text;
} BIOSID;

#define	BIOS_SEGA	1
#define	BIOS_HACK	2

BIOSID bioslist[] = {
{ 	BIOS_SEGA, 
	0x5454841f, 
	"mpr-21068.ic501",
	"v1.004 (Japan)" },	// oldest known mass production version, supports Japan region only
{ 	BIOS_SEGA,
	0x2f551bc5, 
	"mpr-21871.ic501", 
	"v1.01c (World)" },
{ 	BIOS_SEGA, 
	0x89f2b1a1,
	"mpr-21931.ic501",
	"v1.01d (World)" },
{ 	BIOS_SEGA, 
	0x786168f9,
	"mpr-23588.ic501",
	"v1.022 (World)" },
{ 	BIOS_SEGA, 
	0xdcb2e86f, 
	"set5v0.976.ic507",
	"Katana Set5 v0.976 (Japan)" },
{ 	BIOS_SEGA,
	0x5702d38f,
	"set5v1.001.ic507",
	"Katana Set5 v1.001 (Japan)" },
{ 	BIOS_SEGA,
	0x2186e0e5,
	"set5v1.011.ic507",
	"Katana Set5 v1.011 (World)" },
{ 	BIOS_SEGA,
	0x52d01969,
	"set5v0.71.bin",
	"Katana Set5 Checker v0.71" },
{ 	BIOS_SEGA,
	0x485877bd,
	"set5v0.41.bin",
	"Katana Set5 Checker v0.41" },
// Dreamshell
{ 	BIOS_HACK,
	0xD1523DC3,
	"boot_loader_devkit.bios",
	"Dreamshell Devkit" },
{ 	BIOS_HACK,
	0xB3EA8EDF,
	"boot_loader_devkit_nogdrom.bios",
	"Dreamshell No GDROM" },
{ 	BIOS_HACK,
	0xDD1CF514,
	"boot_loader_retail.bios",
	"Dreamshell Retail" },
{ 	BIOS_HACK,
	0xBFA44608,
	"boot_loader_retail_nogdrom.bios",
	"Dreamshell Retail No GDROM" },
{ 	BIOS_HACK,
	0xA18BD6EA,
	"sd_loader_with.bios",
	"Dreamshell SD Loader" },
// Region Free
{ 	BIOS_HACK,
	0x65DE7BCB,
	"Link83_bios_1.0.bios",
	"v1.00 Link83 Region Free" },
{ 	BIOS_HACK,
	0x558F456E,
	"dc_bios_regionfree.bin",
	"v1.01d Region Free" }, 
{ 	BIOS_HACK,
	0xA65736C2,
	"dreamboot.bios",
	"DreamBoot v1.1.0" },
{ 	BIOS_HACK,
	0xEC7B10A1,
	"dreamboot-nogdrom.bios",
	"DreamBoot No GDROM v1.1.0" }, 
{ 	BIOS_HACK,
	0x8A0388E1,
	"dreamboot_1.0.2.bios",
	"DreamBoot v1.0.2" },
{ 	BIOS_HACK,
	0x6B8D1C5F,
	"dreamboot-nogdrom_1.0.2.bios",
	"DreamBoot No GDROM v1.0.2" }, 
{ 	BIOS_HACK,
	0x4DC1A805,
	"dreamboot_1.0.1.bios",
	"DreamBoot v1.0.1" },
{ 	BIOS_HACK,
	0x7FD6AB51,
	"dreamboot-nogdrom_1.0.1.bios",
	"DreamBoot No GDROM v1.0.1" }, 
{ 	BIOS_HACK,
	0x2991537A,
	"dreamboot_1.0.0.bios",
	"DreamBoot v1.0.0" },
{ 	BIOS_HACK,
	0x2E3FD8A9,
	"dreamboot-nogdrom_1.0.0.bios",
	"DreamBoot No GDROM v1.0.0" }, 
{ 	BIOS_HACK,
	0xDC9D84E0,
	"jc-bootROM-devkit-v1.031.bin",
	"JP Cake bootROM v1.031 devkit" },
{ 	BIOS_HACK,
	0x1249EE59,
	"jc-bootROM-retail-v1.031.bin",
	"JP Cake bootROM v1.031 retail" },
{ 	BIOS_HACK,
	0xA06F68D0,
	"jc-bootROM-devkit-v1.032.bin",
	"JP Cake bootROM v1.032 devkit" },
{ 	BIOS_HACK,
	0xF3DB5F40,
	"jc-bootROM-retail-v1.032.bin",
	"JP Cake bootROM v1.032 retail" },
// Other Hacks
{ 	BIOS_HACK,
	0x4B35A953,
	"mpr-21931-hack.ic501",
	"v1.01d (World/hack)" },
{ 	BIOS_HACK,
	0xA2564FAD,
	"dc101d_ch.bin",
	"v1.01d (Chinese hack)" },
{ 	BIOS_HACK,
	0x544C8685,
	"1_011_01_hacked.bios",
	"v1.011 Hack" },
{ 	BIOS_HACK,				// flycast HLE BIOS
	0x419b986d,
	"",
	"HLE BIOS" },
{ 	0, 0, NULL, NULL } }; 

// search known Original BIOS
BIOSID *GetBIOSNamebyCRC(uint32_t checksum)
{
	int i = 0;
	
	while(bioslist[i].crc != 0)
	{		
		if(checksum == bioslist[i].crc)
			return &bioslist[i];
		i++;
	}
	return NULL;
}

void CopyBIOSVersion(char *bios_version)
{
	memcpy(bios_version, (void*)0x07CC, sizeof(char)*5);
	bios_version[6] = '\0';
}

void printflashromdata(int ver)
{
	if(ver == 1)
	{
		unsigned int compare = 0, b = 0;
		uint8 *bytes = NULL;
		
		bytes = (uint8*)&flash_cache.factory_records[ver];
		for(b = 0; b < (int)sizeof(struct factory_record); b++)
		{
			if(bytes[b] == 0xFF)
				compare++;
		}
		if(compare == sizeof(struct factory_record))
		{
			buffer_printf("#GFlashrom backup copy is empty#G (0xFF)\n");
			return;
		}
		if(compare >= sizeof(struct factory_record)/2)
		{
			buffer_printf("#GFlashrom backup copy has too many 0xFF values.#G\n");
			buffer_printf("#GDisplaying as best as possible:#G\n");
		}
	}
	
	buffer_printf("#YMachine Code:#Y     (%c%c) %s\n", 
		protectascii(flash_cache.factory_records[ver].machine_code1),
		protectascii(flash_cache.factory_records[ver].machine_code2),
		flash_cache.factory_records[ver].machine_code1 == flash_cache.factory_records[ver].machine_code2 &&
		flash_cache.factory_records[ver].machine_code1 == '0' ? "Retail" : "Development Box");
	buffer_printf("#YCountry Code:#Y     (%c)  %s\n", 
		protectascii(flash_cache.factory_records[ver].country_code), get_flash_region_str(ver));
	buffer_printf("#YLanguage:#Y         (%c)  %s\n", 
		protectascii(flash_cache.factory_records[ver].language), get_flash_language_str(ver));
	buffer_printf("#YBroadcast Format:#Y (%c)  %s\n", 
		protectascii(flash_cache.factory_records[ver].broadcast_format), get_flash_broadcast_str(ver));
	buffer_printf("#YMachine Name:#Y     %s\n", 
		p_flash_cache.factory_records[ver].machine_name);
	buffer_printf("#YTool Number:#Y      %s\n", 
		p_flash_cache.factory_records[ver].tool_number);
	buffer_printf("#YTool Version:#Y     %s\n", 
		p_flash_cache.factory_records[ver].tool_version);
	buffer_printf("#YTool Type:#Y        %s\n", 
		p_flash_cache.factory_records[ver].tool_type);
	buffer_printf("#YDate:#Y             %s/%s/%s %s:%s\n", 
		p_flash_cache.factory_records[ver].year,
		p_flash_cache.factory_records[ver].month,
		p_flash_cache.factory_records[ver].day,
		p_flash_cache.factory_records[ver].hour,
		p_flash_cache.factory_records[ver].min);
	buffer_printf("#YSerial Number:#Y    %s\n", 
		p_flash_cache.factory_records[ver].serial_number);
	buffer_printf("#YFactory Code:#Y     %s\n", 
		p_flash_cache.factory_records[ver].factory_code);
	buffer_printf("#YTotal Number:#Y     %s\n", 
		p_flash_cache.factory_records[ver].total_number);
	buffer_printf("#YID:#Y               %02X:%02X:%02X:%02X:%02X:%02X\n", 
		flash_cache.factory_records[ver].machine_id.id[0],
		flash_cache.factory_records[ver].machine_id.id[1],
		flash_cache.factory_records[ver].machine_id.id[2],
		flash_cache.factory_records[ver].machine_id.id[3],
		flash_cache.factory_records[ver].machine_id.id[4],
		flash_cache.factory_records[ver].machine_id.id[5]	);
	buffer_printf("#YMachine Type:#Y     0x%02X\n", 
		flash_cache.factory_records[ver].machine_type);
	buffer_printf("#YMachine Version:#Y  0x%02X\n", 
		flash_cache.factory_records[ver].machine_version);
	buffer_printf("#YVersion:#Y          0x%02X\n", 
		flash_cache.unk_version);
}

void printflashromp2data(int ver)
{
	unsigned int compare = 0, b = 0;
	uint8 *bytes = NULL;
	
	bytes = (uint8*)&cid_record.cid[ver];
	for(b = 0; b < (int)sizeof(struct cid_data); b++)
	{
		if(bytes[b] == 0xFF)
			compare++;
	}
	if(compare == sizeof(struct cid_data))
	{
		buffer_printf("#GCID record %d is empty#G (0xFF)\n", ver);
		return;
	}
	buffer_printf("#GCID record %d:#G\n", ver);
	if(compare >= sizeof(struct cid_data)/2)
	{
		buffer_printf("#GCID record %d has too many 0xFF values.#G\n", ver);
		buffer_printf("#GDisplaying as best as possible:#G\n");
	}
	
	buffer_printf("#YRecord Type:#Y      %d\n", 
		cid_record.record_type);
	buffer_printf("#YDate:#Y             %02X%02X/%02X/%02X\n", 
		cid_record.cid[ver].date[0],
		cid_record.cid[ver].date[1],
		cid_record.cid[ver].date[2],
		cid_record.cid[ver].date[3]);
	buffer_printf("#YT Inferior Code:#Y  %c%c%c%c\n", 
		protectascii(cid_record.cid[ver].t_inferior_code[0]),
		protectascii(cid_record.cid[ver].t_inferior_code[1]),
		protectascii(cid_record.cid[ver].t_inferior_code[2]),
		protectascii(cid_record.cid[ver].t_inferior_code[3]));
	buffer_printf("#YRepair voucher:#Y   %c%c%c%c%c%c%c%c\n", 
		protectascii(cid_record.cid[ver].repair_voucher_no[0]),
		protectascii(cid_record.cid[ver].repair_voucher_no[1]),
		protectascii(cid_record.cid[ver].repair_voucher_no[2]),
		protectascii(cid_record.cid[ver].repair_voucher_no[3]),
		protectascii(cid_record.cid[ver].repair_voucher_no[4]),
		protectascii(cid_record.cid[ver].repair_voucher_no[5]),
		protectascii(cid_record.cid[ver].repair_voucher_no[6]),
		protectascii(cid_record.cid[ver].repair_voucher_no[7]));
	buffer_printf("#YSerial Number:#Y    %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", 
		cid_record.cid[ver].serial_no[0],
		cid_record.cid[ver].serial_no[1],
		cid_record.cid[ver].serial_no[2],
		cid_record.cid[ver].serial_no[3],
		cid_record.cid[ver].serial_no[4],
		cid_record.cid[ver].serial_no[5],
		cid_record.cid[ver].serial_no[6],
		cid_record.cid[ver].serial_no[7]);	
	buffer_printf("#YFactory Code:#Y     0x%X\n", 
		cid_record.cid[ver].factory_code);
	buffer_printf("#YOrder No:#Y         %02X:%02X:%02X:%02X:%02X\n", 
		cid_record.cid[ver].order_no[0],
		cid_record.cid[ver].order_no[1],
		cid_record.cid[ver].order_no[2],
		cid_record.cid[ver].order_no[3],
		cid_record.cid[ver].order_no[4]);	
	buffer_printf("#YCRC16:#Y            0x%X\n", 
		cid_record.crc16);
}

void ShowBIOSandFlash()
{
	int 			done = 0, oldvmode = -1, version = 0;
	int				joycnty = 0, joycntx = 0, showcid = 0;
	int				v_scroll = 0, h_scroll = 0;
	int				lines = 0, fillbuffer = 1, bios_lines = 0;
	int				sr_lines = 0, tot_lines = 0;
	uint16			pressed;		
	ImagePtr		back = NULL;
	controller		*st = NULL;
	uint32			crc = 0;
	char			bios_buffer[512], bios_version[8];
	BIOSID 			*bios = NULL;

	if(flashrom_get_factory_data() != FLASHROM_ERR_NONE)
	{
		DrawMessage("Could not get flashrom data");
		return;
	}
	
	updateVMU_wait();
	CopyBIOSVersion(bios_version);
	DrawMessageOnce("Please wait, calculating #YBIOS#Y CRC\n#G(about 1 second)#G");
	
	crc = CalculateCRC(0x0, 0x200000);
	bios = GetBIOSNamebyCRC(crc);
	if(bios)
		sprintf(bios_buffer, "#Y%s      %s\n#YBIOS version:#Y     %s\n#YIC:#Y               %s\n#YCRC:#Y              0x%08" PRIx32, 
			bios->type == BIOS_SEGA ? "BIOS:#Y       " : "Custom BIOS:#Y",
			bios->text,	bios_version, bios->name, crc);
	else
		sprintf(bios_buffer, "#YUnknown BIOS:#Y     #CPlease Report or dump it.#C\n#YBIOS version:#Y     %s\n#YCRC:#Y              0x%08" PRIx32, bios_version, crc);
		
	bios_lines = countLineFeeds(bios_buffer);
	
	if(display_buffer)
	{
		dbglog(DBG_ERROR, "Display text buffer was not NULL"); 
		return;
	}
	
	back = LoadIMG("/rd/biosback.kmg.gz", 0);
	if(!back)
		return;
	
	display_buffer = (char*)malloc(sizeof(char)*DISPLAY_BUFFER_CHAR);
	if(!display_buffer)
	{
		dbglog(DBG_ERROR, "Out of memory for display text buffer\n"); 
		return;
	}

	while(!done && !EndProgram) 
	{
		float y = 20+v_scroll;
		
		if(oldvmode != vmode)
			oldvmode = vmode;
		
		if(fillbuffer)
		{	
			cleanDisplayBuffer();

			printflashromdata(version);
			if(showcid)
				printflashromp2data(version);
			
			lines = countLineFeeds(display_buffer);
			sr_lines = countLineFeeds(p_flash_cache.staff_roll);
			tot_lines = lines + sr_lines;
			
			if(lines + sr_lines == 0)
				buffer_printf("#Yflashrom#Y data is empty.");
			if(sr_lines)
				tot_lines ++;	// staff roll description
			fillbuffer = 0;
		}

		StartScene();
		DrawImage(back);
		DrawStringSCentered(y, 0.0f, 1.0f, 0.0f, "BIOS data");
		y += 2*fh;
		DrawString(20+h_scroll, y, 1.0f, 1.0f, 1.0f, bios_buffer);
		y += (bios_lines+2)*fh;
		if(!version)
			DrawStringSCentered(y, 0.0f, 1.0f, 0.0f, "Flashrom data (main)");
		else
			DrawStringSCentered(y, 0.0f, 1.0f, 0.0f, "Flashrom data (backup)");
		y += 2*fh;
		DrawString(20+h_scroll, y, 1.0f, 1.0f, 1.0f, display_buffer);
		y += lines*fh;
		if(sr_lines)
		{
			DrawString(20+h_scroll, y, 1.0f, 1.0f, 1.0f, "#YStaff Roll:#Y");
			y += fh;
			DrawString(20+4*fw+h_scroll, y, 1.0f, 1.0f, 1.0f, p_flash_cache.staff_roll);
		}
		EndScene();

		VMURefresh("Flashrom", "Data");

		st = ReadController(0, &pressed);
		JoystickDirections(st, &pressed, &joycntx, &joycnty);
		if(st)
		{
			if ( pressed & CONT_LTRIGGER || pressed & CONT_DPAD_UP )
				v_scroll += fw;
			if ( pressed & CONT_RTRIGGER || pressed & CONT_DPAD_DOWN )
				v_scroll -= fw;
			if(v_scroll > 0)
				v_scroll = 0;
			if(v_scroll < -1*tot_lines*fh)
				v_scroll = -1*tot_lines*fh;
			
			if ( pressed & CONT_DPAD_LEFT )
				h_scroll -= fw;
			if ( pressed & CONT_DPAD_RIGHT )
				h_scroll += fw;
			if(h_scroll > 8*fw)
				h_scroll = 8*fw;
			if(h_scroll < -20*fw)
				h_scroll = -20*fw;
				
			if (pressed & CONT_Y)
			{
				showcid = !showcid;
				fillbuffer = 1;
			}
				
			if (pressed & CONT_X)
			{
				version = !version;
				fillbuffer = 1;
			}

			
			if (pressed & CONT_B)
				done = 1;

			if (pressed & CONT_START)
				ShowMenu(BIOSHELP);
		}
	}
	FreeImage(&back);
	if(display_buffer)
	{
		free(display_buffer);
		display_buffer = NULL;
		display_buffer_pos = 0;
	}
	return;
}

#define VISIBLE_HORZ	16
#define VISIBLE_VERT	26
#define MAX_LOCATIONS	17
#define MEM_VIEWER_BUFF	20

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

	back = LoadIMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
		
	if(address == 0)
		address = locations[0];
	while(!done && !EndProgram) 
	{
		int 	i = 0, j = 0, pos = -1;
		uint8_t *mem = NULL;
		char 	buffer[MEM_VIEWER_BUFF];
		
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
					
					memset(buffer, 0, sizeof(char)*MEM_VIEWER_BUFF);

					c = mem[i*VISIBLE_HORZ*factor+j];
					if(c >= 32 && c <= 126)	
						buffer[0] = (char)c;			// ASCII range
					else
						buffer[0] = (char)32;			// Space
				}
				DrawStringNH(3*j*fw, i*fh, 1.0f, 1.0f, 1.0f, buffer);
			}
		}
        EndScene();
		
		VMURefresh("MemView", "");
		st = ReadController(0, &pressed);
		
		JoystickDirections(st, &pressed, &joycntx, &joycnty);
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
	int		saveexists = 0, port = 0, unit = 0;
	char 	error[256], question[256], vmugame[20],
			*msg = "VMU data could not be saved\n";
	
	if(!isVMUPresent())
		return;
		
	if(MemcardOtherGameExists(VMU_CTRL_NAME, vmugame, &port, &unit))
	{
		sprintf(question, "#GVMU Control Test & MDFourier#G\n\nThere is a game (#Y%s#Y) in #YVMU %c-%c#Y\nPlease insert a #YVMU#Y without a game,\nor delete this one first.", 
				vmugame, 'A'+port, '0'+unit);
		DrawMessage(question);
		return;
	}
	
	saveexists = MemcardSaveExists(VMU_CTRL_NAME, NULL, &port, &unit);
	if(saveexists == VMU_SAVE_EXISTS)
	{
		sprintf(question, "#GVMU Control Test & MDFourier#G\n\nReplace the #YVMU Test#Y stored in #YVMU#Y at port #Y%c-%c#Y?", 'A'+port, '0'+unit);
		if(!AskQuestion(question))
			return;
	}
	
	if(!saveexists)
	{
		sprintf(question, "#GVMU Control Test & MDFourier#G\n\nThis will save the application to #YVMU %c-%c#Y\nProceed with save?", 'A'+port, '0'+unit);
		if(!AskQuestion(question))
			return;
	}
	
	if(check_for_bad_lcd())
	{
		sprintf(question, "#YVMU %c-%c#Y reports an LCD but seems to not have one\nWrite test anyway?", 'A'+port, '0'+unit);
		if(!AskQuestion(question))
			return;
	}
	
	sprintf(question, "Please don't remove #YVMU %c-%c#Y while saving...", 'A'+port, '0'+unit);
	sprintf(error, "%s", msg);
	DrawMessageOnce(question);
	if(WriteMemCardControlTest(error+strlen(msg)) != VMU_OK)
		DrawMessage(error);
	else
	{
		sprintf(question, "The software has been saved to #YVMU %c-%c#Y\nPlease dettach the unit and run it from the #YLCD#Y.", 'A'+port, '0'+unit);
		DrawMessage(question);
	}
}

void amplify_samples()
{
	double			ratio = 0;
	unsigned int 	i = 0, size = 0, max = 0;
	int16			*samples = NULL, current = 0;
#ifdef BENCHMARK
	uint64	start, end;

	start = timer_us_gettime64();
#endif					
	samples = (int16*)stream_samples;
	size = rec_buffer.pos/2;
	for(i = 0; i < size; i++)
	{
		current = abs(samples[i]);
		if((unsigned int)current > max)
			max = current;
	}
	
	if(max)
	{
		ratio = 32767.0/(double)max;
		for(i = 0; i < size; i++)
			samples[i] *= ratio;
	}
#ifdef BENCHMARK
	end = timer_us_gettime64();
	dbglog(DBG_INFO, "PCM amplify took %g ms\n", (double)(end - start)/1000.0);
#endif
}

#define cleanSIPtest() \
	if(back) FreeImage(&back);\
	CleanRecordBuffer();\
	CleanStreamSamples();

#define MAX_SECONDS_RECORD	30
void MicrophoneTest()
{
	int					done = 0, samplerate = 0, play = 0, amplify = 1, sip_change = 0;
	int					sampling = 0, tries = 0, state_machine = 0, sr8 = 0;
	int					frame_counter = 0, seconds_record = 5, seconds_in_buffer = 0;
	int					total_frames = 0;
	size_t				recoding_size = 0;
	uint16				pressed;
	controller			*st;
	maple_device_t		*sip = NULL; 
	ImagePtr			back = NULL;
	snd_stream_hnd_t 	hnd = SND_STREAM_INVALID;
	char				*vmuMsg1 = "Mic Test", *vmuMsg2 = "", msg[128], sr[40];
	fmenudata 			resmenudata[] = { {1, "11khz"}, {2, "8khz"} };
	
	if(!isSIPPresent())
		return;
	
	sr8 = SelectMenu("Select samplerate", resmenudata, 2, sr8 + 1);
	if(sr8 == MENU_CANCEL)
		return;
	sr8 -= 1;
	
	do
	{
		sip_change = 0;
		
		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
		if(!sip)
			return;
			
		back = LoadIMG("/rd/back.kmg.gz", 0);
		if(!back)
		{
			cleanSIPtest();
			return;
		}
		
		if(sip->info.function_data[0] == 0x0f000000)		// Version 1.000, 2000/02/24,315-618
			samplerate = sr8 ? 8085 : 11025;
		if(sip->info.function_data[0] == 0x3f000000)
			samplerate = sr8 ? 8000 : 10909;
		if(!samplerate)
		{
			if(!AskQuestion("Your #CMicrophone#C is an unidentified model.\nPlease #Yreport#Y the #CMaple device list#C results\nin order to support and document it.\n#YProceed with test?#Y"))
			{
				cleanSIPtest();
				return;
			}
			if(AskQuestion("Use NTSC Microphone values?"))
				samplerate = sr8 ? 8085 : 11025;
			else
				samplerate = sr8 ? 8000 : 10909;
		}
		
		memset(&rec_buffer, 0, sizeof(struct sip_recording_st));
		
		recoding_size = samplerate*2*(MAX_SECONDS_RECORD+1);
		rec_buffer.buffer = (uint8*)malloc(sizeof(uint8)*recoding_size);
		if(!rec_buffer.buffer)
		{
			cleanSIPtest();
			return;
		}
		rec_buffer.size = sizeof(uint8)*recoding_size;
		rec_buffer.pos = 0;
		rec_buffer.overflow = 0;
		rec_buffer.recording = 0;
			
		if(sip_set_gain(sip, SIP_MAX_GAIN) != MAPLE_EOK) // SIP_DEFAULT_GAIN  
		{
			DrawMessage("Could not set Microphone gain");
			cleanSIPtest();
			return;
		}
		if(sip_set_sample_type(sip, SIP_SAMPLE_16BIT_SIGNED) != MAPLE_EOK)
		{
			DrawMessage("Could not set Microphone sample type");
			cleanSIPtest();
			return;
		}
		if(sip_set_frequency(sip, sr8 ? SIP_SAMPLE_8KHZ : SIP_SAMPLE_11KHZ) != MAPLE_EOK) // 8000/8085 or 10909/11025Hz
		{
			DrawMessage("Could not set Microphone sample frequency");
			cleanSIPtest();
			return;
		}
		
		tries = 0;
		do
		{
			int sipretval = 0;
			
			sipretval = sip_start_sampling(sip, sip_copy, 1);
			if(sipretval == MAPLE_EOK)
				sampling = 1;
#ifdef DC_LOAD
			if(sipretval == MAPLE_ETIMEOUT)
				dbglog(DBG_INFO, "Microphone timed out\n");
			if(sipretval == MAPLE_EAGAIN)
				dbglog(DBG_INFO, "Microphone asked for retry\n");
#endif
			if(sipretval == MAPLE_EFAIL)
			{
				DrawMessage("Microphone Recording Failed, already sampling\nTry reconnecting it");
				cleanSIPtest();
				return;
			}
			tries ++;
		}while(!sampling && tries < 10);
		
		if(!sampling)
		{
			DrawMessage("Microphone Recording Failed");
			cleanSIPtest();
			return;
		}
		
		CleanStreamSamples();
		hnd = snd_stream_alloc(sound_callback, SND_STREAM_BUFFER_MAX);
		if(hnd == SND_STREAM_INVALID)
		{
			cleanSIPtest();
			DrawMessage("Could not get SPU stream");
			return;
		}
		stream_samples_size = recoding_size; // 16 bit samples
		stream_samples = (char*)malloc(sizeof(char)*stream_samples_size);
		if(!stream_samples) 
		{
			DrawMessage("Out of Memory");
			cleanSIPtest();
			return;
		}
		
		stream_samplerate = samplerate;
		snd_stream_volume(hnd, 255);
		
		sprintf(sr, "#CSample Rate:#C%dhz", samplerate);
		while(!done && !EndProgram && !sip_change) 
		{
			sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
			if(!sip)
			{
				DrawMessage("#YSIP Microphone#Y disconnected");
				done = 1;
			}
			
			StartScene();
			DrawImage(back);
			DrawStringSCentered(80, 0.0f, 1.0f, 0.0f, "Microphone Test");
			DrawStringS(40, 200, 1.0f, 1.0f, 1.0f, sr);
			
			if(state_machine != 1)
			{
				if(!play)
					DrawStringSCentered(90, 1.0f, 1.0f, 1.0f, "Press #CX#C to record");
				else
					DrawStringSCentered(90, 1.0f, 1.0f, 0.0f, "Please wait...");
			}
			
			if(state_machine == 1)
			{
				sprintf(msg, "Recording please wait... [%02d:%02d]",
					(int)(frame_counter / (IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE)),
					frame_counter % (IsPAL ? 50 : 60));
				DrawStringSCentered(90, 1.0f, 1.0f, 0.0f, msg);
			}

			if(rec_buffer.pos != 0 && state_machine != 1)
			{
				if(!play)
					DrawStringSCentered(90+fh, 1.0f, 1.0f, 1.0f, "Press #CA#C to play");
				else
				{
					sprintf(msg, "Playing... [%02d:%02d]",
						(int)(frame_counter / (IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE)),
						frame_counter % (IsPAL ? 50 : 60));
					DrawStringSCentered(90+fh, 1.0f, 1.0f, 0.0f, msg);
				}
			}
			
			sprintf(msg, "#CSeconds to record:#C %.2d", seconds_record);
			DrawStringSCentered(90+5*fh, 1.0f, 1.0f, 1.0f, msg);	
			sprintf(msg, "#CAmplify playback:#C %s", amplify ? " ON" : "OFF");
			DrawStringSCentered(90+6*fh, 1.0f, 1.0f, 1.0f, msg);
			
			EndScene();
			
			VMURefresh(vmuMsg1, vmuMsg2);
			
			if(frame_counter)
			{
				frame_counter--;
				if(state_machine == 1 && rec_buffer.pos == 0 &&
					frame_counter < total_frames/10)
				{
					rec_buffer.recording = 0;
					state_machine = 0;
					frame_counter = 0;
					total_frames = 0;
					
					vmuMsg1 = "Reinsert";
					vmuMsg2 = "micro";
					refreshVMU = 1;
					done = 2;
				}
				
				if(play && frame_counter == 0)
					snd_stream_stop(hnd);
				
				if(play)
					snd_stream_poll(hnd);
				
				if(play && frame_counter == 0)
					play = 0;
			}
			
			st = ReadController(0, &pressed);
			if(st) 
			{
				/*
				if(state_machine == 1)				// Allow cancel with X?
				{
					if (pressed & CONT_X)
						frame_counter = 1;
				}*/
				
				if(state_machine != 1 && !play)		// these are not allowed while sampling
				{
					if (pressed & CONT_START)
					{
						snd_stream_stop(hnd);
						stream_pos = stream_samples_size;
						ShowMenu(MICTESTHELP);
					}

					if (pressed & CONT_X)
					{
						memset(rec_buffer.buffer, 0, sizeof(uint8)*recoding_size);
						
						state_machine = 1;
						frame_counter = seconds_record*(IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE);
						total_frames = frame_counter;
						rec_buffer.overflow = 0;
						rec_buffer.pos = 0;
						rec_buffer.recording = 1;
					}
					
					if (pressed & CONT_DPAD_DOWN)
						seconds_record -= 1;
						
					if (pressed & CONT_DPAD_UP)
						seconds_record += 1;

					if (pressed & CONT_RTRIGGER)
					{
						int sr8t = 0;
						
						sr8t = SelectMenu("Select samplerate", resmenudata, 2, sr8 + 1);
						if(sr8t != MENU_CANCEL)
						{
							sr8 = sr8t - 1;
							sip_change = 1;
						}
					}
					
					if(seconds_record < 1)
						seconds_record = 1;
					if(seconds_record > MAX_SECONDS_RECORD)
						seconds_record = MAX_SECONDS_RECORD;
				}
				
				if(state_machine == 2)
				{
					if(pressed & CONT_A && !play)
					{
						if(!rec_buffer.pos)
							DrawMessage("Microphone recording is empty.");
						else
						{
							memset(stream_samples, 0, sizeof(char)*stream_samples_size);
							memcpy(stream_samples, rec_buffer.buffer, sizeof(uint8)*rec_buffer.pos);
							
							if(amplify)
								amplify_samples();

							frame_counter = seconds_in_buffer*(IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE);
							total_frames = frame_counter;
							stream_pos = 0;
							play = 1;
							
							snd_stream_start(hnd, stream_samplerate, 0);
						}
					}
				}
				
				if(pressed & CONT_Y)
					amplify = !amplify;
					
				if(pressed & CONT_B)
					done = 1;
			}
			
			if(state_machine == 1 && frame_counter == 0)
			{	
				state_machine = 2;
				rec_buffer.recording = 0;
				seconds_in_buffer = seconds_record;
#ifdef DCLOAD
				dbglog(DBG_INFO, "Recording stopped, got %d bytes (%d 16 bit samples)\n", 
						(int)rec_buffer.pos, (int)(rec_buffer.pos/2));
#endif
			}
				
		}
		
		snd_stream_stop(hnd);
		snd_stream_destroy(hnd);
		
		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
		if(sip && sampling)
		{
			int sipretval = 0, retries = 0;
			
			tries = 0;
			do
			{	
				sipretval = sip_stop_sampling(sip, 1);
				if(sipretval == MAPLE_EAGAIN)
				{
					retries	++;
					timer_spin_sleep(10);
				}
				else if(sipretval != MAPLE_EOK)
					dbglog(DBG_ERROR, "Got %d from sip_stop_sampling\n", sipretval);

				tries++;
			}while(sipretval == MAPLE_EAGAIN && tries < 100);
			
			if(retries)
				dbglog(DBG_ERROR, "Got MAPLE_EAGAIN %d times from sip_stop_sampling\n", retries);
			if(sipretval != MAPLE_EOK && done != 2)
			{
				dbglog(DBG_ERROR, "Microphone recording could not be stopped\n");
				done = 2;
			}
		}
		
		if(done == 2)
			DrawMessage("#YPlease remove and reinsert Microphone#Y");

		cleanSIPtest();

	}while(!done && !EndProgram);
	
	return;
}

/*
	
	========================================================================
	Official LightGun                                   HKT-7800 315-6125-AG
	========================================================================
	Functions     : 0x81000000  Function int 0: 0x00000000
	Function int 1: 0xfe000000  Function int 2: 0x00000000
	Region:         0xff        Connection:     0x01
	Product Name & License: Dreamcast Gun
	  Produced By or Under License From SEGA ENTERPRISES,LTD.
	Standby power: 0x00dc (220mW) Max: 0x012c (300mW)
	Extra data:
	000 | 56 65 72 73 69 6f 6e 20 31 2e 30 30 30 2c 31 39 | Version 1.000,19
	010 | 39 38 2f 30 39 2f 31 36 2c 33 31 35 2d 36 31 32 | 98/09/16,315-612
	020 | 35 2d 41 47 20 20 20 2c 55 2c 44 2c 4c 2c 52 2c | 5-AG   ,U,D,L,R,
	030 | 53 2c 41 2c 42 20 4b 65 79 20 26 20 53 63 61 6e | S,A,B Key & Scan
	040 | 6e 69 6e 67 20 4c 69 6e 65 20 41 6d 70 2e 20 20 | ning Line Amp.
	050 | 53 2c 41 2c 53 2c 41 2c 53 2c 41 2c 53 2c 41 2c | S,A,S,A,S,A,S,A,
	060 | 53 2c 41 2c 53 2c 41 2c 53 2c 41 2c 00 00 00 00 | S,A,S,A,S,A,
	070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	End of Extra data
	
	========================================================================
	InterAct Starfire LightGun
	========================================================================
	Functions     : 0x81000000  Function int 0: 0x00000000
	Function int 1: 0xfe000000  Function int 2: 0x00000000
	Region:         0x01        Connection:     0x01
	Product Name & License: Dreamcast Gun
	  Produced By or Under License Fro� SEGA ENTERPRISES,LTD.
	Standby power: 0x00dc (220mW) Max: 0x012c (300mW)

	========================================================================
	Dream Blaster MadCatz LightGun
	========================================================================
	Functions     : 0x81000000  Function int 0: 0x00000000
	Function int 1: 0xfe000000  Function int 2: 0x00000000
	Region:         0x01        Connection:     0x01
	Product Name & License: Dreamcast Gun
	  Produced By or Under License From SEGA ENTERPRISES,LTD.+
	Standby power: 0x00a0 (160mW) Max: 0x00fa (250mW)
	Extra data:
	000 | 6d 20 53 45 6d 20 53 45 6d 20 53 45 00 00 00 00 | m SEm SEm SE
	010 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	020 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	030 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	End of Extra data
	
	Official Gun measured range was:
	x: 200-860
	y:  20-320
	
	InterAct measured range was:
	x: 225-855
	y:  20-320
	
	MadCatz measured range was:
	x: 256-860
	y:  20-305
*/

int isLightGunPresent()
{
	maple_device_t	*lightgun = NULL;

	lightgun = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
	if(!lightgun)
		return 0;
	return 1;
}

typedef struct coord_st{
	int x;
	int y;
	int a;
} coord;

#define	LG_TG_COOL		6
#define LG_FRAME_READ	2
#define NUM_HOLES		400
#define MAX_REMAIN		15
#define OB_TIME			4
#define LG_TIMEOUT		15

#define START_MIN_X		1024
#define START_MIN_Y		1024
#define START_MAX_X		0

// min_x must be between 195 and 300
// max_x must be between 830 and 870
// min_y must be between  15 and  30

#define IS_MIN_X_BAD	(min_x > 300)
#define IS_MAX_X_BAD	(max_x < 830)
#define IS_MIN_Y_BAD	(min_y >  30)

#define MIN_X_ACCEPT	(min_x < 350)
#define MAX_X_ACCEPT	(max_x > 780)
#define MIN_Y_ACCEPT	(min_y <  60)

inline void set_color(ImagePtr t, float r, float g, float b)
{
	t->r = r;
	t->g = g;
	t->b = b;
}

void LightGunTest()
{
	int 			done = 0, lg_state = 0, polling_active = 0, cal_timeout = 0, blink = 0;
	int				trigger_cool = 0, calibrating = 1, h = 0, remain = MAX_REMAIN;
	int				x, y, oldx, oldy, values_ok = 1, trigger = 0, oldvmode = -1;
	int				min_x = START_MIN_X, min_y = START_MIN_Y, max_x = START_MAX_X, factor = 1;
	int				arrow_fr_cnt = 0, ob_timeout = 0, show_cal = 0;
	uint16			pressed, OldButtonsLG = 0, frame = 0, curr_hole = 0;
	ImagePtr		white = NULL, cross = NULL, hole = NULL, arrow = NULL;
	controller		*st = NULL;
	maple_device_t	*dev = NULL;
	coord			holes[NUM_HOLES], arrows[3];
	char 			msg[100];
	sfxhnd_t		hit, rld, miss, click;
	uint64			start_cal, curr_cal;

	if(!isLightGunPresent())
		return;

	white = LoadIMG("/rd/white.kmg.gz", 1);
	if(!white)
		return;
	cross = LoadIMG("/rd/cross.kmg.gz", 0);
	if(!cross)
		return;
	hole = LoadIMG("/rd/hole.kmg.gz", 0);
	if(!hole)
		return;
	arrow = LoadIMG("/rd/arrow.kmg.gz", 0);
	if(!arrow)
		return;
		
	hit = snd_sfx_load("/rd/hit.wav");
	if(hit == SFXHND_INVALID)
		return;
	rld = snd_sfx_load("/rd/rld.wav");
	if(rld == SFXHND_INVALID)
		return;
	miss = snd_sfx_load("/rd/miss.wav");
	if(miss == SFXHND_INVALID)
		return;
	click = snd_sfx_load("/rd/click.wav");
	if(click == SFXHND_INVALID)
		return;

	disableSleep();
	
	maple_gun_read_pos(&x, &y);
	oldx = x;
	oldy = y;
	
	for(h = 0; h < NUM_HOLES; h++)
	{
		holes[h].x = 800;
		holes[h].y = 600;
		holes[h].a = 0;
	}
	
	arrows[0].x = 112;
	arrows[0].y = 120;
	
	arrows[1].x = 208;
	arrows[1].y = 120;
	
	arrows[2].x = 160;
	arrows[2].y = 100;
	
	
	arrows[0].a = 90;		// left
	arrows[1].a = -90;		// right
	arrows[2].a = 0;		// up
	
	arrow->r = 0.0f;
	arrow->g = 1.0f;
	arrow->b = 0.0f;
	
	sprintf(msg, "Out of bouds");
	start_cal = timer_us_gettime64();
	while(!done && !EndProgram) 
	{	
		if(oldvmode != vmode)
		{
			if(vmode < HIGH_RES)
				factor = 1;
			else
				factor = 2;
			white->w = dW;
			white->h = dH;
			oldvmode = vmode;
		}
		
		if(polling_active)
		{
			if(dev)
			{
				timer_spin_sleep(10);
				maple_gun_enable(dev->port);
			}
		}
		
		StartScene();
		DrawImage(white);
		if(!calibrating)
		{
			DrawImage(cross);
			for(h = 0; h < NUM_HOLES; h++)
			{
				if(holes[h].x != 800)
				{
					hole->x = holes[h].x;
					hole->y = holes[h].y;
					DrawImageRotate(hole, holes[h].a);
				}
			}
			if(!remain )
			{
				if(blink % 30)
					DrawStringBCenteredFull(111*factor, 1.0f, 1.0f, 1.0f, "RELOAD!");
				blink++;
			}
			if(cal_timeout)
				DrawStringBCenteredFull(30*factor, 1.0f, 1.0f, 0.0f, "Sub-optimal calibration values");
		}
		else
		{	
			if(values_ok)
				DrawStringBCenteredFull(100*factor, 1.0f, 1.00f, 0.0f, "Calibration Done");
			else
			{
				arrow_fr_cnt++;
				if(IS_MIN_X_BAD)
				{
					if(arrow_fr_cnt == 10)
					{
						arrows[0].x -= 24;
						if(arrows[0].x < -8)
							arrows[0].x = 112;
					}
					arrow->x = factor*arrows[0].x - arrow->tw/2;
					arrow->y = factor*arrows[0].y - arrow->th/2;
					if(MIN_X_ACCEPT)
						set_color(arrow, 1, 1, 0);
					else
						set_color(arrow, 0, 1, 0);
					DrawImageRotate(arrow, arrows[0].a);
				}
				
				if(IS_MAX_X_BAD)
				{
					if(arrow_fr_cnt == 10)
					{
						arrows[1].x += 24;
						if(arrows[1].x > 328)
							arrows[1].x = 208;
					}
					arrow->x = factor*arrows[1].x - arrow->tw/2;
					arrow->y = factor*arrows[1].y - arrow->th/2;
					if(MAX_X_ACCEPT)
						set_color(arrow, 1, 1, 0);
					else
						set_color(arrow, 0, 1, 0);
					DrawImageRotate(arrow, arrows[1].a);
				}
				
				if(IS_MIN_Y_BAD)
				{
					if(arrow_fr_cnt == 10)
					{
						arrows[2].y -= 22;
						if(arrows[2].y < -10)
							arrows[2].y = 100;
					}
					if(MIN_Y_ACCEPT)
						set_color(arrow, 1, 1, 0);
					else
						set_color(arrow, 0, 1, 0);
					arrow->x = factor*arrows[2].x - arrow->tw/2;
					arrow->y = factor*arrows[2].y - arrow->th/2;
					DrawImageRotate(arrow, arrows[2].a);
				}
				
				if(arrow_fr_cnt == 10)
					arrow_fr_cnt = 0;
			}
			DrawStringBCenteredFull(111*factor, 1.0f, 1.00f, 1.0f, msg);
			
			if(show_cal)
			{
				char 	data[100], col;
				int 	py = 140;
				
				col = IS_MIN_X_BAD ? 'O' : 'G';
				sprintf(data, "#%cmin X:#%c %03d", col, col, min_x);
				DrawStringBCenteredFull(py*factor, 1.0f, 1.00f, 1.0f, data);
				py+= fh;
				
				col = IS_MAX_X_BAD ? 'O' : 'G';
				sprintf(data, "#%cmax X:#%c %03d", col, col, max_x);
				DrawStringBCenteredFull(py*factor, 1.0f, 1.00f, 1.0f, data);
				py+= fh;
				
				col = IS_MIN_Y_BAD ? 'O' : 'G';
				sprintf(data, "#%cmin Y:#%c %03d", col, col, min_y);
				DrawStringBCenteredFull(py*factor, 1.0f, 1.00f, 1.0f, data);
			}
		}
		
		if(calibrating)
		{	
			values_ok = 1;
			if(IS_MIN_X_BAD)
				values_ok = 0;
			if(IS_MAX_X_BAD)
				values_ok = 0;
			if(IS_MIN_Y_BAD)
				values_ok = 0;

			if(values_ok)	
			{
				if(isLightGun)
					DrawStringBCenteredFull(210*factor, 1.0f, 1.0f, 1.0f, "Press #Ytrigger#Y to continue");
				else
					DrawStringBCenteredFull(210*factor, 1.0f, 1.0f, 1.0f, "Press #Ytrigger#Y or #YA#Y in Ctrl 1 to continu");
			}
			else
			{
				if(cal_timeout)
					DrawStringBCenteredFull(200*factor, 1.0f, 1.0f, 1.0f, "You can test these partial values with #Ytrigger#Y");
				DrawStringBCenteredFull(210*factor, 1.0f, 1.0f, 1.0f, "Point slowly towards the #Yscreen borders#Y");
			}
		}
		EndScene();
		
		/* Allow a time out if values are not perfect, but at least tried */
		curr_cal = timer_us_gettime64();
		if(calibrating && 
			(double)(curr_cal - start_cal)/1000000.0 > LG_TIMEOUT &&
			MIN_X_ACCEPT && MIN_Y_ACCEPT && MAX_X_ACCEPT)
		{
			cal_timeout = !values_ok;
		}
		
		if(trigger_cool)
			trigger_cool --;
		
		if(polling_active)
		{
			if(!calibrating)
			{
				cross->x = 800;
				cross->y = 600;
			}
			
			if(dev)
			{
				timer_spin_sleep(10);
				maple_gun_read_pos(&x, &y);
				if(oldx != x || oldy != y)
				{
					polling_active = 0;
					if(!frame)	// immediate response only
					{						
						if(calibrating)
						{
							int c_x, c_y;
							
							/*
							A value lower than 200 means it was read 
							delayed in the next line
							*/
							c_x = x;
							c_y = y;
							
							if(x > max_x) max_x = x;
							if(x > 190 && x < min_x) min_x = x;
							if(y < min_y) min_y = y;
							
							if(values_ok)
							{
								if(c_x < min_x)
									c_x += max_x;
								c_x = (c_x - min_x)/(factor == 2 ? 1 : 2);
								c_y -= min_y;
								c_y *= factor;
								
								sprintf(msg, "#Y[#YX: %03d Y: %03d#Y]#Y", c_x, c_y);
							}
							else
								sprintf(msg, " X: %03d Y: %03d ", x, y);
						}
						else
						{
							int c_x, c_y;
							
							c_x = x;
							c_y = y;
							if(c_x < min_x)
								c_x += max_x;
							c_x = (c_x - min_x)/(factor == 2 ? 1 : 2);
							c_y -= min_y;
							c_y *= factor;
							
							cross->x = c_x - cross->tw/2;
							cross->y = c_y - cross->th/2;
							
							if(trigger)
							{
								if(remain)
								{
									holes[curr_hole].x = c_x - hole->tw/2;
									holes[curr_hole].y = c_y - hole->th/2;
									holes[curr_hole].a = rand() % 360;
									curr_hole++;
									if(curr_hole >= NUM_HOLES)
										curr_hole = 0;
									remain --;
									if(hit != SFXHND_INVALID)
										snd_sfx_play(hit, 255, is_system_mono ? 128 : 256*c_x/320);
								}
								else
								{
									if(miss != SFXHND_INVALID)
										snd_sfx_play(click, 255, 128);
								}
								trigger = 0;
							}
						}
					}
					oldx = x; 
					oldy = y;
				}
				else
				{
					// Shots outside don't draw, but reload
					if(trigger)
					{
						ob_timeout++;
						if(ob_timeout > OB_TIME)
						{
							ob_timeout = 0;
							trigger = 0;
							
							if(remain == 0)
							{
								remain = MAX_REMAIN;
								if(rld != SFXHND_INVALID)
									snd_sfx_play(rld, 255, 128);
								trigger_cool = 3*LG_TG_COOL;
							}
							else
							{
								remain --;
								if(miss != SFXHND_INVALID)
									snd_sfx_play(miss, 255, 128);
							}
						}
					}
				}
			}
			lg_state++;
		}
		
		frame++;
		if(!polling_active)
		{
			polling_active = 1;
			frame = 0;
			lg_state = 0;
			timer_spin_sleep(10);
		}
		
		VMURefresh("Light Gun", "");

		dev = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
		if(dev)
		{
			cont_state_t	*state;
			
            if((state = (cont_state_t *)maple_dev_status(dev)))
			{
				uint16 	lgpressed;
				
				lgpressed = state->buttons & ~OldButtonsLG & state->buttons;
				OldButtonsLG = state->buttons;
				
				if(lgpressed & CONT_B)
				{
					if(isLightGun && calibrating)
					{
						done = 1;
						ReadController(0, &pressed);
					}
					
					if(!calibrating)
					{
						calibrating = 1;
						cal_timeout = 0;
						start_cal = timer_us_gettime64();
					}
				}

				if(lgpressed & CONT_A && !trigger_cool)
				{
					if(!calibrating)
						trigger = 1;
					else
					{
						if(values_ok || cal_timeout)
							calibrating = !calibrating;
					}
					
					trigger_cool = LG_TG_COOL;
				}
				
				
				if(lgpressed & CONT_START)
				{
					if(!isLightGun)
						ShowHelpWindow(LG_HELP);
					else
						ShowMenu(LG_HELP);
					OldButtonsLG |= CONT_B;
				}
				
				if(lgpressed & CONT_DPAD_UP ||
					lgpressed & CONT_DPAD_LEFT)
					show_cal = !show_cal;
			}
        }
		
		st = ReadController(0, &pressed);
		if(st && !isLightGun)
		{
			if (pressed & CONT_DPAD_UP)
				show_cal = !show_cal;		
				
			if (pressed & CONT_B)
				done =	1;						
			
			if (pressed & CONT_A)
			{
				if(values_ok)
					calibrating = !calibrating;
			}

			if (pressed & CONT_START)
				ShowMenu(LG_HELP);
		}
	}
	
	enableSleep();
	FreeImage(&hole);
	FreeImage(&white);
	FreeImage(&cross);
	FreeImage(&arrow);
	if(hit != SFXHND_INVALID)
		snd_sfx_unload(hit);
	if(rld != SFXHND_INVALID)
		snd_sfx_unload(rld);
	if(miss != SFXHND_INVALID)
		snd_sfx_unload(miss);
	if(click != SFXHND_INVALID)
		snd_sfx_unload(click);
	return;
}

void print_ispcfg(flashrom_ispcfg_t *flashrom_isp, int ignoreflags)
{
	int i = 0;
	
	buffer_printf("#GISP Type:#G             ");
	switch(flashrom_isp->method)
	{
		case FLASHROM_ISP_DHCP:
				buffer_printf("DHCP-based ethernet\n");
				break;
		case FLASHROM_ISP_STATIC:
				buffer_printf("Static IP-based ethernet\n");
				break;
		case FLASHROM_ISP_DIALUP:
				buffer_printf("Dialup ISP\n");
				break;
		case FLASHROM_ISP_PPPOE:
				buffer_printf("PPPoE-based ethernet\n");
				break;
		default:
				buffer_printf("#YUnknown#Y (%d)\n", flashrom_isp->method);
				break;
	}
	
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_IP)
	{
		buffer_printf("#GStatic IP address:#G    ");
		for(i = 0; i < 4; i++)
			buffer_printf("%03u%c", flashrom_isp->ip[i], i != 3 ? '.': '\n');
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_NETMASK )
	{
		buffer_printf("#GNetmask:#G              ");
		for(i = 0; i < 4; i++)
			buffer_printf("%03u%c", flashrom_isp->nm[i], i != 3 ? '.': '\n');
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_BROADCAST)
	{
		buffer_printf("#GBroadcast address:#G    ");
		for(i = 0; i < 4; i++)
			buffer_printf("%03u%c", flashrom_isp->bc[i], i != 3 ? '.': '\n');
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_GATEWAY)
	{
		buffer_printf("#GGateway address:#G      ");
		for(i = 0; i < 4; i++)
			buffer_printf("%03u%c", flashrom_isp->gw[i], i != 3 ? '.': '\n');
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_DNS)
	{
		buffer_printf("#GDNS server 1:#G         ");
		for(i = 0; i < 4; i++)
			buffer_printf("%03u%c", flashrom_isp->dns[0][i], i != 3 ? '.': '\n');
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_DNS)
	{
		buffer_printf("#GDNS server 2:#G         ");
		for(i = 0; i < 4; i++)
			buffer_printf("%03u%c", flashrom_isp->dns[1][i], i != 3 ? '.': '\n');
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_HOSTNAME)
		buffer_printf("#GHostname:#G             %s\n", flashrom_isp->hostname);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_EMAIL)
		buffer_printf("#GEmail address:#G        %s\n", flashrom_isp->email);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_SMTP)
		buffer_printf("#GSMTP server:#G          %s\n", flashrom_isp->smtp);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_POP3)
		buffer_printf("#GPOP3 server:#G          %s\n", flashrom_isp->pop3);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_POP3_USER)
		buffer_printf("#GPOP3 username:#G        %s\n", flashrom_isp->pop3_login);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_POP3_PASS)
		buffer_printf("#GPOP3 password:#G        %s\n", flashrom_isp->pop3_passwd);
	if(ignoreflags || (flashrom_isp->valid_fields & FLASHROM_ISP_PROXY_HOST &&
		flashrom_isp->flags & FLASHROM_ISP_USE_PROXY))
		buffer_printf("#GProxy hostname:#G       %s\n", flashrom_isp->proxy_host);
	if(ignoreflags || (flashrom_isp->valid_fields & FLASHROM_ISP_PROXY_PORT &&
		flashrom_isp->flags & FLASHROM_ISP_USE_PROXY))
		buffer_printf("#GProxy port:#G           %d\n", flashrom_isp->proxy_port);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_PPP_USER) 
		buffer_printf("#GPPP username:#G         %s\n", flashrom_isp->ppp_login);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_PPP_PASS)
		buffer_printf("#GPPP password:#G         %s\n", flashrom_isp->ppp_passwd);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_OUT_PREFIX)
	{
		if(ignoreflags || strlen(flashrom_isp->out_prefix))
			buffer_printf("#GOutside dial prefix:#G  %s\n", flashrom_isp->out_prefix);
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_CW_PREFIX)
	{
		if(ignoreflags || strlen(flashrom_isp->cw_prefix))
			buffer_printf("#GCall waiting prefix:#G  %s\n", flashrom_isp->cw_prefix);
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_REAL_NAME)
		buffer_printf("#GReal name:#G            %s\n", flashrom_isp->real_name);
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_MODEM_INIT)
		buffer_printf("#GModem init string:#G    %s\n", flashrom_isp->modem_init);
	if(ignoreflags || (flashrom_isp->valid_fields & FLASHROM_ISP_AREA_CODE &&
		flashrom_isp->flags & FLASHROM_ISP_DIAL_AREACODE))
	{
		if(ignoreflags || strlen(flashrom_isp->area_code))
			buffer_printf("#GArea code:#G            %s\n", flashrom_isp->area_code);
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_LD_PREFIX)
	{
		if(ignoreflags || strlen(flashrom_isp->ld_prefix))
			buffer_printf("#GLong distance prefix:#G %s\n", flashrom_isp->ld_prefix);
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_PHONE1)
	{
		if(strlen(flashrom_isp->p1_areacode))
			buffer_printf("#GPhone number 1:#G       (%s) %s\n", 
				flashrom_isp->p1_areacode, flashrom_isp->phone1);
		else
			if(ignoreflags || strlen(flashrom_isp->phone1))
				buffer_printf("#GPhone number 1:#G       %s\n",
					flashrom_isp->phone1);
	}
	if(ignoreflags || flashrom_isp->valid_fields & FLASHROM_ISP_PHONE2)
	{
		if(strlen(flashrom_isp->p2_areacode))
			buffer_printf("#GPhone number 2:#G       (%s) %s\n", 
				flashrom_isp->p2_areacode, flashrom_isp->phone2);
		else
			if(ignoreflags || strlen(flashrom_isp->phone2))
				buffer_printf("#GPhone number 2:#G       %s\n",
					flashrom_isp->phone2);
	}

	if(flashrom_isp->flags)
	{
		buffer_printf("#GFlags:#G\n");
		if(flashrom_isp->flags & FLASHROM_ISP_DIAL_AREACODE)
			buffer_printf("                         *Dial area code before number\n");
		if(flashrom_isp->flags & FLASHROM_ISP_USE_PROXY)
			buffer_printf("                         *Proxy enabled\n");
		if(flashrom_isp->flags & FLASHROM_ISP_PULSE_DIAL)
			buffer_printf("                         *Pulse dialing (instead of tone)\n");
		if(flashrom_isp->flags & FLASHROM_ISP_BLIND_DIAL)
			buffer_printf("                         *Blind dial (don't wait for tone)\n");
	}
	if(ignoreflags)
	{
		buffer_printf("#GValid Fields:#G         0x%X\n", (unsigned int)flashrom_isp->valid_fields);
		buffer_printf("#GFlags:#G                0x%X\n", (unsigned int)flashrom_isp->flags);
	}
}

void Show_ISP_Data()
{
	int 			done = 0, oldvmode = -1, joycnty = 0, joycntx = 0, fillbuffer = 1;
	int				v_scroll = 0, h_scroll = 0, lines = 0, ignoreflags = 0;
	uint16			pressed;		
	ImagePtr		back = NULL;
	controller		*st = NULL;

	if(display_buffer)
	{
		dbglog(DBG_ERROR, "Display text buffer was not NULL"); 
		return;
	}
	
	back = LoadIMG("/rd/wallisp.kmg.gz", 0);
	if(!back)
		return;
	
	display_buffer = (char*)malloc(sizeof(char)*DISPLAY_BUFFER_CHAR);
	if(!display_buffer)
	{
		dbglog(DBG_ERROR, "Out of memory for display text buffer\n"); 
		return;
	}

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
			oldvmode = vmode;
		
		if(fillbuffer)
		{
			flashrom_ispcfg_t flashrom_isp;
			
			cleanDisplayBuffer();

			memset(&flashrom_isp, 0, sizeof(flashrom_ispcfg_t));
			flashrom_get_pw_ispcfg(&flashrom_isp);
			if(flashrom_get_pw_ispcfg(&flashrom_isp) == 0)
			{
				buffer_printf("#CPlanetWeb's ISP configuration#C\n");
				print_ispcfg(&flashrom_isp, ignoreflags);
			}
			
			memset(&flashrom_isp, 0, sizeof(flashrom_ispcfg_t));
			flashrom_get_ispcfg(&flashrom_isp);
			if(flashrom_get_ispcfg(&flashrom_isp) == 0)
			{
				buffer_printf("#CDreamPassport's ISP configuration:#C\n");
				print_ispcfg(&flashrom_isp, ignoreflags);
			}
			
			lines = countLineFeeds(display_buffer);
			if(lines == 0)
				buffer_printf("ISP data in #Yflashrom#Y is empty.");
			fillbuffer = 0;
		}

		StartScene();
		DrawImage(back);
		if(!ignoreflags)
			DrawStringSCentered(20+v_scroll, 0.0f, 1.0f, 0.0f, "ISP Info");
		else
			DrawStringSCentered(20+v_scroll, 0.0f, 1.0f, 0.0f, "ISP Info (#Yflags ignored#y)");
		DrawString(20+h_scroll, 20+2*fh+v_scroll, 1.0f, 1.0f, 1.0f, display_buffer);
		EndScene();

		VMURefresh("ISP info", "Flashrom");

		st = ReadController(0, &pressed);
		JoystickDirections(st, &pressed, &joycntx, &joycnty);
		if(st)
		{
			if ( pressed & CONT_LTRIGGER || pressed & CONT_DPAD_UP )
				v_scroll += fw;
			if ( pressed & CONT_RTRIGGER || pressed & CONT_DPAD_DOWN )
				v_scroll -= fw;
			if(v_scroll > 0)
				v_scroll = 0;
			if(v_scroll < -1*lines*fh)
				v_scroll = -1*lines*fh;
			
			if ( pressed & CONT_DPAD_LEFT )
				h_scroll -= fw;
			if ( pressed & CONT_DPAD_RIGHT )
				h_scroll += fw;
			if(h_scroll > 8*fw)
				h_scroll = 8*fw;
			if(h_scroll < -20*fw)
				h_scroll = -20*fw;
				
			if (pressed & CONT_Y)
				h_scroll = v_scroll = 0;
				
			if (pressed & CONT_X)
			{
				ignoreflags = !ignoreflags;
				fillbuffer = 1;
			}
			
			if (pressed & CONT_B)
				done = 1;

			if (pressed & CONT_START)
				ShowMenu(ISPHELP);
		}
	}
	FreeImage(&back);
	if(display_buffer)
	{
		free(display_buffer);
		display_buffer = NULL;
		display_buffer_pos = 0;
	}
	return;
}

// Give us Null terminated strings for output
void clone_factory_data()
{
	int i = 0;
	
	memset(&p_flash_cache, 0, sizeof(flash_data_p));
	for(i = 0; i < 2; i++)
	{
		memcpy(p_flash_cache.factory_records[i].machine_name, 
			flash_cache.factory_records[i].machine_name, sizeof(char)*32);
		memcpy(p_flash_cache.factory_records[i].tool_number, 
			flash_cache.factory_records[i].tool_number, sizeof(char)*4);
		memcpy(p_flash_cache.factory_records[i].tool_version, 
			flash_cache.factory_records[i].tool_version, sizeof(char)*2);
		memcpy(p_flash_cache.factory_records[i].tool_type, 
			flash_cache.factory_records[i].tool_type, sizeof(char)*2);
		memcpy(p_flash_cache.factory_records[i].year, 
			flash_cache.factory_records[i].year, sizeof(char)*4);
		memcpy(p_flash_cache.factory_records[i].month, 
			flash_cache.factory_records[i].month, sizeof(char)*2);
		memcpy(p_flash_cache.factory_records[i].day, 
			flash_cache.factory_records[i].day, sizeof(char)*2);
		memcpy(p_flash_cache.factory_records[i].hour, 
			flash_cache.factory_records[i].hour, sizeof(char)*2);
		memcpy(p_flash_cache.factory_records[i].min, 
			flash_cache.factory_records[i].min, sizeof(char)*2);
		memcpy(p_flash_cache.factory_records[i].serial_number, 
			flash_cache.factory_records[i].serial_number, sizeof(char)*8);
		memcpy(p_flash_cache.factory_records[i].factory_code, 
			flash_cache.factory_records[i].factory_code, sizeof(char)*4);
		memcpy(p_flash_cache.factory_records[i].total_number, 
			flash_cache.factory_records[i].total_number, sizeof(char)*16);
	}
	
	// Skip spaces in Staff Roll
	memcpy(p_flash_cache.staff_roll, 
			flash_cache.staff_roll+0x20, sizeof(char)*(0xc81));
	// Add line feeds for Staff Roll Display
	for(i = 0x40; i < 0xc81; i+=0x40)
		p_flash_cache.staff_roll[i-1] = '\n';
}

int flashrom_get_partition2_data()
{
    int start, size;

	if(flashromp2_is_cached)
		return FLASHROM_ERR_NONE;
	
	if(flashrom_info(FLASHROM_PT_BLOCK_1, &start, &size)) {	// 0x1C000
        dbglog(DBG_ERROR, "flashrom_get_factory_data: can't find partition 0\n");
        return FLASHROM_ERR_NO_PARTITION;
    }

	memset(&cid_record, 0, sizeof(cid_record_st));
	start += 0x40;
    /* Read the data structures from that partition */
    if(flashrom_read(start, &cid_record, sizeof(cid_record_st)) < 0) {
        dbglog(DBG_ERROR, "flashrom_get_partition2_data: can't read partition 0\n");
        return FLASHROM_ERR_READ_PART;
    }
	
	flashromp2_is_cached = 1;
	return FLASHROM_ERR_NONE;
}

/* Modified from KOS */
int flashrom_get_factory_data()
{
    int start, size;

	if(flashrom_is_cached)
		return FLASHROM_ERR_NONE;
	
    /* Find the partition */
    if(flashrom_info(FLASHROM_PT_SYSTEM, &start, &size)) {
        dbglog(DBG_ERROR, "flashrom_get_factory_data: can't find partition 0\n");
        return FLASHROM_ERR_NO_PARTITION;
    }	
	memset(&flash_cache, 0, sizeof(flash_data));

    /* Read the data structures from that partition */
    if(flashrom_read(start, &flash_cache, sizeof(flash_data)) < 0) {
        dbglog(DBG_ERROR, "flashrom_get_factory_data: can't read partition 0\n");
        return FLASHROM_ERR_READ_PART;
    }
	
	dbglog(DBG_INFO, "Read Flashrom Factory Data %c%c%c%c%c\n", 
		protectascii(flash_cache.factory_records[0].machine_code1), 
		protectascii(flash_cache.factory_records[0].machine_code2), 
		protectascii(flash_cache.factory_records[0].country_code), 
		protectascii(flash_cache.factory_records[0].language),
		protectascii(flash_cache.factory_records[0].broadcast_format));
	clone_factory_data();
	flashrom_get_partition2_data();
	flashrom_is_cached = 1;
	return FLASHROM_ERR_NONE;
}

char *get_flash_region_str(int ver)
{
	switch(flashrom_get_region_data(ver))
	{
		case FLASHROM_REGION_JAPAN:
			return("Japan");
			break;
		case FLASHROM_REGION_US:
			return("USA");
			break;
		case FLASHROM_REGION_EUROPE:
			return("Europe");
			break;
		case FLASHROM_REGION_UNKNOWN:
			return("????");
			break;
	}
	return("????");
}
	
int flashrom_get_region_data(int ver)
{
	if(!flashrom_is_cached)
	{
		if(flashrom_get_factory_data() != FLASHROM_ERR_NONE)
			return FLASHROM_REGION_UNKNOWN;
	}
	
	if(ver != 0 && ver != 1)
		return FLASHROM_REGION_UNKNOWN;
	
    /* Now compare cache against known codes */
    if(flash_cache.factory_records[ver].country_code == '0')
        return FLASHROM_REGION_JAPAN;
    else if(flash_cache.factory_records[ver].country_code == '1')
        return FLASHROM_REGION_US;
    else if(flash_cache.factory_records[ver].country_code == '2')
        return FLASHROM_REGION_EUROPE;
    else {
        dbglog(DBG_WARNING, "flashrom_get_region_data: unknown code '%c')\n",
			flash_cache.factory_records[ver].country_code);
        return FLASHROM_REGION_UNKNOWN;
    }
}

int is_flashrom_region_data_changed()
{
	uint8 byte;
	
	if(!flashrom_is_cached)
	{
		if(flashrom_get_factory_data() != FLASHROM_ERR_NONE)
			return 0;
	}
	
	byte = flash_cache.factory_records[1].country_code;
    /* Now compare cache between codes */
	if(byte != 0xFF &&
		flash_cache.factory_records[0].country_code != 
		flash_cache.factory_records[1].country_code)
		return 1;
    return 0;
}

char *get_flash_language_str(int ver)
{
	switch(flashrom_get_language(ver))
	{
		case FLASHROM_LANGUAGE_JAPAN:
			return("Japanese");
			break;
		case FLASHROM_LANGUAGE_ENGLISH:
			return("English");
			break;
		case FLASHROM_LANGUAGE_GERMAN:
			return("German");
			break;
		case FLASHROM_LANGUAGE_FRENCH:
			return("French");
			break;
		case FLASHROM_LANGUAGE_SPANISH:
			return("Spanish");
			break;
		case FLASHROM_LANGUAGE_ITALIAN:
			return("Italian");
			break;
		case FLASHROM_LANGUAGE_UNKNOWN:
			return("????");
			break;
		
	}
	return("????");
}

int flashrom_get_language(int ver)
{
    if(!flashrom_is_cached)
	{
		if(flashrom_get_factory_data() != FLASHROM_ERR_NONE)
			return FLASHROM_LANGUAGE_UNKNOWN;
	}
	
	if(ver != 0 && ver != 1)
		return FLASHROM_LANGUAGE_UNKNOWN;
	
    /* Now compare against known codes */
    if(flash_cache.factory_records[ver].language == '0')
        return FLASHROM_LANGUAGE_JAPAN;
    else if(flash_cache.factory_records[ver].language == '1')
        return FLASHROM_LANGUAGE_ENGLISH;
    else if(flash_cache.factory_records[ver].language == '2')
        return FLASHROM_LANGUAGE_GERMAN;
	else if(flash_cache.factory_records[ver].language == '3')
        return FLASHROM_LANGUAGE_FRENCH;
	else if(flash_cache.factory_records[ver].language == '4')
        return FLASHROM_LANGUAGE_SPANISH;
	else if(flash_cache.factory_records[ver].language == '5')
        return FLASHROM_LANGUAGE_ITALIAN;
    else {
        dbglog(DBG_WARNING, "flashrom_get_language: unknown code '%c'\n",
			flash_cache.factory_records[ver].language);
        return FLASHROM_LANGUAGE_UNKNOWN;
    }
}

char *get_flash_broadcast_str(int ver)
{
	switch(flashrom_get_broadcast(ver))
	{
		case FLASHROM_BROADCAST_NTSC:
			return("NTSC");
			break;
		case FLASHROM_BROADCAST_PAL:
			return("PAL");
			break;
		case FLASHROM_BROADCAST_PALM:
			return("PAL-M");
			break;
		case FLASHROM_BROADCAST_PALN:
			return("PAL-N");
			break;
		case FLASHROM_BROADCAST_UNKNOWN:
			return("????");
			break;
	}
	return("????");
}

int flashrom_get_broadcast(int ver)
{
    if(!flashrom_is_cached)
	{
		if(flashrom_get_factory_data() != FLASHROM_ERR_NONE)
			return FLASHROM_BROADCAST_UNKNOWN;
	}
	
	if(ver != 0 && ver != 1)
		return FLASHROM_BROADCAST_UNKNOWN;
	
    /* Now compare against known codes */
    if(flash_cache.factory_records[ver].broadcast_format == '0')
        return FLASHROM_BROADCAST_NTSC;
    else if(flash_cache.factory_records[ver].broadcast_format == '1')
        return FLASHROM_BROADCAST_PAL;
    else if(flash_cache.factory_records[ver].broadcast_format == '2')
        return FLASHROM_BROADCAST_PALM;
	else if(flash_cache.factory_records[ver].broadcast_format == '3')
        return FLASHROM_BROADCAST_PALN;
    else {
        dbglog(DBG_WARNING, "flashrom_get_broadcast: unknown code '%c'\n",
			flash_cache.factory_records[ver].broadcast_format);
        return FLASHROM_BROADCAST_UNKNOWN;
    }
}

int is_flashrom_broadcast_changed()
{
	uint8 byte;
	
	if(!flashrom_is_cached)
	{
		if(flashrom_get_factory_data() != FLASHROM_ERR_NONE)
			return 0;
	}
	
	byte = flash_cache.factory_records[1].broadcast_format;
    /* Now compare cache between codes */
	if(byte != 0xFF &&
		flash_cache.factory_records[0].broadcast_format != 
		flash_cache.factory_records[1].broadcast_format)
		return 1;
    return 0;
}


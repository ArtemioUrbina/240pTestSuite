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
 
#include <dos.h>
#include <iocs.h>
#include <stdio.h>
#include "video.h"
#include "mdfourier.h"

// Enter supervisor mode, so we can access vblank regiter
void initX68000()
{
	_dos_super(0);
}

// based on John Snowdon x68Launcher code
#define input_group_select_enter			0x09
#define input_group_select_space			0x06
#define input_group_cancel					0x00

#define JOY_BUTTON1	(1 << 6)
#define JOY_BUTTON2	(1 << 5)
#define JOY_RIGHT	(1 << 3)
#define JOY_LEFT	(1 << 2)
#define JOY_DOWN	(1 << 1)
#define JOY_UP		(1 << 0)
// returns 1 on enter, space or joystick 1 buttons
int read_input()
{
	char	key = '\0';
	int		joy = 0;
	
	key = _iocs_bitsns(input_group_select_enter);
	if (key & 0x40) return 1;
	key = _iocs_bitsns(input_group_select_space);
	if (key & 0x20) return 1;
	key = _iocs_bitsns(input_group_cancel);
	if (key & 0x02) return -1;
	
	joy = _iocs_joyget(0);
	if (!(joy & JOY_BUTTON1)) return 1;
	if (!(joy & JOY_BUTTON2)) return 1;
	
	return 0;
}

int main(void)
{
	int input = 0;

	printf("MDFourier for X68000 v 0.1 -- http://junkerhq.net/MDFourier\n");
	printf("  Artemio Urbina 2021\n\n");

	printf("Press SPACE or a joystick button when ready to record\n");
	printf("Press ESC to abort\n");
	
	initX68000();
	
	do
	{
		input = read_input();
		wait_vblank();
	}while(!input);
	
	if(input == 1)
	{
		printf("Starting MDFourier test tones\n");
		ExecuteMDF(20);
	}
	else
		printf("Aborted\n");

	return 0;
}

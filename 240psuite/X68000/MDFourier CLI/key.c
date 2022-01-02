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
 
#include <iocs.h>
#include <stdio.h>
#include <stdlib.h>


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

// returns 1 on enter, space or joystick 1 buttons and -1 on ESC
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

/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2018 Artemio Urbina
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

#include <yaul.h>

#include "control.h"

struct smpc_peripheral_digital controller;

void InitControllers()
{
	smpc_init();
	smpc_peripheral_init();
}

void wait_for_key_press()
{
	//wait for keypress
	while (controller.pressed.raw == 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
	}
}

void wait_for_key_unpress()
{
	//wait for unpress
	while (controller.pressed.raw != 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
	}
}

void wait_for_next_key()
{
	wait_for_key_unpress();
	wait_for_key_press();
	wait_for_key_unpress();
}

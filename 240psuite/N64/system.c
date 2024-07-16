/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 
 #include "system.h"
 #include "video.h"
 
 void initN64() 
 {
	/* Initialize peripherals */
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, SUITE_NUM_BUFFERS, GAMMA_NONE, FILTERS_RESAMPLE);
	
	dfs_init(DFS_DEFAULT_LOCATION);
	
	joypad_init();
	
	register_VI_handler(vblCallback);
	rdpq_init();
 }
 
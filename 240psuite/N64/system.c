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
 #include "font.h"
 
 void initN64() {
	/* Initialize peripherals */
	initVideo();
	setVideo(RESOLUTION_320x240);
	
	dfs_init(DFS_DEFAULT_LOCATION);
	
	joypad_init();
 }
 
 void drawSysData() {
	char str[20];
	bool iQue = sys_bbplayer();
	
	switch(get_tv_type())
	{
		case TV_NTSC:
			drawStringS(224, 216, 0xfa, 0xfa, 0xfa, iQue ? "iQue NTSC" : "N64 NTSC");
			break;
		case TV_PAL:
			drawStringS(224, 216, 0xfa, 0xfa, 0xfa, iQue ? "iQue PAL" : "N64 PAL"); 
			break;
		case TV_MPAL:
			drawStringS(224, 216, 0xfa, 0xfa, 0xfa, iQue ? "iQue M-PAL" : "N64 M-PAL"); 
			break;
		default:
			drawStringS(224, 216, 0xfa, 0xfa, 0xfa, iQue ? "iQue ???" : "N64 ???"); 
			break;
	}
	
	sprintf(str, "RAM %d MB", get_memory_size()/0x100000);
	drawStringS(224, 224, 0xfa, 0xfa, 0xfa, str);
 }
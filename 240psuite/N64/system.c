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
 #include <unistd.h>
 
 int isPAL = 0;
 
 void detectConsoleType() {
	switch(get_tv_type()) {
		case TV_PAL:
		case TV_MPAL:
			isPAL = 1;
			break;
		case TV_NTSC:
		default:
			isPAL = 0;
			break;
	}
}
 
 void initN64() {
	uint32_t seed;
	
	/* Initialize peripherals */
	detectConsoleType();
	
	dfs_init(DFS_DEFAULT_LOCATION);
	loadFont();
	
	initVideo();
	setVideoInternal(RESOLUTION_320x240);
	
	joypad_init();
	
	getentropy(&seed, 4);
	srand(seed);
 }
 
 #define START_SYSINFO_X	190
 #define START_SYSINFO_Y	216
 
 void drawSysData() {
	char str[20];
	int iQue = sys_bbplayer();
	
	sprintf(str, "%s %d MB", iQue ? "iQue" : "N64", get_memory_size()/0x100000);
	drawStringS(START_SYSINFO_X, START_SYSINFO_Y, 0xfa, 0xfa, 0xfa, str);
	
	switch(get_tv_type()) {
		case TV_NTSC:
			sprintf(str, "NTSC ");
			break;
		case TV_PAL:
			sprintf(str, "PAL  ");
			break;
		case TV_MPAL:
			sprintf(str, "MPAL ");
			break;
		default:
			sprintf(str, "???  ");
			break;
	}

	getVideoModeStr(str+5, 0);
	drawStringS(START_SYSINFO_X, START_SYSINFO_Y+fh, 0xfa, 0xfa, 0xfa, str);
}

// Code from Fazana
// 0xF1C00 taken form the max that malloc can return to on either 4Mb or 8MB
int getUsedRAM() {
	struct mallinfo mem_info = mallinfo();
	return(mem_info.uordblks - ((unsigned int)HEAP_START_ADDR - 0x80000000 - 0x10000 - 0xF1C00));
}

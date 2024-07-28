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
 #include "hardware.h"
 #include "menu.h"
 #include "font.h"
 
#define VISIBLE_HORZ	16
#define VISIBLE_VERT	22

#define MAX_LOCATIONS 1

#define BUFFER_SIZE 128
 
 // locks at 0x83FFE6E0
 
 void drawMemoryViewer() {
	int 			done = 0, ascii = 0, locpos = 0, docrc = 0;
	unsigned int	address = 0, crc = 0, offset = 6;
	unsigned int	locations[MAX_LOCATIONS] = { 0x80000000};
	char 			buffer[BUFFER_SIZE];
	joypad_buttons_t keys;
	
	enableHalfWidthSpace(true);
	address = locations[0];
	while(!done) {
		int 	i = 0, j = 0;
		uint8_t *mem = NULL;
		
		getDisplay();

		clearScreen = true;
		rdpqStart();
		rdpqEnd();
		
		mem = (uint8_t*)address;
		
		/*
		if(docrc)
			crc = calculateCRC(address, VISIBLE_HORZ*VISIBLE_VERT);
		*/
	
		sprintf(buffer, "%08X", address);
		drawString(VISIBLE_HORZ*3*fw, 0, 0x00, 0xff, 0x00, buffer);
		
		sprintf(buffer, "%08X", address+VISIBLE_HORZ*VISIBLE_VERT);
		drawString(VISIBLE_HORZ*3*fw, (VISIBLE_VERT-1)*fh, 0x00, 0xff, 0x00, buffer);

		/*
		if(docrc) {
			sprintf(buffer, "%08X", crc);
			drawStringS(VISIBLE_HORZ*3*fw, (VISIBLE_VERT/2)*fh, 0xff, 0xff, 0x00, buffer);		
		}
		*/
		
		for(i = 0; i < VISIBLE_VERT; i++) {
			if(ascii)
				memset(buffer, 32, sizeof(char)*BUFFER_SIZE);
			
			for(j = 0; j < VISIBLE_HORZ; j++) {
				if(!ascii)
					sprintf(buffer+j*3, "%02X ", mem[i*VISIBLE_HORZ+j]);
				else {
					uint16_t c;
					
					c = mem[i*VISIBLE_HORZ+j];
					if(c >= 32 && c <= 126)			// ASCII range
						buffer[j*3+1] = c;
				}
			}
			drawString(offset, offset+i*fh, 0xff, 0xff, 0xff, buffer);
		}
		checkMenu(NULL, NULL);
		
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsHeld();
	
		
		if(keys.d_left) {
			if(address > locations[0])
				address -= VISIBLE_HORZ*VISIBLE_VERT;
			else
				address = locations[0];
		}
		
		if(keys.d_right) {
			address += VISIBLE_HORZ*VISIBLE_VERT;

			if(address >= 0xFFFFFFFF) 
				address = 0xFFFFFFFF-VISIBLE_HORZ*VISIBLE_VERT;
		}
		
		if(keys.d_up) {
			if(address > locations[0]+0x10000)
				address -= 0x10000;
			else
				address = locations[0];
		}
		
		if(keys.d_down) {
			address += 0x10000;
			
			if(address >= 0xFFFFFFFF)
				address = 0xFFFFFFFF-VISIBLE_HORZ*VISIBLE_VERT;
		}
		
		if(keys.b)
			done =	1;
			
		if(keys.a) {
			locpos ++;
			if(locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
		}
			
		if(keys.c_left)
			ascii =	!ascii;
			
		if(keys.c_right)
			docrc =	!docrc;
	
		/*
		if(keys.start) {
			
		}
		*/
	}
	enableHalfWidthSpace(false);
}
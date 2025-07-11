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


void CRC32_reset() {
	_state = ~0L;
}


void CRC32_update(uint8_t data) {
	uint8_t tbl_idx = 0;

	tbl_idx = _state ^ (data >> (0 * 4));
	_state = (*(uint32_t*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
	tbl_idx = _state ^ (data >> (1 * 4));
	_state = (*(uint32_t*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
}


uint32_t CRC32_finalize() {
	return ~_state;
}

/**********************************/

uint32_t calculateCRC(uint32_t startAddress, uint32_t size) {
	uint8_t *start = NULL;
	uint32_t address = 0, checksum = 0;
	CRC32_reset();

	start = (void*)startAddress;
	for (address = 0; address < size; address ++) {
		uint8_t data;
		
		data = start[address];

		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

 
#define VISIBLE_HORZ	16
#define VISIBLE_VERT	27
#define CHARS_PER_BYTE	2

#define MAX_LOCATIONS	1
#define BASE_ADDRESS	0xA0000000
#define MAX_ADDRESS		0xA03FFFF0
#define MAX_ADDRESSXP	0xA07ffff0
#define ADDR_JUMP		0x4000

#define BUFFER_SIZE 128
 
 void drawMemoryViewer(void *startAddress) {
	int 			done = 0, ascii = 0, locpos = 0, docrc = 0;
	uint32_t		address = 0, crc = 0, offset = fh, maxAddress = MAX_ADDRESS;
	uint32_t		locations[MAX_LOCATIONS] = { BASE_ADDRESS };
	char 			buffer[BUFFER_SIZE];
	joypad_buttons_t keys;
	
	if(startAddress)
		address = (uint32_t)startAddress;
	else
		address = locations[0];
		
	if(get_memory_size()/0x100000 == 8)
		maxAddress = MAX_ADDRESSXP;
	
	while(!done) {
		int 	i = 0, j = 0;
		uint8_t *mem = NULL;
		
		getDisplay();

		setClearScreen();
		rdpqStart();
		rdpqEnd();
		
		mem = (uint8_t*)address;
		
		if(docrc)
			crc = calculateCRC(address, VISIBLE_HORZ*VISIBLE_VERT);
	
		sprintf(buffer, "%08lX", address - 0xA0000000);
		drawString((VISIBLE_HORZ-2)*2*fw, 0, 0x00, 0xff, 0x00, buffer);
		
		sprintf(buffer, "%08lX", address+VISIBLE_HORZ*VISIBLE_VERT- 0xA0000000);
		drawString((VISIBLE_HORZ-2)*2*fw, (VISIBLE_VERT)*fhR+fh, 0x00, 0xff, 0x00, buffer);

		if(docrc) {
			sprintf(buffer, "Screen CRC: %08lX", crc);
			drawStringS(4*fw, (VISIBLE_VERT)*fhR+fh, 0xff, 0xff, 0x00, buffer);		
		}
		
		for(i = 0; i < VISIBLE_VERT; i++) {
			if(ascii)
				memset(buffer, 32, sizeof(char)*BUFFER_SIZE);
			
			for(j = 0; j < VISIBLE_HORZ; j++) {
				if(!ascii)
					sprintf(buffer+j*CHARS_PER_BYTE, "%02X", mem[i*VISIBLE_HORZ+j]);
				else {
					uint16_t c;
					
					c = mem[i*VISIBLE_HORZ+j];
					if(c >= 32 && c <= 126)			// ASCII range
						buffer[j*CHARS_PER_BYTE+1] = c;
				}
			}
			// In order to have ASCII values aligned when toggling, we use the default width here
			useReducedWidthSpace(0);
			drawString(offset*3, offset+i*fhR, 0xff, 0xff, 0xff, buffer);
			useReducedWidthSpace(1);
		}
		checkMenu(MEMORYHELP, NULL);
		
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
	
		checkStart(keys);
		if(keys.d_left) {
			if(address > locations[0])
				address -= VISIBLE_HORZ*VISIBLE_VERT;
			else
				address = locations[0];
		}
		
		if(keys.d_right) {
			address += VISIBLE_HORZ*VISIBLE_VERT;

			if(address >= maxAddress) 
				address = maxAddress-VISIBLE_HORZ*VISIBLE_VERT;
		}
		
		if(keys.d_up) {
			if(address > locations[0]+ADDR_JUMP)
				address -= ADDR_JUMP;
			else
				address = locations[0];
		}
		
		if(keys.d_down) {
			address += ADDR_JUMP;
			
			if(address >= maxAddress)
				address = maxAddress-VISIBLE_HORZ*VISIBLE_VERT;
		}
		
		if(keys.b)
			done =	1;
			
		if(keys.a)  {
			locpos ++;
			if(locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
		}
			
		if(keys.c_left)
			ascii =	!ascii;
			
		if(keys.c_right)
			docrc =	!docrc;
	}
}

void drawButtonAt(int press, int x, int y, int r, int g, int b, char *text) {
	drawStringS(x, y, press ? 0xff : r, press ? 0xff : g, press ? 0xff : b, text);
}

/*
 L         R
      Z     U
 U         L R
L R   S  A  D
 D        B
     255  
   255 255
     255
*/
void drawControllerAtCoord(int controller, int x, int y) {
	char buffer[5];
	joypad_inputs_t pad_inputs;
	joypad_buttons_t pad_held;
	int analog_x, analog_y;
	
	switch(controller) {
		case 1:
			pad_inputs = joypad_get_inputs(JOYPAD_PORT_1);
			pad_held = joypad_get_buttons_held(JOYPAD_PORT_1);
			break;
		case 2:
			pad_inputs = joypad_get_inputs(JOYPAD_PORT_2);
			pad_held = joypad_get_buttons_held(JOYPAD_PORT_2);
			break;
		case 3:
			pad_inputs = joypad_get_inputs(JOYPAD_PORT_3);
			pad_held = joypad_get_buttons_held(JOYPAD_PORT_3);
			break;
		case 4:
			pad_inputs = joypad_get_inputs(JOYPAD_PORT_4);
			pad_held = joypad_get_buttons_held(JOYPAD_PORT_4);
			break;
		default:
			return;
	}
	
	drawButtonAt(pad_held.l, 		x + 1*fw, y, 114, 119, 128, "L");
	drawButtonAt(pad_held.r, 		x + 11*fw, y, 114, 119, 128, "R");
	y += fh;
	
	drawButtonAt(pad_held.z, 		x + 6*fw, y, 114, 119, 128, "Z");
	drawButtonAt(pad_held.c_up, 	x + 12*fw, y, 238, 178, 0, "U");
	y += fh;
	
	drawButtonAt(pad_held.d_up, 	x + 1*fw, y, 114, 119, 128, "U");
	drawButtonAt(pad_held.c_left, 	x + 11*fw, y, 238, 178, 0, "L");
	drawButtonAt(pad_held.c_right, 	x + 13*fw, y, 238, 178, 0, "R");
	y += fh;
	
	drawButtonAt(pad_held.d_left, 	x, y, 114, 119, 128, "L");
	drawButtonAt(pad_held.d_right, 	x + 2*fw, y, 114, 119, 128, "R");
	drawButtonAt(pad_held.start, 	x + 6*fw, y, 255, 69, 68, "S");
	drawButtonAt(pad_held.b, 		x + 9*fw, y, 0, 137, 92, "B");
	drawButtonAt(pad_held.c_down, 	x + 12*fw, y, 238, 178, 0, "D");
	y += fh;
	
	drawButtonAt(pad_held.d_down, 	x + 1*fw, y, 114, 119, 128, "D");
	drawButtonAt(pad_held.a, 		x + 10*fw, y, 0, 62, 162, "A");
	y += fh;
	
	analog_x = pad_inputs.stick_x;
	analog_y = pad_inputs.stick_y;
	
	sprintf(buffer, "%03d", analog_y > 0 ? analog_y : 0);
	drawButtonAt(analog_y > 0, 	x + 5*fw, y, 185, 193, 216, buffer);
	y += fh;
	
	sprintf(buffer, "%03d", analog_x < 0 ? -1*analog_x : 0);
	drawButtonAt(analog_x < 0, 	x + 3*fw, y, 185, 193, 216, buffer);
	sprintf(buffer, "%03d", analog_x > 0 ? analog_x : 0);
	drawButtonAt(analog_x > 0, 	x + 7*fw, y, 185, 193, 216, buffer);
	y += fh;
	
	sprintf(buffer, "%03d", analog_y < 0 ? -1*analog_y : 0);
	drawButtonAt(analog_y < 0, 	x + 5*fw, y, 185, 193, 216, buffer);
}

void drawControllerTest() {
	int end = 0;
	image *back = NULL;
	joypad_buttons_t pad_held;

	back = loadImage("rom:/controller.sprite");
	if(!back)
		return;
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqClearScreen();
		rdpqDrawImage(back);
		rdpqEnd();
		
		drawStringC(15, 0x00, 0xff, 0x00, "Controller Test");
		drawStringC(25, 0xFF, 0xff, 0x00, "Left+Start in controller 1 to exit");
		drawControllerAtCoord(1, 30, 40);
		drawControllerAtCoord(2, 170, 40);
		
		drawControllerAtCoord(3, 30, 40+9*fh);
		drawControllerAtCoord(4, 170, 40+9*fh);
		waitVsync();
		
		joypad_poll();
		pad_held = joypad_get_buttons_held(JOYPAD_PORT_1);
		
		if(pad_held.start && pad_held.d_left)
			end = 1;
	}
	
	freeImage(&back);
}

/*
 * Based on code Copyright (C) 2016 FIX94
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * https://github.com/FIX94/GoombaSend/blob/master/source/main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "font.h"
#include "video.h"
#include "options.h"
#include "controller.h"
#include "240pee_mb_gba.h"

#include "gba.h"

#include "help.h"
#include "menu.h"

#include <ogc/machine/processor.h>

// From my tests, 50us seems to be the lowest
// safe si transfer delay in between calls (FIX94)
// Used by Extrems: 65

#define SI_TRANS_DELAY 65

#define GBA_RAM 262144
#define GBA_KEY0 0x7365646F
#define GBA_KEY1 0x6177614B
#define GBA_KEY2 0x20796220

#define SI_TIMEOUT		200000
#define TIMEOUT			10000
#define BYTE_CHUNK 		20000
#define CHECK_TIMEOUT	10

u8 *resbuf = NULL;
u8 *cmdbuf = NULL;
volatile u32 resval = 0;
volatile u32 transval = 0;

void InitGBARAM() {
	if(!cmdbuf)
		cmdbuf = memalign(32, 32);
	if(!resbuf)
		resbuf = memalign(32, 32);
}

void ReleaseGBARAM() {
	if(cmdbuf) {
		free(cmdbuf);
		cmdbuf = NULL;
	}
	if(resbuf) {
		free(resbuf);
		resbuf = NULL;
	}
}

void transcb(s32 chan, u32 ret) {
    transval = 1;
}

void portcb(s32 res, u32 val) {
    resval = val;
}

unsigned int docrc(u32 crc, u32 val) {
    int i;
	
    for (i = 0; i < 0x20; i++) {
        if ((crc ^ val) & 1) {
            crc >>= 1;
            crc ^= 0xa1c1;
        }
        else
            crc >>= 1;
        val >>= 1;
    }
    return crc;
}

unsigned int calckey(unsigned int size) {
    unsigned int ret = 0;
    size = (size - 0x200) >> 3;
    int res1 = (size & 0x3F80) << 1;
    res1 |= (size & 0x4000) << 2;
    res1 |= (size & 0x7F);
    res1 |= 0x380000;
    int res2 = res1;
    res1 = res2 >> 0x10;
    int res3 = res2 >> 8;
    res3 += res1;
    res3 += res2;
    res3 <<= 24;
    res3 |= res2;
    res3 |= 0x80808080;

    if ((res3 & 0x200) == 0) {
        ret |= (((res3) & 0xFF) ^ 0x4B) << 24;
        ret |= (((res3 >> 8) & 0xFF) ^ 0x61) << 16;
        ret |= (((res3 >> 16) & 0xFF) ^ 0x77) << 8;
        ret |= (((res3 >> 24) & 0xFF) ^ 0x61);
    }
    else {
        ret |= (((res3) & 0xFF) ^ 0x73) << 24;
        ret |= (((res3 >> 8) & 0xFF) ^ 0x65) << 16;
        ret |= (((res3 >> 16) & 0xFF) ^ 0x64) << 8;
        ret |= (((res3 >> 24) & 0xFF) ^ 0x6F);
    }
    return ret;
}

int doreset(unsigned int port) {	
	unsigned int timeout = 0;
	
    cmdbuf[0] = 0xFF; // reset
    transval = 0;

    SI_Transfer(port, cmdbuf, 1, resbuf, 3, transcb, SI_TRANS_DELAY);

    while (transval == 0) {
		if(timeout ++ > SI_TIMEOUT)
			return 0;
	}
	return 1;
}

int getstatus(unsigned int port) {
	unsigned int timeout = 0;
	
    cmdbuf[0] = 0; // status
    transval = 0;

    SI_Transfer(port, cmdbuf, 1, resbuf, 3, transcb, SI_TRANS_DELAY);

    while (transval == 0) {
		if(timeout ++ > SI_TIMEOUT)
			return 0;
	}
	return 1;
}

int recv(unsigned int port, u32 *data) {
	unsigned int timeout = 0;
	
    memset(resbuf, 0, 32);
	memset(data, 0, 32);
	
    cmdbuf[0] = 0x14; // read
    transval = 0;

    SI_Transfer(port, cmdbuf, 1, resbuf, 5, transcb, SI_TRANS_DELAY);

    while (transval == 0) {
		if(timeout ++ > SI_TIMEOUT)
			return 0;
	}
    *data = *(vu32*)resbuf;
	return 1;
}

int send(u32 msg, unsigned int port) {
	unsigned int timeout = 0;
	
    cmdbuf[0] = 0x15;
    cmdbuf[1] = (msg >> 0) & 0xFF;
    cmdbuf[2] = (msg >> 8) & 0xFF;
    cmdbuf[3] = (msg >> 16) & 0xFF;
    cmdbuf[4] = (msg >> 24) & 0xFF;

    transval = 0;
    resbuf[0] = 0;

    SI_Transfer(port, cmdbuf, 5, resbuf, 1, transcb, SI_TRANS_DELAY);

    while (transval == 0) {
		if(timeout ++ > SI_TIMEOUT)
			return 0;
	}
	return 1;
}

int gba_displaymsg(ImagePtr title, char *msg) {
	u32 pressed = 0;
	
    StartScene();
	DrawImage(title);
	DrawStringC(40, 0x00, 0xff, 0x00, "Transfer GBA 160p Suite via Link Cable");
	DrawStringC(70, 0xaa, 0xaa, 0xaa, "Executing Transfer");
    DrawStringS(80, 105, 0xff, 0xff, 0xff, msg);
	DrawStringC(200, 0xee, 0xee, 0xee, "Press B to cancel"); 
    EndScene();
	
	ControllerScan();
	pressed = Controller_ButtonsDown(0);

	if (pressed & PAD_BUTTON_B)
		return 1;
	return 0;
}

int gba_displayProgress(ImagePtr title, unsigned int i, unsigned int sendsize) {
	char msg[512];
	
	sprintf(msg, "Sending GBA ROM %0.2f%%", (float)i*100.0/(float)sendsize);
	
	return(gba_displaymsg(title, msg));
}

u32 checkGBAConnected(unsigned int port) {
	return(SI_GetType(port) & SI_GBA);
}

int detectLink(ImagePtr title, unsigned int port) {
	char 			msg[256];
	unsigned int 	timeout = 0;
	unsigned int 	detected = 0, close = 0;
	
	sprintf(msg, "Waiting for GBA in port %d...", port + 1);
	
	resval = 0;
	
	SI_GetTypeAsync(port, portcb);
    while (!detected && !close) {
		if(!detected && gba_displaymsg(title, msg))
			close = 1;
		
        if (resval) {
            if (resval == SI_ERROR_BUSY || resval & SI_ERROR_NO_RESPONSE) {
                resval = 0;
                SI_GetTypeAsync(port, portcb);
            }
            else if (resval)
                detected = 1;
        }
		
		if(timeout ++ > SI_TIMEOUT)
			return 0;
    }
	
	return(!close && detected && (resval & SI_GBA));
}

int GBASendROM(ImagePtr title, unsigned int port) {
    unsigned int i = 0, timeout = 0;
	unsigned int sendsize = 0;
	unsigned int ourkey = 0;
	unsigned int fcrc = 0x15a0;
	u32 sessionkeyraw = 0, sessionkey = 0;

	if(port > 3) {
        return GBA_INVALID_PORT;
	}
	
    if (_240pee_mb_gba_size > GBA_RAM) {
        return GBA_ROM_TOO_BIG;
    }

	if(!detectLink(title, port))
		return GBA_NOTCONNECTED;
	

	gba_displaymsg(title, "Waiting for GBA BIOS");
	
	resbuf[2] = 0;
	while (!(resbuf[2] & 0x10))	{
		if(!doreset(port)) {
			return GBA_BIOS_TIMEOUT;
		}
		if(!getstatus(port)) {
			return GBA_BIOS_TIMEOUT;
		}
		
		if(timeout++ >= TIMEOUT) {
			return GBA_BIOS_TIMEOUT;
		}
	}

	sendsize = (((_240pee_mb_gba_size) + 7) & ~7);
	ourkey = calckey(sendsize);
	
	if(!recv(port, &sessionkeyraw)) {
		return GBA_INVALID_SESSION;
	}
	sessionkey = __builtin_bswap32(sessionkeyraw ^ GBA_KEY0);
	
	if(!send(__builtin_bswap32(ourkey), port)) {
		return GBA_TRANSFER_ERROR;
	}

	for(i = 0; i < 0xC0; i += 4) {
		if(!send(__builtin_bswap32(*(vu32*)(_240pee_mb_gba + i)), port)) {
			return GBA_TRANSFER_ERROR;
		}
	}
	
	gba_displaymsg(title, "Sending GBA ROM");
	for(i = 0xC0; i < sendsize; i += 4)	{
		u32 enc = ((_240pee_mb_gba[i+3] << 24) | (_240pee_mb_gba[i+2] << 16) |
				   (_240pee_mb_gba[i+1] << 8) | (_240pee_mb_gba[i]));
		fcrc = docrc(fcrc, enc);
		sessionkey = (sessionkey * GBA_KEY1) + 1;
		enc ^= sessionkey;
		enc ^= ((~(i + (0x20 << 20))) + 1);
		enc ^= GBA_KEY2;
		
		if(!send(enc, port)) {
			return GBA_TRANSFER_ERROR;
		}
		
		if(i % BYTE_CHUNK == 0 && gba_displayProgress(title, i, sendsize)) {
			return GBA_TRANSFER_ERROR;
		}
	}
	

	fcrc |= (sendsize << 16);
	sessionkey = (sessionkey * GBA_KEY1) + 1;
	fcrc ^= sessionkey;
	fcrc ^= ((~(i + (0x20 << 20))) + 1);
	fcrc ^= GBA_KEY2;
	if(!send(fcrc, port)) {
		return GBA_TRANSFER_ERROR;
	}

	if(!recv(port, &sessionkeyraw)) {
		return GBA_FINISH_ERROR;
	}

    return GBA_TRANSFER_OK;
}

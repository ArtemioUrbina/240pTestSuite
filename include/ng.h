/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _NG_H_
#define _NG_H_

#define BKP_SIZE	0X0100

typedef struct bkp_ram_info {
	BYTE debug_dip1;
	BYTE debug_dip2;
	BYTE data[BKP_SIZE-2];
} bkp_ram_info;

extern bkp_ram_info bkp_data;

#define DP_DEBUG1			0x01
#define DP_DEBUG2			0x02
#define DP_DEBUG3			0x04
#define DP_DEBUG4			0x08
#define DP_DEBUG5			0x10
#define DP_DEBUG6			0x20
#define DP_DEBUG7			0x40
#define DP_DEBUG8			0x80

extern BYTE isMVS, is4S, is6S, isMulti, hwChange, allowIRE107;
extern BYTE vmode_snk, isPAL, usePAL256, isPALinMVS, enable_shadow;
extern BYTE p1,p2,ps,pse,p1e,p2e,p1b,p2b;
extern BYTE first_grid;
extern BYTE first_overscan;

#define SYSTEM_AES			0x00
#define SYSTEM_MVS			0x80
#define BIOS_MVS_FLAG		0x10FD82
#define BIOS_COUNTRY_CODE	0x10FD83

#define SYSTEM_EUROPE		0x02
#define SYSTEM_JAPAN		0x00
#define SYSTEM_USA			0x01

#define fontColorBIOS		0
#define fontColorWhite		1
#define fontColorRed		2
#define fontColorGreen		3
#define fontColorBlue		4
#define fontColorGrayLight	5
#define fontColorGrayDark	6
#define fontColorBlack		7
#define fontColorSolid		8

#define REG_DIPSW			0x300001
#define DP_SETTINGS			0x01
#define DP_CHUTES			0x02
#define DP_CONTROL			0x04
#define DP_COMM_1			0x08
#define DP_COMM_2			0x10
#define DP_MULTI			0x20
#define DP_FREE				0x40
#define DP_FREEZE			0x80

#define REG_SYSTYPE			0x300081
#define MVS_MULTI			0x40
#define MVS_TEST_B			0x80

#define REG_STATUS_A		0x320001
#define P1_CREDIT			0x01
#define P2_CREDIT			0x02
#define MVS_SERV_B			0x04
#define P3_CREDIT			0x08
#define P4_CREDIT			0x10
#define MVS_4_OR_6			0x20
#define RTC_TIME			0x40
#define RTC_DATA			0x80

#define REG_STATUS_B		0x380000
#define MVS_P1_STRT			0x01
#define MVS_SEL1			0x02
#define MVS_P2_STRT			0x04
#define MVS_SEL2			0x08
#define HAS_MC_1			0x10
#define HAS_MC_2			0x20
#define MC_PTRCT			0x40
#define MVS_OR_AES			0x80

#define BIOS_USER_REQS		0x10FDAE
#define BIOS_UR_INIT		0
#define BIOS_UR_EYE			1
#define BIOS_UR_DEMO		2
#define BIOS_UR_TITLE		3

#define BIOS_USER_MODE		0x10FDAF
#define BIOS_UM_INIT		0
#define BIOS_UM_TITLEDEMO	1
#define BIOS_UM_INGAME		2

#define BIOS_START_FLAG		0x10FDB4
#define BIOS_DEV_MODE		0x10FE80

// Detect if MVS/AES IDs are in conflict
#define MVS_AS_AES			((volMEMBYTE(REG_STATUS_B) & MVS_OR_AES) && !isMVS)
#define AES_AS_MVS			(!(volMEMBYTE(REG_STATUS_B) & MVS_OR_AES) && isMVS)

#define BIOS_PLAYER_MOD1	0x10FDB6
#define BIOS_PM_INIT		0
#define BIOS_PM_PLAYING		1
#define BIOS_PM_CONTINUE	2
#define BIOS_PM_GAMEOVER	3

#define BIOS_TITLE_MODE		0x10FEC5

#define BIOS_NM_CREDIT		0xD00034	// Backup data, only MVS
#define BIOS_CREDIT_DB		0x10FE00	// Debug version, no backup, works in AES with UNIBIOS
#define BIOS_COMP_TIME		0x10FDDA

// BIOS Calls
#define BIOS_FIX_CLEAR	__asm__ ("jsr 0xC004C2 \n")
#define RETURN_TO_BIOS	__asm__ ("jmp 0xc00444 \n")
#define BIOS_READ_CLDR	__asm__ ("jmp 0xC0045C \n")

#define SOFT_DIP_1			0x10FD8A	// ENABLE MVS DEMO
#define SOFT_DIP_2			0x10FD8B	// DISPLAY AVAILABLE CREDITS
#define SOFT_DIP_3			0x10FD8C	// FORCE 4P CONTROLLER TEST
#define SOFT_DIP_4			0x10FD8D	// ENABLE 320 HORIZONTAL RESOLUTION AS DEFAULT
#define SOFT_DIP_5			0x10FD8E	// ENABLE WARNINGS

#define BIOS_ADDRESS		0xC00000
#define BIOS_SIZE			0x20000

#define REG_LSPCMODE		0x3C0006
#define LPSC2_NTSC_PAL		0x08

// 4 player related
#define BIOS_4P_REQUESTED	0x10FEF8
#define BIOS_4P_MODE		0x10FEFA
#define BIOS_4P_PLUGGED		0x10FEFB

// Slots
#define BIOS_SLOTS_DETECTED	0xD00047	

#define REG_NOSHADOW		0x3A0001
#define REG_SHADOW			0x3A0011

// Colors
#define WH_107	0x7fff
#define WH_100	0xffff
#define	_BLACK	0x8000
#define	___RED	0x4f00
#define _GREEN	0x20f0
#define __BLUE	0x100f
#define __CYAN	0x30ff
#define MAGENT	0x5f0f
#define YELLOW	0x6ff0

#define IRE2_5	0xf000
#define IRE4_5	0x8111
#define IRE7_5	0xf111
#define IRE_10	0x8222
#define IRE_20	0x7333
#define IRE_24	0x0444
#define IRE_40	0x8777
#define IRE_50	0x8888
#define IRE_75	0x8ccc
#define IRE_80	0x8ddd

// TEMP... sticking these here for now 
#define FIX32_INT_BITS			22
#define FIX32_FRAC_BITS			(32 - FIX32_INT_BITS)

#define FIX32_INT_MASK			(((1 << FIX32_INT_BITS) - 1) << FIX32_FRAC_BITS)
#define FIX32_FRAC_MASK			((1 << FIX32_FRAC_BITS) - 1)

#define FIX32(value)			((fix32) ((value) * (1 << FIX32_FRAC_BITS)))

#define fix32Div(val1, val2)	(((val1) << (FIX32_FRAC_BITS / 2)) / ((val2) >> (FIX32_FRAC_BITS / 2)))

#define intToFix32(value)		((fix32) ((value) << FIX32_FRAC_BITS))
#define fix32ToInt(value)		((s32) ((value) >> FIX32_FRAC_BITS))
#define fix32Frac(value)		((value) & FIX32_FRAC_MASK)
#define fix32Mul(val1, val2)	(((val1) >> (FIX32_FRAC_BITS / 2)) * ((val2) >> (FIX32_FRAC_BITS / 2)))
#define fix32Div(val1, val2)	(((val1) << (FIX32_FRAC_BITS / 2)) / ((val2) >> (FIX32_FRAC_BITS / 2)))

#endif /* _NG_H_ */
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

extern BYTE isMVS, is4S, is6S;

#define SYSTEM_AES 0x00
#define SYSTEM_MVS 0x80
#define BIOS_MVS_FLAG 0x10FD82
#define BIOS_COUNTRY_CODE 0x10FD83

#define SYSTEM_EUROPE 0x02
#define SYSTEM_JAPAN 0x00
#define SYSTEM_USA 0x01

#define fontColorWhite 0
#define fontColorRed   1
#define fontColorGreen 2
#define fontColorBlue  3

#define REG_SYSTYPE 0x300081
#define MVS_MULTI	0x40
#define MVS_TEST_B	0x80

#define REG_STATUS_A 0x320001
#define P1_CREDIT	0x01
#define P2_CREDIT	0x02
#define MVS_SERV_B	0x04
#define P3_CREDIT	0x08
#define P4_CREDIT	0x10
#define MVS_4_OR_6	0x20
#define RTC_TIME	0x40
#define RTC_DATA	0x80

#define REG_STATUS_B 0x380000
#define MVS_P1_STRT	0x01
#define MVS_SEL1	0x02
#define MVS_P2_STRT	0x04
#define MVS_SEL2	0x08
#define HAS_MC_1	0x10
#define HAS_MC_2	0x20
#define MC_PTRCT	0x40
#define MVS_OR_AES	0x80

#define BIOS_USER_REQS 0x10FDAE
#define BIOS_USER_MODE 0x10FDAF

#define SOFT_DIP_1	0x10FD8A

#endif /* _NG_H_ */
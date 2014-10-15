/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
 
 This version of the suite is compiled with HuC from https://github.com/uli/huc
 
 */
 
#include "huc.h"

extern int Enabled240p;

void Set224p()
{
	Enabled240p = 0;
// VSR/VPR 0C 17
// VDW DF 00
#asm

	cly 
	ldy   #$0C		; select the VDC register
	tya
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	ldy   #$02		; send the 16-bit data
	tya
	sta   video_data_l
	sta   _vdc,X
	ldy   #$17
	tya
	sta   video_data_h
	sta   _vdc+1,X
	
	cly 
	ldy   #$0D		; select the VDC register
	tya
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	ldy   #$DF		; send the 16-bit data
	tya
	sta   video_data_l
	sta   _vdc,X
	ldy   #$00
	tya
	sta   video_data_h
	sta   _vdc+1,X
	
	rts
#endasm	
}

void Set240p()
{
	Enabled240p = 1;
// VSR/VPR 0D 02
// VDW EF 00
#asm
	cly 
	ldy   #$0C		; select the VDC register
	tya
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	ldy   #$02		; send the 16-bit data
	tya
	sta   video_data_l
	sta   _vdc,X
	ldy   #$0D
	tya
	sta   video_data_h
	sta   _vdc+1,X
	
	cly 
	ldy   #$0D		; select the VDC register
	tya
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	ldy   #$EF		; send the 16-bit data
	tya
	sta   video_data_l
	sta   _vdc,X
	ldy   #$00
	tya
	sta   video_data_h
	sta   _vdc+1,X
	
	rts
#endasm	
}


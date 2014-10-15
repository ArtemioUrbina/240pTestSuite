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
// VSR/VPR 02 17
// VDW DF 00
#asm

	lda   #$0C
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02
	sta   video_data_l
	sta   _vdc,X
	lda   #$17
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0D
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$DF
	sta   video_data_l
	sta   _vdc,X
	lda   #$00
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
	lda   #$0C
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02
	sta   video_data_l
	sta   _vdc,X
	lda   #$0D
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0D
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$EF
	sta   video_data_l
	sta   _vdc,X
	lda   #$00
	sta   video_data_h
	sta   _vdc+1,X
	
	rts
#endasm	
}


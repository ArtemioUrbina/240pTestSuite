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

int xres_flags;
unsigned char Enabled240p;
unsigned char UseDefault;
unsigned char EnabledSoft;
unsigned char Enabled_C_BW;

int controller;   
int read; 

unsigned char redraw;
unsigned char refresh;

int row;
int x;
int x1;
int x2;
int x3;
int x4;
int y;
int y2;

int speed;
int color;
int clicks[10];
unsigned char text;
unsigned char variation;
unsigned char change;
unsigned char audio;
unsigned char view;
int vary;

unsigned char draw;
int top;
int bottom;
int left;
int right;
unsigned char previous;
unsigned char screen;


void ResetVideo()
{
#ifdef CDROM1
	gfx_clear(0x1000);
#endif
	cls();

	init_satb();
	satb_update();
			
	ResetScroll();
	set_xres(320, xres_flags);
	set_screen_size(SCR_SIZE_64x32);
}

void ResetScroll()
{
	scroll(0, 0, 0, 0, 240, 0xC0);
	scroll(1, 0, 0, 0, 240, 0xC0);
	scroll(2, 0, 0, 0, 240, 0xC0);
	scroll(3, 0, 0, 0, 240, 0xC0);
}


void Center224in240()
{
	if(!Enabled240p)
		scroll(0, 0, 8, 0, 240, 0xC0);
}



#ifndef HELP_OVL

void Set224p()
{
	Enabled240p = 0;
	vsync();
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

/*
	This shows the full 240 lines, however it starts at line 22
	and is not used in comemrcial games. Line 21 containc closed
	captioned data in NTSC, and teh signal typicaly starts at 
	line 23.
*/

void Set240p()
{
	Enabled240p = 1;
	vsync();
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

/*
	This is the typical setting in PC Engine games,
	since it respects the NTSC standard of starting 
	at line 23. However this displays only 239 active 
	lines of video signal.
*/

void Set239p() 
{
	Enabled240p = 1;
	vsync();
// VSR/VPR 0F 02
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
	lda   #$0F
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

void SetArcadeMode()
{
	vsync();
// HSR 09 03
// HDR 0A 1F
#asm

	lda   #$0A
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$03
	sta   video_data_l
	sta   _vdc,X
	lda   #$09
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0B
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$1F
	sta   video_data_l
	sta   _vdc,X
	lda   #$0A
	sta   video_data_h
	sta   _vdc+1,X
	
	rts
#endasm	
}

void SetNormalHMode()
{
	vsync();
// HSR 02 02
// HDR 04 1F
#asm

	lda   #$0A
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02
	sta   video_data_l
	sta   _vdc,X
	lda   #$02
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0B
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$1F
	sta   video_data_l
	sta   _vdc,X
	lda   #$04
	sta   video_data_h
	sta   _vdc+1,X
	
	rts
#endasm	
}

/* from obeybrew.com */
void spr_make(int spriteno, int spritex, int spritey, int spritepattern, int ctrl1, int ctrl2, int sprpal, int sprpri)
{
	spr_set(spriteno);
	spr_x(spritex);
	spr_y(spritey);
	spr_pattern(spritepattern);
	spr_ctrl(ctrl1, ctrl2);
	spr_pal(sprpal);
	spr_pri(sprpri);
}

#endif // HELP_OVL

#ifdef CDROM

extern int xres_flags_g;
extern unsigned char Enabled240p_g;
extern unsigned char UseDefault_g;
extern unsigned char EnabledSoft_g;
extern unsigned char Enabled_C_BW_g;

void RestoreGlobals()
{
	xres_flags = xres_flags_g;
	Enabled240p = Enabled240p_g;
	UseDefault = UseDefault_g;
	EnabledSoft = EnabledSoft_g;
	Enabled_C_BW = Enabled_C_BW_g;
}
#endif

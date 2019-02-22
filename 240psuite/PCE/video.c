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

#ifdef CDROM1
#include "graphics.h"
#endif

void ResetVideo()
{
#ifdef CDROM1
	set_map_data(fs_map, 64, 32);
	set_tile_data(white_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 32);
	set_color_rgb(1, 0, 0, 0); 
#endif

	cls();

	init_satb();
	satb_update();
			
	ResetScroll();
	Set320H();

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
//VSR/VPR 17 02
//VDR/VDW 00 DF
//VCR     00 0A
#asm
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
							
	lda   #$0C				; VPR - 'Vertical synchronous register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02				; 'VSW' Vertical synchronous pulse width.
	sta   video_data_l
	sta   _vdc,X
	lda   #$17				; 'VDS' Vertical display start position -2.
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0D				; VDW - 'Vertical display register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$DF				; Vertical display width in pixels -1.
	sta   video_data_l
	sta   _vdc,X
	
	lda   #$0E				; VCR - 'Vertical display END position register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$0A				; Vertical display end position.
	sta   video_data_l
	sta   _vdc,X
	
	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
#endasm	
}

/*
	This shows the full 240 lines, however it starts at line 22
	and is not used in commercial games. Line 21 contains closed
	captioned data in NTSC, and the signal typicaly starts at 
	line 23.
*/

void Set240p()
{
	Enabled240p = 1;	
//VSR/VPR 0D 02
//VDR/VDW 00 EF
//VCR     00 04
#asm
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
							
	lda   #$0C				; VPR - 'Vertical synchronous register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02				; 'VSW' Vertical synchronous pulse width.
	sta   video_data_l
	sta   _vdc,X
	lda   #$0E				; 'VDS' Vertical display start position -2.
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0D				; VDW - 'Vertical display register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$EF				; Vertical display width in pixels -1.
	sta   video_data_l
	sta   _vdc,X
	
	lda   #$0E				; VCR - 'Vertical display END position register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$04				; Vertical display end position.
	sta   video_data_l
	sta   _vdc,X
	
	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
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
//VSR/VPR 0F 02
//VDR/VDW 00 EF
//VCR     00 04
#asm
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
	
	lda   #$0C				; VPR - 'Vertical synchronous register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02				; 'VSW' Vertical synchronous pulse width.
	sta   video_data_l
	sta   _vdc,X
	lda   #$0F				; 'VDS' Vertical display start position -2.
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0D				; VDW - 'Vertical display register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$EF				; Vertical display width in pixels -1.
	sta   video_data_l
	sta   _vdc,X
	
	lda   #$0E				; VCR - 'Vertical display END position register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$04				; Vertical display end position.
	sta   video_data_l
	sta   _vdc,X
	
	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
#endasm	
}

void Set256H()
{
	//HSR     0202
	//HDR     041F

#asm
	lda	_xres_flags
	sta	<cl	
	
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
	
	lda	#$00				; dot-clock values, 256: 0, 320: 1, 512 2
	ora	<cl
	sta	color_ctrl			; dot-clock (x-resolution)

	lda	#$0a				; HSR - 'Horizontal Sync Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$02				; 'HSW' Horizontal synchronous pulse width
	sta	video_data_l
	sta	_vdc,X
	lda	#$02				; 'HDS' Horizontal display start position -1
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$0b				; HDR - 'Horizontal Display Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$1F				; 'HDW' Horizontal display width in tiles -1.				
	sta	video_data_l
	sta	_vdc,X
	lda	#$04				; 'HDE' Horizontal display ending period -1.
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
#endasm	

	if(Enabled240p)
	{
		if(UseDefault)
			Set239p();
		else
			Set240p();
	}
	else
		Set224p();
}


void Set352H()
{
//R-Type
//HSR     0303
//HDR     062B
//VSR/VPR 0F02
//VDR/VDW 00EF
//VCR     0003

#asm
	lda	_xres_flags
	sta	<cl	
	
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
	
	lda	#$01				; dot-clock values, 256: 0, 320: 1, 512 2
	ora	<cl
	sta	color_ctrl			; dot-clock (x-resolution)

	lda	#$0a				; HSR - 'Horizontal Sync Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$03				; 'HSW' Horizontal synchronous pulse width
	sta	video_data_l
	sta	_vdc,X
	lda	#$03				; 'HDS' Horizontal display start position -1
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$0b				; HDR - 'Horizontal Display Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$2B				; 'HDW' Horizontal display width in tiles -1.				
	sta	video_data_l
	sta	_vdc,X
	lda	#$06				; 'HDE' Horizontal display ending period -1.
	sta	video_data_h
	sta	_vdc+1,X
	
	lda   #$0C				; VPR - 'Vertical synchronous register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$02				; 'VSW' Vertical synchronous pulse width.
	sta   video_data_l
	sta   _vdc,X
	lda   #$0F				; 'VDS' Vertical display start position -2.
	sta   video_data_h
	sta   _vdc+1,X
	
	lda   #$0D				; VDW - 'Vertical display register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$EF				; Vertical display width in pixels -1.
	sta   video_data_l
	sta   _vdc,X
	
	lda   #$0E				; VCR - 'Vertical display END position register'
	sta   <vdc_reg
	sta   video_reg
	asl   A
	tax
	lda   #$03				; Vertical display end position.
	sta   video_data_l
	sta   _vdc,X


	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
#endasm	
}


// Soldier Blade Arcade Mode 256H
// HSR 09 03
// HDR 0A 1F


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

void Set320H()
{
//HSR     0503
//HDR     0627

#asm
	lda	_xres_flags
	sta	<cl	
	
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
	
	lda	#$01				; dot-clock values, 256: 0, 320: 1, 512 2
	ora	<cl
	sta	color_ctrl			; dot-clock (x-resolution)

	lda	#$0a				; HSR - 'Horizontal Sync Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$03				; 'HSW' Horizontal synchronous pulse width
	sta	video_data_l
	sta	_vdc,X
	lda	#$05				; 'HDS' Horizontal display start position -1
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$0b				; HDR - 'Horizontal Display Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$27				; 'HDW' Horizontal display width in tiles -1.				
	sta	video_data_l
	sta	_vdc,X
	lda	#$06				; 'HDE' Horizontal display ending period -1.
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
#endasm	

#ifndef HELP_OVL
	if(Enabled240p)
	{
		if(UseDefault)
			Set239p();
		else
			Set240p();
	}
	else
		Set224p();
#endif
}

void Set512H()
{
//HSR     0A05 -> 0C03
//HDR     083F
#asm
	lda	_xres_flags
	sta	<cl	
	
	lda	#$20				; reset resource-usage flag
	tsb	<irq_m				; to skip joystick read portion of vsync
							; (temporarily disable VSYNC processing)
	
	lda	#$02				; dot-clock values, 256: 0, 320: 1, 512 2
	ora	<cl
	sta	color_ctrl			; dot-clock (x-resolution)

	lda	#$0a				; HSR - 'Horizontal Sync Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$03				; 'HSW' Horizontal synchronous pulse width, R-Type uses 03
	sta	video_data_l
	sta	_vdc,X
	lda	#$0C				; 'HDS' Horizontal display start position -1, R-Type uses 03
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$0b				; HDR - 'Horizontal Display Register' 
	sta	<vdc_reg
	sta	video_reg
	asl	A
	sax
	lda	#$3F				; 'HDW' Horizontal display width in tiles -1.
	sta	video_data_l
	sta	_vdc,X
	lda	#$08				; 'HDE' Horizontal display ending period -1.
	sta	video_data_h
	sta	_vdc+1,X


	lda	#$20
	trb	<irq_m				; re-enable VSYNC processing
#endasm

#ifndef HELP_OVL
	if(Enabled240p)
	{
		if(UseDefault)
			Set239p();
		else
			Set240p();
	}
	else
		Set224p();
#endif
}


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

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
#include "font.h"
#include "video.h"
#include "help.h"
#include "tools.h"

#ifndef CDROM1
#include "graphics.h"
#endif

#ifdef CDROM1 || HELP_OVL
#include "gdata.h"
extern char palCD[];
#endif

#ifndef CDROM1
#ifndef HELP_OVL
void showHelp(char data)
#else
#include "gdata.h"
#incchr(my_font, "graphics/font.pcx", 32, 3);
char palCD[512];

void main()
#endif
{
	unsigned char end = 0;
	unsigned char page = 1;
	unsigned char total = 1;
	unsigned char count = 1;

#ifdef HELP_OVL
	char data;
	
	RestoreGlobals();
	data = HelpItem;
#endif
	redraw = 1;
	refresh = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			setupFont();
			
			SetFontColors(13, RGB(3, 3, 3), RGB(0, 6, 0), 0);
			SetFontColors(15, RGB(2, 2, 2), RGB(0, 6, 0), 0);

#ifndef HELP_OVL
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_palette(0, MB512_pal, 1);  
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_back512_PAL_bin, palCD, SIZE_back512_PAL_bin); 
			set_bgpal(0, palCD); 
			cd_loadvram(GPHX_OVERLAY, OFS_back512_DATA_bin, 0x1000, SIZE_back512_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
#endif
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
			set_xres(512, xres_flags);
        }
		
		if(refresh)
		{
#ifndef HELP_OVL
			set_map_data(MB512_map, 64, 30);
			load_map(0, 0, 0, 0, 64, 30);
#else
			cd_loadvram(GPHX_OVERLAY, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
#endif
		
			set_font_pal(15);
			switch(data)
			{
				case COLOR601_HELP:
					display_color601_00();
					break;
				case COLORS_HELP:
					total = 2;
					if(page == 1)
						display_colors_00();
					else if (page == 2)
						display_colors_01();
					break;
				case DSHADOW_HELP:
					display_dshadow_00();
					break;
				case GRAY_HELP:
					display_gray_00();
					break;
				case GRID_HELP:
				case GRID_HELP2:
				case GRID_HELP3:
					display_grid_00();
					break;
				case GRIDSCROLL_HELP:
					display_gridscroll_00();
					break;
				case LINEARITY_HELP:
					display_linearity_00();
					break;
				case LINEARITY256_HELP:
					display_linearity256_00();
					break;
				case MANUALLAG_HELP:
					total = 2;
					if(page == 1)
						display_manuallag_00();
					else if(page == 2)
						display_manuallag_01();
					break;
				case OPTIONS_HELP:
				case OPTIONS_HELP2:
					display_options_00();
					break;
				case OVERSCAN_HELP:
					display_overscan_00();
					break;
				case PLUGE_HELP:
					total = 2;
					if(page == 1)
						display_pluge_00();
					else if(page == 2)
						display_pluge_01();
					break;
				case SCROLL_HELP:
					display_scroll_00();
					break;
				case SHARPNESS_HELP:
					display_sharpness_00();
					break;
				case SMPTECOLOR_HELP:
					display_SMPTEColor_00();
					break;
				case SOUND_HELP:
					display_sound_00();
					break;
				case STRIPED_HELP:
					display_striped_00();
					break;
				case WHITE_HELP:
					display_white_00();
					break;	
				case GENERAL_HELP:
				case GENERAL_HELP2:
					total = 2;
					DrawGeneralHelp(page);
					break;
				case BACKLIT_HELP:
					DrawBacklitHelp();
					break;
				case BLEED_HELP:
					DrawBleedHelp();
					break;
				case CHECK_HELP:
					DrawChecksHelp();
					break;
				case STRIPES_HELP:
					total = 2;
					DrawStripesHelp(page);
					break;
				case PASSIVE_HELP:
					DrawPassiveLagHelp(page);
					break;
				case IRE100HELP:
					DrawIre100Help();
					break;
				case AUDIOSYNC_HELP:
					display_audiosync_00();
					break;
			}
			
			set_font_pal(13);
			if(total > 1 && page != total)
				put_string("(cont...)", 50, 25);

			refresh = 0;
		}

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_RUN)
			end = 1;
			
		if (controller & JOY_LEFT)
			count--;
		
		if (controller & JOY_RIGHT)
			count++;
			
		if(count > total)
			count = total;
		if(count < 1)
			count = 1;
			
		if(page != count)
		{
			refresh = 1;
			page = count;
		}
    }	
	disp_off();
	read = controller = 0;
#ifdef HELP_OVL
	if(data < HELP_PART_2)
		cd_execoverlay(MAIN_OVERLAY);
	else
		cd_execoverlay(PATTERNS_OVERLAY);
#endif
}

#else

extern int HelpItem;

void showHelp(char data)
{
	HelpItem = data;
	cd_execoverlay(HELP_OVERLAY);
}

#endif

#ifndef CDROM1
/* This is defined in C to use up the constants bank */
void DrawGeneralHelp(char page)
{	
	row = 5;
	
	switch(page)
	{
		case 1:
			put_string("HELP (1/2)", 26, row++);
			set_font_pal(14);
			row++;
			put_string("The 240p Test Suite was designed with two goals in", 6, row++);
			put_string("mind:", 6, row++);
			row++;
			put_string("1) Evaluate 240p signal processing on modern TV", 6, row++);
			put_string("sets and video processing equipment; and", 6, row++);
			row++;
			put_string("2) Provide calibration patterns generated on your", 6, row++);
			put_string("game console to help in properly calibrating the", 6, row++);
			put_string("display's black, white and color levels.", 6, row++);
			row++;
			put_string("Help is available everywhere by pressing RUN.", 6, row++);
			put_string("Use right and left in the d-pad to navigate between", 6, row++);
			put_string("help pages. Options can be changed via SELECT", 6, row++);
			put_string("when in the main menus.",6, row++);
			row++;
			DetectPCEType();
			if(region == 0x0B)
				put_string("The TurboGrafx-16 version of the suite is in", 6, row++);
			else
				put_string("The PC Engine version of the suite is", 6, row++);
			put_string("320x240 by default, but can be changed to 320x224p.", 6, row++);
			break;
		case 2:
			put_string("HELP (2/2)", 26, row++);
			set_font_pal(14);
			row++;
			put_string("Grids and other screens are also available in", 6, row++);
			put_string("256x224, 256x240, 512x224 and 512x240 resolutions.", 6, row++);
			row++;
			put_string("When in 240p mode the system can start drawing", 6, row++);
			put_string("from video line 22, which is the default of this", 6, row++);
			put_string("suite in order to display all 240 lines.", 6, row++);
			row++;
			put_string("However all games start from line 24 when in 240p", 6, row++);
			put_string("mode, showing only 239 visible lines. You can", 6, row++);
			put_string("select that mode from the options menu in order to", 6, row++);
			put_string("display the regular signal you'll encounter when", 6, row++);
			put_string("using this console.", 6, row++);
			row++;

			put_string("The 240p suite is also available in other systems,", 6, row++);
			put_string("including: Sega Genesis/Mega Drive, Sega CD/", 6, row++);
			put_string("Mega CD, NES, Super Nintendo, Nintendo GameCube,", 6, row++);
			put_string("Nintendo Wii and Sega Dreamcast.", 6, row++);
			row++;
			put_string("Full info available at:", 6, row);
			set_font_pal(13);
			put_string("http://junkerhq.net/240p/", 30, row);
			
#ifdef HELP_OVL
			set_font_pal(13);
			x = cd_getver() >> 8;
			if(x > 2)
				put_string("With Sys Card 3 you can use the SCD ver of the suite.", 6, ++row);
#endif
			break;
	}
}

void DrawBacklitHelp()
{
	row = 5;
	
	put_string("BACKLIT TEST", 26, row++);
	set_font_pal(14);
	row++;
	put_string("This test allows you to check how the display's", 6, row++);
	put_string("backlit works when only a small array of pixels", 6, row++);
	put_string("is shown. This can be very revealing, and can", 6, row++);
	put_string("be used to detect the zones in LED backlit", 6, row++);
	put_string("displays, or to evaluate global and local dimming.", 6, row++);
	row++;
	put_string("The user can move around the white pixel arrays", 6, row++);
	put_string("with the d-pad or joystiq, and change the size", 6, row++);
	put_string("of the pixel array with button I. The SELECT", 6, row++);
	put_string("button allows the user to hide the pixel array", 6, row++);
	put_string("in order to alternate a fully black screen.", 6, row++);
	row++;
	put_string("It might be necessary to have a fully-off panel", 6, row++);
	put_string("(if possible) when displaying the black", 6, row++);
	put_string("background,  in order to evaluate full-backlit", 6, row++);
	put_string("displays by adjusting brightness accordingly", 6, row++);
	put_string("for this test.", 6, row++);
}

void DrawChecksHelp()
{
	row = 5;
					
	put_string("CHECKERBOARD", 26, row++);
	set_font_pal(14);
	row++;
	put_string("This pattern shows all the visible pixels in an", 6, row++);
	put_string("alternating white and black grid array.", 6, row++);
	row++;
	put_string("You can toggle the pattern with SELECT, or turn", 6, row++);
	put_string("on auto-toggle each frame with the I button.", 6, row++);
	row++;
	put_string("When auto-toggle is set, you should see it", 6, row++);
	put_string("alternating rapidly. On some setups, the pattern", 6, row++);
	put_string("doesn't change at all. This means that the signal", 6, row++);
	put_string("is being treated as 480i/576i and odd or even", 6, row++);
	put_string("frames are being discarded completely.", 6, row++);
	row++;
	put_string("A frame counter can be displayed on screen by", 6, row++);
	put_string("pressing left on the d-pad.", 6, row++);
}

void DrawBleedHelp()
{
	row = 5;
	
	put_string("COLOR BLEED", 26, row++);
	set_font_pal(14);
	row+=3;
	put_string("This pattern helps diagnose unneeded color", 6, row++);
	put_string("upsampling. It consists of one pixel width color", 6, row++);
	put_string("bars alternating with one pixel width black bars.", 6, row++);
	row++;
	put_string("You can change the vertical bars to a checker", 6, row++);
	put_string("board with button I.", 6, row++);
}

void DrawStripesHelp(char page)
{	
	row = 5;
	
	switch(page)
	{
		case 1:
			put_string("HORIZONTAL STRIPES (1/2)", 19, row++);
			set_font_pal(14);
			row++;
			put_string("This pattern is designed to show if all lines are", 6, row++);
			put_string("visible in your setup, and it handles 240p signals.", 6, row++);
			row++;
			put_string("You should see a pattern of lines, each one pixel", 6, row++);
			put_string("in height, starting with a white one at the top of", 6, row++);
			put_string("the screen. You can toggle the pattern with", 6, row++);
			put_string("SELECT, or turn on auto-toggle each frame with I.", 6, row++);
			row++;
			put_string("When auto-toggle is set, you should see the lines", 6, row++);
			put_string("alternating rapidly. On some setups, the pattern", 6, row++);
			put_string("doesn't change at all. This means that the signal", 6, row++);
			put_string("is being treated as 480i and odd or even",6, row++);
			put_string("frames are being discarded completely.", 6, row++);
			row++;
			put_string("A frame counter can be displayed on screen by", 6, row++);
			put_string("pressing left.", 6, row++);
			break;
		case 2:
			put_string("HORIZONTAL STRIPES (2/2)", 19, row++);
			set_font_pal(14);
			row++;
			put_string("You can also display vertical bars by pressing up,", 6, row++);
			put_string("that pattern will help you evaluate if the signal", 6, row++);
			put_string("is not distorted horizontaly, since all lines", 6, row++);
			put_string("should be one pixel wide.", 6, row++);
			break;
	}
}


void DrawPassiveLagHelp()
{
	row = 5;
					
	put_string("LAG TEST", 26, row++);
	set_font_pal(14);
	row++;
	put_string("This test is designed to be used with two displays", 6, row++);
	put_string("conected at the same time. One being a CRT, or a", 6, row++);
	put_string("display with a known lag as reference, and the", 6, row++);
	put_string("other the display to test.", 6, row++);
	row++;
	put_string("Using a camera, a picture should be taken of both", 6, row++);
	put_string("screens at the same time. The picture will show", 6, row++);
	put_string("the frame discrepancy between them even when the", 6, row++);
	put_string("numbers are blurry.", 6, row++);
	row++;
	put_string("The Sega Genesis version of the test can be used", 6, row++);
	put_string("with a Nomad as the reference display.", 6, row++);
	row++;
	put_string("You can also chain the output ports of a PVM/BVM", 6, row++);
	put_string("to an upscaler and use the CRT as reference, or", 6, row++);
	put_string("split the video signal.", 6, row++);
}

void DrawIre100Help()
{
	row = 5;
	
	put_string("100 IRE", 26, row++);
	set_font_pal(14);
	row+=2;
	put_string("An IRE is a unit used in the measurement of", 6, row++);
	put_string("composite video signals. Its name is derived from", 6, row++);
	put_string("the initials of the Institute of Radio Engineers.", 6, row++);
	row++;
	put_string("A value of 100 IRE was originally defined to be", 6, row++);
	put_string("the range from black to white in a video signal.", 6, row++);
	put_string("A value of 0 IRE corresponds to the zero voltage", 6, row++);
	put_string("value during the blanking period. The sync pulse", 6, row++);
	put_string("is normally 40 IRE below the zero value, so, peak", 6, row++);
	put_string("to peak, an all white signal should be equal to", 6, row++);
	put_string("140 IRE.", 6, row++);
	row++;
	put_string("You can press left and right to advance in IRE", 6, row++);
	put_string("steps in the selected range.", 6, row++);
	put_string("Button I toggles the range between [0-100] and", 6, row++);
	put_string("[100-140] IRE.", 6, row++);
}

#endif

#asm
p_string	.macro
	__ldwi	\1
	__stw	si
	__ldwi	\2
	  stx	bl
	__ldwi	\3
	call	_put_string.3
	.endm
#endasm

#ifndef CDROM1
#include "helpdata.c"
#endif

/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
#include "huc.h"
#include "font.h"
#include "video.h"
#include "help.h"
#include "tools.h"

#ifndef SYSCARD1
#include "graphics.h"
#endif

#ifdef SYSCARD1
#include "gdata.h"
extern char palCD[];
#endif

#ifndef SYSCARD1
#ifndef HELP_OVL
void showHelp(char data)
#else
#include "gdata.h"
#incchr(my_font, "graphics/font.pcx", 32, 3);
char palCD[512];

void main()
#endif
{
#ifdef HELP_OVL
	char data;
	
	RestoreGlobals();
	data = HelpItem;
#endif

	right = 1;	// page
	bottom = 1;	// total pages


	end = 0;
	redraw = 1;
	refresh = 1;
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			Rewdraw512Menu();
			
			SetFontColors(13, RGB(3, 3, 3), RGB(0, 6, 0), 0);
			SetFontColors(15, RGB(2, 2, 2), RGB(0, 6, 0), 0);
			
            redraw = 0;
			refresh = 1;
			disp_sync_on();

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
					bottom = 2;
					if(right == 1)
						display_colors_00();
					else if (right == 2)
						display_colors_01();
					break;
				case DSHADOW_HELP:
					display_dshadow_00();
					break;
				case GRAY_HELP:
					display_gray_00();
					break;
				case GRID_HELP:
					display_grid_00();
					break;
				case GRIDSCROLL_HELP:
					display_gridscroll_00();
					break;
				case MONOSCOPE_HELP:
					bottom = 2;
					if(right == 1)
						display_monoscope_00();
					else if (right == 2)
						display_monoscope_01();
					break;
				case REFLEX_HELP:
					bottom = 2;
					if(right == 1)
						display_reflex_00();
					else if(right == 2)
						display_reflex_01();
					break;
				case OPTIONS_HELP:
				case OPTIONS_PAT_HELP:
				case OPTIONS_VID_HELP:
				case OPTIONS_AUD_HELP:
				case OPTIONS_HW_HELP:
					display_options_00();
					break;
				case OVERSCAN_HELP:
					display_overscan_00();
					break;
				case PLUGE_HELP:
					bottom = 2;
					if(right == 1)
						display_pluge_00();
					else if(right == 2)
						display_pluge_01();
					break;
				case SCROLL_HELP:
					display_scroll_00();
					break;
				case SHARPNESS_HELP:
					display_sharpness_00();
					break;
				case SMPTECOLOR_HELP:
					DrawSMPTEHelp();
					break;
				case EBU_HELP:
					display_ebucb_00();
					break;
				case SOUND_HELP:
#ifdef CDROM
					display_soundCD_00();
#else
					display_sound_00();
#endif
					break;
				case STRIPED_HELP:
					display_striped_00();
					break;
				case WHITE_HELP:
					display_white_00();
					break;	
				case GENERAL_HELP:
				case GENERAL_PAT_HELP:
				case GENERAL_VID_HELP:
				case GENERAL_AUD_HELP:
				case GENERAL_HW_HELP:
					bottom = 2;
					DrawGeneralHelp(right);
					break;
				case BACKLIT_HELP:
					display_backlit_00();
					break;
				case BLEED_HELP:
					display_colorbleed_00();
					break;
				case CHECK_HELP:
					display_check_00();
					break;
				case VERT_STRIPES_HELP:
					DrawVertStripesHelp();
					break;
				case HORI_STRIPES_HELP:
					DrawHoriStripesHelp();
					break;
				case PASSIVE_HELP:
					DrawPassiveLagHelp();
					break;
				case IRE100HELP:
					DrawIre100Help();
					break;
				case AUDIOSYNC_HELP:
					display_audiosync_00();
					break;
				case MDFOURIER_HELP:
					display_mdfourier_00();
					break;
				case CONSINTER_HELP:
					DrawConsInter();
					break;
				case CONVERGENCE_HELP:
					display_convergence_00();
					break;
				case MEMVIEW_HELP:
					display_memory_00();
					break;
				case PHASE_HELP:
					display_phase_00();
					break;
				case DISAP_HELP:
					display_disappear_00();
					break;
			}
			
			set_font_pal(13);
			if(bottom > 1 && right != bottom)
				put_string("(cont...)", 50, 24);

			refresh = 0;
		}

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_RUN)
			end = 1;
			
		if (controller & JOY_LEFT)
		{
			right--;
			if(right < 1)
				right = 1;
			else
				refresh = 1;
		}
		
		if (controller & JOY_RIGHT)
		{
			right++;
			if(right > bottom)
				right = bottom;
			else
				refresh = 1;
		}
		
    }	
	disp_off();
	end = read = controller = 0;
#ifdef HELP_OVL
	if(data >= PATTERNSHELP && data < VIDEOHELP)
		cd_execoverlay(PATTERNS_OVERLAY);
	else if(data >= HARDWAREHELP)
	{
		ToolItem = HARDWARETOOL;
		cd_execoverlay(TEST_SND_OVERLAY);
	}
	else
		cd_execoverlay(MAIN_OVERLAY);
#endif
}

#else

#ifndef HELP_OVL
extern int HelpItem;
#endif

void showHelp(char data)
{
	HelpItem = data;
	cd_execoverlay(HELP_OVERLAY);
}

#endif

#ifndef SYSCARD1
/* This is defined in C to use up the constants bank */
void DrawGeneralHelp(char right)
{	
	row = 4;
	
	switch(right)
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
			put_string("help rights. Options can be changed via SELECT", 6, row++);
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
			put_string("Grids and other screens are available in 256x224,", 6, row++);
			put_string("256x240, 352x240, 512x224 and 512x240 resolutions.", 6, row++);
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
			break;
	}
}

void DrawVertStripesHelp()
{	
	row = 4;
	
	put_string("VERTICAL STRIPES", 26, row++);
	set_font_pal(14);
	row++;
	put_string("You should see a pattern of lines, each one pixel", 6, row++);
	put_string("in width. This can help you evaluate if the signal", 6, row++);
	put_string("is not distorted horizontaly, since all lines", 6, row++);
	put_string("should be one pixel wide.", 6, row++);
	row++;
	put_string("- You can switch between all supported resolutions", 6, row++);
	put_string("by pressing SELECT.", 6, row);
	row++;
	put_string("- A frame counter can be displayed on screen by", 6, row++);
	put_string("pressing left.", 6, row++);
}

void DrawHoriStripesHelp()
{	
	row = 4;
	
	put_string("HORIZONTAL STRIPES", 24, row++);
	set_font_pal(14);
	row++;
	put_string("This pattern is designed to show if all lines are", 6, row++);
	put_string("visible in your setup, and it handles 240p signals.", 6, row++);
	row++;
	put_string("You should see a pattern of lines, each one pixel", 6, row++);
	put_string("in height, starting with a white one at the top of", 6, row++);
	put_string("the screen. You can toggle the pattern with", 6, row++);
	put_string("Right, or turn on auto-toggle each frame with I.", 6, row++);
	row++;
	put_string("When auto-toggle is set, you should see the lines", 6, row++);
	put_string("alternating rapidly. On some setups, the pattern", 6, row++);
	put_string("doesn't change at all. This means that the signal", 6, row++);
	put_string("is being treated as 480i and odd or even",6, row++);
	put_string("frames are being discarded completely.", 6, row++);
	row++;
	put_string("- A frame counter can be displayed on screen by", 6, row++);
	put_string("pressing left.", 6, row++);
}


void DrawPassiveLagHelp()
{
	row = 4;
					
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
	row = 4;
	
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
	put_string("- You can press left and right to advance in IRE", 6, row++);
	put_string("steps in the selected range.", 6, row++);
	put_string("Button I toggles the range between [0-100] and", 6, row++);
	put_string("[100-140] IRE.", 6, row++);
}

void DrawSMPTEHelp()
{
	row = 4;
	
	put_string("SMPTE COLOR BARS", 24, row++);
	set_font_pal(14);
	row+=2;
	put_string("- This pattern can be used to calibrate colors, but", 6, row++);
	put_string("black level does not comply to the standard due", 6, row++);
	put_string("to console capabilities.", 6, row++);
	row++;
	put_string("- You can toggle between 75% and 100% bars with I.", 6, row++);
}

void DrawConsInter()
{
	row = 4;
	
	put_string("Audio Clipping", 26, row++);
	set_font_pal(14);
	row+=2;
	put_string("This test generates an audio signal that adds", 6, row++);
	put_string("several channels using harmonic frequencies in", 6, row++);
	put_string("order to create constructive interferance and ", 6, row++);
	put_string("find if the resulting audio signal clips.", 6, row++);
}


#endif

#asm
p_string	.macro
	.ifdef	HUCC
		lda.l	#\1
		sta.l	<_bp
		lda.h	#\1
		sta.h	<_bp
		lda	#bank(\1)
		sta	<_bp_bank
		lda	#\2
		sta.l	<_di
		lda	#\3
		sta.h	<_di
	.else
		__ldwi	\1
		__stw	__si
		__ldwi	\2
		  stx	__bl
		__ldwi	\3
	.endif
		call	_put_string.3
	.endm
#endasm

#ifndef SYSCARD1
#include "helpdata.c"
#endif

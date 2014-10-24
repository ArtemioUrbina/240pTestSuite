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

#define HPOS 6

extern char MB512_map[];
extern int MB512_bg[];
extern int MB512_pal[];

#ifdef CDROM
extern int xres_flags;
extern int Enabled240p;
extern int UseDefault;
extern int EnabledSoft;
extern int Enabled_C_BW;
#endif

void showHelp(int data)
{
	int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
	int end = 0;
	int page = 1;
	int total = 1;
	int count = 1;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			setupFont();
			
			SetFontColors(13, RGB(3, 3, 3), RGB(0, 6, 0), RGB(0, 0, 0));
			SetFontColors(15, RGB(2, 2, 2), RGB(0, 6, 0), RGB(0, 0, 0));
			
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_palette(0, MB512_pal, 1);  
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
			set_xres(512, xres_flags);
        }
		
		if(refresh)
		{
			int row = 5;
			
			set_map_data(MB512_map, 64, 30);
			load_map(0, 0, 0, 0, 64, 30);
			
			set_font_pal(15);
			switch(data)
			{
				case GENERAL_HELP:
					total = 2;
					switch(page)
					{
						case 1:
							put_string("HELP (1/2)", HPOS+20, row++);
							set_font_pal(14);
							row++;
							put_string("The 240p Test Suite was designed with two goals in", HPOS, row++);
							put_string("mind:", HPOS, row++);
							row++;
							put_string("1) Evaluate 240p signal processing on modern TV", HPOS, row++);
							put_string("sets and video processing equipment; and", HPOS, row++);
							row++;
							put_string("2) Provide calibration patterns generated on your", HPOS, row++);
							put_string("game console to help in properly calibrating the", HPOS, row++);
							put_string("display's black, white and color levels.", HPOS, row++);
							row++;
							put_string("Help is available everywhere by pressing RUN.", HPOS, row++);
							put_string("Use right and left in the d-pad to navigate between", HPOS, row++);
							put_string("help pages. Options can be changed via SELECT", HPOS, row++);
							put_string("when in the main menus.", HPOS, row++);
							row++;
							if(!DetectTG16())
								put_string("The PC Engine version of the suite is", HPOS, row++);
							else
								put_string("The TurboGrafx-16 version of the suite is in", HPOS, row++);
							put_string("320x240 by default, but can be changed to 320x224p.", HPOS, row++);
							put_string("Grids and other screens are also available in ", HPOS, row++);
							break;
						case 2:
							put_string("HELP (2/2)", HPOS+20, row++);
							set_font_pal(14);
							row++;
							put_string("256x224, 256x240, 512x224 and 512x240 resolutions.", HPOS, row++);
							row++;
							put_string("When in 240p mode the system can start drawing", HPOS, row++);
							put_string("from video line 22, which is the default of this", HPOS, row++);
							put_string("suite in order to display all 240 lines.", HPOS, row++);
							row++;
							put_string("However all games start from line 24 when in 240p", HPOS, row++);
							put_string("mode, showing only 239 visible lines. You can", HPOS, row++);
							put_string("select that mode from the options menu in order to", HPOS, row++);
							put_string("display the regular signal you'll encounter when", HPOS, row++);
							put_string("using this console.", HPOS, row++);
							row++;
#ifdef CDROM
							put_string("This Super CDROM2 version of the suite can use the", HPOS, row++);
							put_string("Arcade Card in order to minimize loading times.", HPOS, row++);
							row++;
#endif
							put_string("The 240p suite is also available in other systems,", HPOS, row++);
							put_string("including: Sega Genesis/Mega Drive, Sega CD/", HPOS, row++);
							put_string("Mega CD, Super Nintendo, Nintendo GameCube,", HPOS, row++);
							put_string("Nintendo Wii and Sega Dreamcast.", HPOS, row++);
							break;
					}
					break;
					
				case BACKLIT_HELP:
					put_string("BACKLIT TEST", HPOS+20, row++);
					set_font_pal(14);
					row++;
					put_string("This test allows you to check how the display's", HPOS, row++);
					put_string("backlit works when only a small array of pixels", HPOS, row++);
					put_string("is shown. This can be very revealing, and can", HPOS, row++);
					put_string("be used to detect the zones in LED backlit", HPOS, row++);
					put_string("displays, or to evaluate global and local dimming.", HPOS, row++);
					row++;
					put_string("The user can move around the white pixel arrays", HPOS, row++);
					put_string("with the d-pad or joystiq, and change the size", HPOS, row++);
					put_string("of the pixel array with button I. The SELECT", HPOS, row++);
					put_string("button allows the user to hide the pixel array", HPOS, row++);
					put_string("in order to alternate a fully black screen.", HPOS, row++);
					row++;
					put_string("It might be necessary to have a fully-off panel", HPOS, row++);
					put_string("(if possible) when displaying the black", HPOS, row++);
					put_string("background,  in order to evaluate full-backlit", HPOS, row++);
					put_string("displays by adjusting brightness accordingly", HPOS, row++);
					put_string("for this test.", HPOS, row++);
					break;
				case CHECK_HELP:
					put_string("CHECKERBOARD", HPOS+20, row++);
					set_font_pal(14);
					row++;
					put_string("This pattern shows all the visible pixels in an", HPOS, row++);
					put_string("alternating white and black grid array.", HPOS, row++);
					row++;
					put_string("You can toggle the pattern with SELECT, or turn", HPOS, row++);
					put_string("on auto-toggle each frame with the I button.", HPOS, row++);
					row++;
					put_string("When auto-toggle is set, you should see it", HPOS, row++);
					put_string("alternating rapidly. On some setups, the pattern", HPOS, row++);
					put_string("doesn't change at all. This means that the signal", HPOS, row++);
					put_string("is being treated as 480i/576i and odd or even ", HPOS, row++);
					put_string("frames are being discarded completely.", HPOS, row++);
					row++;
					put_string("A frame counter can be displayed on screen by ", HPOS, row++);
					put_string("pressing left on the d-pad.", HPOS, row++);
					break;
/*****************************************/					
				case BLEED_HELP:
					put_string("COLOR BLEED", HPOS+20, row++);
					set_font_pal(14);
					row++;
					put_string("This pattern helps diagnose unneeded color", HPOS, row++);
					put_string("upsampling. It consists of one pixel width color", HPOS, row++);
					put_string("bars alternating with one pixel width black bars.", HPOS, row++);
					row++;
					put_string("You can change the vertical bars to a checker", HPOS, row++);
					put_string("board with button I.", HPOS, row++);
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
}
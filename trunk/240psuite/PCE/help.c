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
							put_string("Help is available everywhere by pressing the Start", HPOS, row++);
							put_string("button. Use right and left in the d-pad to navigate", HPOS, row++);
							put_string("between help pages.", HPOS, row++);
							row++;
							put_string("This version of the suite is in 320x240p by", HPOS, row++);
							put_string("default; but can be changed to 320x224p. Grids and", HPOS, row++);
							put_string("other screens also available in 256x224, 256x240,", HPOS, row++);
							put_string("512x224 and 512x240 resolutions.", HPOS, row++);
							break;
						case 2:
							put_string("HELP (2/2)", HPOS+20, row++);
							set_font_pal(14);
							row++;
							put_string("When in 240p mode the system can start drawing", HPOS, row++);
							put_string("from line 22, which is the default of this suite", HPOS, row++);
							put_string("in order to draw all 240 lines.", HPOS, row++);
							row++;
							put_string("However all games start from line 24 when in that", HPOS, row++);
							put_string("mode, displaying only 239 visible lines. You can", HPOS, row++);
							put_string("select that mode from the options menu in order to", HPOS, row++);
							put_string("display the regular signal you'll encounter when.", HPOS, row++);
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
			}
			
			set_font_pal(15);
			if(total > 1 && page != total)
				put_string("(cont...)", 50, 27);

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
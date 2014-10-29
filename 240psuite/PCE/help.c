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

#ifndef CDROM
#include "graphics.h"
#endif

#ifdef CDROM1
#include "gdata.h"
extern char palCD[];
#endif

#ifdef CDROM
extern int xres_flags;
extern int Enabled240p;
extern int UseDefault;
extern int EnabledSoft;
extern int Enabled_C_BW;

extern char MB512_map[];
extern int MB512_bg[];
extern int MB512_pal[];
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

#ifndef CDROM1			
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_palette(0, MB512_pal, 1);  
#else
			cd_loadvram(4, OFS_back512_DATA_bin, 0x1000, SIZE_back512_DATA_bin);
			cd_loaddata(4, OFS_back512_PAL_bin, palCD, SIZE_back512_PAL_bin); 
			set_bgpal(0, palCD); 
#endif
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
			set_xres(512, xres_flags);
        }
		
		if(refresh)
		{
#ifndef CDROM1
			set_map_data(MB512_map, 64, 30);
			load_map(0, 0, 0, 0, 64, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(4, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
#endif
			
			set_font_pal(15);
			
#ifndef CDROM1
			display_general_00(data, page, &total);
#endif
			
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
	   
#asm
p_string	.macro
	__ldwi	\1
	ldx	#low(\1)
	lda	#high(\1)
	__stw	_si
	stx	_si
	sta	_si+1
	__ldwi	\2
	ldx	#low(\2)
	lda	#high(\2)
	stx	_bl
	__ldwi	\3
	ldx	#low(\3)
	lda	#high(\3)
	call	_put_string.3
	.endm
#endasm

#ifndef CDROM1
#include "helpdata.c"
#endif
			
/*				

							if(!DetectTG16())
								put_string("", HPOS, row++);
							else
								put_string("The TurboGrafx-16 version of the suite is in", HPOS, row++);
*/
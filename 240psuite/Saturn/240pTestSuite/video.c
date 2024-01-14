/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2018 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <yaul.h>
#include "video.h"
#include "font.h"

void print_screen_mode(_svin_screen_mode_t screenmode)
{
	char buf[128];
    ClearTextLayer();
	sprintf(buf,"%i : ",get_screenmode_number(screenmode));
	//strcpy(buf, "");
	strcat(buf, scanmode_text_value(screenmode));
	strcat(buf, " ");
	strcat(buf, x_res_text_value(screenmode));
	strcat(buf, "x");
	strcat(buf, y_res_text_value(screenmode));
	strcat(buf, "");
	DrawStringWithBackground(buf, 160-strlen(buf)*_fw/2, 120, FONT_WHITE,FONT_YELLOW);
}

double get_screen_square_pixel_ratio(_svin_screen_mode_t screenmode)
{	
	//calculating X:Y ratio for the corresponding pixel mode
	
	double ratio;
	switch (screenmode.colorsystem)
	{
		case VDP2_TVMD_TV_STANDARD_NTSC:
			//pll input quartz = 14.31818 MHz (4x NTSC colorburst frequency of 3.579545 MHz)
			//square pixel clock is 12 + 27/99 MHz = 1215/99 MHz  (ANSI/SMPTE 170M-1994)
			switch (screenmode.x_res)
			{
				case _SVIN_X_RESOLUTION_320:
					//pixel clock = 14.31818 * 1706 / 910 = 26.8426
					//ratio is (14.31818 * 1706	 / 910)/(2*1215/99) = 1.0935896
					ratio = 1.0935896;
				break; 
				case _SVIN_X_RESOLUTION_352:
					//pixel clock = 14.31818 * 1820 / 910 = 28.63636
					//ratio is (14.31818 * 1820 / 910)/(2*1215/99) = 1.1666665
					ratio = 1.1666665;
				break; 
			}
			break;
		case VDP2_TVMD_TV_STANDARD_PAL:
			//pll input quartz = 17.734475 MHz (4x PAL colorburst frequency of 4.43361875 MHz)
			//square pixel clock is 14.75 MHz  (Rec. ITU-R BT.470-3)
			switch (screenmode.x_res)
			{
				case _SVIN_X_RESOLUTION_320:
					//pixel clock = 17.734475 * 1706 / 1135 = 26.6564 MHz
					//ratio is (17.734475 * 1706 / 1135)/(2*14.75) = 0.90360679
					ratio = 0.90360679;
				break; 
				case _SVIN_X_RESOLUTION_352:
					//pixel clock = 17.734475 * 1820 / 1135  = 28.43766 MHz
					//ratio is (17.734475 * 1820 / 1135)/(2*14.75) = 0.96398849
					ratio = 0.96398849;
				break; 
			}
			break;
	}
	if (screenmode.x_res_doubled) ratio*=2.0;
	if (_SVIN_SCANMODE_480I == screenmode.scanmode) ratio*=0.5;
	return ratio;
}

_svin_screen_mode_t next_screen_mode(_svin_screen_mode_t screenmode)
{
    int number = get_screenmode_number(screenmode);
    number++;
    if (number > 35) 
        number=0;
    if ((VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem) && (number > 23))
        number=0; //only 24 modes for NTSC
    _svin_screen_mode_t new_mode = create_screenmode_by_number(screenmode.colorsystem,number);
    return new_mode;
}

_svin_screen_mode_t prev_screen_mode(_svin_screen_mode_t screenmode)
{
    int number = get_screenmode_number(screenmode);
    number--;
    if (VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem)
    {
        if (number < 0) number=23; //only 24 modes for NTSC
    }
    else
    {
        if (number < 0) number=35;
    }
    _svin_screen_mode_t new_mode = create_screenmode_by_number(screenmode.colorsystem,number);
    return new_mode;
}

int get_screenmode_number(_svin_screen_mode_t screenmode)
{
	int number;
    if (VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem)
    {
        // x256 video resolutions are not supported on NTSC machines
        //2*2*2*3 = 24 videomodes
        number = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 0 : 1;
        number = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? number : number+2;
        number = (screenmode.x_res_doubled) ? number+4 : number;
        number = (_SVIN_SCANMODE_480I == screenmode.scanmode) ? number+16 :
                        (_SVIN_SCANMODE_240P == screenmode.scanmode) ? number+8 : number;
    }
    else //PAL
    {
        //2*3*2*3 = 36 videomodes
        number = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 0 : 1;
        number = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? number :
                        (VDP2_TVMD_VERT_240 == screenmode.y_res) ? number+2 : number+4;
        number = (screenmode.x_res_doubled) ? number+6 : number;
        number = (_SVIN_SCANMODE_480I == screenmode.scanmode) ? number+24 :
                        (_SVIN_SCANMODE_240P == screenmode.scanmode) ? number+12 : number;
    }
	return number;
}

_svin_screen_mode_t create_screenmode_by_number(vdp2_tvmd_tv_standard_t colorsystem, int number)
{
    _svin_screen_mode_t new_mode;
    new_mode.colorsystem = colorsystem;
    if (VDP2_TVMD_TV_STANDARD_NTSC == colorsystem)
    {
        // x256 video resolutions are not supported on NTSC machines
        //2*2*2*3 = 24 videomodes
        new_mode.x_res = (number%2 == 0) ? _SVIN_X_RESOLUTION_320 : _SVIN_X_RESOLUTION_352;
        new_mode.y_res = ((number%4)/2 == 0) ? VDP2_TVMD_VERT_224 : VDP2_TVMD_VERT_240;
        new_mode.x_res_doubled = ((number%8)/4 == 0) ? false : true;
        new_mode.scanmode = (number/8 == 0) ? _SVIN_SCANMODE_240I :
                                (number/8 == 1) ? _SVIN_SCANMODE_240P : _SVIN_SCANMODE_480I;
    }
    else //PAL
    {
        //2*3*2*3 = 36 videomodes
        new_mode.x_res = (number%2 == 0) ? _SVIN_X_RESOLUTION_320 : _SVIN_X_RESOLUTION_352;
        new_mode.y_res = ((number%6)/2 == 0) ? VDP2_TVMD_VERT_224 : 
                            ((number%6)/2 == 1) ? VDP2_TVMD_VERT_240 : VDP2_TVMD_VERT_256;
        new_mode.x_res_doubled = ((number%12)/6 == 0) ? false : true;
        new_mode.scanmode = (number/12 == 0) ? _SVIN_SCANMODE_240I :
                                (number/12 == 1) ? _SVIN_SCANMODE_240P : _SVIN_SCANMODE_480I;
    }
    return new_mode;
}

int get_screenmode_resolution_x(_svin_screen_mode_t screenmode)
{
	int number = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 320 : 352;
    number = (screenmode.x_res_doubled) ? number*2 : number;
	return number;
}

int get_screenmode_resolution_y(_svin_screen_mode_t screenmode)
{
	int number = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? 224 : 
				(VDP2_TVMD_VERT_240 == screenmode.y_res) ? 240 : 256;
    number = (_SVIN_SCANMODE_480I == screenmode.scanmode) ? number*2 : number;
	return number;
}
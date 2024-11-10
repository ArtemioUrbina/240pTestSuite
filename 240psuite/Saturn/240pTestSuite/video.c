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

uint8_t video_init_done = 0;

void video_init(video_screen_mode_t screen_mode, bool bmp_mode)
{
    video_init_done = 0;

    video_vdp2_init(screen_mode, bmp_mode);

    video_vdp1_init(screen_mode);

    smpc_peripheral_init();

    video_init_done = 1;
}

void video_deinit()
{
    video_vdp2_deinit();
    video_vdp1_deinit();
    video_init_done = 0;
}

int video_is_inited()
{
	return video_init_done;
}

char * scanmode_text_value(video_screen_mode_t screenmode)
{
	switch (screenmode.colorsystem)
	{
		case VDP2_TVMD_TV_STANDARD_NTSC:
			switch (screenmode.scanmode)
			{
				case VIDEO_SCANMODE_240I:
					return "480i same fields";
					break;
				case VIDEO_SCANMODE_240P:
					return "240p";
					break;
				case VIDEO_SCANMODE_480I:
					return "480i";
					break;
				case VIDEO_SCANMODE_480P:
					return "480p";
					break;
			}
			return "????";
			break;
		case VDP2_TVMD_TV_STANDARD_PAL:
			switch (screenmode.scanmode)
			{
				case VIDEO_SCANMODE_240I:
					return "576i same fields";
					break;
				case VIDEO_SCANMODE_240P:
					return "288p";
					break;
				case VIDEO_SCANMODE_480I:
					return "576i";
					break;
				case VIDEO_SCANMODE_480P:
					return "576p";
					break;
			}
			return "????";
			break;
		default:
			return "????";
			break;		
	}
}

char * x_res_text_value(video_screen_mode_t screenmode)
{
	switch (screenmode.x_res)
	{
		case VIDEO_X_RESOLUTION_320:
			return (screenmode.x_res_doubled) ? "640":"320";
			break;
		case VIDEO_X_RESOLUTION_352:
			return (screenmode.x_res_doubled) ? "704":"352";
			break;
	}
	return "???";
}

char * y_res_text_value(video_screen_mode_t screenmode)
{
	if (VIDEO_SCANMODE_480P == screenmode.scanmode)
		return "480";
	switch (screenmode.y_res)
	{
		case VDP2_TVMD_VERT_224:
			return ( get_screenmode_resolution_y(screenmode) > 256 ) ? "448":"224";
			break;
		case VDP2_TVMD_VERT_240:
			return ( get_screenmode_resolution_y(screenmode) > 256 ) ? "480":"240";
			break;
		case VDP2_TVMD_VERT_256:
			return ( get_screenmode_resolution_y(screenmode) > 256 ) ? "512":"256";
			break;
	}
	return "???";
}

char * y_lines_text_value(video_screen_mode_t screenmode)
{
	switch (screenmode.y_res)
	{
		case VDP2_TVMD_VERT_224:
			return "224";
			break;
		case VDP2_TVMD_VERT_240:
			return "240";
			break;
		case VDP2_TVMD_VERT_256:
			return "256";
			break;
	}
	return "????";
}

void print_screen_mode(video_screen_mode_t screenmode)
{
	char buf[128];
    //ClearTextLayer();
    ClearText(0,120,FONT_QUAD_WIDTH,10);
	sprintf(buf,"%i : ",get_screenmode_number(screenmode));
	//strcpy(buf, "");
	strcat(buf, scanmode_text_value(screenmode));
	strcat(buf, " ");
	strcat(buf, x_res_text_value(screenmode));
	strcat(buf, "x");
	strcat(buf, y_res_text_value(screenmode));
	strcat(buf, "");
	DrawStringWithBackground(buf, 160-strlen(buf)*_fw/2, 120, FONT_WHITE,FONT_BLUE);
}

double get_screen_square_pixel_ratio(video_screen_mode_t screenmode)
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
				case VIDEO_X_RESOLUTION_320:
					//pixel clock = 14.31818 * 1706 / 910 = 26.8426
					//ratio is (14.31818 * 1706	 / 910)/(2*1215/99) = 1.0935896
					ratio = 1.0935896;
				break; 
				case VIDEO_X_RESOLUTION_352:
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
				case VIDEO_X_RESOLUTION_320:
					//pixel clock = 17.734475 * 1706 / 1135 = 26.6564 MHz
					//ratio is (17.734475 * 1706 / 1135)/(2*14.75) = 0.90360679
					ratio = 0.90360679;
				break; 
				case VIDEO_X_RESOLUTION_352:
					//pixel clock = 17.734475 * 1820 / 1135  = 28.43766 MHz
					//ratio is (17.734475 * 1820 / 1135)/(2*14.75) = 0.96398849
					ratio = 0.96398849;
				break; 
			}
			break;
	}
	if (screenmode.x_res_doubled) ratio*=2.0;
	if ( (VIDEO_SCANMODE_480I == screenmode.scanmode) || (VIDEO_SCANMODE_480P == screenmode.scanmode) ) ratio*=0.5;
	return ratio;
}

// x256 video resolutions are not supported on NTSC machines
//NTSC : 8 240p modes + 16 480i modes + 4 480p modes = 28 modes
//PAL : 12 240p modes + 24 480i modes + 4 480p modes = 40 modes

video_screen_mode_t next_screen_mode(video_screen_mode_t screenmode)
{
    int number = get_screenmode_number(screenmode);
    number++;
    if (number > 39) 
        number=0;
    if ((VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem) && (number > 27))
        number=0; //only 28 modes for NTSC
    video_screen_mode_t new_mode = create_screenmode_by_number(screenmode.colorsystem,number);
    return new_mode;
}

video_screen_mode_t prev_screen_mode(video_screen_mode_t screenmode)
{
    int number = get_screenmode_number(screenmode);
    number--;
    if (VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem)
    {
        if (number < 0) number=27; //only 28 modes for NTSC
    }
    else
    {
        if (number < 0) number=39;
    }
    video_screen_mode_t new_mode = create_screenmode_by_number(screenmode.colorsystem,number);
    return new_mode;
}

int get_screenmode_number(video_screen_mode_t screenmode)
{
	int number;
    if (VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem)
    {
        if (VIDEO_SCANMODE_480P == screenmode.scanmode)
        {
            //480p NTSC modes have numbers 24-27
            number = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 24 : 25;
            number = (screenmode.x_res_doubled) ? number+2 : number;
        }
        else
        {
            //2*2*2*3 = 24 videomodes
            number = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 0 : 1;
            number = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? number : number+2;
            number = (screenmode.x_res_doubled) ? number+4 : number;
            number = (VIDEO_SCANMODE_480I == screenmode.scanmode) ? number+16 :
                            (VIDEO_SCANMODE_240P == screenmode.scanmode) ? number+8 : number;
        }
    }
    else //PAL
    {
        if (VIDEO_SCANMODE_480P == screenmode.scanmode)
        {
            //480p PAL modes have numbers 36-39
            number = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 36 : 37;
            number = (screenmode.x_res_doubled) ? number+2 : number;
        }
        else
        {
            //2*3*2*3 = 36 videomodes
            number = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 0 : 1;
            number = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? number :
                            (VDP2_TVMD_VERT_240 == screenmode.y_res) ? number+2 : number+4;
            number = (screenmode.x_res_doubled) ? number+6 : number;
            number = (VIDEO_SCANMODE_480I == screenmode.scanmode) ? number+24 :
                            (VIDEO_SCANMODE_240P == screenmode.scanmode) ? number+12 : number;
        }
    }
	return number;
}

video_screen_mode_t create_screenmode_by_number(vdp2_tvmd_tv_standard_t colorsystem, int number)
{
    video_screen_mode_t new_mode;
    new_mode.colorsystem = colorsystem;
    if (VDP2_TVMD_TV_STANDARD_NTSC == colorsystem)
    {
        if (number >= 24)
        {
            //480p NTSC modes have numbers 24-27
            new_mode.x_res = (number%2 == 0) ? VIDEO_X_RESOLUTION_320 : VIDEO_X_RESOLUTION_352;
            new_mode.y_res = VDP2_TVMD_VERT_240;
            new_mode.x_res_doubled = ((number%4)/2 == 0) ? false : true;
            new_mode.scanmode = VIDEO_SCANMODE_480P;
        }
        else
        {
            //2*2*2*3 = 24 videomodes
            new_mode.x_res = (number%2 == 0) ? VIDEO_X_RESOLUTION_320 : VIDEO_X_RESOLUTION_352;
            new_mode.y_res = ((number%4)/2 == 0) ? VDP2_TVMD_VERT_224 : VDP2_TVMD_VERT_240;
            new_mode.x_res_doubled = ((number%8)/4 == 0) ? false : true;
            new_mode.scanmode = (number/8 == 0) ? VIDEO_SCANMODE_240I :
                                    (number/8 == 1) ? VIDEO_SCANMODE_240P : VIDEO_SCANMODE_480I;
        }
    }
    else //PAL
    {
        if (number >= 36)
        {
            //480p PAL modes have numbers 36-39
            new_mode.x_res = (number%2 == 0) ? VIDEO_X_RESOLUTION_320 : VIDEO_X_RESOLUTION_352;
            new_mode.y_res = VDP2_TVMD_VERT_240;
            new_mode.x_res_doubled = ((number%4)/2 == 0) ? false : true;
            new_mode.scanmode = VIDEO_SCANMODE_480P;
        }
        else
        {
            //2*3*2*3 = 36 videomodes
            new_mode.x_res = (number%2 == 0) ? VIDEO_X_RESOLUTION_320 : VIDEO_X_RESOLUTION_352;
            new_mode.y_res = ((number%6)/2 == 0) ? VDP2_TVMD_VERT_224 : 
                                ((number%6)/2 == 1) ? VDP2_TVMD_VERT_240 : VDP2_TVMD_VERT_256;
            new_mode.x_res_doubled = ((number%12)/6 == 0) ? false : true;
            new_mode.scanmode = (number/12 == 0) ? VIDEO_SCANMODE_240I :
                                    (number/12 == 1) ? VIDEO_SCANMODE_240P : VIDEO_SCANMODE_480I;
        }
    }
    return new_mode;
}

int get_screenmode_resolution_x(video_screen_mode_t screenmode)
{
	int number = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 320 : 352;
    number = (screenmode.x_res_doubled) ? number*2 : number;
	return number;
}

int get_screenmode_resolution_y(video_screen_mode_t screenmode)
{
	int number = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? 224 : 
				(VDP2_TVMD_VERT_240 == screenmode.y_res) ? 240 : 256;
    number = ( (VIDEO_SCANMODE_480I == screenmode.scanmode) || (VIDEO_SCANMODE_480P == screenmode.scanmode) ) ? number*2 : number;
	return number;
}

bool is_screenmode_special(video_screen_mode_t screenmode)
{
	return ( (VIDEO_SCANMODE_480P == screenmode.scanmode) && (screenmode.x_res_doubled) ) ? true : false;
}
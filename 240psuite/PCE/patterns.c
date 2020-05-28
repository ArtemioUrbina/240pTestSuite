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
#include "patterns.h"
#include "help.h"
#include "font.h"
#include "float.h"

#ifndef CDROM1
#include "graphics.h"
#include "graphics_patterns.h"
#endif

#ifdef CDROM1
#include "res_patterns.h"
#endif

#include "video.h"

#define HPOS 5

#ifdef CDROM1
char palCD[512];
#endif
			
#ifndef CDROM1
void TestPatterns()
#else
void main()
#endif
{
	redraw = 1;
	refresh = 1;
	
	sel = 0;
	end = 0;
	
#ifdef CDROM1
	RestoreGlobals();

	if(prev_select)
	{
		sel = prev_select;
		prev_select = 0;
	}
#endif

	disp_off();
	while(!end)
	{		
		vsync();
		
#ifdef CDROM1
		if(!HelpItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_on();
		}
		
		if(refresh)
		{	
			RefreshTestPatterns();
			refresh = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_SEL)
		{
#ifdef CDROM1
			x_g = OPTIONS_PAT_HELP;
#endif
			Options();
			redraw = 1;
		}
		
		if (controller & JOY_DOWN) 
		{
			sel++;
			if(sel > 16)
				sel = 0;
			refresh = 1;
		}

		if (controller & JOY_UP) 
		{
			sel--;
			if(sel < 0)
				sel = 16;
			refresh = 1;
		}
		
		if (controller & JOY_RUN)
		{
#ifdef CDROM1
			showHelp(GENERAL_PAT_HELP);
#else
			showHelp(GENERAL_HELP);
#endif
			redraw = 1;
		}
#ifdef CDROM1
		}
		else
		{
			if(HelpItem)
			{
				sel = HelpItem - PATTERNSHELP;
				if(HelpItem <= CONVERGENCE_HELP)
					controller = JOY_I;
			}
				
			HelpItem = 0;
		}
#endif
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
			switch(sel)
			{
				case 0:
					DrawPluge();
					break;
				case 1:
					DrawColor();
					break;
				case 2:
					DrawSMPTE_EBU(1);
					break;
				case 3:
					DrawSMPTE_EBU(0);
					break;
				case 4:
					DrawCB601();
					break;
				case 5:
					DrawColorBleed();
					break;
				case 6:
					DrawGrid();
					break;
				case 7:
					DrawLinearity();
					break;
				case 8:
					DrawGray();
					break;
				case 9:
					DrawWhite();
					break;
				case 10:
					Draw100IRE();
					break;
				case 11:
					DrawSharpness();
					break;
				case 12:
					DrawOverscan();
					break;
				case 13:
					DrawConvergence();
					break;
				case 14:
#ifdef CDROM1
					x_g = OPTIONS_PAT_HELP;
#endif
					Options();
					break;
				case 15:
#ifdef CDROM1
					showHelp(GENERAL_PAT_HELP);
#else
					showHelp(GENERAL_HELP);
#endif
					break;
				case 16:
					end = 1;
					break;
			}
			if(sel != 16)
				end = 0;
				
			redraw = 1;	
			disp_off();
		}
	}
#ifdef CDROM1
	xres_flags_g = xres_flags;
	Enabled240p_g = Enabled240p;
	UseDefault_g = UseDefault;
	EnabledSoft_g = EnabledSoft;
	Enabled_C_BW_g = Enabled_C_BW;
	
	prev_select = 0;
	ToolItem = 0;
	ResetVideo();
	cd_execoverlay(MAIN_OVERLAY);
#endif
	sel = 0;
}

void RefreshTestPatterns()
{
	//set_font_pal(12);
	//put_string("Test Patterns", 14, 6);
	
	row = 7;
	
	drawmenutext(0, "Pluge");
	drawmenutext(1, "Color Bars");
	drawmenutext(2, "EBU Color Bars");
	drawmenutext(3, "SMPTE Color Bars");
	drawmenutext(4, "Color Bars w/ Gray Ref");
	drawmenutext(5, "Color Bleed Check");
	drawmenutext(6, "Grids");
	drawmenutext(7, "Linearity");
	drawmenutext(8, "Gray Ramp");
	drawmenutext(9, "White & RGB Screen");
	drawmenutext(10, "100 IRE");
	drawmenutext(11, "Sharpness");
	drawmenutext(12, "Overscan");
	drawmenutext(13, "Convergence");
	
	row++;
	DrawMenuBottom(14, 0);
}

void DrawPluge()
{
	color = 1;
	end = 0;
	redraw = 1;
	refresh = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
	
#ifndef CDROM1		
			set_map_data(pluge_map, 40, 30);
			set_tile_data(pluge_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, pluge_pal, 1);	
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_pluge_PAL_bin, palCD, SIZE_pluge_PAL_bin); 
			set_bgpal(0, palCD); 
			cd_loadvram(GPHX_OVERLAY, OFS_pluge_DATA_bin, 0x1000, SIZE_pluge_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_pluge_BAT_bin, 0x0000, SIZE_pluge_BAT_bin);
#endif

			Center224in240();
		 
			redraw = 0;
			refresh = 1;
			disp_on();
		}
		
		if(refresh)
		{
			switch(color)
			{
#ifndef CDROM1		
				case 0:
					set_color_rgb(1, 1, 1, 1);
					break;
				case 1:
					set_color_rgb(1, 0, 0, 1);
					break;
				case 2:
					set_color_rgb(1, 0, 1, 0);
					break;
				case 3:
					set_color_rgb(1, 1, 0, 0);
					break;
#else
				case 0:
					set_color_rgb(2, 1, 1, 1);
					break;
				case 1:
					set_color_rgb(2, 0, 0, 1);
					break;
				case 2:
					set_color_rgb(2, 0, 1, 0);
					break;
				case 3:
					set_color_rgb(2, 1, 0, 0);
					break;
#endif
			}
			refresh = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(PLUGE_HELP);
			redraw = 1;
		}

		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			color ++;
			if(color > 3)
				color = 0;
			refresh = 1;
		}
	}
}

void DrawColor()
{
	end = 0;
	redraw = 1;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
#ifndef CDROM1	
			load_background(color_bg, color_pal, color_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_color_PAL_bin, palCD, SIZE_color_PAL_bin); 
			load_palette(0, palCD, 16); 
			cd_loadvram(GPHX_OVERLAY, OFS_color_DATA_bin, 0x1000, SIZE_color_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_color_BAT_bin, 0x0000, SIZE_color_BAT_bin);
#endif
			Center224in240();
			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(COLORS_HELP);
			redraw = 1;
		}

		
		if (controller & JOY_II)
			end = 1;
	}
}

void DrawCB601()
{
	redraw = 1;
	end = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();

#ifndef CDROM1
			set_map_data(cb601_map, 40, 30);
			set_tile_data(cb601_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, cb601_pal, 1);	
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_cb601_PAL_bin, palCD, OFS_cb601_PAL_bin); 
			set_bgpal(0, palCD, 1); 
			cd_loadvram(GPHX_OVERLAY, OFS_cb601_DATA_bin, 0x1000, SIZE_cb601_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_cb601_BAT_bin, 0x0000, SIZE_cb601_BAT_bin);
#endif
			Center224in240();
			
			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(COLOR601_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
	}
}

void DrawColorBleed()
{
	type = FloatMenuResMinus352(1);
	if(type == FLOAT_CANCEL)
		return;

	option = 0;		// check
	end = 0;
	redraw = 1;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();

			ChangeResType();
			
			if(type == RES_320)
				scroll(0, 0, 0, 0, 240, 0xC0);
			if(type == RES_256)
				scroll(0, 40, 0, 0, 240, 0xC0);
			if(type == RES_512)
				scroll(0, -96, 0, 0, 240, 0xC0);
			
#ifndef CDROM1			
			set_map_data(colorbleed_map, 40, 30);
			if(option)
				set_tile_data(colorbleedchk_bg);
			else
				set_tile_data(colorbleed_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, colorbleed_pal, 1);  
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_colorbleed_PAL_bin, palCD, SIZE_colorbleed_PAL_bin); 
			set_bgpal(0, palCD);
			if(option)
				cd_loadvram(GPHX_OVERLAY, OFS_colorbldchk_DATA_bin, 0x1000, SIZE_colorbleed_DATA_bin);
			else
				cd_loadvram(GPHX_OVERLAY, OFS_colorbleed_DATA_bin, 0x1000, SIZE_colorbleed_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_colorbleed_BAT_bin, 0x0000, SIZE_colorbleed_BAT_bin);
#endif
			Center224in240();
			
			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(BLEED_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_I)
		{
			if(option)
			{
#ifndef CDROM1
				set_tile_data(colorbleed_bg);
				load_tile(0x1000);
#else
				cd_loadvram(GPHX_OVERLAY, OFS_colorbleed_DATA_bin, 0x1000, SIZE_colorbleed_DATA_bin);
#endif
				option = 0;
			}
			else
			{
#ifndef CDROM1
				set_tile_data(colorbleedchk_bg);
				load_tile(0x1000);
#else
				cd_loadvram(GPHX_OVERLAY, OFS_colorbldchk_DATA_bin, 0x1000, SIZE_colorbleed_DATA_bin);
#endif
				option = 1;
			}
		}
		
		if(controller & JOY_SEL)
		{
			ntype = FloatMenuResMinus352(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}
	}
}

void DrawSMPTE_EBU(unsigned char ebu)
{
	option = 0;  // 100%
	text = 0;
	redraw = 1;
	end = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
			
			setupFont();
#ifndef CDROM1
			if(ebu)
			{
				set_map_data(EBU_map, 40, 30);	
				set_tile_data(EBU_bg);
			}
			else
			{
				set_map_data(SMPTE75_map, 40, 30);	
				set_tile_data(SMPTE75_bg);
			}
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			if(option)
				load_palette(0, SMPTE100_pal, 1);  
			else
				load_palette(0, SMPTE75_pal, 1);
#else
			set_screen_size(SCR_SIZE_64x32); 
			if(option)
				cd_loaddata(GPHX_OVERLAY, ebu ? OFS_EBUCB100_PAL_bin: OFS_SMPTE100_PAL_bin, palCD, SIZE_SMPTE100_PAL_bin); 
			else
				cd_loaddata(GPHX_OVERLAY, ebu ? OFS_EBUCB75_PAL_bin : OFS_SMPTE75_PAL_bin, palCD, SIZE_SMPTE75_PAL_bin); 
			set_bgpal(0, palCD, 1); 
			cd_loadvram(GPHX_OVERLAY, ebu ? OFS_EBUCB_DATA_bin : OFS_SMPTE75_DATA_bin, 0x1000, ebu ? SIZE_EBUCB_DATA_bin : SIZE_SMPTE75_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, ebu ? OFS_EBUCB_BAT_bin : OFS_SMPTE75_BAT_bin, 0x0000, ebu ? SIZE_EBUCB_BAT_bin : SIZE_SMPTE75_BAT_bin);
#endif
			Center224in240();  
			
			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(ebu ? EBU_HELP : SMPTECOLOR_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_I)
		{
			if(option)
			{
#ifndef CDROM1
				load_palette(0, SMPTE75_pal, 1);  
#else
				cd_loaddata(GPHX_OVERLAY, ebu ? OFS_EBUCB75_PAL_bin : OFS_SMPTE75_PAL_bin, palCD, SIZE_SMPTE75_PAL_bin); 
				set_bgpal(0, palCD, 1); 
#endif
				option = 0;
				put_string(" 75%", 30, 2);
				text = 30;
			}
			else
			{
#ifndef CDROM1
				load_palette(0, SMPTE100_pal, 1);  
#else
				cd_loaddata(GPHX_OVERLAY, ebu ? OFS_EBUCB100_PAL_bin: OFS_SMPTE100_PAL_bin, palCD, SIZE_SMPTE100_PAL_bin); 
				set_bgpal(0, palCD, 1); 
#endif
				option = 1;
				put_string("100%", 30, 2);
				text = 30;
			}
		}
		
		if(text)
		{
			text--;
			if(!text)
			{
#ifndef CDROM1
				if(ebu)
					set_map_data(EBU_map, 40, 3);
				else
					set_map_data(SMPTE75_map, 40, 3);
				load_map(0, 0, 0, 0, 40, 3);
#else
				cd_loadvram(GPHX_OVERLAY, ebu ? OFS_EBUCB_BAT_bin : OFS_SMPTE75_BAT_bin, 0x0000, ebu ? SIZE_EBUCB_BAT_bin : SIZE_SMPTE75_BAT_bin);
#endif
			}
		}
	}
}


void DrawGrid()
{
	type = FloatMenuRes(1);
	if(type == FLOAT_CANCEL)
		return;
		
	color = 0;
	redraw = 1;
	end = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
			
			ChangeResType();
			
#ifndef CDROM1		
			set_tile_data(grid_bg);
			load_tile(0x1000);
			
			switch(type)
			{
				case RES_256:
					if(Enabled240p)
						set_map_data(grid256_240_map, 32, 30);
					else
						set_map_data(grid256_224_map, 32, 28);

					load_map(0, 0, 0, 0, 32, Enabled240p ? 30 : 28);
					break;
				case RES_320:
					if(Enabled240p)
						set_map_data(grid320_240_map, 40, 30);
					else
						set_map_data(grid320_224_map, 40, 28);

					load_map(0, 0, 0, 0, 40, Enabled240p ? 30 : 28);
					break;
				case RES_352:
					if(Enabled240p)
						set_map_data(grid352_240_map, 44, 30);
					else
						set_map_data(grid352_224_map, 44, 30);
						
					load_map(0, 0, 0, 0, 44, Enabled240p ? 30 : 28);
					break;
				case RES_512:
					if(Enabled240p)
						set_map_data(grid512_240_map, 64, 30);
					else
						set_map_data(grid512_224_map, 64, 28);
					load_map(0, 0, 0, 0, 64, Enabled240p ? 30 : 28);
					break;
			}
			
			load_palette(0, grid_pal, 1);  
#else
			cd_loaddata(GPHX_OVERLAY, OFS_grid256_240_PAL_bin, palCD, SIZE_grid256_240_PAL_bin); 
			set_bgpal(0, palCD); 
			
			cd_loadvram(GPHX_OVERLAY, OFS_grid256_240_DATA_bin, 0x1000, SIZE_grid256_240_DATA_bin);
			switch(type)
			{
				case RES_256:
					set_screen_size(SCR_SIZE_32x32); 
					if(Enabled240p)
						cd_loadvram(GPHX_OVERLAY, OFS_grid256_240_BAT_bin, 0x0000, SIZE_grid256_240_BAT_bin);
					else
						cd_loadvram(GPHX_OVERLAY, OFS_grid256_224_BAT_bin, 0x0000, SIZE_grid256_224_BAT_bin);
					break;
				case RES_320:
					set_screen_size(SCR_SIZE_64x32); 				
					if(Enabled240p)
						cd_loadvram(GPHX_OVERLAY, OFS_grid320_240_BAT_bin, 0x0000, SIZE_grid320_240_BAT_bin);
					else
						cd_loadvram(GPHX_OVERLAY, OFS_grid320_224_BAT_bin, 0x0000, SIZE_grid320_224_BAT_bin);
					break;
				case RES_352:
					set_screen_size(SCR_SIZE_64x32); 
					cd_loadvram(GPHX_OVERLAY, OFS_grid352_240_BAT_bin, 0x0000, SIZE_grid352_240_BAT_bin);
					break;
				case RES_512:
					set_screen_size(SCR_SIZE_64x32); 
					if(Enabled240p)
						cd_loadvram(GPHX_OVERLAY, OFS_grid512_240_BAT_bin, 0x0000, SIZE_grid512_240_BAT_bin);
					else
						cd_loadvram(GPHX_OVERLAY, OFS_grid512_224_BAT_bin, 0x0000, SIZE_grid512_224_BAT_bin);
					break;
			}
#endif
			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GRID_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
			
		if(controller & JOY_I)
		{
			color = !color;
			if(color)
				set_color_rgb(256, 7, 7, 7);
			else
				set_color_rgb(256, 0, 0, 0);
		}
	
		if(controller & JOY_SEL)
		{
			ntype = FloatMenuRes(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}
	}
}


void DrawLinearity()
{
	type = FloatMenuRes320n256_224(1);
	if(type == FLOAT_CANCEL)
		return;
	redraw = 1;
	end = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
			
			if(type == RES_320)
			{
				if(Enabled240p)
				{
#ifndef CDROM1
					set_map_data(linearity240_map, 40, 30);
					set_tile_data(linearity240_bg);
					load_tile(0x1000);
					load_map(0, 0, 0, 0, 40, 30);
					load_palette(0, linearity240_pal, 1);  
#else
					set_screen_size(SCR_SIZE_64x32); 
					cd_loaddata(GPHX_OVERLAY, OFS_lin240_PAL_bin, palCD, SIZE_lin240_PAL_bin); 
					set_bgpal(0, palCD); 
					cd_loadvram(GPHX_OVERLAY, OFS_lin240_DATA_bin, 0x1000, SIZE_lin240_DATA_bin);
					cd_loadvram(GPHX_OVERLAY, OFS_lin240_BAT_bin, 0x0000, SIZE_lin240_BAT_bin);
#endif
				}
				else
				{
#ifndef CDROM1
					set_map_data(linearity224_map, 40, 28);
					set_tile_data(linearity224_bg);
					load_tile(0x1000);
					load_map(0, 0, 0, 0, 40, 28);
					load_palette(0, linearity224_pal, 1);  
#else
					set_screen_size(SCR_SIZE_64x32); 
					cd_loaddata(GPHX_OVERLAY, OFS_lin224_PAL_bin, palCD, SIZE_lin224_PAL_bin); 
					set_bgpal(0, palCD); 
					cd_loadvram(GPHX_OVERLAY, OFS_lin224_DATA_bin, 0x1000, SIZE_lin224_DATA_bin);
					cd_loadvram(GPHX_OVERLAY, OFS_lin224_BAT_bin, 0x0000, SIZE_lin224_BAT_bin);
#endif
				}
				Set320H();
			}

			if(type == RES_256)
			{
#ifndef CDROM1			
				set_map_data(linearity256_map, 32, 28);
				set_tile_data(linearity256_bg);
				load_tile(0x1000);
				load_map(0, 0, 0, 0, 32, 28);
				load_palette(0, linearity256_pal, 1);  
#else
				set_screen_size(SCR_SIZE_32x32); 
				cd_loaddata(GPHX_OVERLAY, OFS_lin256_PAL_bin, palCD, SIZE_lin256_PAL_bin); 
				set_bgpal(0, palCD); 
				cd_loadvram(GPHX_OVERLAY, OFS_lin256_DATA_bin, 0x1000, SIZE_lin256_DATA_bin);
				cd_loadvram(GPHX_OVERLAY, OFS_lin256_BAT_bin, 0x0000, SIZE_lin256_BAT_bin);
#endif
			
				redraw = 0;
				Set256H();
				if(Enabled240p)
				{
					Set224p();
					Enabled240p = 1;
				}
			}
			
			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			if(type == RES_256)
			{
				if(Enabled240p)
				{
					if(!UseDefault)
						Set240p();
					else
						Set239p();
				}
			}
			showHelp(LINEARITY_HELP);
			redraw = 1;
		}

		if(controller & JOY_SEL)
		{
			ntype = FloatMenuRes320n256_224(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
	}

	if(type == RES_256 && Enabled240p)
	{
		if(!UseDefault)
			Set240p();
		else
			Set239p();
	}
}

void DrawSharpness()
{
	redraw = 1;
	end = 0;
	option = 0;
	type = RES_320;
	
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
			
			ChangeResType();
			
			if(!option)
			{
#ifndef CDROM1			
				set_map_data(sharpness_map, 40, 30);
				set_tile_data(sharpness_bg);
				load_tile(0x1000);
				load_map(0, 0, 0, 0, 40, 30);
				load_palette(0, sharpness_pal, 1);	
#else
				set_screen_size(SCR_SIZE_64x32); 
				cd_loaddata(GPHX_OVERLAY, OFS_sharpness_PAL_bin, palCD, SIZE_sharpness_PAL_bin); 
				set_bgpal(0, palCD);
				cd_loadvram(GPHX_OVERLAY, OFS_sharpness_DATA_bin, 0x1000, SIZE_sharpness_DATA_bin);
				cd_loadvram(GPHX_OVERLAY, OFS_sharpness_BAT_bin, 0x0000, SIZE_sharpness_BAT_bin);
#endif
			}
			else
			{
				set_tile_data(bricks_bg);
				load_tile(0x1000);
				load_palette(0, bricks_pal, 1);  
				
				set_map_data(fs_map, 64, 30);
				load_map(0, 0, 0, 0, 64, 30);
			}
			
			Center224in240();

			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(SHARPNESS_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
			option = !option;
			if(option)
			{
				ntype = FloatMenuRes(type);
				if(ntype != FLOAT_CANCEL)
					type = ntype;
			}
			else
				type = RES_320;
			redraw = 1;
		}
		
		if(option && controller & JOY_SEL)
		{
			ntype = FloatMenuRes(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}
	}
}

void DrawGray()
{
	redraw = 1;
	end = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			ResetVideo();
			Set256H();

#ifndef CDROM1			
			set_map_data(gray_map, 32, 30);
			set_tile_data(gray_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 30);
			load_palette(0, gray_pal, 1);  
#else
			set_screen_size(SCR_SIZE_32x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_gray_PAL_bin, palCD, SIZE_gray_PAL_bin); 
			set_bgpal(0, palCD); 
			cd_loadvram(GPHX_OVERLAY, OFS_gray_DATA_bin, 0x1000, SIZE_gray_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_gray_BAT_bin, 0x0000, SIZE_gray_BAT_bin);
#endif
			Center224in240();

			redraw = 0;
			disp_on();
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GRAY_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
	}
}

#ifndef CDROM1
#include "patterns_ext.c"
#else

void DrawWhite()
{
	prev_select = 9;
	ToolItem = TOOL_WHITE;
	cd_execoverlay(PATT_EXT_OVERLAY);
}

void DrawOverscan()
{
	prev_select = 12;
	ToolItem = TOOL_OVERSCAN;
	cd_execoverlay(PATT_EXT_OVERLAY);
}

void Draw100IRE()
{
	prev_select = 10;
	ToolItem = TOOL_IRE;
	cd_execoverlay(PATT_EXT_OVERLAY);
}

void DrawConvergence()
{
	prev_select = 13;
	ToolItem = TOOL_CONVERG;
	cd_execoverlay(PATT_EXT_OVERLAY);
}

#endif


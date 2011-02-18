/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#include <kos.h>
#include <stdlib.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

/* romdisk */
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
KOS_INIT_FLAGS(INIT_DEFAULT);

ImagePtr    scanlines = NULL;

static inline void DrawScanlines()
{
	if(vmode == FAKE_640_SL && scanlines)
		DrawImage(scanlines);
}

void TestPatternsMenu();

void DrawPlunge();
void DrawWhiteScreen();
void DrawGrayRamp();
void DrawColorBars();
void Draw601ColorBars();
void DrawGrid();
void DrawLinearity();


void DropShadowTest();
void StripedSpriteTest();
void LagTest();
void ScrollTest();
void DrawStripes();
void DrawCheckBoard();
void SoundTest();
void DrawCredits();

int main(void)
{
	int         done = 0, sel = 1, joycnt = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    title;

	/* init kos  */
	// PM_RGB555 PM_RGB565 PM_RGB888
	vcable = vid_check_cable();
	vid_set_mode(DM_320x240_NTSC, PM_RGB565); 
	pvr_init_defaults();

 	// Disable deflicker filter, 
 	if(PVR_GET(PVR_SCALER_CFG) != 0x400)
 	{
		dbglog(DBG_KDEBUG, "Disabling pvr deflicker filter for 240p tests\n");
		PVR_SET(PVR_SCALER_CFG, 0x400);
	}

    // Turn off texture dithering
	if(PVR_GET(PVR_FB_CFG_2) != 0x00000001)
	{
		dbglog(DBG_KDEBUG, "Disabling pvr dithering for 240p tests\n");
		PVR_SET(PVR_FB_CFG_2, 0x00000001);
	}

start:
	vid_border_color(0, 0, 0);
	pvr_set_bg_color(0.0f, 0.0f, 0.0f);
    
	LoadFont();
	title = LoadImage("/rd/title.png", 1);
	if(!scanlines && vmode == FAKE_640_SL)
	{
		scanlines = LoadImage("/rd/scanlines.png", 0);
		scanlines->layer = 5.0;
		scanlines->alpha = 0.7f; // XRGB-3 175 
		scanlines->scale = 0;
	}
    
 	while(!done) 
	{
		char  	res[40];
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int 	c = 1;        
		float 	x = 40.0f;
		float 	y = 55.0f;
        
		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_TR_POLY);

		DrawImage(title);
		
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Sound Test"); y += fh; c++;

		switch(vmode)
		{
			case NATIVE_320:
				sprintf(res, "Video: 240p");
				break;
			case NATIVE_640:
				sprintf(res, "Video: 240p in 480i");
				break;
			case FAKE_640:
				sprintf(res, "Video: Fake 480i");
				break;
			case NATIVE_640_FS:
				sprintf(res, "Video: 480i");
				break;
			case FAKE_640_SL:
				sprintf(res, "Video: 480p/scanlines");
				break;
		}
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += fh; 

		switch(vcable)
		{
			case CT_RGB:
				DrawStringS(265.0f, 225.0f, 0, g,  b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(265.0f, 225.0f, 0, g,  b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(215.0f, 225.0f, 0, g,  b, "Composite");
				break;
		}
    
		DrawScanlines();
        
		pvr_list_finish();        
		pvr_scene_finish();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
#ifdef SERIAL
			if (st->buttons & CONT_START && st->buttons & CONT_B)
			{
				updateVMU(" Goodbye ", " m(_ _)m ", 1);
				done =  1;
			}
#endif

			if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
			{
				if(scanlines)
				{
					scanlines->alpha += 0.1f; 
					if(scanlines->alpha > 1.0f)
						scanlines->alpha = 1.0f;
				}
			}

			if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
			{
				if(scanlines)
				{
					scanlines->alpha -= 0.1f; 
					if(scanlines->alpha < 0.0f)
						scanlines->alpha = 0.0f;
				}
			}

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c;				
			}
    
			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c)
					sel = 1;				
			}
    
			if(st->joyy != 0)
			{
				if(++joycnt > 5)
				{
					if(st->joyy > 0)
						sel ++;
					if(st->joyy < 0)
						sel --;
    
					if(sel < 1)
						sel = c;
					if(sel > c)
						sel = 1;					
					joycnt = 0;
				}
			}
			else
				joycnt = 0;
    
			if (pressed & CONT_A)
			{
				switch(sel)
				{
					case 1:
						TestPatternsMenu();
						break;
					case 2:					
						DropShadowTest();
						break;
					case 3:
						StripedSpriteTest();
						break;
					case 4:
						LagTest();
						break;
					case 5:
						ScrollTest();
						break;
					case 6:
						DrawStripes();
						break;
					case 7:
						DrawCheckBoard();
						break;
					case 8:
						SoundTest();
						break;
					case 9:
						ChangeResolution();
						FreeImage(&scanlines);    
						FreeImage(&title);    
						ReleaseFont();
						// we need to reload textures and stuff..
						// not pretty, but "clean"
						goto start;
						break;
					case 10:
						DrawCredits();
						break;
				}                         
				updateVMU("240p Test", "", 1);				
			}
		}
		MAPLE_FOREACH_END()

		updateVMU("240p Test", "", 0);
	}

	FreeImage(&scanlines);    
	FreeImage(&title);    
	ReleaseFont();
	return 0;
}

void TestPatternsMenu()
{
	int         done = 0, sel = 1, joycnt = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    title;

	title = LoadImage("/rd/title.png", 1);    
 	while(!done) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int 	c = 1;        
		float 	x = 40.0f;
		float 	y = 55.0f;
        
		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_TR_POLY);

		DrawImage(title);

		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Plunge"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Linearity"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "White Screen"); y += fh; c++;				
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		switch(vcable)
		{
			case CT_RGB:
				DrawStringS(265.0f, 225.0f, 0, g,  b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(265.0f, 225.0f, 0, g,  b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(215.0f, 225.0f, 0, g,  b, "Composite");
				break;
		}
    
		DrawScanlines();
        
		pvr_list_finish();        
		pvr_scene_finish();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;

			if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
			{
				if(scanlines)
				{
					scanlines->alpha += 0.1f; 
					if(scanlines->alpha > 1.0f)
						scanlines->alpha = 1.0f;
				}
			}

			if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
			{
				if(scanlines)
				{
					scanlines->alpha -= 0.1f; 
					if(scanlines->alpha < 0.0f)
						scanlines->alpha = 0.0f;
				}
			}

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c;				
			}
    
			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c)
					sel = 1;				
			}
    
			if(st->joyy != 0)
			{
				if(++joycnt > 5)
				{
					if(st->joyy > 0)
						sel ++;
					if(st->joyy < 0)
						sel --;
    
					if(sel < 1)
						sel = c;
					if(sel > c)
						sel = 1;					
					joycnt = 0;
				}
			}
			else
				joycnt = 0;
    
			if (pressed & CONT_B)			
				done = 1;			
		
			if (pressed & CONT_A)
			{
				switch(sel)
				{
					case 1:
						DrawPlunge();
						break;
					case 2:
						DrawColorBars();
						break;
					case 3:
						Draw601ColorBars();
						break;
					case 4:
						DrawGrid();
						break;
					case 5:
						DrawLinearity();
						break;					
					case 6:
						DrawGrayRamp();
						break;
					case 7:
						DrawWhiteScreen();
						break;
					case 8:
						done = 1;
						break;
				}                         
				updateVMU("Patterns", "", 1);
			}			
		}
		MAPLE_FOREACH_END()

		updateVMU("Patterns", "", 0);
	}
	
	FreeImage(&title);    	
	return;
}

void DrawPlunge()
{
	int         done = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/plunge.png", 1);
    
	updateVMU(" Plunge ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}

void DrawGrayRamp()
{
	int         done = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/grayramp.png", 1);
    
	updateVMU("Gray Ramp", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}

void DrawWhiteScreen()
{
	int         done = 0, color = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/white.png", 1);
    
	updateVMU("White scr", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                

			if (pressed & CONT_A)
				color ++;

			if (pressed & CONT_B)
				color --;
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		if(color > 4)
			color = 0;
		if(color < 0)
			color = 4;

		switch(color)
		{
				case 0:
					back->r = 1.0f;
					back->g = 1.0f;
					back->b = 1.0f;
					break;
				case 1:
					back->r = 0.0f;
					back->g = 0.0f;
					back->b = 0.0f;
					break;
				case 2:
					back->r = 1.0f;
					back->g = 0.0f;
					back->b = 0.0f;
					break;
				case 3:
					back->r = 0.0f;
					back->g = 1.0f;
					back->b = 0.0f;
					break;
				case 4:
					back->r = 0.0f;
					back->g = 0.0f;
					back->b = 1.0f;
					break;
		}

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}

void DrawColorBars()
{
	int         done = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/color.png", 1);
    
	updateVMU("Colorbars", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}

void Draw601ColorBars()
{
	int         done = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/601701cb.png", 1);
    
	updateVMU("Colorbars", "with gray", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}

void DrawGrid()
{
	int         done = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

  if(vmode != NATIVE_640_FS)
		back = LoadImage("/rd/grid.png", 1);
	else
	{
		back = LoadImage("/rd/480/grid-480.png", 1);
		back->scale = 0;
	}

    
	updateVMU("   Grid  ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}

	FreeImage(&back);
}

void DrawLinearity()
{
	int         done = 0, gridpattern = 0, showgrid = 1;
	uint16	    oldbuttons = 0xffff, pressed;
	ImagePtr    circles, grid, gridd;

	circles = LoadImage("/rd/circles.png", 1);
	grid = LoadImage("/rd/circles_grid.png", 1);
	gridd = LoadImage("/rd/circles_griddot.png", 1);
    
	updateVMU("Linearity", "   A B   ", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_A)
				showgrid = !showgrid;
    
			if (pressed & CONT_B)
				gridpattern = !gridpattern;
    
			if (pressed & CONT_START)
				done =  1;        
            
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		if(showgrid)
		{
			if(gridpattern)
				DrawImage(gridd);
			else
				DrawImage(grid);
		}
		DrawImage(circles);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}

	FreeImage(&gridd);
	FreeImage(&grid);
	FreeImage(&circles);
}

void DropShadowTest()
{
	char		  msg[50];
	int		    done = 0, x = 0, y = 0, invert = 0, frame = 0, text = 0, selback = 0, sprite = 0;
	uint16		oldbuttons = 0xffff, pressed;    
	ImagePtr	back[4], ssprite, shadow, buzz, buzzshadow;

  if(vmode != NATIVE_640_FS)
	{
		back[0] = LoadImage("/rd/motoko.png", 1);
		back[1] = LoadImage("/rd/sonicbg-240.png", 1);
		back[2] = LoadImage("/rd/checkpos.png", 1);
		back[3] = LoadImage("/rd/stripespos.png", 1);
	}
  else
  {
		back[0] = LoadImage("/rd/480/motoko-480.png", 1);
    back[0]->scale = 0;
    back[1] = LoadImage("/rd/480/sonicbg-480.png", 1);
    back[1]->scale = 0;
		back[2] = LoadImage("/rd/480/checkpos-480.png", 1);
    back[2]->scale = 0;
		back[3] = LoadImage("/rd/480/stripespos-480.png", 1);
    back[3]->scale = 0;
  }
    
  ssprite = LoadImage("/rd/shadow.png", 0);	
  buzz = LoadImage("/rd/buzzbomber.png", 0);
  buzzshadow = LoadImage("/rd/buzzbomberShadow.png", 0);
  
  srand((int)(time(0) ^ getpid()));
  sprite = rand() % 2;
  if(sprite == 0)
		shadow = ssprite;
  else
		shadow = buzzshadow;
	updateVMU(" Shadow  ", "   even  ", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (st->buttons & CONT_DPAD_UP)
				y --;
    
			if (st->buttons & CONT_DPAD_DOWN)
				y ++;
    
			if (st->buttons & CONT_DPAD_LEFT)
				x --;
    
			if (st->buttons & CONT_DPAD_RIGHT)
				x ++;

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/20;
    
			if(st->joyy != 0)
				y += st->joyy/20;
    
			if (pressed & CONT_START)
				done =  1;        

			if (pressed & CONT_X)		
			{
				invert = !invert;                       	
				if(invert)
				{
					sprintf(msg, "Shadow on odd frames");
					updateVMU(" Shadow  ", "    odd  ", 1);
				}
				else
				{
					sprintf(msg, "Shadow on even frames");
					updateVMU(" Shadow  ", "   even  ", 1);
				}
				text = 60;
			}
        
			if (pressed & CONT_A)
			{
				if(selback < 3)
					selback ++;
				else
					selback = 0;
			}
    
			if (pressed & CONT_B)
      {
        if(sprite == 0)
        {
          shadow = buzzshadow;
          sprite = 1;
        }
        else
        {
          shadow = ssprite;
          sprite = 0;
        }
      }

		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back[selback]);

		if(text)
		{
			if(vmode != NATIVE_640_FS)
				DrawStringB(140, 12, 0, 1.0, 0, msg);
			else
				DrawStringB(450, 20, 0, 1.0, 0, msg);
			text --;
		}

		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(x > back[selback]->w - shadow->w)
			x = back[selback]->w - shadow->w;
		if(y > back[selback]->h - shadow->h)
			y = back[selback]->h - shadow->h;

		if(frame == invert)
		{
			shadow->x = x;
			shadow->y = y;
			DrawImage(shadow);
			frame = !frame;
		}
		else
			frame = !frame;
        
    if(sprite == 1)
    {
      buzz->x = x - 20;
    	buzz->y = y - 20;
      DrawImage(buzz);
    }       
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back[0]);
	FreeImage(&back[1]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&ssprite);
	FreeImage(&buzz);
	FreeImage(&buzzshadow);
}

void StripedSpriteTest()
{	
	int		done = 0, x = 0, y = 0, selback = 0;
	uint16		oldbuttons = 0xffff, pressed;    
	ImagePtr	back[4], striped;

  if(vmode != NATIVE_640_FS)
	{
		back[0] = LoadImage("/rd/motoko.png", 1);
		back[1] = LoadImage("/rd/sonicbg-240.png", 1);
		back[2] = LoadImage("/rd/checkpos.png", 1);
		back[3] = LoadImage("/rd/stripespos.png", 1);
	}
  else
  {
		back[0] = LoadImage("/rd/480/motoko-480.png", 1);
    back[0]->scale = 0;
    back[1] = LoadImage("/rd/480/sonicbg-480.png", 1);
    back[1]->scale = 0;
		back[2] = LoadImage("/rd/480/checkpos-480.png", 1);
    back[2]->scale = 0;
		back[3] = LoadImage("/rd/480/stripespos-480.png", 1);
    back[3]->scale = 0;
  }
	striped = LoadImage("/rd/striped.png", 0);
    
	updateVMU(" Striped ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if(st->buttons & CONT_Y)
			{
				if (pressed & CONT_DPAD_UP)
					y --;
    	
				if (pressed & CONT_DPAD_DOWN)
					y ++;
	
				if (pressed & CONT_DPAD_LEFT)
					x --;

				if (pressed & CONT_DPAD_RIGHT)
					x ++;
			}
			else
			{
				if (st->buttons & CONT_DPAD_UP)
					y --;
    	
				if (st->buttons & CONT_DPAD_DOWN)
					y ++;
	
				if (st->buttons & CONT_DPAD_LEFT)
					x --;

				if (st->buttons & CONT_DPAD_RIGHT)
					x ++;
			}

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/20;
    
			if(st->joyy != 0)
				y += st->joyy/20;
    
			if (pressed & CONT_START)
				done =  1;        
            
			if (pressed & CONT_A)
			{
				if(selback > 0)
					selback --;
				else
					selback = 3;
			}
    
			if (pressed & CONT_B)
			{
				if(selback < 3)
					selback ++;
				else
					selback = 0;
			}

		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back[selback]);

		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(x > back[selback]->w - striped->w)
			x = back[selback]->w - striped->w;
		if(y > back[selback]->h - striped->h)
			y = back[selback]->h - striped->h;
		striped->x = x;
		striped->y = y;
		DrawImage(striped);

		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back[0]);
	FreeImage(&back[1]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&striped);
}

void LagTest()
{
	char											msg[60];
	int												clicks[10], done = 0, view = 0, speed = 1, change = 1;
	int												x, y, x2, y2, audio = 1, pos = 0, i = 0;
	uint16										oldbuttons = 0xffff, pressed;    
	ImagePtr									back, spriteA, spriteB, spriteneg;
	sfxhnd_t									beep;
	maple_device_t						*purupuru = NULL;
	static purupuru_effect_t	effect;

	effect.duration = 1;
	effect.effect2 = PURUPURU_EFFECT2_UINTENSITY(1);
	effect.effect1 = PURUPURU_EFFECT1_INTENSITY(1);
	effect.special = PURUPURU_SPECIAL_MOTOR1;

	snd_init();

	updateVMU("Lag Test ", "", 1);
	back = LoadImage("/rd/lag-per.png", 0);
	spriteA = LoadImage("/rd/lag-per.png", 0);
	spriteB = LoadImage("/rd/lag-per.png", 0);
	spriteneg = LoadImage("/rd/lag-full.png", 0);

	beep = snd_sfx_load("/rd/beep.wav");
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
    
	back->x = 144;
	back->y = 96;
	spriteneg->x = 144;
	spriteneg->y = 96;

	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;

	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_A)
			{
				if(change)
				{
					clicks[pos] = (y - 96) *speed;
					change = 0;
    
					sprintf(msg, " Off: %d", clicks[pos]);
					updateVMU("Lag Test ", msg, 1);
    
					if(clicks[pos] >= 0)
						pos ++;
	    
					if(pos > 9)
						done = 1;
				}
			}

			if (pressed & CONT_B)
			{
				view ++;
				if(view > 2)
					view = 0;
			}
            
			if (pressed & CONT_Y)
				audio =  !audio;        
    
			if (pressed & CONT_START)
				done =  1;        
    
		MAPLE_FOREACH_END()
    
		if(y > 132)
		{
			speed = -1;
			change = 1;
		}

		if(y < 60)
		{
			speed = 1;
			change = 1;
		}

		y += speed;
		x2 += speed;

		spriteA->x = x;
		spriteA->y = y;
		spriteB->x = x2;
		spriteB->y = y2;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

		if(y == 96)
		{
			DrawImage(spriteneg);
			if(audio && beep != SFXHND_INVALID)
				snd_sfx_play(beep, 255, speed*255);	// toggle pan to 0 and 255, l & r

			purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
			if(purupuru)
				purupuru_rumble(purupuru, &effect);
		}
		else
			DrawImage(back);

		if(view == 0 || view == 2)
			DrawImage(spriteA);

		if(view == 1 || view == 2)
			DrawImage(spriteB);

		for(i = 0; i < 10; i++)
		{
			if(clicks[i] != 0xFF)
			{
				int px, py;

				px = 10;
				py = i*fh + 20;
	
				sprintf(msg, "Offset %02d:", i + 1);
				DrawStringS(px, py, 1.0f, 1.0f, 1.0f, msg);
			
				px += (strlen(msg) + 2)*fw;
				sprintf(msg, "%2d", clicks[i]);

				if(clicks[i] >= 0)
				{
					if(clicks[i] == 0)
						DrawStringS(px, py, 0.0f, 1.0f, 0.0f, msg);
					else
						DrawStringS(px, py, 1.0f, 1.0f, 1.0f, msg);
				}
				else
					DrawStringS(px, py, 1.0f, 0.0f, 0.0f, msg);
			}
		}

		DrawStringS(20, 190, 1.0f, 1.0f, 1.0f, "Press \"A\" when the sprite is aligned with the background.");
		DrawStringS(20, 190+fh, 1.0f, 1.0f, 1.0f, "Negative values mean you pressed \"A\" before they intersected");
		DrawStringS(20, 190+2*fh, 1.0f, 1.0f, 1.0f, "\"B\" button toggles horizontal and vertical movement.");

		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	FreeImage(&spriteA);
	FreeImage(&spriteB);
	FreeImage(&spriteneg);
	snd_shutdown();

	updateVMU("Lag Test ", "-Results-", 1);
	if(pos > 9)
	{
		int	total = 0;
		double  res = 0, ms = 0;
		ImagePtr wall;
    
		done = 0;
		wall = LoadImage("/rd/back.png", 1);

		while(!done)
		{
			pvr_wait_ready();

			MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
				
				if (st->buttons & CONT_START)
					done =  1;

			MAPLE_FOREACH_END()
	
    
			pvr_scene_begin();
			pvr_list_begin(PVR_LIST_TR_POLY);

			DrawImage(wall);

			total = 0;
			for(i = 0; i < 10; i++)
			{
				int px, py;
	
				px = 70;
				py = i*fh + 70;
	
				sprintf(msg, "%2d", clicks[i]);
				if(clicks[i] >= 0)
				{
					if(clicks[i] == 0)
						DrawStringS(px, py, 0, 1.0f, 0, msg);
					else
						DrawStringS(px, py, 1.0f, 1.0f, 1.0f, msg);
				}
				else
					DrawStringS(px, py, 1.0f, 0, 0, msg);
	
				if(clicks[i] >= 0)
					total += clicks[i];
	
			}

			res = (double)total / 10.0;
			ms = (double)(res*(1000.0/60.0));
			sprintf(msg, "%d/10 = %0.2f average frames ~= %0.2f ms", total, res, ms);
			DrawStringS(60, 110, 1.0f, 0, 0, "+");
			DrawStringS(55, 70 + fh*10, 1.0f, 0, 0, "-----");
			DrawStringS(60, 70 + fh*11, 1.0f, 1.0f, 1.0f, msg);
			DrawStringS(60, 70 + fh*12, 0.0f, 1.0f, 1.0f, "Keep in mind that a frame is around 16.67 ms");

			if(total && total < 5)
			{
				DrawStringS(100, 120, 0.0f, 1.0f, 0.0f, "EXCELLENT REFLEXES!");
    				updateVMUFlash("Lag Test ", " AWESOME", 1);
			}
			if(total == 0)
			{
				DrawStringS(100, 120, 0.0f, 1.0f, 0.0f, "INCREDIBLE REFLEXES!!");
    				updateVMUFlash("Lag Test ", "-PERFECT-", 1);
			}

			DrawScanlines();
			pvr_list_finish();        

			pvr_scene_finish();
		}
		FreeImage(&wall);
	}
}

void ScrollTest()
{
	int         done = 0, speed = 1, acc = -1, x = 0, pause = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/MSBG-1.png", 0);
	if(!back)
		return;
	
	back->y = (dH - 240)/2;

	updateVMU(" Scroll  ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_DPAD_UP)
				speed ++;             

			if (pressed & CONT_DPAD_DOWN)
				speed --;          

			if (pressed & CONT_START)
				done =  1;                

			if (pressed & CONT_A)
				pause = !pause;

			if (pressed & CONT_B)
				acc *= -1;
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

		if(speed > 15)
			speed = 15;

		if(speed < 1)
			speed = 1;

		if(!pause)
		{
			if(!(x <= (back->tw - W) && x >= 0))
				acc *= -1;

			x += speed * acc;
		}

		CalculateUV(x, 0, dW, 240, back);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}

void DrawStripes()
{
	int         done = 0, field = 1, alternate = 0,
    		    frame = 0, dframe = 0, vertical = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    stripespos, stripesneg;
	ImagePtr    vstripespos, vstripesneg;

  if(vmode != NATIVE_640_FS)
	{
		stripespos = LoadImage("/rd/stripespos.png", 1);
		stripesneg = LoadImage("/rd/stripesneg.png", 1);
	}
	else
	{
		stripespos = LoadImage("/rd/480/stripespos-480.png", 1);
		stripespos->scale = 0;
		stripesneg = LoadImage("/rd/480/stripesneg-480.png", 1);
		stripesneg->scale = 0;
	}
	vstripespos = LoadImage("/rd/vertstripespos.png", 1);
	vstripesneg = LoadImage("/rd/vertstripesneg.png", 1);
    
	updateVMU(" Stripes", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;        
            
			if (pressed & CONT_A)
			{
				alternate = !alternate;        
				if(alternate)
					updateVMU(" Stripes", "alternate", 1);
				else
					updateVMU(" Stripes", "  still  ", 1);
			}
            
			if (pressed & CONT_Y)
				vertical = !vertical;
            
			if (pressed & CONT_X)
			{
				dframe = !dframe;
				frame = 0;
			}
            
			if (pressed & CONT_B && !alternate)
				field = !field;
        
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

		if(!vertical)
		{
			if(field == 1)
				DrawImage(stripespos);
			else
				DrawImage(stripesneg);
		}
		else
		{
			if(field == 1)
				DrawImage(vstripespos);
			else
				DrawImage(vstripesneg);
		}

		if(alternate)
			field = !field;

		if(dframe)
		{
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			if(vmode != NATIVE_640_FS)
					DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
					DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > 59)
				frame = 0;
		}

		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}

	FreeImage(&stripespos);
	FreeImage(&vstripesneg);
	FreeImage(&stripespos);
	FreeImage(&stripesneg);
}

void DrawCheckBoard()
{
	int         done = 0, field = 1, alternate = 0,
    		    frame = 0, dframe = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    checkpos, checkneg;

	if(vmode != NATIVE_640_FS)
	{
		checkpos = LoadImage("/rd/checkpos.png", 1);
		checkneg = LoadImage("/rd/checkneg.png", 1);
	}
	else
	{
		checkpos = LoadImage("/rd/480/checkpos-480.png", 1);
		checkneg = LoadImage("/rd/480/checkneg-480.png", 1);
	}
    
	updateVMU("CHKB PTTN", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;        
            
			if (pressed & CONT_A)
			{
				alternate = !alternate;        
				if(alternate)
					updateVMU("CHKB PTTN", "alternate", 1);
				else
					updateVMU("CHKB PTTN", "  still  ", 1);
			}
    
			if (pressed & CONT_X)
			{
				dframe = !dframe;
				frame = 0;
			}
            
			if (pressed & CONT_B && !alternate)
				field = !field;
        
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

		if(field == 1)
			DrawImage(checkpos);
		else
			DrawImage(checkneg);

		if(alternate)
			field = !field;

		if(dframe)
		{
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			if(vmode != NATIVE_640_FS)
					DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
					DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > 59)
				frame = 0;
		}

		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&checkpos);
	FreeImage(&checkneg);
}

void SoundTest()
{
	int         done = 0, sel = 1, play = 0, pan = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;
	sfxhnd_t		beep;

	snd_init();
	back = LoadImage("/rd/back.png", 1);
	beep = snd_sfx_load("/rd/beep.wav");
    
	updateVMU(" Plunge ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                

			if (pressed & CONT_A)
				play =  1;             

			if (pressed & CONT_DPAD_LEFT)
				sel --;

			if (pressed & CONT_DPAD_RIGHT)
				sel ++;
		MAPLE_FOREACH_END()
		
		if(sel < 0)
			sel = 2;

		if(sel > 2)
			sel = 0;

		switch(sel)
		{
				case 0:
					pan = 0;
					break;
				case 1:
					pan = 128;
					break;
				case 2:
					pan = 255;
					break;
		}

		if(play && beep != SFXHND_INVALID)
		{
				snd_sfx_play(beep, 255, pan);
				play = 0;
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);

		DrawStringS(120, 60, 1.0f, 1.0f, 1.0f, "Sound Test"); 
		DrawStringS(80, 120, 1.0f, sel == 0 ? 0 : 1.0f,  sel == 0 ? 0 : 1.0f, "Left Channel"); 
		DrawStringS(120, 130, 1.0f, sel == 1 ? 0 : 1.0f,  sel == 1 ? 0 : 1.0f, "Center Channel");
		DrawStringS(160, 120, 1.0f, sel == 2 ? 0 : 1.0f,  sel == 2 ? 0 : 1.0f, "Right Channel");
		DrawScanlines();
		
		pvr_list_finish();        

		pvr_scene_finish();
	}
	if(beep != SFXHND_INVALID)
			snd_sfx_unload(beep);
	FreeImage(&back);
}

void DrawCredits()
{
	int         done = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;

	back = LoadImage("/rd/back.png", 1);
    
	updateVMU("  Credits", "", 1);
	while(!done) 
	{
		int x = 30, y = 60;
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			
			if (pressed & CONT_START)
				done =  1;        
        
		MAPLE_FOREACH_END()

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);

		DrawStringS(x, y, 0.0, 1.0, 0.0, "Code and Patterns:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Artemio Urbina"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "SDK:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "KallistiOS"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "SDK Assistance:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "BlueCrab"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Menu Pixel Art:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Asher"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Advisor:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Fudoh"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Toolchain built with:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "https://github.com/losinggeneration/buildcross"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Info on using this suite:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "http://junkerhq.net/xrgb/"); y += fh; 

		DrawStringS(220, 58, 1.0, 1.0, 1.0, "Ver. 1.3"); y += fh; 

		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	FreeImage(&back);
}


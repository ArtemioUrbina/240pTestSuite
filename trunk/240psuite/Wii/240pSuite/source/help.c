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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <stdio.h>

#include "image.h"
#include "font.h"
#include "video.h"

#include "help.h"
#include "menu.h"
#include "controller.h"
#include "options.h"

char *SMPTEColor_txt[] = { 
"            SMPTE COLOR BARS (1/2)\n\nIn an SMPTE color bar image, the top two-thirds\nof the television picture contain seven vertical\nbars of 75% intensity. In order from left to\nright, the colors are gray, yellow, cyan, green,\nmagenta, red, and blue. This sequence runs \nthrough all seven possible combinations that use\nat least one of the three basic color components\nof green, red, and blue, with blue cycling on \nand off between every bar, red cycling on and \noff every two bars, and green on for the leftmost\nfour bars and off for the rightmost three.\n\n                                         (cont...)\n",
"            SMPTE COLOR BARS (2/2)\n\nBelow the main set of seven bars is a strip of \nblue, magenta, cyan, and white castellations. \nWhen a television receiver is set to filter out \nall colors except for blue, these castellations, \ncombined with the main set of color bars, are \nused to properly adjust the color controls; they\nappear as four solid blue bars, with no visible \ndistinction between the bars and the \ncastellations, if the color controls are properly\nadjusted.\n",
NULL
};
char *alt240p_txt[] = { 
"             ALTERNATING 240p/480i \n\nSome devices have a delay when the source changes\nbetween resolutions, which happens in some games.\nThis test allows to time that delay manually. \n\nPress '$A' to switch the resolution; press it again\nwhen you are able to see the screen back in your\ndisplay.\n",
NULL
};
char *backlit_txt[] = { 
"              Backlit Test (1/2)\n\nThis test allows you to check how the display's\nbacklit works when only a small array of pixels\nis shown. This can be very revealing, and can\nbe used to detect the zones in LED backlit \ndisplays, or to evaluate global and local\ndimming.\n\nThe user can move around the white pixel arrays\nwith the d-pad or joystiq, and change the size\nof the pixel array with \"$R\" and \"$L\". The $A\nbutton allows the user to hide the pixel array\nin order to alternate a fully black screen.\n\n                                     (cont...)",
"              Backlit Test (2/2)\n\nIt might be necessary to have a fully-off panel\n(if possible) when displaying the black \nbackground,  in order to evaluate full-backlit\ndisplays by adjusting brightness accordingly for\nthis test.\n\n",
NULL
};
char *bleed_txt[] = { 
"                  COLOR BLEED \n\nThis pattern helps diagnose unneeded color\nupsampling. It consists of one pixel width color\nbars alternating with one pixel width black bars.\n\nYou can change the vertical bars to a checker\nboard with the $A button.\n",
NULL
};
char *check_txt[] = { 
"             CHECKERBOARD (1/2)\n\nThis pattern shows all the visible pixels in an\nalternating white and black grid array. \n\nYou can toggle the pattern with button $X, or\nturn on auto-toggle each frame with the $A button.\n\nWhen auto-toggle is set, you should see it\nalternating rapidly. On some setups, the pattern\ndoesn't change at all. This means that the signal\nis being treated as 480i and odd or even frames \nare being discarded completely.\n\n                                         (cont...)\n",
"             CHECKERBOARD (2/2)\n\nA frame counter can be displayed on screen by \npressing button $Y.\n\nUnfortunately the Wii cannot resolve this pattern\ncorrectly in full 480p mode as other consoles,\nbut 240p and 480i resolve just fine.\n\nThe GameCube can give a better output, but it is\nnot fully visible under most conditions either.",
NULL
};
char *check480pWii_txt[] = { 
"             CHECKERBOARD 480p\n\nThis is not a problem in your console or setup.\n\nUnfortunately this hardware cannot resolve this\npattern correctly in full 480p mode as other \nconsoles.\n",
NULL
};
char *color601_txt[] = { 
"           COLOR BARS WITH GRAY SCALE\n\nThis pattern idea and layout borrows form the \nDigital Video Essentials color adjust pattern.\n\nIt has a gray background as reference, and can \nbe used with color filters in order to adjust \ncolor accuracy. If your TV set has a blue only\nmode, you can also use that to correct color\nbalance.",
NULL
};
char *colors_txt[] = { 
"               COLORBARS (1/2)\n\nThis pattern allows you to calibrate each color:\nRed, Green and Blue; as  well as the white level.\n\nThis is adjusted in the same way black levels\nare set with the PLUGE. If you adjusted the black\nlevel correctly with the PLUGE, the rectangles \nunder the \"1\" column should be barely visible.\n\nAdjust the white level first, this is done using\nthe \"Contrast\" control on your TV set. Raise it\nuntil you cannot distinguish between the two\nblocks under \"F\", and lower it slowly until you\ncan clearly tell them apart.          \n                                      (cont...)",
"               COLORBARS (2/2)\n\nThe same procedure should be used with each\ncolor, raise the control that adjusts the\nindividual color level (not the color/tint one)\nuntil you can't tell the rightmost box under \"F\"\nfrom the one to its left, and then lower it until\nyou can distinguish them clearly. \n\nRepeat for each color and you are set.\n\n\n\n\n",
NULL
};
char *dshadow_txt[] = { 
"                DROP SHADOW TEST \n\nThis is a crucial test for 240p upscan converters.\nIt displays a simple sprite shadow (32x32 pixels)\nagainst a background, but the shadow is shown only\non each other frame. On a CRT this achieves a\ntransparency effect, since you are watching a 30hz\n(25Hz) shadow on a 60hz (50hz) signal. No \nbackground detail should be lost and the shadow \nshould be visible. \n\nThe user can toggle the frame used to draw the\nshadow with button $X. Backgrounds can be\nswitched with the A button and button $Y\ntoggles sprites. \n",
NULL
};
char *general_txt[] = { 
"                  HELP (1/2)\n\nThe 240p Test Suite was designed with two goals\nin mind:\n\n1) Evaluate 240p signal processing on modern TV\nsets and video processing equipment; and \n\n2) Provide calibration patterns generated on your\ngame console to help in properly calibrating the\ndisplay's black, white and color levels.\n\nHelp is available on some patterns by pressing the\n$S button.\n\n                                         (cont...)",
"                  HELP (2/2)\n\nThis version of the suite supports 240p, 288p, \n576i, 480i and 480p video modes. However, 480p is\nonly available via component cables. \n\nIn 288p and 576i PAL modes, the system is capable\nof drawing the whole visible signal, 264 and 528\nlines respectively. However, since most screens\nare only 240p tall, the suite centers them \nvertically. Grids and other 264p patterns are \nshown using the full resolution.\n\nPAL and 480p modes must be enabled from the \noptions menu. ",
NULL
};
char *gray_txt[] = { 
"                  GRAY RAMP \n\nThis gray ramp pattern can be used to check color\nbalance and to adjust the white level. \n\nYou should make sure the bars are gray, with no\nred or blue bias. This can be adjusted with the\nindividual color settings, or the \"tint\" control.\n\nIn order to adjust white level, you need to use\nthe \"contrast\" control of your TV set. Raise the\ncontrast to the point where the white bars on the\nsides are undistinguishable from each other, and \nthen lower it to the point where you can clearly\nidentify both.",
NULL
};
char *grid_txt[] = { 
"                  GRID (1/2)\n\nThis grid uses a 320x240 pattern, and should fill\nall of the 4:3 visible area. \n\nYou can use it to verify that all the visible area\nis being displayed, and that there is no distortion\npresent. Consumer TVs may not show the red squares.\n\nA 640x480 Grid can be shown in compatible NTSC\nvideo modes. \n\nIn PAL resolutions, there are also two different \ngrids. \n\n                                    (cont...)",
"                  GRID (2/2)\n\nIn the 288p PAL video, only 264 are visible lines.\nSince 264 is not an exact multiple of 16, there is\na centered line of smaller blue rectangles. \n\nIn 576i unscaled 1:1 mode, a full 640x528 grid is\nshown. \n\nThe 240 test suite draws from line 22 onwards from \nthe blanking interval of NTSC signals, and line 23\nfor PAL. It uses the whole visible area in both.\n\nThe d-pad can be used to move the grid around the\nvideo signal. A button resets it to its position.\n",
NULL
};
char *gridscroll_txt[] = { 
"                GRID SCROLL TEST\n\nA grid is scrolled vertically or horizontally,\nwhich can be used to test linearity of the signal\nand how well the display copes with scrolling.\n\nThe button $Y can be used to toggle between\nhorizontal and vertical, while the d-pad regulates\nspeed; $A button stops and button $X changes\ndirection. \n",
NULL
};
char *ire100_txt[] = { 
"                  100 IRE(1/2)\n\nAn IRE is a unit used in the measurement of\ncomposite video signals. Its name is derived from\nthe initials of the Institute of Radio Engineers.\n\nA value of 100 IRE was originally defined to be\nthe range from black to white in a video signal.\n\nA value of 0 IRE corresponds to the zero voltage\nvalue during the blanking period. The sync pulse\nis normally 40 IRE below the zero value, so, peak\nto peak, an all white signal should be equal to\n140 IRE.\n\n                                     (cont...)",
"                  100 IRE(2/2)\n\nThe reason IRE is a relative measurement (percent)\nis because a video signal may be any amplitude.\n\nThis unit is used in the ITU-R BT.470 which\ndefines PAL, NTSC and SECAM\n\nYou can use the $R and $L buttons to change between\n100 IRE to 0 IRE in steps.\n\nWith button $A you can change the range to 100-140\nIRE.\n",
NULL
};
char *linearity_txt[] = { 
"                LINEARITY\n\nThis pattern shows 5 circles, that are matched\nto the 10:11 NTSC and the 59:54 PAL aspect ratios\ndepending on the currently selected resolution.\n\nThe linearity of the display or upscaler can be \nverified by measuring the diameter of the circles.\nOf course the linearity should be kept in all \ndirections.\n\nA grid is also available with the $L and $R \nbuttons.",
NULL
};
char *manuallag_txt[] = { 
"             MANUAL LAG TEST (1/3)\n\nThe main intention is to show a changing pattern\non the screen, and given a visual and repetitive\ncue, play a beep alternating speakers. This\nshould show to some degree the lag (if present)\nwhen processing the signal.\n\nAs an added feature, the user can click the $A\nbutton when the sprite is aligned with the one on\nthe background, and the offset in frames form the\nactual intersection will be shown on screen. This\ncan be repeated ten times and the software will\ncalculate the average. Whenever the button was\npressed before the actual intersection frame, the\n                                       (cont...)",
"             MANUAL LAG TEST (2/3)\n\nresult will be ignored (but still shown onscreen).\nButton $X can be used to change the direction\nof the sprite from vertical to horizontal, or\ndisplay both at the same time.\n\nOf course the evaluation is dependent on reflexes\nand/or rhythm more than anything. The visual and\naudio cues are the more revealing aspects which the\nuser should consider, of course the interactive\nfactor can give an experienced player the hang of\nthe system when testing via different connections.\n\n\n                                          (cont...)",
"             MANUAL LAG TEST (3/3)\n\nSince a frame is around 16 ms (1000/60), that value\nmust be considered the general error when using the\ntest results. \n\n",
NULL
};
char *overscan_txt[] = { 
"                  OVERSCAN\n\nWith this pattern you can interactively find out\nthe overscan in pixels and percentage of each\nedge in a display. \n\nUse the $R and $L buttons to increment the \noverscan until you see the red border, then go \nback one pixel. The resulting number is the\namount of overscan in pixels in each direction.",
NULL
};
char *passivelag_txt[] = { 
"                   LAG TEST \n\nThis test is designed to be used with two displays\nconected at the same time. One being a CRT, or a\ndisplay with a known lag as reference, and the\nother the display to test.\n\nUsing a camera, a picture should be taken of both\nscreens at the same time. The picture will show \nthe frame discrepancy between them. \n\nThe circles in the bottom help determine the frame\neven when the numbers are blurry.\n\nThe Sega Genesis version of the test can be used\nwith a Nomad as the reference display.\n",
NULL
};
char *pluge_txt[] = { 
"                 PLUGE (1/2)\n\nThe picture line-up generation equipment (PLUGE)\nis a test pattern used to adjust the black level\nand contrast of a video signal.\n\nThe pattern in this suite is mainly designed to\nadjust the black level. The control for setting \nthis on your TV is usually named \"Brightness\".\n\nReduce the Brightness until these bars: \n      ->   <-                        ->   <-  \nare no longer visible, and then slowly raise it\nuntil they are *just* visible. \n\n(Do it without this help window open)    (cont...)",
"                 PLUGE (2/2)\n\nThere are three bars on each side, bars closer\nto the center are at 3.5 IRE, the center ones at \n7.5 IRE and the outer ones are at 11.5 IRE. Since\nthe Wii/GC color is 8 bits per pixel it can \ncorrectly display them at exact values.\n\nThis adjustment assures that anything intended\nto be black is actually black, and allows all\ndetail to be seen on dark scenes.\n",
NULL
};
char *scroll_txt[] = { 
"                   SCROLL TEST\n\nThis test shows a two layer background from Sonic\nthe Hedgehog. The speed can be varied with the\nd-pad, and change direction with the button $X.\nThe A button stops the scroll. \n\nSonic The Hedgehog is a trademark of \nSega Enterprises Ltd.",
NULL
};
char *sharpness_txt[] = { 
"                SHARPNESS\n\nYou should set the sharpness of your CRT to a\nvalue that shows clean black and gray transitions,\nwith no white ghosting in between.\n\nOn most modern displays, the Sharpness control is \nan edge-enhancement control, and most probably\nshould be set to zero, or in the middle.",
NULL
};
char *sound_txt[] = { 
"                      SOUND\n\nThis simple test plays a sound in either or both\nchannels.\n\nIt can be used to determine if the audio chain is\nworking properly.\n",
NULL
};
char *striped_txt[] = { 
"              STRIPED SPRITE TEST\n\nThere are actually deinterlacers out there which\ncan display the drop shadows correctly and still\ninterpreted 240p as 480i. With a striped sprite it\nshould be easy to tell if a processor tries to\ndeinterlace (plus interpolate) or not.\n\nYou can change backgrounds with $A.\n",
NULL
};
char *stripes_txt[] = { 
"            HORIZONTAL STRIPES (1/2)\n\nThis pattern is designed to show if all lines are\nvisible in your setup, and how your video\nprocessor is handling 240p video. \n\nYou should see a pattern of lines, each one pixel\nin height, starting with a white one at the top of\nthe screen. You can toggle the pattern with \nbutton $X, or turn on auto-toggle each frame with\nthe $A button.\n\nWhen auto-toggle is set, you should see the lines\nalternating rapidly. On some setups, the pattern\ndoesn't change at all. This means that the signal\n                                         (cont...)",
"            HORIZONTAL STRIPES (2/2)\n\nis being treated as 480i and odd or even frames \nare being discarded completely.\n\nA frame counter can be displayed on screen by \npressing button $Y.\n\nYou can also display vertical bars by pressing $R,\nthat pattern will help you evaluate if the signal\nis not distorted horizontaly, since all one pixel\nwide lines should be of the same width.\n",
NULL
};
char *white_txt[] = { 
"                WHITE SCREEN\n\nThis pattern can be changed between white, black,\nred, green and blue full screens using the $R and \n$L buttons.\n\nSome displays and scalers have issues when\nchanging between a fully black and a fully white\nscreen.\n\nIt can also be used to check color purity, and \nfor dead pixels.\n",
NULL
};

char *PrepareHelpText(char *text)
{
	u32  tsize = 0, pos = 0, ntextpos = 0;
	char *ntext = NULL;
	char **ControllerButtons = NULL;
	
	if(!text)
		return NULL;
	tsize = strlen(text);
	ntext = (char*)malloc(sizeof(char)*tsize+20);
	if(!ntext)
		return NULL;
		
#ifdef WII_VERSION
	if(ControllerType == ControllerWiimote)
		ControllerButtons = ControlNamesWii;

	if(ControllerType == ControllerWiiClassic)
	{
		if(Options.SFCClassicController)
			ControllerButtons = ControlNamesWiiSFC_CC;
		else
			ControllerButtons = ControlNamesWiiCC;
	}
	
	if(ControllerType == ControllerGC)
#endif
		ControllerButtons = ControlNamesGC;
	
	for(pos = 0; pos < tsize; pos++)
	{
		if(text[pos] != '$')
			ntext[ntextpos++] = text[pos];
		else
		{
			if(pos + 1 < tsize)
			{
				char button;
				char *buttonName = NULL;
				u32  bNamePos = 0, bNameLen= 0;
				
				button = text[pos+1];
				switch(button)
				{
					case 'S':
						buttonName = ControllerButtons[ControlButtonStart];
						break;
					case 'A':
						buttonName = ControllerButtons[ControlButtonA];
						break;
					case 'B':
						buttonName = ControllerButtons[ControlButtonB];
						break;
					case 'X':
						buttonName = ControllerButtons[ControlButtonX];
						break;
					case 'Y':
						buttonName = ControllerButtons[ControlButtonY];
						break;
					case 'L':
						buttonName = ControllerButtons[ControlButtonL];
						break;
					case 'R':
						buttonName = ControllerButtons[ControlButtonR];
						break;
					default:
						buttonName = ControllerButtons[ControlButtonA];
						break;
				}
				pos++;
				bNameLen = strlen(buttonName);
				for(bNamePos = 0; bNamePos < bNameLen; bNamePos++)				
				{
					if(ntextpos+1 < tsize+20)
						ntext[ntextpos++] = buttonName[bNamePos];
				}
			}
		}
	}
	if(ntextpos+1 < tsize+20)
		ntext[ntextpos++] = '\0';
	return ntext;
}

void DrawHelpWindow()
{	
	ImagePtr	cFB;	
	
	cFB = CopyFrameBufferToImage();	
	if(cFB)
		cFB->alpha = 0x77;
	HelpWindow(cFB);
	HelpData = GENERALHELP;
	FreeImage(&cFB);
}

void HelpWindow(ImagePtr screen)
{
	int 			done = 0, npages = 0, page = 0, refresh = 1;
	u32				pressed = 0;		
	ImagePtr		back;
	char 			*HelpDisplay = NULL;

	if(!HelpData)
		return;
		
	while(HelpData[page++] != NULL)
		npages ++;
	
	page = 0;

	back = LoadImage(HELPIMG, 0);
	if(!back)
		return;
		
	back->alpha = 0xAA;
			
	while(!done) 
	{
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
					
		if (pressed & PAD_BUTTON_A)
			refresh = 1;
			
		if (pressed & PAD_BUTTON_B)
			done =	1;	
							
		if (pressed & PAD_BUTTON_LEFT)
		{
			page --;
			refresh = 1;
		}
		
		if (pressed & PAD_BUTTON_RIGHT)
		{			
			page ++;
			refresh = 1;
		}

		if(page > npages - 1)
			page = npages - 1;

		if(page < 0)
			page = 0;
			
		if(refresh)
		{
			if(HelpDisplay)
			{
				free(HelpDisplay);
				HelpDisplay = NULL;
			}
			HelpDisplay = PrepareHelpText(HelpData[page]);
			refresh = 0;
		}

		StartScene();
		if(screen)
			DrawImage(screen);
		DrawImage(back);
		DrawStringS(34, 42, 0xff, 0xff, 0xff, HelpDisplay); 

		DrawStringS(120, 200, 0xee, 0xee, 0xee, "Press B to return"); 
		EndScene();
	}
	
	if(HelpDisplay)
	{
		free(HelpDisplay);
		HelpDisplay = NULL;
	}
	
	FreeImage(&back);
	return;
}



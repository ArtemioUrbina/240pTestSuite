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
"              SMPTE COLOR BARS (1/3)\n\nThis pattern can be used to calibrate for NTSC\nlevels regarding contrast and brightness, and \nappropiate colors as well. \n\nYou can toggle between 75% and 100% SMPTE color \nbars by pressing $A.\n\nWhen in PAL this pattern shows the #YEBU#Y color\nbars intead, since setup starts at 0mV.\n\nRemember that the black level in the SMPTE pattern\nrefers to 7.5 IRE for video, console games usually\nstart at 0 IRE. This pattern can be used to \n                                         #C(cont...)#C",
"              SMPTE COLOR BARS (2/3)\n\ncalibrate colors - or auto calibrated in a \nprofessional display - and then black levels can\nbe adjusted with the PLUGE pattern using the #GFull \nRGB Mode#G.\n\nIn an SMPTE color bar image, the top two-thirds\nof the television picture contain seven vertical\nbars of 75% intensity. In order from left to\nright, the colors are gray, yellow, cyan, green,\nmagenta, red, and blue. This sequence runs \nthrough all seven possible combinations that use\nat least one of the three basic color components\nof green, red, and blue, with blue cycling on \n                                         #C(cont...)#C",
"              SMPTE COLOR BARS (3/3)\n\nand off between every bar, red cycling on and \noff every two bars, and green on for the leftmost\nfour bars and off for the rightmost three.\n\nBelow the main set of seven bars is a strip of \nblue, magenta, cyan, and white castellations. \nWhen a television receiver is set to filter out \nall colors except for blue, these castellations, \ncombined with the main set of color bars, are \nused to properly adjust the color controls; they\nappear as four solid blue bars, with no visible \ndistinction between the bars and the \ncastellations, if the color controls are properly\nadjusted.",
NULL
};
char *alt240p_txt[] = { 
"              ALTERNATING 240p/480i \n\nSome devices have a delay when the source changes\nbetween resolutions, which happens in some games.\nThis test allows to time that delay manually. \n\nPress '$A' to switch the resolution; press it again\nwhen you are able to see the screen back in your\ndisplay.\n",
NULL
};
char *backlit_txt[] = { 
"                BACKLIT TEST (1/2)\n\nThis test allows you to check how the display's\nbacklit works when only a small array of pixels\nis shown. This can be very revealing, and can\nbe used to detect the zones in LED backlit \ndisplays, or to evaluate global and local\ndimming.\n\nThe user can move around the white pixel arrays\nwith the d-pad or joystiq, and change the size\nof the pixel array with \"$R\" and \"$L\". The $A\nbutton allows the user to hide the pixel array\nin order to alternate a fully black screen.\n\n                                     #C(cont...)#C",
"                BACKLIT TEST (2/2)\n\nIt might be necessary to have a fully-off panel\n(if possible) when displaying the black \nbackground,  in order to evaluate full-backlit\ndisplays by adjusting brightness accordingly for\nthis test.\n\n",
NULL
};
char *bleed_txt[] = { 
"                   COLOR BLEED \n\nThis pattern helps diagnose unneeded color\nupsampling. It consists of one pixel width color\nbars alternating with one pixel width black bars.\n\nYou can change the vertical bars to a checker\nboard with the $A button.\n",
NULL
};
char *check_txt[] = { 
"                CHECKERBOARD (1/2)\n\nThis pattern shows all the visible pixels in an\nalternating white and black grid array. \n\nYou can toggle the pattern with button $X, or\nturn on auto-toggle each frame with the $A button.\n\nWhen auto-toggle is set, you should see it\nalternating rapidly. On some setups, the pattern\ndoesn't change at all. This means that the signal\nis being treated as 480i and odd or even frames \nare being discarded completely.\n\n                                         #C(cont...)#C\n",
"                CHECKERBOARD (2/2)\n\nA frame counter can be displayed on screen by \npressing button $Y.\n\nUnfortunately the Wii cannot resolve this pattern\ncorrectly in full 480p mode as other consoles,\nbut 240p and 480i resolve just fine.\n\nThe GameCube can give a better output, but it is\nnot fully visible under most conditions either.",
NULL
};
char *check480pWii_txt[] = { 
"                CHECKERBOARD 480p\n\nThis is not a problem in your console or setup.\n\nUnfortunately this hardware cannot resolve this\npattern correctly in full 480p mode as other \nconsoles.\n",
NULL
};
char *color601_txt[] = { 
"            COLOR BARS WITH GRAY SCALE\n\nThis pattern idea and layout borrows from the \n#YDigital Video Essentials#Y color adjust pattern.\n\nIt has a gray background as reference, and can \nbe used with color filters in order to adjust \ncolor accuracy. If your display set has a blue \nonly mode, you can also use that to correct \ncolor balance.",
NULL
};
char *colors_txt[] = { 
"                 COLORBARS (1/2)\n\nThis pattern allows you to calibrate each color:\nRed, Green and Blue; as  well as the white level.\n\nThis is adjusted in the same way black levels\nare set with the #GPLUGE#G. If you adjusted the black\nlevel correctly with the PLUGE, the rectangles \nunder the #G1#G column should be barely visible.\n\nAdjust the white level first, this is done using\nthe #YContrast#Y control on your TV set. Raise it\nuntil you cannot distinguish between the two\nblocks under #GF#G, and lower it slowly until you\ncan clearly tell them apart.          \n                                      #C(cont...)#C",
"                 COLORBARS (2/2)\n\nThe same procedure should be used with each\ncolor, raise the control that adjusts the\nindividual color level (not the #Ycolor#Y/#Ytint#Y one)\nuntil you can't tell the rightmost box under #GF#G\nfrom the one to its left, and then lower it until\nyou can distinguish them clearly. \n\nRepeat for each color and you are set.\n\n\n\n\n",
NULL
};
char *dshadow_txt[] = { 
"                 DROP SHADOW TEST \n\nThis is a crucial test for 240p upscan converters.\nIt displays a simple sprite shadow (32x32 pixels)\nagainst a background, but the shadow is shown only\non each other frame. On a CRT this achieves a\ntransparency effect, since you are watching a 30hz\n(25Hz) shadow on a 60hz (50hz) signal. No \nbackground detail should be lost and the shadow \nshould be visible. \n\nThe user can toggle the frame used to draw the\nshadow with button $X. Backgrounds can be\nswitched with the $A button and button $Y\ntoggles sprites. ",
NULL
};
char *general_txt[] = { 
"                    HELP (1/3)\n\nThe 240p Test Suite was designed with two goals\nin mind:\n\n1) Evaluate 240p signal processing on modern TV\nsets and video processing equipment; and \n\n2) Provide calibration patterns generated on your\ngame console to help in properly calibrating the\ndisplay's black, white and color levels.\n\nHelp is available on some patterns by pressing the\n$S button.\n\n                                         #C(cont...)#C",
"                    HELP (2/3)\n\nThis version of the suite supports 240p, 288p, \n576i, 480i and 480p video modes. However, 480p is\nonly available via component cables. \n\nIn 288p and 576i PAL modes, the system is capable\nof drawing the whole visible signal, 264 and 528\nlines respectively. However, since most screens\nare only 240p tall, the suite centers them \nvertically. Grids and other 264p patterns are \nshown using the full resolution.\n\nPAL and 480p modes must be enabled from the \noptions menu. \n                                         #C(cont...)#C",
"                    HELP (2/3)\n\nThe 240p suite is also available in other systems,\nincluding: #YSega Genesis#Y/#YMega Drive#Y, #YSega CD#Y/\n#YMega CD#Y, #YNintendo GameCube#Y, #YNintendo Wii#Y and\n#YSega Dreamcast#Y.",
NULL
};
char *gray_txt[] = { 
"                    GRAY RAMP \n\nThis gray ramp pattern can be used to check color\nbalance and to adjust the white level. \n\nYou should make sure the bars are gray, with no\nred or blue bias. This can be adjusted with the\nindividual color settings, or the #Ytint#Y control\nin NTSC displays.\n\nIn order to adjust white level, you need to use\nthe #Ycontrast#Y control of your TV set. Raise the\ncontrast to the point where the white bars on the\nsides are undistinguishable from each other, and \nthen lower it to the point where you can clearly\nidentify both.",
NULL
};
char *grid_txt[] = { 
"                    GRID (1/3)\n\nThis grid uses a 320x240 pattern, and should fill\nall of the 4:3 visible area. \n\nYou can use it to verify that all the visible area\nis being displayed, and that there is no distortion\npresent. Consumer TVs may not show the red squares.\n\nA 640x480 Grid can be shown in compatible NTSC\nvideo modes. \n\nIn PAL resolutions, there are also two different \ngrids. \n\n                                    #C(cont...)#C",
"                    GRID (2/3)\n\nIn the 288p PAL video, only 264 are visible lines.\nSince 264 is not an exact multiple of 16, there is\na centered line of smaller blue rectangles. \n\nIn 576i unscaled 1:1 mode, a full 640x528 grid is\nshown. You can also use the 576i stretched option \nto fill the whole screen at the cost of 1:1 pixel \nmapping.\n\nThe 240 test suite draws from line 22 onwards from \nthe blanking interval of NTSC signals, and line 23\nfor PAL. You can adjust this value for PAL in 1:1\nmapping modes.\n                                    #C(cont...)#C",
"                    GRID (3/3)\n\nThe d-pad can be used to move the grid around the\nvideo signal. $A button resets it to its position.\n",
NULL
};
char *gridscroll_txt[] = { 
"                 GRID SCROLL TEST\n\nA grid is scrolled vertically or horizontally,\nwhich can be used to test linearity of the signal\nand how well the display or video processor copes\nwith scrolling and framerate.\n\nButton $Y can be used to toggle between horizontal\nand vertical, while the d-pad regulates speed. \nButton $A stops the scroll and button $X changes\ndirection. \n",
NULL
};
char *ire100_txt[] = { 
"                   100 IRE(1/2)\n\nAn IRE is a unit used in the measurement of\ncomposite video signals. Its name is derived from\nthe initials of the #YInstitute of Radio Engineers#Y.\n\nA value of 100 IRE was originally defined to be\nthe range from black to white in a video signal.\n\nA value of 0 IRE corresponds to the zero voltage\nvalue during the blanking period. The sync pulse\nis normally 40 IRE below the zero value, so, peak\nto peak, an all white signal should be equal to\n140 IRE.\n\n                                     #C(cont...)#C",
"                   100 IRE(2/2)\n\nThe reason IRE is a relative measurement (percent)\nis because a video signal may be any amplitude.\n\nThis unit is used in the #YITU-R BT.470#Y which\ndefines PAL, NTSC and SECAM\n\nYou can press $R and $L buttons advance in 10 IRE\nsteps, or hold $X or $Y to quickly advance in the\nselected range.\n\nWith button $A you can toggle the range between\n#C[0-100]#C and #C[100-140]#C IRE.",
NULL
};
char *linearity_txt[] = { 
"                    LINEARITY\n\nThis pattern shows 5 circles, that are matched\nto the 10:11 NTSC and the 59:54 PAL aspect ratios\ndepending on the currently selected resolution.\n\nThe linearity of the display or upscaler can be \nverified by measuring the diameter of the circles.\nOf course the linearity should be kept in all \ndirections.\n\nA grid is also available with the $L and $R \nbuttons.",
NULL
};
char *manuallag_txt[] = { 
"              MANUAL LAG TEST (1/3)\n\nThe main intention is to show a changing pattern\non the screen, which can be complemented with \naudio and vibration on the controller if set. \nThis should show to some degree any lag when  \nprocessing the signal.\n\nAs an added feature, the user can click the $A\nbutton when the sprite is aligned with the one on\nthe background, and the offset in frames form the\nactual intersection will be shown on screen. This\ncan be repeated ten times and the software will\ncalculate the average. Whenever the button was\npressed before the actual intersection frame, the\n                                        #C(cont...)#C",
"              MANUAL LAG TEST (2/3)\n\nresult will be ignored (but still shown onscreen).\nButton $X can be used to change the direction\nof the sprite from vertical to horizontal, or\ndisplay both at the same time.\n\n\n#ROf course the evaluation is dependent on reflexes\nand/or rhythm more than anything.#R The visual and\naudio cues are the more revealing aspects which \nthe user should consider, of course the interactive\nfactor can give an experienced player the hang of\nthe system when testing via different connections.\n\n                                        #C(cont...)#C",
"              MANUAL LAG TEST (3/3)\n\nSince a frame is around 16 ms (1000/60) in NTSC \nand 20 ms (1000/50) in PAL, that value must be \nconsidered the general error when using the test \nresults. \n\n",
NULL
};
char *options_txt[] = { 
"                  OPTIONS (1/4)\n\n#GShow Wii region:#G Determines if the Wii region\n    is shown in the lower left corner.\n\n#GEnable 480p Modes:#G If a component cable is\n    present, this option enables progressive\n    640x480 modes.\n\n#GEnable PAL Modes:#G These are disabled by default\n    since many NTSC TVs don't support them.\n\n#GEnable PAL Background:#G Since most of the menus\n    and screens are designed for 240p, this fills\n    the borders with the selected color, however\n                                        #C(cont...)#C",
"                  OPTIONS (2/4)\n\n    there are specific PAL patterns that do not \n    need this, such as grids and overscan.\n\n#GChange PAL Background:#G Select the fill color for\n    the PAL background.\n                \n#GPAL starting line:#G PAL specifies the start of\n    video data at line 23 1/2, this setting allows\n    starting from line 24, centering the 528 \n    active video lines in the 576i signal or \n    moving the whole screen to the bottom.\n\n\n                                        #C(cont...)#C",
"                  OPTIONS (3/4)\n\n#GStretch to full 288/576:#G Since the $H can't render\n    the full 288p/576i screen in 1:1 pixel \n    mapping, this option enables a video mode for \n    all PAL settings where the 264/528 lines are \n    stretched vertically to fill the 576i PAL \n    signal. #YThis is uneven and should only be \n    used to visually check for overscan.#Y\n\n#G480p Scanline Intensity:#G When in line doubled\n    480p mode, the suite allows the display of\n    rendered scanlines. This determines the alpha\n    value for them, 0% to eliminates them.\n\n                                        #C(cont...)#C",
"                  OPTIONS (4/4)\n\n#G480p Scanlines:#G This setting allows the display\n    or EVEN or ODD scanline display in 480p line\n    doubled mode.\n\n#GSFC Classic Controller:#G The #YSuper Famicom#Y\n    version of the #YClassic Controller#Y needs a \n    different button layout since it doesn't have\n    a HOME button. This changes the layout for it.",
NULL
};
char *overscan_txt[] = { 
"                     OVERSCAN\n\nWith this pattern you can interactively find out\nthe overscan in pixels and percentage of each\nedge in a display. \n\nUse the $R and $L buttons to increment the \noverscan until you see the red border, then go \nback one pixel. The resulting number is the\namount of overscan in pixels in each direction.\n\n#YWhen in 576i stretched PAL modes, the pixel\ncount is not accurate#Y. For precise results, \ninstead use the #GPAL start line option#G in #Y23 1/2#Y\nand #YBottom#y modes, each one to count top or bottom\noverscan accurately.\n",
NULL
};
char *passivelag_txt[] = { 
"                  LAG TEST (1/2)\n\nThis test is designed to be used with two displays\nconected at the same time. One being a CRT, or a\ndisplay with a known lag as reference, and the\nother the display to test.\n\nUsing a camera, a picture should be taken of both\nscreens at the same time. The picture will show \nthe frame discrepancy between them. \n\nThe circles in the bottom help determine the frame\neven when the numbers are blurry.\n\nThe #YSega Genesis#Y version of the test can be used\nwith a #YNomad#Y as the reference display.   #C(cont...)#C",
"                  LAG TEST (2/2)\n\nYou can also chain the output ports of a #YPVM/BVM#Y\nto an upscaler and use the CRT as reference.",
NULL
};
char *pluge_txt[] = { 
"                   PLUGE (1/3)\n\nThe #Ypicture line-up generation equipment#Y (PLUGE)\nis a test pattern used to adjust the black level\nand contrast of a video signal.\n\nThe pattern in this suite is mainly designed to\nadjust the black level. The control for setting \nthis on your TV is usually named #YBrightness#Y.\n\nThis adjustment ensures that anything intended\nto be black is actually black, and allows all\ndetail to be seen on dark scenes.\n\nThere are three bars at each side, you can \nhighlight them with the $X button.        #C(cont...)#C",
"                   PLUGE (2/3)\n\nThe $A button changes between #GNTSC#G and #GRGB Full \nRange#G [0-255] when in any NTSC mode.        \n                                         \nThe values of the bars depend on your current\nvideo mode. Reduce the #YBrightness#Y until the \ndesired bars are no longer visible, and then \nslowly raise it until they are #Gjust#G visible,\nand go back one notch where they blend with the\nbackground. \n\nValues for the bars when in NTSC mode are:  \nclosest to the center 3.5 IRE, central ones \n7.5 IRE, and outer ones 11.5 IRE. \n                                         #C(cont...)#C",
"                   PLUGE (3/3)\n\nWhen calibrating for NTSC material, the 7.5 IRE\nbars should blend in the background. The 7.5 IRE\nin NTSC is the black level, also known as setup.\n\nWhen in PAL mode or #GRGB Full Range#G, the 2 IRE \nbars are the ones you should calibrate against.",
NULL
};
char *scroll_txt[] = { 
"                    SCROLL TEST\n\nThis test shows a two layer background from #YSonic\nthe Hedgehog#Y. The speed can be varied with the\nd-pad, and change direction with the button $X.\nThe A button stops the scroll. \n\nThis can be used to notice any drops in framerate\nwhen using a video processor.\n\n#YSonic The Hedgehog#Y is a trademark of \n#YSega Enterprises Ltd#Y.",
NULL
};
char *sharpness_txt[] = { 
"                    SHARPNESS\n\nYou should set the sharpness of your CRT to a\nvalue that shows clean black and gray transitions,\nwith no white ghosting in between.\n\nOn most modern displays, the #Ysharpness#Y control is \nan edge-enhancement control, and most probably\nshould be set to zero, or in the middle.\n\nIn some #YPVM#Y/#YBVM#Y displays this is also referred \nto as #Yaperture#Y.",
NULL
};
char *sound_txt[] = { 
"                      SOUND\n\nThis simple test plays a sound in either or both\nchannels.\n\nIt can be used to determine if the audio chain is\nworking properly.\n",
NULL
};
char *striped_txt[] = { 
"               STRIPED SPRITE TEST\n\nThere actually are deinterlacers out there which\ncan display the drop shadows correctly and still\ninterpreted 240p as 480i. With a striped sprite it\nshould be easy to tell if a processor tries to\ndeinterlace (plus interpolate) or not.\n\nYou can change backgrounds with $A.\n",
NULL
};
char *stripes_txt[] = { 
"             HORIZONTAL STRIPES (1/2)\n\nThis pattern is designed to show if all lines are\nvisible in your setup, and how your video\nprocessor is handling 240p video. \n\nYou should see a pattern of lines, each one pixel\nin height, starting with a white one at the top of\nthe screen. You can toggle the pattern with \nbutton $X, or turn on auto-toggle each frame with\nthe $A button.\n\nWhen auto-toggle is set, you should see the lines\nalternating rapidly. On some setups, the pattern\ndoesn't change at all. This means that the signal\n                                         #C(cont...)#C",
"             HORIZONTAL STRIPES (2/2)\n\nis being treated as 480i and odd or even frames \nare being discarded completely.\n\nA frame counter can be displayed on screen by \npressing button $Y.\n\nYou can also display vertical bars by pressing $R,\nthat pattern will help you evaluate if the signal\nis not distorted horizontaly, since all lines \nshould be one pixel wide.\n\n#YThis pattern is completely distorted when using\nthe 576i stretched mode.#Y",
NULL
};
char *vmode_txt[] = { 
"                VIDEO MODES (1/8)\n\nThe $H version of the 240p suite supports the \nfollowing video modes:\n\n#YNTSC compatible modes (60hz):#Y The first 21\nlines of NTSC are reserved and cannot contain \nvideo material in broadcast standards, since \nline 21 usually has close caption (CC) data.\nConsole games typically respect this, since \ntheir target display were consumer TVs. It has\n525 lines in interlaced mode, of which only 480\nare visible, 240 in each field. The following\nmodes start at line 22 of every field.\n\n                                   #C(cont...)#C",
"                VIDEO MODES (2/8)\n\n #G240p:#G The progressive variant of NTSC 525i\n    line video, it has 262 lines of which 240\n    video lines are active. This format is \n    used in most SD game consoles, from the \n    #YAtari VCS#Y to the #YPS2#Y. The $H version\n    of the suite displays a full 320x240p screen.\n\n #G480i scaled 240p assets (NTSC):#G This displays\n    the 240p patterns in 480i mode. It uses the \n    480 visible lines, 240 in each field. The \n    whole content of the suite is linedoubled in \n    this mode.\n\n                                   #C(cont...)#C",
"                VIDEO MODES (3/8)\n\n #G480i mixed 480p/240p assets (1:1/NTSC):#G This\n    mode has an identical 480i NTSC signal, and\n    the content of the suite is never line \n    doubled.\n\n    As a result, most screens end up filling 1/4th\n    of the screen in 1:1 pixel mode. However, \n    there are several patterns in 480 resolution\n    that fill the screen, such as the grid, scroll\n    and overscan.\n\n\n\n                                   #C(cont...)#C",
"                VIDEO MODES (4/8)\n\n#YPAL compatible modes (50hz):#Y These modes must\nbe enabled from the #YOptions#Y menu.  The first\n22 lines of PAL are reserved and cannot contain\nvideo material in broadcast standards. Console \ngames typically respect this, since their target \ndisplay were consumer TVs. It has 625 lines in \ninterlaced mode, of which only 576 are visible, \n288 in each field. \n\nThe $H cannot use the whole 288 lines as active\nvideo while doing 1:1 pixel mapping, instead it\ncan draw 264 lines on each field.\n\n                                   #C(cont...)#C",
"                VIDEO MODES (5/8)\n\nIn order to help overcome this limitation, the\n240p suite adds several options for evaluation of\nPAL modes. \n\nThe first and recommended one, is using the #GPAL \nstarting line#G option. It allows the PAL active\nvideo to start at either line 23 1/2, 24, centered\nin the signal, or at the bottom of the signal. \nThis allows using the 264/528 patterns to verify\noverscan, alignment, geometry, etc.\n\n\n\n                                   #C(cont...)#C",
"                VIDEO MODES (6/8)\n\nThe other option - #GStretch to full 288/576#G - must\nonly be used to visually check how the whole video\nsignal is filled. It works by using the same \n264/528 patterns stretched by the $H hardware to \nfill the active video, resulting in distorted \npatterns that are not 1:1 pixel mapped. \n\nFinally, the #GEnable PAL Background#G option fills\nthe whole screen with the selected color, enabling\nvisualization of the full 264/528 active video \nsignal when the menu or pattern shown does not \nmatch the current resolution. This can be combined \nwith the stretched video option as well.\n                                   #C(cont...)#C",
"                VIDEO MODES (7/8)\n\nThe following modes start at line 23 1/2 of every\nfield, as the PAL standard determines.\n\n #G288p#G: The progressive variant of PAL 625i\n    line video, it has 312 lines of which 288\n    video lines are active. This format is \n    used in most SD game consoles. The $H version\n    of the suite displays a 320x264p screen.\n\n #G576i scaled 264/240p assets (PAL):#G This displays\n    the 264p patterns in 576i mode. It uses the \n    528 visible lines, 264 in each field. The \n    whole content of the suite is linedoubled in \n    this mode.                     #C(cont...)#C",
"                VIDEO MODES (8/8)\n\n#G576i 528/480/240p assets (1:1/PAL):#G This\n    mode has an identical 576i PAL signal, and\n    the content of the suite is never line \n    doubled.\n\n    As a result, most screens end up filling 1/4th\n    of the screen in 1:1 pixel mode. However, \n    there are several patterns in 528 resolution\n    that fill the screen, such as the grid, scroll\n    and overscan.\n\nAll of these PAL modes do have a stretched variant\nwhen that option is enabled.\n",
NULL
};
char *white_txt[] = { 
"                WHITE SCREEN (1/2)\n\nThis pattern can be changed between white, black,\nred, green, blue and custom color full screens by \nusing the $R and $L buttons.\n\nPressing $A while displaying the black screen and \nin NTSC modes will toggle between 7.5 IRE and the \n0 IRE below black levels. \n\nPressing $A while displaying the white screen will\nenter color edit mode, to select the RGB color to\nfill the screen with. Use the d-pad, $X and $Y\nbuttons to change each color.\n\n                                   #C(cont...)#C",
"                WHITE SCREEN (2/2)\n\nSome displays and scalers have issues when\nchanging between a black 0 IRE and a white screen.\n\nThis pattern can also be used to check color \npurity, and for dead pixels in some displays.\n",
NULL
};

#define	HELPCOUNT	28
char **HelpArray[HELPCOUNT] = {
	SMPTEColor_txt,
	alt240p_txt,
	backlit_txt,
	bleed_txt,
	check_txt,
	check480pWii_txt,
	color601_txt,
	colors_txt,
	dshadow_txt,
	general_txt,
	gray_txt,
	grid_txt,
	gridscroll_txt,
	ire100_txt,
	linearity_txt,
	manuallag_txt,
	options_txt,
	overscan_txt,
	passivelag_txt,
	pluge_txt,
	scroll_txt,
	sharpness_txt,
	sound_txt,
	striped_txt,
	stripes_txt,
	vmode_txt,
	white_txt,
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
	ntext = (char*)malloc(sizeof(char)*tsize+40);
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
					case 'H':
#ifdef WII_VERSION					
						buttonName = "Wii";
#else
						buttonName = "GC";
#endif
						break;
					default:
						buttonName = ControllerButtons[ControlButtonA];
						break;
				}
				pos++;
				
				ntext[ntextpos++] = '#';
				ntext[ntextpos++] = 'G';
				
				bNameLen = strlen(buttonName);
				for(bNamePos = 0; bNamePos < bNameLen; bNamePos++)				
				{
					if(ntextpos+1 < tsize+20)
						ntext[ntextpos++] = buttonName[bNamePos];
				}
				
				ntext[ntextpos++] = '#';
				ntext[ntextpos++] = 'G';
			}
		}
	}
	if(ntextpos+1 < tsize+40)
		ntext[ntextpos++] = '\0';
	return ntext;
}

void ShowAllHelp()
{
	int h = 0;
	
	for(h = 0; h < HELPCOUNT; h++)
	{
		HelpData = HelpArray[h];
		DrawHelpWindow();
	}
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



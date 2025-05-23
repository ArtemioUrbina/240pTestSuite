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


/****** paste the results from converthelpfiles starts here **************************/
char *alt240p_txt[] = { 
"              ALTERNATING 240p/480i \n\nSome devices have a delay when the source changes\nbetween resolutions, which happens in some games.\nThis test allows to time that delay manually. \n\nPress $A to switch the resolution; press it again\nwhen you are able to see the screen back in your\ndisplay.\n",
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
"                CHECKERBOARD (1/2)\n\nThis pattern shows all the visible pixels in an\nalternating white and black grid array. \n\nYou can toggle the pattern with button $X, or\nturn on auto-toggle each frame with the $A button.\n\nWhen auto-toggle is set, you should see it\nalternating rapidly. On some setups, the pattern\ndoesn't change at all. This means that the signal\nis being treated as 480i/576i and odd or even \nframes are being discarded completely.\n\n\n                                         #C(cont...)#C",
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
"                 COLORBARS (1/2)\n\nThis pattern allows you to calibrate each color:\nRed, Green and Blue; as  well as the white level.\n\nThe default pattern shows 32 shades of each color,\nbut this platform is capable of 256. You can view\nthe last 32 and the first ones by pressing left \nor right. An extra pattern is also availabe with a \nwhite grid as background, for contrast balance.\n\nThis is adjusted in the same way black levels\nare set with the #GPLUGE#G. If you adjusted the black\nlevel correctly with the PLUGE, the rectangles \nunder the #G1#G column should be barely visible.\n                                      #C(cont...)#C",
"                 COLORBARS (2/2)\n\nAdjust the white level first, this is done using\nthe #YContrast#Y control on your TV set. Raise it\nuntil you cannot distinguish between the two\nblocks under #GF#G, and lower it slowly until you\ncan clearly tell them apart.  \n\nThe same procedure should be used with each\ncolor, raise the control that adjusts the\nindividual color level (not the #Ycolor#Y/#Ytint#Y one)\nuntil you can't tell the rightmost box under #GF#G\nfrom the one to its left, and then lower it until\nyou can distinguish them clearly. \n\nRepeat for each color and you are set.",
NULL
};
char *convergence_txt[] = { 
"                 CONVERGENCE TESTS\n\nThese are used to adjust color convergence in CRT \ndisplays.\n\n- Button 'A' changes the pattern between cross\nhatch lines, crosses, dots and a color pattern \nwith or without a black grid, for transition \nboundary check.\n\n- Left and Right on the D-Pad allow cycling \nbetween patterns.\n\n",
NULL
};
char *diagonal_txt[] = { 
"            Diagonal line patterns\n\nThese patterns can be used to adjust the vertical\nand horizontal scaling in video procesors in\ndifferent resolutions. The test uses higher \nresolution assets for 480p when in that mode.\n\nThe angle increment for line rotation can be\nadjusted by fractions up to 1/20th of a degree\nwith $X and $Y. The angle can be adjusted with\n$L and $R.\n\nThe bars can be set to autorotate with $A and the\nbars can be toggled to full screen by pressing Up.",
NULL
};
char *disappear_txt[] = { 
"              Disappearing Logo\n\nThis test allows you to use a slow motion camera\nto measure how long it takes from a button\npress to when the logo disappears.\n\nYou can also use a photodiode and press #GX#G,\nwhich turns the whole back to white for 2 frames.",
NULL
};
char *dshadow_txt[] = { 
"                 DROP SHADOW TEST\n\nThis is a crucial test for 240p upscan converters.\nIt displays a simple sprite shadow (32x32 pixels)\nagainst a background, but the shadow is shown only\non each other frame. On a CRT this achieves a\ntransparency effect, since you are watching a 30hz\n(25Hz) shadow on a 60hz (50hz) signal. No \nbackground detail should be lost and the shadow \nshould be visible. \n\nThe user can toggle the frame used to draw the\nshadow with button $X. Backgrounds can be\nswitched with the $A button and button $Y\ntoggles sprites.\n\nDonna art by Jose Salot",
NULL
};
char *gba_transfer_txt[] = { 
"      GBA 160p test Suite via Link Cable\n\nThis tool allows the transfer of pinobatch's\nport of the Suite to Game Boy Advance.\n\nYou need to have a link cable connected to a\ncontroller port and the GBA turned on without\na catridge.\n\nThe GBA ROM will be transferred and you'll be\nable to use the software even with the cable\ndetached, until you turn off the system.\n\n#Chttps://github.com/pinobatch/240p-test-mini#C\n\n\n",
NULL
};
char *general_txt[] = { 
"                    HELP (1/3)\n\nThe 240p Test Suite was designed with two goals\nin mind:\n\n1) Evaluate 240p signal processing on modern TV\nsets and video processing equipment; and \n\n2) Provide calibration patterns generated on your\ngame console to help in properly calibrating the\ndisplay's black, white and color levels.\n\nHelp and options are available everywhere by \npressing the $S button.\n#Y[Please press right for next help page]#Y\n                                         #C(cont...)#C",
"                    HELP (2/3)\n\nThis version of the suite supports 240p, 288p, \n576i, 480i and 480p video modes. However, 480p is\nonly available via component cables. \n\nIn 288p and 576i PAL modes, the system is capable\nof drawing the whole visible signal, 264 and 528\nlines respectively. However, since most screens\nare only 240p tall, the suite centers them \nvertically. Grids and other 264p patterns are \nshown using the full resolution.\n\nPAL and 480p modes must be enabled from the \noptions menu. \n                                         #C(cont...)#C",
"                    HELP (3/3)\n\nThe 240p suite is also available in other systems,\nincluding: #YSega Genesis#Y/#YMega Drive#Y, #YSega CD#Y/\n#YMega CD#Y, #YPC Engine#Y/#YTG-16#Y, #YSuper Nintendo#Y/#YSFC#Y, #YNES#Y,\n#YNintendo GameCube#Y, #YNintendo Wii#Y, #YNeo Geo#Y,\n#YGameBoy#Y, #YJaguar#Y, #Y32x#Y, #YSega Dreamcast#Y, etc.",
NULL
};
char *gray_txt[] = { 
"                    GRAY RAMP \n\nThis gray ramp pattern can be used to check color\nbalance and to adjust the white level. \n\nYou should make sure the bars are gray, with no\nred or blue bias. This can be adjusted with the\nindividual color settings, or the #Ytint#Y control\nin NTSC displays.\n\nIn order to adjust white level, you need to use\nthe #Ycontrast#Y control of your TV set. Raise the\ncontrast to the point where the white bars on the\nsides are undistinguishable from each other, and \nthen lower it to the point where you can clearly\nidentify both.",
NULL
};
char *grid_txt[] = { 
"                    GRID (1/3)\n\nThis grid uses a 320x240 pattern, and should fill\nall of the 4:3 visible area. \n\nYou can use it to verify that all the visible area\nis being displayed, and that there is no distortion\npresent. Consumer TVs may not show the red squares.\n\nA 640x480 Grid can be shown in compatible NTSC\nvideo modes. \n\nIn PAL resolutions, there are also two different \ngrids. \n\n                                    #C(cont...)#C",
"                    GRID (2/3)\n\nIn the 288p PAL video, only 264 are visible lines.\nSince 264 is not an exact multiple of 16, there is\na centered line of smaller blue rectangles. \n\nIn 576i unscaled 1:1 mode, a full 640x528 grid is\nshown. You can also use the 576i stretched option \nto fill the whole screen at the cost of 1:1 pixel \nmapping.\n\nThe 240 test suite draws from line 22 onwards from \nthe blanking interval of NTSC signals, and line 23\nfor PAL. You can adjust this value for PAL in 1:1\nmapping modes.\n                                    #C(cont...)#C",
"                    GRID (3/3)\n\nThe d-pad can be used to move the grid around the\nvideo signal. $A button resets it to its position.\n\nThe $X button toggles a green background.\n\nThe $Y button changes the grid type.\n",
NULL
};
char *grid224_txt[] = { 
"                    GRID 224p\n\nThis grid uses a 320x224 pattern in a 240p signal,\nit is intended to mock-up the typical display area \nfor other consoles, such as the SNES, Sega Genesis \nand Playstation.\n\nYou can use it to verify that all the visible area\nis being displayed, and that there is no distortion\npresent. Consumer TVs may not show the red squares.\n\nYou can press $Y to toggle the Grid type\nThe current modes simulate the Genesis and SNES.",
NULL
};
char *gridscroll_txt[] = { 
"                 GRID SCROLL TEST\n\nA grid is scrolled vertically, horizontally and\ndiagonally which can be used to test linearity of\nthe display or video processor; and how it copes\nwith scrolling and framerate.\n\nButton $Y can be used to switch between horizontal\nand vertical, while the d-pad regulates speed. \n\nDiagonal scroll can be enabled with the $R button.\n\nButton $A stops the scroll and button $X changes\ndirection. \n",
NULL
};
char *hcfr_txt[] = { 
"                HCFR PATTERNS (1/6)\n\nThe #YHCFR Colormeter#Y software allows for profiling\nand calibration of displays using the combination\nof a pattern generator to create test patterns,\nand a #Ctristimulous colorimeter#C to read and\nmeasure the outputs. Then you, as the user, will\nhave to change settings on your display in order\nto refine the results and attempt to match the\nchosen standard of video color reproduction.\n\nThe #YHCFR Colormeter#Y software is at the heart\nof the system. It interprets the results from the\ncolorimeter and provides #CCIE color#C differences\n(Delta E values), chromaticity diagrams, gamma\n                                         #C(cont...)#C",
"                HCFR PATTERNS (2/6)\n\ncurves and other values to assist you in\nprofiling and calibrating your display. \n\n#Ghttps://sourceforge.net/p/hcfr/wiki/Home/#G\n\nTo set up #YHCFR Colormeter#Y, attach your colorimeter\nto your computer, open the program, click\n#CFile -> New#C, and select DVD Manual as your\nselected generator. Select your colorimeter\nfrom the list, and click OK.\n\nOnce in #YHCFR Colormeter#Y, click\n#CAdvance -> Preferences#C. \n                                         #C(cont...)#C",
"                HCFR PATTERNS (3/6)\n\nUnder the #CGeneral#C tab, ensure that the section\n#CColors to add when measuring primaries and\nsecondaries#C is set to #YWhite#Y. \n\nUnder the #CReferences#C tab, choose your color\nspace. #YPAL/SECAM#Y is recommended for #CPAL CRT#C\nusers. #YSDTV - REC601 (NTSC)#Y is recommended for\n#CNTSC CRT#C users. #YHD TV REC 709#Y is recommended\nfor #CHD#C and #CUHD/4K#C in #CSDR flat panel TV#C users.\n#YsRGB#Y is recommended for #CPC VGA#C and up #CCRT\nmonitors#C. \n\n\n                                         #C(cont...)#C",
"                HCFR PATTERNS (4/6)\n\nWhite point is normally #YD65#Y (default) for\nwestern regions of all color space types.\n#YJapanese SDTV#Y often used #YD93#Y as a white point\n(which will appear cooler, or bluer, than #YD65#Y). \n\nMost gamma calculations normally default to\n#CITU-R BT.1886#C, which uses #CDisplay Gamma Power\nLaw 2.4#C. Your device may have a different native\ngamma response, so choose a different Power Law\nvalue if required. #YsRGB#Y has a specific non-linear\ngamma that cannot be customized. \n\n\n                                         #C(cont...)#C",
"                HCFR PATTERNS (5/6)\n\nEnsure your options chosen here match the test\npattern list you choose in the #C240p Test Suite#C. \n\nWhen you are ready to start measuring, click the\n#CMeasure#C menu, and select your measurement. If\nany large scale changes are made to your display,\nit is recommended to re-run a #CGray-scale sweep#C\noften, so that baseline calculations can be\naccurately made for #CDelta E#C measurements.\n\nA menu will pop up asking you to select a given\n#CDVD chapter#C and #Ccolor name#C. Use the #YL#Y and #YR#Y\nbuttons to scroll through the color list and\n                                         #C(cont...)#C",
"                HCFR PATTERNS (6/6)\n\nselect the color desired. Once ready to measure,\nclick #COK#C in #YHCFR Colormeter#Y to measure\nthe test pattern.\n\nUse the #CContinuous measure#C mode to be able to\nadjust your display's settings to match the\nvalues required for your chosen color space,\nwhite point, gamma, etc. \n\nPattern data and testing by #GDan Mons#G and\n#GKeith Raney#G.\n\n",
NULL
};
char *hcfr_menu_txt[] = { 
"             HCFR STANDARD SELECTOR\n\nThis menu shows the #CHCFR software#C profiles.\n\n- #YREC 601 (NTSC)#Y is recommended for #CNTSC CRT#C\n    users.\n- #YPAL/SECAM#Y is recommended for #CPAL CRT#C users.\n- #YREC 709#Y is recommended for #CHD#C and #CUHD/4K#C\n    in #CSDR#C flat panel TV users.\n- #YsRGB#Y is recommended for #CPC VGA#C and up #CCRT\n     monitors#C. \n\nFor #Cwestern regions#C of any color space use #YD65#Y,\nand #YD93#Y is for #CJapanese SDTVs#C that support it.\n\nAccess detailed #Yhelp#Y within the test with #GSTART#G.	",
NULL
};
char *ire100_txt[] = { 
"                   100 IRE(1/2)\n\nAn IRE is a unit used in the measurement of\ncomposite video signals. Its name is derived from\nthe initials of the #YInstitute of Radio Engineers#Y.\n\nA value of 100 IRE was originally defined to be\nthe range from black to white in a video signal.\n\nA value of 0 IRE corresponds to the zero voltage\nvalue during the blanking period. The sync pulse\nis normally 40 IRE below the zero value, so, peak\nto peak, an all white signal should be equal to\n140 IRE.\n\n                                     #C(cont...)#C",
"                   100 IRE(2/2)\n\nThe reason IRE is a relative measurement (percent)\nis because a video signal may be any amplitude.\n\nThis unit is used in the #YITU-R BT.470#Y which\ndefines PAL, NTSC and SECAM\n\nYou can press $R and $L buttons to advance in 10 IRE\nsteps in the selected range.\n\nWith button $A you can toggle the range between\n#C[0-100]#C and #C[100-140]#C IRE.",
NULL
};
char *mdfourier_txt[] = { 
"                    MDFOURIER\n\nThis test generates a series of audio signals that\nare to be recorded and analyzed for comparison\nwith the #CMDFourier#C tool on a computer.\n\nWe use the Audio Equipment test at 48khz.\n\nThe GC DAC pitch is off by 1.61663 cents, since it\nruns at 48042.704626334519572953736654804 hz.\n\nVisit #Yhttp://junkerhq.net/MDFourier#Y for details.",
NULL
};
char *mdfourier_wii_txt[] = { 
"                    MDFOURIER\n\nThis test generates a series of audio signals that\nare to be recorded and analyzed for comparison\nwith the #CMDFourier#C tool on a computer.\n\nWe use the Audio Equipment test at 48khz.\n\nVisit #Yhttp://junkerhq.net/MDFourier#Y for details.",
NULL
};
char *memory_txt[] = { 
"                 Memory Viewer\n\nThis shows selected regions of the main CPU memory\nmap.\n\nThe current address range is shown in green at the\nright from top to bottom.\n\nThe left part of the screen shows #Y0x1A0#Y bytes in\nhex.\n\n- Button A jumps to relevant memory locations.\n\n- Button X enabled CRC of current screen.\n\n- Button Y toggles #YASCII#Y mode.",
NULL
};
char *monoscope_txt[] = { 
"                 MONOSCOPE (1/4)\n\nThe Monoscope Pattern is the traditional tool\nfor calibrating CRTs. This version aims to\nprovide several key elements to ensure you make\ninformed decisions about Convergence and \nDeflection. Refer to your monitor service manual\nfor procedures and safety equipment. \n\n1. #CBrightness Adjustable Pattern#C. Electron Gun\nAlignment, or Convergence, is usually adjusted\nwith a lowered brightness. Full brightness can\nmask issues with excessive bloom, especially \nif your peak white is too high. You can double\ncheck your adjustments against the 30IRE pattern.\n                                         #C(cont...)#C",
"                 MONOSCOPE (2/4)\n\n2. #CStatic/Center Convergence#C. The center crosshair\nis used to evaluate static or center screen\nconvergence. \n\n3. #CDynamic/Edge#C can be evaluated with the patterns\non the side of the screen.\n\n4. #CCorner Convergence#C, corner purity and corner\ngeometry are tightly linked. Static and Dynamic\nconvergence must be set first before magnets can\nsuccessfully correct corner issues.\n\n\n                                         #C(cont...)#C",
"                 MONOSCOPE (3/4)\n\n5. #CAspect Ratio#C. A new feature to help set VSIZE\nand HSIZE correctly is the red squares embedded in\nthe pattern. They are calculated to display as\nperfect squares on a properly setup display.\nYou may want to start with a small amount of\nhorizontal overscan, then use a tape measure to\nadjust VSIZE such that the outside edges of the\nred square are the same in width and height.\nThis usually achieves an amount of overscan that\npreserves the Title Safe Area. Some games may not\nadhere to those boundaries, but most do.\n\n\n                                         #C(cont...)#C",
"                 MONOSCOPE (4/4)\n\n6. #CLinearity#C. Is the hardest part of the deflection\ncircuit. Scrolling tests are the best way to\nconfirm good Linearity but some compensated square\nelements have added to each corner where a tape\nmeasure may help get you started.\n\nYou can press $R and $L buttons to advance in 10 IRE\nsteps in the selected range.\n\n#CPattern designed by Keith Raney#C\n",
NULL
};
char *options_txt[] = { 
"                  OPTIONS (1/5)\n\n#GVI Trap Filter (Composite):#G A Trap filter is\n    generally used to improve #YLuma#Y/#YChroma#Y\n    separation in the #Ycomposite#Y video signal. When \n    disabled, the video signal is unfiltered, \n    which sometimes produces visual artefacts such \n    as color bleeding. A notch filter centered at \n    the color subcarrier frequency is used to \n    extract color from 2.5MHz to 4.5MHz.This method \n    has two drawbacks. First, it limits the color \n    bandwidth (which extends to 2.1MHz) cutting out\n    fine color detail. Second, luminance info in \n    the notch frequency range contaminates color \n    and generates unwanted color or rainbows.\n                                        #C(cont...)#C",
"                  OPTIONS (2/5)\n\n#GGX Deflickering Filter:#G A filter that removes\n    ficker in interlaced modes by softening the\n    rendered image. \n\n#GEnable 480p Modes:#G If a component cable is\n    present, this option enables progressive\n    640x480 modes.\n\n#GEnable PAL Modes:#G These are disabled by default\n    since many NTSC TVs don't support them.\n\n\n\n                                        #C(cont...)#C",
"                  OPTIONS (3/5)\n\n#GEnable PAL Background:#G Since most of the menus\n    and screens are designed for 240p, this fills\n    the borders with the selected color, however\n    there are specific PAL patterns that do not \n    need this, such as grids and overscan.\n\n#GChange PAL Background:#G Select the fill color for\n    the PAL background.\n\n#GPAL starting line:#G PAL specifies the start of\n    active video at line 23 1/2, this setting \n    allows starting from line 24, centering the \n    528 active video lines in the 576i signal or \n    moving it to the bottom.            #C(cont...)#C",
"                  OPTIONS (4/5)\n                \n#GStretch to full 288/576:#G Since the $H can't render\n    the full 288p/576i screen in 1:1 pixel \n    mapping, this option enables a video mode for \n    all PAL settings where the 264/528 lines are \n    stretched vertically to fill the 576i PAL \n    signal. #YThis is uneven and should only be \n    used to visually check for overscan.#Y\n                \n#G480p Scanline Intensity:#G When in line doubled\n    480p mode, the suite allows the display of\n    rendered scanlines. This determines the alpha\n    value for them, 0% eliminates them.\n\n                                        #C(cont...)#C",
"                  OPTIONS (5/5)\n\n#G480p Scanlines:#G This setting displays EVEN or\n    ODD scanlines in 480p line doubled mode.\n\n#GSFC Classic Controller:#G The #YSuper Famicom#Y\n    version of the #YClassic Controller#Y needs a \n    different button layout since it doesn't have\n    a HOME button. This changes the layout for it.\n\n\nHolding $L and $R buttons while in the options menu\nwill reset all options to their defaults.",
NULL
};
char *optionsgc_txt[] = { 
"                  OPTIONS (1/4)\n\n#GGX Deflickering Filter:#G A filter that removes\n    ficker in interlaced modes by softening the\n    rendered image. \n\n#GEnable 480p Modes:#G If a component cable is\n    present, this option enables progressive\n    640x480 modes.\n                                      \n#GEnable PAL Modes:#G These are disabled by default\n    since many NTSC TVs don't support them.\n\n\n\n                                        #C(cont...)#C",
"                  OPTIONS (2/4)\n\n#GEnable PAL Background:#G Since most of the menus\n    and screens are designed for 240p, this fills\n    the borders with the selected color, however\n    there are specific PAL patterns that do not \n    need this, such as grids and overscan.\n\n#GChange PAL Background:#G Select the fill color for\n    the PAL background.\n\n\n\n\n\n                                        #C(cont...)#C",
"                  OPTIONS (3/4)\n\n#GPAL starting line:#G PAL specifies the start of\n    video data at line 23 1/2, this setting allows\n    starting from line 24, centering the 528 \n    active video lines in the 576i signal or \n    moving the whole screen to the bottom. The #GGC#G\n    version starts at line 23.\n                \n#GStretch to full 288/576:#G Since the $H can't render\n    the full 288p/576i screen in 1:1 pixel \n    mapping, this option enables a video mode for \n    all PAL settings where the 264/528 lines are \n    stretched vertically to fill the 576i PAL \n    signal. #YThis is uneven and should only be \n    used to visually check for overscan.#Y  #C(cont...)#C               ",
"                  OPTIONS (4/4)\n\n#G480p Scanline Intensity:#G When in line doubled\n    480p mode, the suite allows the display of\n    rendered scanlines. This determines the alpha\n    value for them, 0% eliminates them.\n\n#G480p Scanlines:#G This setting allows the display\n    or EVEN or ODD scanline display in 480p line\n    doubled mode.\n\nHolding $L and $R buttons while in the options menu\nwill reset all options to their defaults.",
NULL
};
char *overscan_txt[] = { 
"                     OVERSCAN\n\nWith this pattern you can interactively find out\nthe overscan in pixels and percentage of each\nedge in a display. \n\nUse the $R and $L buttons to increment the \noverscan until you see the white border, then go \nback one pixel. The resulting number is the\namount of overscan in pixels in each direction.\n\n#YWhen in 576i stretched PAL modes, the pixel\ncount is not accurate#Y. For precise results, \ninstead use the #GPAL start line option#G in #Y23 1/2#Y\nand #YBottom#y modes, each one to count top or bottom\noverscan accurately.\n",
NULL
};
char *passivelag_txt[] = { 
"                  LAG TEST (1/2)\n\nThis test is designed to be used with two displays\nconected at the same time. One being a CRT, or a\ndisplay with a known lag as reference, and the\nother the display to test.\n\nUsing a camera, a picture should be taken of both\nscreens at the same time. The picture will show \nthe frame discrepancy between them. \n\nThe circles in the bottom help determine the frame\neven when the numbers are blurry.\n\nThe #YSega Genesis#Y version of the test can be used\nwith a #YNomad#Y as the reference display.   #C(cont...)#C",
"                  LAG TEST (2/2)\n\nYou can also chain the output ports of a #YPVM/BVM#Y\nto an upscaler and use the CRT as reference, or\nsplit the video signal.",
NULL
};
char *phase_txt[] = { 
"           PHASE & SAMPLE RATE CHECK\n\nThis test allows you to check phase alignment\nin upscalers.\n\nPress left or right to move, #GX#G for checkerboard\nbackground.\n\nDesigned by #CFirebrandX#C.\n",
NULL
};
char *pluge_txt[] = { 
"                   PLUGE (1/3)\n\nThe #Ypicture line-up generation equipment#Y (PLUGE)\nis a test pattern used to adjust the black level\nand contrast of a video signal.\n\nThe pattern in this suite is mainly designed to\nadjust the black level. The control for setting \nthis on your TV is usually named #YBrightness#Y.\n\nThis adjustment ensures that anything intended\nto be black is actually black, and allows all\ndetail to be seen on dark scenes.\n\nThere are three bars at each side, you can \nhighlight them with the $X button.        #C(cont...)#C",
"                   PLUGE (2/3)\n\nThe $A button changes between #GNTSC#G and #GRGB Full \nRange#G [0-255] when in any NTSC mode.        \n                                         \nThe values of the bars depend on your current\nvideo mode. Reduce the #YBrightness#Y until the \ndesired bars are no longer visible, and then \nslowly raise it until they are #Gjust#G visible,\nand go back one notch where they blend with the\nbackground. \n\nFor games always use #GRGB Full Range#G. The 1 IRE \nbars are the ones you should calibrate against.\nThe values of the bars are: 1, 2 and 3.5 IRE.\n                                         #C(cont...)#C",
"                   PLUGE (3/3)\n\nValues for the bars when in NTSC mode are:  \nclosest to the center 3.5 IRE, central ones \n7.5 IRE, and outer ones 11.5 IRE. \n\nWhen calibrating for NTSC material, the 7.5 IRE\nbars should blend in the background. The 7.5 IRE\nin NTSC is the black level, also known as setup.\n",
NULL
};
char *scroll_txt[] = { 
"                    SCROLL TEST\n\nThis test shows a two layer background from #YSonic\nthe Hedgehog#Y or a single one from #YKiki Kaikai#Y.\nThe speed can be varied with the d-pad, and\ndirection changed with the button #GX#G.\n\nVertical and horizntal patterns can be swapped with\nthe #GY#G button, and #GA#G stops the scroll.\n\nThis can be used to notice any drops in framerate\nwhen using a video processor.\n\n#YSonic The Hedgehog#Y is a trademark of #YSega\nEnterprises Ltd#Y. #YKiki Kaikai#Y is a trademark\nof #YNamco/Bandai#Y. \n",
NULL
};
char *sharpness_txt[] = { 
"                    SHARPNESS\n\nYou should set the sharpness of your CRT to a\nvalue that shows clean black and gray transitions,\nwith no white ghosting in between.\n\nOn most modern displays, the #Ysharpness#Y control is \nan edge-enhancement control, and most probably\nshould be set to zero, or in the middle.\n\nIn some #YPVM#Y/#YBVM#Y displays this is also referred \nto as #Yaperture#Y.",
NULL
};
char *SMPTEColor_txt[] = { 
"            EBU/SMPTE COLOR BARS (1/3)\n\nThis pattern can be used to calibrate for NTSC\nlevels regarding contrast and brightness, and \nappropiate colors as well. \n\nYou can toggle between 100% and 75% #YSMPTE#Y color \nbars by pressing $A.\n\nThe #YEBU#Y color bars start at 0mV.\n\nRemember that the black level in the SMPTE pattern\nrefers to 7.5 IRE for video, console games usually\nstart at 0 IRE. This pattern can be used to \ncalibrate colors - or auto calibrated in a \n                                         #C(cont...)#C",
"            EBU/SMPTE COLOR BARS (2/3)\n\nprofessional display - and then black levels can\nbe adjusted with the PLUGE pattern using the #GFull \nRGB Mode#G.\n\nIn an #YSMPT#YE color bar image, the top two-thirds\nof the television picture contain seven vertical\nbars of 75% intensity. In order from left to\nright, the colors are gray, yellow, cyan, green,\nmagenta, red, and blue. This sequence runs \nthrough all seven possible combinations that use\nat least one of the three basic color components\nof green, red, and blue, with blue cycling on\nand off between every bar, red cycling on and \n                                         #C(cont...)#C",
"            EBU/SMPTE COLOR BARS (3/3)\n\noff every two bars, and green on for the leftmost\nfour bars and off for the rightmost three.\n\nBelow the main set of seven bars is a strip of \nblue, magenta, cyan, and white castellations. \nWhen a television receiver is set to filter out \nall colors except for blue, these castellations, \ncombined with the main set of color bars, are \nused to properly adjust the color controls; they\nappear as four solid blue bars, with no visible \ndistinction between the bars and the \ncastellations, if the color controls are properly\nadjusted.",
NULL
};
char *sound_txt[] = { 
"                      SOUND\n\nThis simple test plays a sound in either or both\nchannels.\n\nIt can be used to determine if the audio chain is\nworking properly.\n",
NULL
};
char *striped_txt[] = { 
"               STRIPED SPRITE TEST\n\nThere actually are deinterlacers out there which\ncan display the drop shadows correctly and still\ninterpret 240p as 480i. With a striped sprite it\nshould be easy to tell if a processor tries to\ndeinterlace (plus interpolate) or not.\n\nYou can change backgrounds with $A.\n",
NULL
};
char *stripes_txt[] = { 
"             HORIZONTAL STRIPES (1/2)\n\nThis pattern is designed to show if all lines are\nvisible in your setup, and how your video\nprocessor is handling 240p video. \n\nYou should see a pattern of lines, each one pixel\nin height, starting with a white one at the top of\nthe screen. You can toggle the pattern with \nbutton $X, or turn on auto-toggle each frame with\nthe $A button.\n\nWhen auto-toggle is set, you should see the lines\nalternating rapidly. On some setups, the pattern\ndoesn't change at all. This means that the signal\n                                         #C(cont...)#C",
"             HORIZONTAL STRIPES (2/2)\n\nis being treated as 480i/576i and odd or even\nframes are being discarded completely.\n\nA frame counter can be displayed on screen by \npressing button $Y.\n\nButton $R toggles vertical and horizontal bars,\nwhich will help you evaluate if the signal is not\ndistorted horizontaly, since all lines should be\none pixel wide.\n\n#YThis pattern is completely distorted when using\nthe 576i stretched mode.#Y",
NULL
};
char *timereflex_txt[] = { 
"             TIMING & REFLEX TEST (1/3)\n\nThe main intention is to show a changing pattern\non the screen, which can be complemented with \naudio and vibration on the controller if set. \nThis should show to some degree any lag when  \nprocessing the signal.\n\nAs an added feature, the user can click the $A\nbutton when the sprite is aligned with the one on\nthe background, and the offset in frames from the\nactual intersection will be shown on screen. This\ncan be repeated ten times and the software will\ncalculate the average. Whenever the button was\npressed before the actual intersection frame, the\n                                        #C(cont...)#C",
"             TIMING & REFLEX TEST (2/3)\n\nresult will be ignored (but still shown onscreen).\nButton $X can be used to change the direction\nof the sprite from vertical to horizontal, or\ndisplay both at the same time.\n\n\n#ROf course the evaluation is dependent on reflexes\nand/or rhythm more than anything.#R The visual and\naudio cues are the more revealing aspects which \nthe user should consider, of course the interactive\nfactor can give an experienced player the hang of\nthe system when testing via different connections.\n\n                                        #C(cont...)#C",
"             TIMING & REFLEX TEST (3/3)\n\nSince a frame is around 16 ms (1000/60) in NTSC \nand 20 ms (1000/50) in PAL, that value must be \nconsidered the general error when using the test \nresults. \n\n",
NULL
};
char *vmode_txt[] = { 
"                VIDEO MODES (1/10)\n\nThe $H version of the 240p suite supports the \nfollowing video modes:\n\n#YNTSC compatible modes (60hz):#Y The first 21\nlines of NTSC are reserved and cannot contain \nvideo material in broadcast standards, since \nline 21 usually has close caption (CC) data.\nConsole games typically respect this, since \ntheir target display were consumer TVs. It has\n525 lines in interlaced mode, of which only 480\nare visible, 240 in each field. The following\nmodes start at line 22 of every field.\n\n                                   #C(cont...)#C",
"                VIDEO MODES (2/10)\n\n #G240p:#G The progressive variant of NTSC 525i\n    line video, it has 262 lines of which 240\n    video lines are active. This format is \n    used in most SD game consoles, from the \n    #YAtari VCS#Y to the #YPS2#Y. The $H version\n    of the suite displays a full 320x240p screen.\n\n #G480i scaled 240p assets (NTSC):#G This displays\n    the 240p patterns in 480i mode. It uses the \n    480 visible lines, 240 in each field. The \n    whole content of the suite is linedoubled in \n    this mode.\n\n                                   #C(cont...)#C",
"                VIDEO MODES (3/10)\n\n #G480i mixed 480p/240p assets (1:1/NTSC):#G In this\n    mode the content of the suite is never line \n    doubled.\n\n    As a result, most screens end up filling 1/4th\n    of the display in 1:1 pixel mode. However, \n    there are several patterns in 480 resolution\n    that fill the screen, such as the grid, scroll\n    and overscan.\n\n\n\n\n                                   #C(cont...)#C",
"                VIDEO MODES (4/10)\n\n#YPAL compatible modes (50hz):#Y These modes must\nbe enabled from the #YOptions#Y menu.  The first\n22 lines of PAL are reserved and cannot contain\nvideo material in broadcast standards. Console \ngames typically respect this, since their target \ndisplay were consumer TVs. It has 625 lines in \ninterlaced mode, of which only 576 are visible, \n288 in each field. \n\nThe $H cannot use the whole 288 lines as active\nvideo while doing 1:1 pixel mapping, instead it\ncan draw 264 lines on each field.\n\n                                   #C(cont...)#C",
"                VIDEO MODES (5/10)\n\nIn order to help overcome this limitation, the\n240p suite adds several options for evaluation of\nPAL modes. \n\nThe first and recommended one, is using the #GPAL \nstarting line#G option. It allows the PAL active\nvideo to start at either line 23 1/2, 24, centered\nin the signal, or at the bottom of the signal. \nThis allows using the 264/528 patterns to verify\noverscan, alignment, geometry, etc.\n\nThe #GGC#G version starts at line 23, since the HW\nignores the PAL definition.\n                                   #C(cont...)#C",
"                VIDEO MODES (6/10)\n\nThe other option - #GStretch to full 288/576#G - must\nonly be used to visually check how the whole video\nsignal is filled. It works by using the same \n264/528 patterns stretched by the $H hardware to \nfill the active video, resulting in distorted \npatterns that are not 1:1 pixel mapped. \n\nFinally, the #GEnable PAL Background#G option fills\nthe whole screen with the selected color, enabling\nvisualization of the full 264/528 active video \nsignal when the menu or pattern shown does not \nmatch the current resolution. This can be combined \nwith the stretched video option as well.\n                                   #C(cont...)#C",
"                VIDEO MODES (7/10)\n\nThe following modes start at line 23 1/2 of every\nfield, as the PAL standard determines. (23 in #GGC#G)\n\n #G288p#G: The progressive variant of PAL 625i\n    line video, it has 312 lines of which 288\n    video lines are active. This format is \n    used in most SD game consoles. The $H version\n    of the suite displays a 320x264p screen.\n\n #G576i scaled 264/240p assets (PAL):#G This displays\n    the 264p patterns in 576i mode. It uses the \n    528 visible lines, 264 in each field. The \n    whole content of the suite is linedoubled in \n    this mode.                     #C(cont...)#C",
"                VIDEO MODES (8/10)\n\n #G576i 528/480/240p assets (1:1/PAL):#G This\n    mode has an identical 576i PAL signal, and\n    the content of the suite is never line \n    doubled.\n\n    As a result, most screens end up filling 1/4th\n    of the screen in 1:1 pixel mode. However, \n    there are several patterns in 528 resolution\n    that fill the screen, such as the grid, scroll\n    and overscan.\n\nAll of these PAL modes do have a stretched variant\nwhen that option is enabled.\n                                   #C(cont...)#C",
"                VIDEO MODES (9/10)\n\n#Y480p compatible modes (60hz):#Y These modes must\nbe enabled from the #YOptions#Y menu, and are only\navailable when a #YComponent cable#Y is present.\n\nPlease keep in mind that the 480p signal of the $H\nis not as good as it should be, and complex \npatterns will be too soft or not resolve properly.\n\n #G480p scaled 240p assets & scanlines:#G  This \n    displays the 240p patterns in 480p. The whole\n    content of the suite is linedoubled, and \n    scanline parameters can be modified from the \n    #GOptions#G menu, they can also be completely\n    disabled if needed.            #C(cont...)#C",
"                VIDEO MODES (10/10)\n\n #G480p mixed 480p/240p assets (1:1):#G In this\n    mode the content of the suite is never line \n    doubled.\n\n    As a result, most screens end up filling 1/4th\n    of the display in 1:1 pixel mode. However, \n    there are several patterns in 480 resolution\n    that fill the screen, such as the grid, scroll\n    and overscan.",
NULL
};
char *white_txt[] = { 
"                WHITE SCREEN (1/2)\n\nThis pattern can be changed between white, black,\nred, green, blue and custom color full screens by \nusing the $R and $L buttons.\n\nPressing $A while displaying the black screen and \nin NTSC modes will toggle between 7.5 IRE and the \n0 IRE below black levels. \n\nPressing $A while displaying the white screen will\nenter color edit mode, to select the RGB color to\nfill the screen with. Use the d-pad, $X and $Y\nbuttons to change each color.\n\n                                   #C(cont...)#C",
"                WHITE SCREEN (2/2)\n\nSome displays and scalers have issues when\nchanging between a black 0 IRE and a white screen.\n\nThis pattern can also be used to check color \npurity, and for dead pixels in some displays.\n",
NULL
};

#define	HELPCOUNT	40
char **HelpArray[HELPCOUNT] = {
	alt240p_txt,
	backlit_txt,
	bleed_txt,
	check_txt,
	check480pWii_txt,
	color601_txt,
	colors_txt,
	convergence_txt,
	diagonal_txt,
	disappear_txt,
	dshadow_txt,
	gba_transfer_txt,
	general_txt,
	gray_txt,
	grid_txt,
	grid224_txt,
	gridscroll_txt,
	hcfr_txt,
	hcfr_menu_txt,
	ire100_txt,
	mdfourier_txt,
	mdfourier_wii_txt,
	memory_txt,
	monoscope_txt,
	options_txt,
	optionsgc_txt,
	overscan_txt,
	passivelag_txt,
	phase_txt,
	pluge_txt,
	scroll_txt,
	sharpness_txt,
	SMPTEColor_txt,
	sound_txt,
	striped_txt,
	stripes_txt,
	timereflex_txt,
	vmode_txt,
	white_txt,
NULL
};
/************************** paste ends here ****************************/

#define EXTRA_BUFFER 500

char *PrepareHelpText(char *text)
{
	u32  tsize = 0, pos = 0, ntextpos = 0;
	char *ntext = NULL;
	char **ControllerButtons = NULL;
	
	if(!text)
		return NULL;
	tsize = strlen(text);
	ntext = (char*)malloc(sizeof(char)*tsize+EXTRA_BUFFER);
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
					if(ntextpos+1 < tsize+EXTRA_BUFFER)
						ntext[ntextpos++] = buttonName[bNamePos];
				}
				
				ntext[ntextpos++] = '#';
				ntext[ntextpos++] = 'G';
			}
		}
	}
	if(ntextpos+1 < tsize+EXTRA_BUFFER)
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



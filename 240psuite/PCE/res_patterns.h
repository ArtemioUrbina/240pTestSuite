/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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

#ifdef SYSCARD1
/*
            GRAPHICS
*/

#incchr(my_font, "graphics/font.pcx", 32, 3);

#incbin(fs_map, "graphics/fullscreen.fmp");
#incchr_ex(check_bg, "graphics/check.pcx", 0, 0, 1, 1, 0);
#incpal(check_pal, "graphics/check.pcx", 0, 1);

#incchr_ex(hstripes_bg, "graphics/check.pcx", 8, 0, 1, 1, 0);
#incchr_ex(vstripes_bg, "graphics/check.pcx", 0, 8, 1, 1, 0);
#incchr_ex(white_bg, "graphics/check.pcx", 8, 8, 1, 1, 0);

#incchr_ex(cgrid_bg, "graphics/circles_grid.pcx", 0, 0, 1, 1, 0);

#include "graphics.h"

#endif

#incchr_ex(convlinestile_bg, "graphics/convlinestile.pcx", 0, 0, 1, 1, 0);
#incchr_ex(convcrosstile_bg, "graphics/convcrosstile.pcx", 0, 0, 1, 1, 0);
#incchr_ex(convdottile_bg, "graphics/convdottile.pcx", 0, 0, 1, 1, 0);

#incchr_ex(bricks_bg, "graphics/bricks.pcx", 0, 0, 1, 1, 0);

/*
			PATTERNS
*/

#ifndef SYSCARD1

#incbin(pluge_map, "graphics/pluge.fmp");
#incchr_ex(pluge_bg, "graphics/pluge.pcx", 0, 0, 2, 4, 0);

#incchr_ex(grid_bg, "graphics/grid.pcx", 0, 0, 2, 4, 0);

#incbin(grid320_240_map, "graphics/grid320_240.fmp");
#incbin(grid320_224_map, "graphics/grid320_224.fmp");
#incbin(grid352_240_map, "graphics/grid352_240.fmp");
#incbin(grid352_224_map, "graphics/grid352_224.fmp");

#incbin(grid256_224_map, "graphics/grid256_224.fmp");
#incbin(grid256_240_map	, "graphics/grid256_240.fmp");

#incbin(grid512_224_map, "graphics/grid512_224.fmp");
#incbin(grid512_240_map, "graphics/grid512_240.fmp");

#incbin(color_map, "graphics/color_BAT.bin");
#incbin(color_bg, "graphics/color_DATA.bin");

#incbin(gray_map, "graphics/gray.fmp");
#incchr_ex(gray_bg, "graphics/gray.pcx", 0, 0, 4, 2, 0);

#incbin(cb601_map, "graphics/cb601.fmp");
#incchr_ex(cb601_bg, "graphics/cb601.pcx", 0, 0, 2, 4, 0);

#incbin(colorbleed_map, "graphics/colorbleed.fmp");
#incchr_ex(colorbleed_bg, "graphics/colorbleed.pcx", 0, 0, 2, 5, 0);
#incchr_ex(colorbleedchk_bg, "graphics/colorbleedcheck.pcx", 0, 0, 2, 5, 0);

#incbin(SMPTE75_map, "graphics/SMPTE75.fmp");
#incchr_ex(SMPTE75_bg, "graphics/SMPTE75.pcx", 0, 0, 2, 27, 0);

#incbin(EBU_map, "graphics/EBUCB.fmp");
#incchr_ex(EBU_bg, "graphics/EBUCB.pcx", 0, 0, 4, 2, 0);

#incbin(monoscope_map, "graphics/monoscope.fmp");
#incchr_ex(monoscope_bg, "graphics/monoscope.pcx", 0, 0, 2, 61, 0);

#incbin(sharpness_map, "graphics/sharpness.fmp");
#incchr_ex(sharpness_bg, "graphics/sharpness.pcx", 0, 0, 4, 28, 0);

#incbin(ire100_map, "graphics/100IRE.fmp");
#incchr_ex(ire100_bg, "graphics/100IRE.pcx", 0, 0, 2, 2, 0);

#incbin(convcolor_map, "graphics/convcolor.fmp");
#incchr_ex(convcolor_bg, "graphics/convcolor.pcx", 0, 0, 4, 4, 0);

/*
		Palettes
*/
#incpal(grid_pal, "graphics/grid.pcx", 0, 1);
#incbin(color_pal, "graphics/color_PAL.bin");
#incpal(gray_pal, "graphics/gray.pcx", 0, 1);
#incpal(cb601_pal, "graphics/cb601.pcx", 0, 1);
#incpal(colorbleed_pal, "graphics/colorbleed.pcx", 0, 1);
#incpal(SMPTE75_pal, "graphics/SMPTE75.pcx", 0, 1);
#incpal(SMPTE100_pal, "graphics/SMPTE100.pcx", 0, 1);
#incpal(monoscope_pal, "graphics/monoscope.pcx", 0, 1);
#incpal(pluge_pal, "graphics/pluge.pcx", 0, 1);
#incpal(sharpness_pal, "graphics/sharpness.pcx", 0, 1);
#incpal(convcolor_pal, "graphics/convcolor.pcx", 0, 1);

#endif

#incpal(convlinestile_pal, "graphics/convlinestile.pcx", 0, 1);
#incpal(bricks_pal, "graphics/bricks.pcx", 0, 1);


#ifdef SYSCARD1
#incpal(MB_pal, "graphics/mainbg.pcx", 0, 1);
#incpal(SD_pal, "graphics/SD.pcx", 0, 1);
#endif




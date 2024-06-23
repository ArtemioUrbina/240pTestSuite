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
 
/*
            GRAPHICS
*/

#incchr(my_font, "graphics/font.pcx", 32, 3);

/* TEST*/
#incbin(fs_map, "graphics/fullscreen.fmp");
#incchr_ex(check_bg, "graphics/check.pcx", 0, 0, 1, 1, 0);
#incpal(check_pal, "graphics/check.pcx", 0, 1);

#incchr_ex(hstripes_bg, "graphics/check.pcx", 8, 0, 1, 1, 0);
#incchr_ex(vstripes_bg, "graphics/check.pcx", 0, 8, 1, 1, 0);
#incchr_ex(white_bg, "graphics/check.pcx", 8, 8, 1, 1, 0);

#incchr_ex(cgrid_bg, "graphics/circles_grid.pcx", 0, 0, 1, 1, 0);

#ifndef SYSCARD1
#incbin(float_map, "graphics/float.fmp");
#incchr_ex(float_bg, "graphics/float.pcx", 0, 0, 5, 3, 0);
#incpal(float_pal, "graphics/float.pcx", 0, 1);
#endif

#ifdef SYSCARD1
#include "graphics.h"
#elseif

#incbin(MB_map, "graphics/mainbg.fmp");
#incchr_ex(MB_bg, "graphics/mainbg.pcx", 0, 0, 4, 28, 0);
#incpal(MB_pal, "graphics/mainbg.pcx", 0, 1);

#incbin(MB512_map, "graphics/back512.fmp");

#incbin(n_map, "graphics/N_BAT.bin");
#incbin(n_pal, "graphics/N_PAL.bin");
#incbin(n_bg, "graphics/N_DATA.bin");


/*
			TEST 
*/

#incbin(donna_map, "graphics/donna_BAT.bin");
#incbin(donna_bg, "graphics/donna_DATA.bin");
#incbin(donna_pal, "graphics/donna_PAL.bin");

#incbin(sonic_map, "graphics/sonic_BAT.bin");
#incbin(sonic_bg, "graphics/sonic_DATA.bin");
#incbin(sonic_pal, "graphics/sonic_PAL.bin");

#incbin(kiki_map, "graphics/kiki_BAT.bin");
#incbin(kiki_bg, "graphics/kiki_DATA.bin");
#incbin(kiki_pal, "graphics/kiki_PAL.bin");

#incbin(lagback_map, "graphics/lagback.fmp");
#incchr_ex(lagback_bg, "graphics/lagback.pcx", 0, 0, 2, 39, 0);
#incpal(lagback_pal, "graphics/lagback.pcx", 0, 1);

#incbin(audiosync_map, "graphics/audiosync.fmp");
#incchr_ex(audiosync_bg, "graphics/audiosync.pcx", 0, 0, 2, 4, 0);

#incbin(phase_map, "graphics/phase.fmp");
#incchr_ex(phase_bg, "graphics/phase.pcx", 0, 0, 2, 19, 0);
#incpal(phase_pal, "graphics/phase.pcx", 0, 1);

/*
		SPRITES
*/


#incspr(SD_sp, "graphics/SD.pcx", 0, 0, 4, 7);
#incspr(SDb1_sp, "graphics/SD_b1.pcx", 0, 0, 2, 1);
#incspr(SDb2_sp, "graphics/SD_b2.pcx", 0, 0, 2, 1);

#incspr(palm_sp, "graphics/sonicpalm.pcx", 0, 0, 5, 7);

#incspr(bee_sp, "graphics/bee.pcx", 0, 0, 2, 4);

#incspr(shadow_sp, "graphics/shadow.pcx", 0, 0, 2, 2);
#incspr(striped_sp, "graphics/striped.pcx", 0, 0, 2, 2);
#incspr(lagspr_sp, "graphics/lagspr.pcx", 0, 0, 2, 2);

#incspr(LED_sp, "graphics/LEDsprites.pcx", 0, 0, 2, 2);

#incspr(numbers_sp, "graphics/numbers.pcx", 0, 0, 2, 22);

#endif // SYSCARD1

/*
		Palettes
*/

#ifndef SYSCARD1
#include "res_patterns.h"
#endif

#incpal(SD_pal, "graphics/SD.pcx", 0, 1);
#incpal(palm_pal, "graphics/sonicpalm.pcx", 0, 1);
#incpal(bee_pal, "graphics/bee.pcx", 0, 1);

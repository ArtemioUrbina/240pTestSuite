#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"
#include "image_big_digits.h"

void draw_help(_svin_screen_mode_t screenmode, int page)
{
	//removing text
	ClearTextLayer();

	int x=15;
	int y=35;
	char buf[256];
	switch (page)
	{
		case 0:
			DrawString("HELP (1/4)", 120, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("The 240p Test Suite was designed with two goals", x, y, FONT_WHITE); y+=_fh;
			DrawString("in mind:", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("1)Evaluate 240p signal processing on modern TV", x, y, FONT_WHITE); y+=_fh;
			DrawString("sets and video processing equipment; and", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("2)Provide calibration patterns generated on your", x, y, FONT_WHITE); y+=_fh;
			DrawString("game console to help in properly calibrating the", x, y, FONT_WHITE); y+=_fh;
			DrawString("display's black, white and color levels.", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("Use #GRight#W and #GLeft#W buttons in the D-pad to", x, y, FONT_WHITE); y+=_fh;
			DrawString("navigate between the help pages", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("(cont..)", 240, y, FONT_CYAN); y+=_fh;
			y+=_fh; y+=5;
			DrawString("Press B to close", 110, y, FONT_GREEN); y+=_fh;
			break;
		case 1:
			DrawString("HELP (2/4)", 120, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("This version of the suite supports 240i and 480i", x, y, FONT_WHITE); y+=_fh;
			DrawString("video modes, as well as 480i with the same", x, y, FONT_WHITE); y+=_fh;
			DrawString("content in both fields.", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("Most patterns support modes and resolution", x, y, FONT_WHITE); y+=_fh;
			DrawString("switching with #GL#W and #GR#W buttons. x256", x, y, FONT_WHITE); y+=_fh;
			DrawString("and x512 resolutions are only available on PAL", x, y, FONT_WHITE); y+=_fh;
			DrawString("consoles.", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			DrawString("(cont..)", 240, y, FONT_CYAN); y+=_fh;
			y+=_fh; y+=5;
			DrawString("Press B to close", 110, y, FONT_GREEN); y+=_fh;
			break;
		case 2:
			DrawString("HELP (3/4)", 120, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("Only two digital controllers are supported in", x, y, FONT_WHITE); y+=_fh;
			DrawString("this version. There is no support for 3D pad,", x, y, FONT_WHITE); y+=_fh;
			DrawString("mouse and keyboards yet.", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			DrawString("(cont..)", 240, y, FONT_CYAN); y+=_fh;
			y+=_fh; y+=5;
			DrawString("Press B to close", 110, y, FONT_GREEN); y+=_fh;
			break;
		case 3:
			DrawString("HELP (4/4)", 120, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("The 240p Suite is also available in other", x, y, FONT_WHITE); y+=_fh;
			DrawString("systems, including: #YSega Genesis#W/#YSega Megadrive#W,", x, y, FONT_WHITE); y+=_fh;
			DrawString("#YSega CD#W/#YMega CD#W, #YPC Engine#W/#YTurbo Grafx-16#W,", x, y, FONT_WHITE); y+=_fh;
			DrawString("#YPC Engine Duo#W, #YSuper Nintendo#W, #YSuper Nintendo#W,", x, y, FONT_WHITE); y+=_fh;
			DrawString("#YNES#W/#YFamicom#W, #YNintendo Gamecube#W,", x, y, FONT_WHITE); y+=_fh;
			DrawString("#YNintendo Wii#W, #YNeo Geo#W, #Y32x#W and #YSega Dreamcast#W.", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			DrawString("Info on using this suite:", x, y, FONT_WHITE); y+=_fh;
			DrawString(" http://junkerhq.net/240p/", x, y, FONT_WHITE); y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh;
			y+=_fh; y+=5;
			DrawString("Press B to close", 110, y, FONT_GREEN); y+=_fh;
			break;
		default:
			break;
	}
}

void help(_svin_screen_mode_t screenmode)
{
	int page = 0;
	_svin_screen_mode_t curr_screenmode =
	{
		.scanmode = _SVIN_SCANMODE_240P,
		.x_res = _SVIN_X_RESOLUTION_352,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};

	draw_help(curr_screenmode,page);
	update_screen_mode(curr_screenmode);
	draw_bg_with_expansion(curr_screenmode,false);
	wait_for_key_unpress();

	while (1)
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
		if ( (controller.pressed.button.right) )
		{
			page++;
			if (page > 3) page = 3;
			draw_help(curr_screenmode,page);
			wait_for_key_unpress();
		}
		if ( (controller.pressed.button.left) )
		{
			page--;
			if (page < 0) page = 0;
			draw_help(curr_screenmode,page);
			wait_for_key_unpress();
		}
		else if ( (controller.pressed.button.b) )
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode);
			return;
		}
		vdp2_tvmd_vblank_out_wait();
		vdp2_tvmd_vblank_in_wait();
	}
}

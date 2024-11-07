#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"
#include "image_big_digits.h"

uint32_t
rc_crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;

	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}

	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}

void draw_sysinfo(video_screen_mode_t screenmode)
{
	char buf[256];
	//removing text
	ClearTextLayer();
	
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,0,0,0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.r = IRE_bot + (IRE_top-IRE_bot)/5;
	Color.g = IRE_bot + (IRE_top-IRE_bot)/5;
	Color.b = IRE_top;
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE lightblue

	video_vdp2_set_cycle_patterns_cpu();
	//create single-color tile for the color
	int *_pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
	for (int j=0; j<16; j++)
		_pointer32[16+j] = 0x02020202;
	//fill everything with our tile
    _pointer32 = (int *)VIDEO_VDP2_NBG0_PNDR_START;
    for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200002; //palette 2, transparency on, black
    }
	video_vdp2_set_cycle_patterns_nbg(screenmode);

	DrawString("System information", 160 - _fw*strlen("System information")/2, 20, FONT_WHITE);

	//bios CRC32 calculation
	int crc32 = rc_crc32(0,(uint8_t *)(0x00000000),512*1024);
	sprintf(buf,"BIOS CRC32 : 0x%X",crc32);
	switch (crc32)
	{
		case 0x75660AAC:
			strcat(buf," (Yabause HLE?)");
			break;
		case 0x2ABA43C2:
			strcat(buf," (v1.00 1994 J)");
			break;
		case 0xF90F0089:
			strcat(buf," (v1.00a 1994 U)");
			break;
		case 0x721E1B60:
			strcat(buf," (Hi-Saturn v1.01 1995 J)");
			break;
		case 0x224B752C:
			strcat(buf," (v1.01 1995 J)");
			break;
		case 0x4AFCF0FA:
			strcat(buf," (v1.01a 1995 E)");
			break;
		case 0xDD008C8B:
			strcat(buf," (v1.02 1995 J)");
			break;
		case 0x95B0211D:
			strcat(buf," (v1.00 1994 JPM6)");
			break;
		case 0xE4D61811:
			strcat(buf," (V-Saturn v1.01 1995 J)");
			break;
		default:
			strcat(buf," (unknown)");
			break;
	}
	DrawString(buf, 20, 40, FONT_WHITE);

	//area code	
	smpc_peripheral_intback_issue();
	uint8_t areacode = MEMORY_READ(8, OREG(9));
	sprintf(buf,"SMPC Area Code : 0x%X",areacode);
	switch (areacode)
	{
		case 0:
			strcat(buf," (PROHIBITED)");
			break;
		case 1:
			strcat(buf," (Japan)");
			break;
		case 2:
			strcat(buf," (Asia NTSC)");
			break;
		case 4:
			strcat(buf," (North America)");
			break;
		case 5:
			strcat(buf," (Central/South America NTSC)");
			break;
		case 6:
			strcat(buf," (Korea)");
			break;
		case 10:
			strcat(buf," (Asia PAL)");
			break;
		case 12:
			strcat(buf," (Europe PAL)");
			break;
		case 15:
			strcat(buf," (PROHIBITED)");
			break;
		default:
			strcat(buf," (SEGA RESERVED)");
			break;
	}
	DrawString(buf, 20, 60, FONT_WHITE);

	//VDP2 colormode
	sprintf(buf,"VDP2 color system : ");
	switch (vdp2_tvmd_tv_standard_get())
	{
		case VDP2_TVMD_TV_STANDARD_NTSC:
			strcat(buf," NTSC");
			break;
		case VDP2_TVMD_TV_STANDARD_PAL:
			strcat(buf," PAL");
			break;
		default:
			strcat(buf," SECAM??? NO WAY!");
			break;
	}
	DrawString(buf, 20, 80, FONT_WHITE);

}

void hwtest_sysinfo(video_screen_mode_t screenmode)
{
	bool key_pressed = false;
	int results[10];
	int results_number = 0;

	//using 240p screenmode, no switching supported
	video_screen_mode_t curr_screenmode =
	{
		.scanmode = VIDEO_SCANMODE_240P,
		.x_res = VIDEO_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};
	update_screen_mode(curr_screenmode,false);
	
	int offset = 0;
	int offset_dir = 1;
	int mode = 1;
	char buf[64];
	draw_sysinfo(curr_screenmode);
	wait_for_key_unpress();
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.b) )
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode,false);
			return;
		}
		else
			key_pressed = false;
		vdp2_tvmd_vblank_out_wait();
		vdp2_tvmd_vblank_in_wait();
	}
}

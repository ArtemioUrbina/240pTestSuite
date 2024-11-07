#include <yaul.h>
#include "image_buzzbomber.h"
#include "image_not_really.h"

void _yaul_early_access()
{
    //nothing is initialized yet. so we can't use most of the yaul functionality
    //checking if there actually is logo on-screen (in some cases there might be not, like ODE or HLE BIOS in emulators)
    uint16_t * p16 = (uint16_t *)(VDP2_VRAM_ADDR(0,0));
    if ( (p16[0x8D0/2]!=0xDDDD) ||  (p16[0x8D2/2]!=0xDDD0) || (p16[0x8D4/2]!=0x0D00) || (p16[0x8D6/2]!=0x0D00) )
    {
        //no logo, not drawing anything
        return;
    }
    //wait for vblank in
    vdp2_tvmd_vblank_in_wait();
    //enabling drawing in VDP1
    volatile vdp1_ioregs_t * const vdp1_ioregs = (volatile vdp1_ioregs_t *)VDP1_IOREG_BASE;
    vdp1_ioregs->tvmr = 0x0000;
    vdp1_ioregs->fbcr = 0x0000;
    vdp1_ioregs->ptmr = 0x0002;
    vdp1_ioregs->ewdr = 0x0000;
    vdp1_ioregs->ewlr = 0x0000;
    vdp1_ioregs->ewrr = 0x50F0;
    //creating VDP1 list
    vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
    memset(cmdlist,0,32*5);
    vdp1_cmdt_system_clip_coord_set(&cmdlist[0]);
    vdp1_cmdt_t *cmdt_system_clip_coords = &cmdlist[0];
    cmdt_system_clip_coords->cmd_xc = 319;
    cmdt_system_clip_coords->cmd_yc = 239;
    int16_vec2_t local_coord_ul = INT16_VEC2_INITIALIZER(0,0);
    vdp1_cmdt_local_coord_set(&cmdlist[1]);
    vdp1_cmdt_vtx_local_coord_set(&cmdlist[1], local_coord_ul);
    //quad for the bee
    vdp1_cmdt_draw_mode_t sprite_draw_mode = {.raw = 0x0000, .pre_clipping_disable = true};
    vdp1_cmdt_color_bank_t font_color_bank = {.raw = 0};
    vdp1_cmdt_normal_sprite_set(&cmdlist[2]);
    vdp1_cmdt_draw_mode_set(&cmdlist[2], sprite_draw_mode);
    vdp1_cmdt_color_mode4_set(&cmdlist[2],font_color_bank);//8bpp
    cmdlist[2].cmd_xa = -40;
    if (VDP2_TVMD_TV_STANDARD_NTSC == vdp2_tvmd_tv_standard_get())
        cmdlist[2].cmd_ya = 119;
    else
        cmdlist[2].cmd_ya = 135;
    cmdlist[2].cmd_draw_mode.end_code_disable = 1;
    vdp1_cmdt_char_base_set(&cmdlist[2],VDP1_VRAM(0x4000));
    cmdlist[2].cmd_size=((32/8)<<8)|(32);
    //quad for the text
    vdp1_cmdt_normal_sprite_set(&cmdlist[3]);
    vdp1_cmdt_draw_mode_set(&cmdlist[3], sprite_draw_mode);
    vdp1_cmdt_color_mode4_set(&cmdlist[3],font_color_bank);//8bpp
    cmdlist[3].cmd_xa = -100;
    if (VDP2_TVMD_TV_STANDARD_NTSC == vdp2_tvmd_tv_standard_get())
        cmdlist[3].cmd_ya = 119+8;
    else
        cmdlist[3].cmd_ya = 135+8;
    cmdlist[3].cmd_draw_mode.end_code_disable = 1;
    vdp1_cmdt_char_base_set(&cmdlist[3],VDP1_VRAM(0x8000));
    cmdlist[3].cmd_size=((80/8)<<8)|(16);

    //end of commands list
    vdp1_cmdt_end_set(&cmdlist[4]);
    
    //changing sprite priority in VDP2 to 5
    p16 = (uint16_t *)(VDP2_IOREG_BASE+PRISA);
    p16[0] = 0x0505;
    p16[1] = 0x0505;
    p16[2] = 0x0505;
    p16[3] = 0x0505;
    //changing NBG0 priority in VDP2 to 3
    p16 = (uint16_t *)(VDP2_IOREG_BASE+PRINA);
    p16[0] = 0x0303;

    //copy sprite with mirror
    uint8_t * p8 = (uint8_t *)VDP1_VRAM(0x4000);
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            p8[y*32+x] = image_buzzbomber_data[y*32+31-x]+128;
        }
    }

    p8 = (uint8_t *)VDP1_VRAM(0x8000);
    for (int y = 0; y < 16; y++)
        for (int x = 0; x < 80; x++)
        {
            p8[y*80+x] = image_not_really_data[y*80+x]+192;
        }

    //copy palette
    uint16_t *cram16 = (uint16_t *)VDP2_CRAM_ADDR(0x0);
    for (int i = 0; i < 128; i++)
    {
        uint16_t color = 0x8000 | (image_buzzbomber_cmap[i][0] & 0xF1)<<7
                                | (image_buzzbomber_cmap[i][1] & 0xF1)<<2
                                | (image_buzzbomber_cmap[i][2] & 0xF1)>>3;
        cram16[128+i] = color;
    }
    for (int i = 0; i < 2; i++)
    {
        uint16_t color = 0x8000 | (image_not_really_cmap[i][0] & 0xF1)<<7
                                | (image_not_really_cmap[i][1] & 0xF1)<<2
                                | (image_not_really_cmap[i][2] & 0xF1)>>3;
        cram16[192+i] = color;
    }

    for (int x=0;x<400;x+=2)
    {
        //for (int i=0;i<2;i++)
        {
		    vdp2_tvmd_vblank_out_wait();
		    vdp2_tvmd_vblank_in_wait();
        }
        cmdlist[2].cmd_xa = -32+x;
        if (x<242)
            cmdlist[3].cmd_xa = -117+x;
    }

    //wait for 120 fields
    for (int i=0;i<120;i++)
    {
	    vdp2_tvmd_vblank_out_wait();
	    vdp2_tvmd_vblank_in_wait();
    }
}
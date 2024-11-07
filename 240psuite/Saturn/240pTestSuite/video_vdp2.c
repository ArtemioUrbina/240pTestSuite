#include <yaul.h>
#include "video_vdp2.h"

int current_color_mode = 0;

void video_vdp2_set_cycle_patterns_cpu()
{
    // switching everything to CPU access.
    // no data could be displayed during this, so be sure to blank/disable the screen beforehand

    struct vdp2_vram_cycp vram_cycp;

    vram_cycp.pt[0].t0 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[0].t1 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[0].t2 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[0].t3 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[0].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[1].t0 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[1].t1 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[1].t2 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[1].t3 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[1].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[2].t0 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[2].t1 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[2].t2 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[2].t3 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[2].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[3].t0 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[3].t1 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[3].t2 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[3].t3 = VDP2_VRAM_CYCP_CPU_RW;
    vram_cycp.pt[3].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[3].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[3].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[3].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vdp2_vram_cycp_set(&vram_cycp);
    vdp2_sync();
}

void video_vdp2_set_cycle_patterns_nbg(video_screen_mode_t screen_mode)
{
    //swithcing everything to NBG accesses, CPU can't write data anymore

    if ( (VIDEO_SCANMODE_480P == screen_mode.scanmode) && (screen_mode.x_res_doubled) )
    {
        //      T0 T1 T2 T3
        // A0 : D0 D0 D0 D0
        // A1 : i0 i0  x  x
        // B0 : i1 i1  x  x
        // B1 : D1 D1 D1 D1

        struct vdp2_vram_cycp vram_cycp;

        vram_cycp.pt[0].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[0].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[0].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[0].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[0].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[0].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[0].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[0].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vram_cycp.pt[1].t0 = VDP2_VRAM_CYCP_PNDR_NBG0;
        vram_cycp.pt[1].t1 = VDP2_VRAM_CYCP_PNDR_NBG0;
        vram_cycp.pt[1].t2 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t3 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vram_cycp.pt[2].t0 = VDP2_VRAM_CYCP_PNDR_NBG1;
        vram_cycp.pt[2].t1 = VDP2_VRAM_CYCP_PNDR_NBG1;
        vram_cycp.pt[2].t2 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t3 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vram_cycp.pt[3].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[3].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[3].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[3].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[3].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[3].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[3].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[3].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vdp2_vram_cycp_set(&vram_cycp);
        vdp2_sync();
    }
    else
    {
        //normal setup
        //      T0 T1 T2 T3
        // A0 : D0 D0 D1 D1
        // A1 : D0 D0 D1 D1
        // B0 : D0 D0 D1 D1
        // B1 : i0 i1 D1 D1

        struct vdp2_vram_cycp vram_cycp;

        vram_cycp.pt[0].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[0].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[0].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[0].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[0].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[0].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[0].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[0].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vram_cycp.pt[1].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[1].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[1].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[1].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[1].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[1].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vram_cycp.pt[2].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[2].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
        vram_cycp.pt[2].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[2].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[2].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[2].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vram_cycp.pt[3].t0 = VDP2_VRAM_CYCP_PNDR_NBG0;
        vram_cycp.pt[3].t1 = VDP2_VRAM_CYCP_PNDR_NBG1;
        vram_cycp.pt[3].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[3].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
        vram_cycp.pt[3].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[3].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[3].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
        vram_cycp.pt[3].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

        vdp2_vram_cycp_set(&vram_cycp);
        vdp2_sync();
    }
}

void video_vdp2_init(video_screen_mode_t screen_mode, bool bmp_mode)
{
    int *_pointer32;
    //-------------- setup VDP2 -------------------

    //clearing vdp2
    vdp2_tvmd_display_clear();

    //setting cycle patterns for cpu access
    video_vdp2_set_cycle_patterns_cpu();
    
    struct vdp2_scrn_bitmap_format bmp_format;
    struct vdp2_scrn_cell_format cell_format;
    vdp2_scrn_normal_map_t normal_map;

    if (bmp_mode)
    {
        //bmp mode
        if ( (VIDEO_SCANMODE_480P == screen_mode.scanmode) && (screen_mode.x_res_doubled) )
        {
            //480p high res, special bmp setup
            //setup nbg0
            memset(&bmp_format, 0x00, sizeof(bmp_format));
            bmp_format.scroll_screen = VDP2_SCRN_NBG0;
            bmp_format.ccc = VDP2_SCRN_CCC_PALETTE_16;
            bmp_format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
            bmp_format.palette_base = 0x400;
            bmp_format.bitmap_base = VIDEO_VDP2_NBG0_CHPNDR_START;
            vdp2_scrn_bitmap_format_set(&bmp_format);

            //setup nbg1 same as nbg0
            bmp_format.scroll_screen = VDP2_SCRN_NBG1;
            vdp2_scrn_bitmap_format_set(&bmp_format);

            vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0 | VDP2_SCRN_DISP_NBG1);

            vdp2_scrn_reduction_set(VDP2_SCRN_NBG0,VDP2_SCRN_REDUCTION_HALF);
            vdp2_scrn_reduction_set(VDP2_SCRN_NBG1,VDP2_SCRN_REDUCTION_HALF);
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(2.0f));
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(2.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1, FIX16(1.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0 , FIX16(0.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1 , FIX16(1.0f));	
            vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 5);
            vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5);
        }
        else
        {
            //normal bmp setup
            //setup nbg0
            memset(&bmp_format, 0x00, sizeof(bmp_format));
            bmp_format.scroll_screen = VDP2_SCRN_NBG0;
            bmp_format.ccc = VDP2_SCRN_CCC_PALETTE_16;
            bmp_format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
            bmp_format.palette_base = 0x800;
            bmp_format.bitmap_base = VIDEO_VDP2_NBG0_CHPNDR_START;
            vdp2_scrn_bitmap_format_set(&bmp_format);

            //setup nbg1 in tile mode, because it is not used in normal bmp setup
            memset(&cell_format, 0x00, sizeof(cell_format));
            memset(&normal_map, 0x00, sizeof(normal_map));
            cell_format.scroll_screen = VDP2_SCRN_NBG1;
            cell_format.ccc = VDP2_SCRN_CCC_PALETTE_256;
            cell_format.char_size = VDP2_SCRN_CHAR_SIZE_1X1;
            cell_format.pnd_size = 2;
            cell_format.aux_mode = VDP2_SCRN_AUX_MODE_1;
            cell_format.cpd_base = 0;
            cell_format.palette_base = 0;
            cell_format.plane_size = VDP2_SCRN_PLANE_SIZE_2X1;
            normal_map.plane_a = VIDEO_VDP2_NBG1_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0 | VDP2_SCRN_DISPTP_NBG1);

            vdp2_scrn_reduction_set(VDP2_SCRN_NBG0,VDP2_SCRN_REDUCTION_NONE);
            vdp2_scrn_reduction_set(VDP2_SCRN_NBG1,VDP2_SCRN_REDUCTION_NONE);
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(1.0f));
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(1.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1, FIX16(1.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0 , FIX16(0.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1 , FIX16(0.0f));	
            vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 3);
            vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5);

            //-------------- setup pattern names -------------------

            //writing pattern names for nbg1
            //nbg1  is mostly transparent, so fill with that one
            _pointer32 = (int *)VIDEO_VDP2_NBG1_PNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG1_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + VIDEO_VDP2_NBG1_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //-------------- setup character pattern names -------------------

            //clearing character pattern names data for nbg1
            _pointer32 = (int *)VIDEO_VDP2_NBG1_CHPNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG1_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "transparent" character for nbg1
            _pointer32 = (int *)VIDEO_VDP2_NBG1_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg1
            _pointer32 = (int *)(VIDEO_VDP2_NBG1_CHPNDR_SPECIALS_ADDR + VIDEO_VDP2_CHARACTER_BYTES);
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }
        }
    }
    else
    {
        if ( (VIDEO_SCANMODE_480P == screen_mode.scanmode) && (screen_mode.x_res_doubled) )
        {
            //480p high res, special tile setup
            //setup nbg0
            memset(&cell_format, 0x00, sizeof(cell_format));
            memset(&normal_map, 0x00, sizeof(normal_map));
            cell_format.scroll_screen = VDP2_SCRN_NBG0;
            cell_format.ccc = VDP2_SCRN_CCC_PALETTE_256;
            cell_format.char_size = VDP2_SCRN_CHAR_SIZE_1X1;
            cell_format.pnd_size = 2;
            cell_format.aux_mode = VDP2_SCRN_AUX_MODE_1;
            cell_format.cpd_base = 0;
            cell_format.palette_base = 0;
            cell_format.plane_size = VDP2_SCRN_PLANE_SIZE_2X1;
            normal_map.plane_a = VIDEO_VDP2_NBG0_SPECIAL_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            //setup nbg1 same as nbg0
            cell_format.scroll_screen = VDP2_SCRN_NBG1;
            normal_map.plane_a = VIDEO_VDP2_NBG1_SPECIAL_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0 | VDP2_SCRN_DISP_NBG1);
            
            vdp2_scrn_reduction_set(VDP2_SCRN_NBG0,VDP2_SCRN_REDUCTION_HALF);
            vdp2_scrn_reduction_set(VDP2_SCRN_NBG1,VDP2_SCRN_REDUCTION_HALF);
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(2.0f));
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(2.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1, FIX16(1.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0 , FIX16(0.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1 , FIX16(1.0f));	
            vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 5);
            vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5);

            //-------------- setup pattern names -------------------

            //writing pattern names for nbg0
            //starting with plane 0
            _pointer32 = (int *)VIDEO_VDP2_NBG0_SPECIAL_PNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //writing pattern names for nbg1
            //nbg1  is mostly transparent, so fill with that one
            _pointer32 = (int *)VIDEO_VDP2_NBG1_SPECIAL_PNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG1_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //-------------- setup character pattern names -------------------

            //clearing character pattern names data for nbg0
            _pointer32 = (int *)VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //clearing character pattern names data for nbg1
            _pointer32 = (int *)VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "transparent" character for nbg0
            _pointer32 = (int *)VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg0
            _pointer32 = (int *)(VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_SPECIALS_ADDR + VIDEO_VDP2_CHARACTER_BYTES);
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }

            //setting up "transparent" character for nbg1
            _pointer32 = (int *)VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg1
            _pointer32 = (int *)(VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_SPECIALS_ADDR + VIDEO_VDP2_CHARACTER_BYTES);
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }
        }
        else
        {
            //normal tile setup
            //setup nbg0
            memset(&cell_format, 0x00, sizeof(cell_format));
            memset(&normal_map, 0x00, sizeof(normal_map));
            cell_format.scroll_screen = VDP2_SCRN_NBG0;
            cell_format.ccc = VDP2_SCRN_CCC_PALETTE_256;
            cell_format.char_size = VDP2_SCRN_CHAR_SIZE_1X1;
            cell_format.pnd_size = 2;
            cell_format.aux_mode = VDP2_SCRN_AUX_MODE_1;
            cell_format.cpd_base = 0;
            cell_format.palette_base = 0;
            cell_format.plane_size = VDP2_SCRN_PLANE_SIZE_2X1;
            normal_map.plane_a = VIDEO_VDP2_NBG0_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            //setup nbg1
            cell_format.scroll_screen = VDP2_SCRN_NBG1;
            normal_map.plane_a = VIDEO_VDP2_NBG1_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0 | VDP2_SCRN_DISPTP_NBG1);

            vdp2_scrn_reduction_set(VDP2_SCRN_NBG0,VDP2_SCRN_REDUCTION_NONE);
            vdp2_scrn_reduction_set(VDP2_SCRN_NBG1,VDP2_SCRN_REDUCTION_NONE);
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(1.0f));
            vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(1.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0f));
            vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1, FIX16(1.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0 , FIX16(0.0f));
            vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1 , FIX16(0.0f));	
            vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 3);
            vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5); 

            //-------------- setup pattern names -------------------

            //writing pattern names for nbg0
            //starting with plane 0
            _pointer32 = (int *)VIDEO_VDP2_NBG0_PNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + VIDEO_VDP2_NBG0_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //writing pattern names for nbg1
            //nbg1  is mostly transparent, so fill with that one
            _pointer32 = (int *)VIDEO_VDP2_NBG1_PNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG1_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + VIDEO_VDP2_NBG1_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //-------------- setup character pattern names -------------------

            //clearing character pattern names data for nbg0
            _pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //clearing character pattern names data for nbg1
            _pointer32 = (int *)VIDEO_VDP2_NBG1_CHPNDR_START;
            for (unsigned int i = 0; i < VIDEO_VDP2_NBG1_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "transparent" character for nbg0
            _pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg0
            _pointer32 = (int *)(VIDEO_VDP2_NBG0_CHPNDR_SPECIALS_ADDR + VIDEO_VDP2_CHARACTER_BYTES);
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }

            //setting up "transparent" character for nbg1
            _pointer32 = (int *)VIDEO_VDP2_NBG1_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg1
            _pointer32 = (int *)(VIDEO_VDP2_NBG1_CHPNDR_SPECIALS_ADDR + VIDEO_VDP2_CHARACTER_BYTES);
            for (unsigned int i = 0; i < VIDEO_VDP2_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }
        }
    }

    vdp2_tvmd_interlace_t interlace;
    vdp2_tvmd_horz_t horz;

    if (VIDEO_SCANMODE_480P == screen_mode.scanmode)
    {
        //480p 31kHz modes
        if (screen_mode.x_res_doubled)
        {
            vdp2_cram_mode_set(0);//colormap 2 is unsupported in 480p high res
            current_color_mode = 0;
        }
        else
        {
            vdp2_cram_mode_set(2);
            current_color_mode = 2;
        }

        interlace = VDP2_TVMD_INTERLACE_NONE; //interlace is not supported in 480p

        horz = (VIDEO_X_RESOLUTION_320 == screen_mode.x_res) ? 
                                        screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_AE : VDP2_TVMD_HORZ_NORMAL_AE :
                                        screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_BE : VDP2_TVMD_HORZ_NORMAL_BE;
    }
    else
    {
        //normal PAL/NTSC

        vdp2_cram_mode_set(2);
        current_color_mode = 2;

        interlace = (VIDEO_SCANMODE_240P == screen_mode.scanmode) ? VDP2_TVMD_INTERLACE_NONE :
                                                (VIDEO_SCANMODE_240I == screen_mode.scanmode)  ? VDP2_TVMD_INTERLACE_SINGLE : VDP2_TVMD_INTERLACE_DOUBLE;

        horz = (VIDEO_X_RESOLUTION_320 == screen_mode.x_res) ? 
                                        screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_A : VDP2_TVMD_HORZ_NORMAL_A :
                                        screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_B : VDP2_TVMD_HORZ_NORMAL_B;
    }

    vdp2_tvmd_display_res_set(interlace, horz, screen_mode.y_res);

    rgb1555_t bs_color;
    bs_color = RGB1555(1, 0, 0, 0);
    vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), bs_color);

    vdp2_sprite_priority_set(0, 6);
    vdp2_sprite_priority_set(1, 6);
    vdp2_sprite_priority_set(2, 6);
    vdp2_sprite_priority_set(3, 6);
    vdp2_sprite_priority_set(4, 6);
    vdp2_sprite_priority_set(5, 6);
    vdp2_sprite_priority_set(6, 6);
    vdp2_sprite_priority_set(7, 6);
    
    vdp2_tvmd_display_set();
    //cpu_intc_mask_set(0); //?????
    vdp2_sync();

    //setting cycle patterns for nbg access
    video_vdp2_set_cycle_patterns_nbg(screen_mode);
}

void video_vdp2_deinit()
{

}

//---------------------------------------------- Palette stuff ----------------------------------------------------
void video_vdp2_set_palette_part(int number, rgb888_t *pointer, int start, int end)
{
    uint8_t *my_vdp2_cram8;
    if (current_color_mode == 0)
    {
        my_vdp2_cram8 = (uint8_t *)VDP2_CRAM_ADDR(0x100 * number);
        for (int i = start; i <= end; i++)
        {
            my_vdp2_cram8[i*2+0] = (pointer[i-start].cc<<7) | ((pointer[i-start].b & 0xF8)>>1) | ((pointer[i-start].g & 0xC0)>>6);
            my_vdp2_cram8[i*2+1] = ((pointer[i-start].g & 0x38)<<2) | ((pointer[i-start].r & 0xF8)>>3);
        }
    }
    else
    {
        my_vdp2_cram8 = (uint8_t *)VDP2_CRAM_ADDR(0x200 * number);
        for (int i = start; i <= end; i++)
        {
            my_vdp2_cram8[i*4+0] = pointer[i-start].cc;
            my_vdp2_cram8[i*4+1] = pointer[i-start].b;
            my_vdp2_cram8[i*4+2] = pointer[i-start].g;
            my_vdp2_cram8[i*4+3] = pointer[i-start].r;
        }
    }
}

void video_vdp2_clear_palette(int number)
{
    video_vdp2_clear_palette_part(number,0,255);
}

void video_vdp2_clear_palette_part(int number, int start, int end)
{
    uint32_t *my_vdp2_cram32 = (uint32_t *)VDP2_CRAM_ADDR(0x200 * number);
    for (int i = start; i <= end; i++)
    {
        my_vdp2_cram32[i] = 0;
    }

}

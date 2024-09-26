#include <yaul.h>
#include <vdp1/vram.h>
#include <tga.h>
#include <svin.h>
//#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void _svin_vblank_out_handler(void *);

static smpc_peripheral_digital_t _digital;

fad_t _svin_background_pack_fad;
uint8_t *_svin_background_index;
uint16_t _svin_background_files_number;
vdp1_cmdt_list_t *_svin_cmdt_list;
uint8_t _svin_init_done;
int _svin_frame_count;
bool _svin_vdp1_cmdlist_toggle_at_vblank;

int current_color_mode = 0;

void _svin_set_cycle_patterns_cpu()
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

void _svin_set_cycle_patterns_nbg()
{
    //swithcing everything to NBG accesses, CPU can't write data anymore

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

    //enable transparency for NBG1 over NBG0 (might not work with sprites between
    //MEMORY_WRITE(16, VDP2(CCCTL), 0x0000); //disable cc both layers
    //MEMORY_WRITE(16, VDP2(CCRNA), 0x0C00); //enable cc for NBG1
}

static inline void __always_inline
vdp1_cmdt_color_mode5_set(vdp1_cmdt_t *cmdt)
{
        cmdt->cmd_pmod &= 0xFFC7;
        cmdt->cmd_pmod |= 0x0028;
        cmdt->cmd_colr = 0;
}

void _svin_init(_svin_screen_mode_t screen_mode, bool bmp_mode)
{
    int *_pointer32;
    _svin_init_done = 0;
    _svin_frame_count = 0;
    _svin_vdp1_cmdlist_toggle_at_vblank = true;

    //-------------- setup VDP2 -------------------

    //clearing vdp2
    vdp2_tvmd_display_clear();

    //setting cycle patterns for cpu access
    _svin_set_cycle_patterns_cpu();
    
    struct vdp2_scrn_bitmap_format bmp_format;
    struct vdp2_scrn_cell_format cell_format;
    vdp2_scrn_normal_map_t normal_map;

    if (bmp_mode)
    {
        //bmp mode
        if (_SVIN_SCANMODE_480P == screen_mode.scanmode)
        {
            //480p high res, special bmp setup
            //setup nbg0
            memset(&bmp_format, 0x00, sizeof(bmp_format));
            bmp_format.scroll_screen = VDP2_SCRN_NBG0;
            bmp_format.ccc = VDP2_SCRN_CCC_PALETTE_16;
            bmp_format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
            bmp_format.palette_base = 0x400;
            bmp_format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
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
            bmp_format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
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
            normal_map.plane_a = _SVIN_NBG1_PNDR_START;
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
            _pointer32 = (int *)_SVIN_NBG1_PNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG1_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + _SVIN_NBG1_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //-------------- setup character pattern names -------------------

            //clearing character pattern names data for nbg1
            _pointer32 = (int *)_SVIN_NBG1_CHPNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG1_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "transparent" character for nbg1
            _pointer32 = (int *)_SVIN_NBG1_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg1
            _pointer32 = (int *)(_SVIN_NBG1_CHPNDR_SPECIALS_ADDR + _SVIN_CHARACTER_BYTES);
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }
        }
    }
    else
    {
        if (_SVIN_SCANMODE_480P == screen_mode.scanmode)
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
            normal_map.plane_a = _SVIN_NBG0_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            //setup nbg1 same as nbg0
            cell_format.scroll_screen = VDP2_SCRN_NBG1;
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
            _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + _SVIN_NBG0_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //-------------- setup character pattern names -------------------

            //clearing character pattern names data for nbg0
            _pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG0_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "transparent" character for nbg0
            _pointer32 = (int *)_SVIN_NBG0_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg0
            _pointer32 = (int *)(_SVIN_NBG0_CHPNDR_SPECIALS_ADDR + _SVIN_CHARACTER_BYTES);
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
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
            normal_map.plane_a = _SVIN_NBG0_PNDR_START;
            vdp2_scrn_cell_format_set(&cell_format,&normal_map);

            //setup nbg1
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
            normal_map.plane_a = _SVIN_NBG1_PNDR_START;
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
            _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + _SVIN_NBG0_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //writing pattern names for nbg1
            //nbg1  is mostly transparent, so fill with that one
            _pointer32 = (int *)_SVIN_NBG1_PNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG1_PNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0x00000000 + _SVIN_NBG1_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
            }

            //-------------- setup character pattern names -------------------

            //clearing character pattern names data for nbg0
            _pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG0_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //clearing character pattern names data for nbg1
            _pointer32 = (int *)_SVIN_NBG1_CHPNDR_START;
            for (unsigned int i = 0; i < _SVIN_NBG1_CHPNDR_SIZE / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "transparent" character for nbg0
            _pointer32 = (int *)_SVIN_NBG0_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg0
            _pointer32 = (int *)(_SVIN_NBG0_CHPNDR_SPECIALS_ADDR + _SVIN_CHARACTER_BYTES);
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }

            //setting up "transparent" character for nbg1
            _pointer32 = (int *)_SVIN_NBG1_CHPNDR_SPECIALS_ADDR;
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0;
            }

            //setting up "semi-transparent" character for nbg1
            _pointer32 = (int *)(_SVIN_NBG1_CHPNDR_SPECIALS_ADDR + _SVIN_CHARACTER_BYTES);
            for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
            {
                _pointer32[i] = 0x7F7F7F7F;
            }
        }
    }

    vdp2_tvmd_interlace_t interlace;
    vdp2_tvmd_horz_t horz;

    if (_SVIN_SCANMODE_480P == screen_mode.scanmode)
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

        horz = (_SVIN_X_RESOLUTION_320 == screen_mode.x_res) ? 
                                        screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_AE : VDP2_TVMD_HORZ_NORMAL_AE :
                                        screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_BE : VDP2_TVMD_HORZ_NORMAL_BE;
    }
    else
    {
        //normal PAL/NTSC

        vdp2_cram_mode_set(2);
        current_color_mode = 2;

        interlace = (_SVIN_SCANMODE_240P == screen_mode.scanmode) ? VDP2_TVMD_INTERLACE_NONE :
                                                (_SVIN_SCANMODE_240I == screen_mode.scanmode)  ? VDP2_TVMD_INTERLACE_SINGLE : VDP2_TVMD_INTERLACE_DOUBLE;

        horz = (_SVIN_X_RESOLUTION_320 == screen_mode.x_res) ? 
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

    vdp_sync_vblank_out_set(_svin_vblank_out_handler, NULL);
    
    vdp2_tvmd_display_set();
    //cpu_intc_mask_set(0); //?????
    vdp2_sync();

    //setting cycle patterns for nbg access
    _svin_set_cycle_patterns_nbg();

    //-------------- setup VDP1 -------------------
    //setting up a small VDP1 list with 3 commands: sys clip, local coords, end
    //everything else will be appended to this list later
    _svin_cmdt_list = vdp1_cmdt_list_alloc(_SVIN_VDP1_ORDER_LIMIT+1);

    static const int16_vec2_t local_coord_ul =
        INT16_VEC2_INITIALIZER(0,
                               0);

    static const vdp1_cmdt_draw_mode_t sprite_draw_mode = {
        .raw = 0x0000,
        .pre_clipping_disable = true};

    vdp1_cmdt_color_bank_t font_color_bank;
    font_color_bank.raw = 0;

    assert(_svin_cmdt_list != NULL);

    (void)memset(_svin_cmdt_list->cmdts, 0x00, sizeof(vdp1_cmdt_t) * (_SVIN_VDP1_ORDER_LIMIT+1));

    vdp1_vram_partitions_set(64,//VDP1_VRAM_DEFAULT_CMDT_COUNT,
                              0x7F000, //  VDP1_VRAM_DEFAULT_TEXTURE_SIZE,
                               0,//  VDP1_VRAM_DEFAULT_GOURAUD_COUNT,
                               0);//  VDP1_VRAM_DEFAULT_CLUT_COUNT);

    vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);

    _svin_cmdt_list->count = _SVIN_VDP1_ORDER_LIMIT+1;
    
    //setting clipping coordinates
    vdp1_cmdt_system_clip_coord_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX]);
    vdp1_cmdt_t *cmdt_system_clip_coords;
    cmdt_system_clip_coords = &_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX];
    cmdt_system_clip_coords->cmd_xc = (_SVIN_X_RESOLUTION_320 == screen_mode.x_res) ? 319 : 351;
    cmdt_system_clip_coords->cmd_yc = (VDP2_TVMD_VERT_224 == screen_mode.y_res) ? 224 : 
                                                (VDP2_TVMD_VERT_240 == screen_mode.y_res) ? 240 : 256;

    vdp1_cmdt_local_coord_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LOCAL_COORDS_INDEX]);
    vdp1_cmdt_vtx_local_coord_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LOCAL_COORDS_INDEX], local_coord_ul);
    
    //setting up VDP1 text layer quads
    //there are from 1 to 4 quads required depending on X and Y resolution configuration
    //VDP1 quads can't be bigger than 508, thus for high-res X (640 or 704) we use 2 quads along X axis
    //for 480p VDP1 outputs 1 half-resolution progressive frame at 50/60 fps, using 2 quads along Y axis
    //for 480i VDP1 outputs 2 different fields at 25/30 fps each, using 2 quads along Y axis
    //for 240p VDP1 outputs 1 progressive frame at 50/60 fps, using 1 quad along Y axis

    //quad for text, always 320x224 and always centered 
    int index = _SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_svin_cmdt_list->cmdts[index]);
    vdp1_cmdt_draw_mode_set(&_svin_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_color_mode4_set(&_svin_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _svin_cmdt_list->cmdts[index].cmd_xa= (_SVIN_X_RESOLUTION_320 == screen_mode.x_res) ? 0 : 16;
    _svin_cmdt_list->cmdts[index].cmd_ya= (VDP2_TVMD_VERT_224 == screen_mode.y_res) ? 0 : 
                                                (VDP2_TVMD_VERT_240 == screen_mode.y_res) ? 8 : 16;
    vdp1_cmdt_char_base_set(&_svin_cmdt_list->cmdts[index],vdp1_vram_partitions.texture_base);
    _svin_cmdt_list->cmdts[index].cmd_size=((320/8)<<8)|(224);

    //quad for shadowdrop test, 32x32, out-of-screen, storing after text quad at 0x11800
    index = _SVIN_VDP1_ORDER_TEXT_SPRITE_1_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_svin_cmdt_list->cmdts[index]);
    vdp1_cmdt_draw_mode_set(&_svin_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_color_mode4_set(&_svin_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _svin_cmdt_list->cmdts[index].cmd_xa = -40;
    _svin_cmdt_list->cmdts[index].cmd_ya = -40;
    vdp1_cmdt_char_base_set(&_svin_cmdt_list->cmdts[index],vdp1_vram_partitions.texture_base+0x11800);
    _svin_cmdt_list->cmdts[index].cmd_size=((32/8)<<8)|(32);

    //end of commands list
    vdp1_cmdt_end_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LIMIT]);

    vdp1_sync_cmdt_list_put(_svin_cmdt_list, 0);

/*#define VDP1_FBCR_DIE (0x0008)
    if ( (_SVIN_SCANMODE_240P == screen_mode.scanmode) || (_SVIN_SCANMODE_480P == screen_mode.scanmode) )
        MEMORY_WRITE(16, VDP1(FBCR), 0);
    else
        MEMORY_WRITE(16, VDP1(FBCR), VDP1_FBCR_DIE);*/

/*#define VDP1_TVMR_VBE (0x0008)
    if (_SVIN_SCANMODE_480P == screen_mode.scanmode)
        _vdp1_env.hdtv = VDP1_ENV_HDTV_ON;
        MEMORY_WRITE(16, VDP1(TVMR), 0x4);
    else
        MEMORY_WRITE(16, VDP1(TVMR), VDP1_TVMR_VBE | 0x0);*/

    static vdp1_env_t vdp1_env = {
                .erase_color = RGB1555(0, 0, 0, 0),
                .erase_points[0] = {
                        .x = 0,
                        .y = 0
                },
                .erase_points[1] = {
                        .x = 703,
                        .y = 511
                },
                .bpp = VDP1_ENV_BPP_16,
                .rotation = VDP1_ENV_ROTATION_0,
                .hdtv = VDP1_ENV_HDTV_OFF,
                .color_mode = VDP1_ENV_COLOR_MODE_RGB_PALETTE,
                .sprite_type = 0x0
    };

    if (_SVIN_SCANMODE_480P == screen_mode.scanmode)
        vdp1_env.hdtv = VDP1_ENV_HDTV_ON;
    vdp1_env.erase_points[1].x = screen_mode.x_res_doubled ? 703 : 351;
    vdp1_env.erase_points[1].y = (_SVIN_SCANMODE_480I == screen_mode.scanmode) ? 511 : 255;

    vdp1_env_set(&vdp1_env);

    vdp1_sync();
    vdp1_sync_wait();

    smpc_peripheral_init();

    //-------------- init end -------------------  
    //vdp1_cmdt_jump_assign(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX], _SVIN_VDP1_ORDER_LOCAL_COORDS_B_INDEX * 4);
    _svin_init_done = 1;
}

void _svin_deinit()
{

    free(_svin_cmdt_list);
    //_svin_sprite_deinit();
    _svin_init_done = 0;
}

//---------------------------------------------- Palette stuff ----------------------------------------------------
void _svin_set_palette_part(int number, rgb888_t *pointer, int start, int end)
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

void _svin_clear_palette(int number)
{
    _svin_clear_palette_part(number,0,255);
}

void _svin_clear_palette_part(int number, int start, int end)
{
    uint32_t *my_vdp2_cram32 = (uint32_t *)VDP2_CRAM_ADDR(0x200 * number);
    for (int i = start; i <= end; i++)
    {
        my_vdp2_cram32[i] = 0;
    }

}

//---------------------------------------------- Misc ----------------------------------------------------

void _svin_set_vdp1_cmdlist_toggle_at_vblank(bool enable)
{
    _svin_vdp1_cmdlist_toggle_at_vblank = enable;
}

void _svin_vblank_out_handler(void *work __unused)
{
    _svin_frame_count++;
    
    if (0==_svin_init_done)
        return;

    //smpc_peripheral_intback_issue();
    /*uint8_t * p = (uint8_t *)VDP1_VRAM(0); 

    if (_svin_vdp1_cmdlist_toggle_at_vblank)
    {
        if (VDP2_TVMD_TV_FIELD_SCAN_ODD == vdp2_tvmd_field_scan_get())
            //vdp1_cmdt_jump_assign(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX], _SVIN_VDP1_ORDER_LOCAL_COORDS_A_INDEX);
            p[3]=0x2C;//1C;
        else
            //vdp1_cmdt_jump_assign(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX], _SVIN_VDP1_ORDER_LOCAL_COORDS_B_INDEX);
            p[3]=0x04;
    }
    else
    {
        p[3]=0x04;
    }*/

    //vdp1_sync_cmdt_list_put(_svin_cmdt_list, 0, NULL, NULL);
    
    smpc_peripheral_intback_issue();

}

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
bool _svin_cram_24bpp;

void _svin_delay(int milliseconds)
{
    //delay
    volatile int dummy = 0;
    for (dummy = 0; dummy < 3000 * milliseconds; dummy++)
        ;
}

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

    // D0 D0 -- --
    // D0 D0 -- --
    // D0 D0 -- --
    // i0 D0 D0 --

    struct vdp2_vram_cycp vram_cycp;

    vram_cycp.pt[0].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[0].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[0].t2 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t3 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[0].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[1].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[1].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[1].t2 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t3 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[1].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[2].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[2].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[2].t2 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t3 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[3].t0 = VDP2_VRAM_CYCP_PNDR_NBG0;
    vram_cycp.pt[3].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[3].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[3].t3 = VDP2_VRAM_CYCP_NO_ACCESS;
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
vdp1_cmdt_param_color_mode5_set(vdp1_cmdt_t *cmdt)
{
        cmdt->cmd_pmod &= 0xFFC7;
        cmdt->cmd_pmod |= 0x0028;
        cmdt->cmd_colr = 0;
}

void _svin_init(_svin_screen_mode_t screen_mode)
{
    int *_pointer32;
    _svin_init_done = 0;
    _svin_frame_count = 0;
    _svin_vdp1_cmdlist_toggle_at_vblank = true;
    _svin_cram_24bpp = false;

    //-------------- setup VDP2 -------------------

    //clearing vdp2
    vdp2_tvmd_display_clear();

    //setup nbg0
    struct vdp2_scrn_cell_format format;
    memset(&format, 0x00, sizeof(format));
    vdp2_scrn_normal_map_t normal_map;
    memset(&normal_map, 0x00, sizeof(normal_map));

    format.scroll_screen = VDP2_SCRN_NBG0;
    format.ccc = VDP2_SCRN_CCC_PALETTE_256;
    format.char_size = VDP2_SCRN_CHAR_SIZE_1X1;
    format.pnd_size = 2;
    format.aux_mode = VDP2_SCRN_AUX_MODE_1;
    format.cpd_base = 0;
    format.palette_base = 0;
    format.plane_size = VDP2_SCRN_PLANE_SIZE_2X1;
    normal_map.plane_a = _SVIN_NBG0_PNDR_START;
    //format.sf_type = 0;//VDP2_SCRN_SF_TYPE_COLOR_CALCULATION;
    //format.sf_code = VDP2_SCRN_SF_CODE_A;
    //format.sf_mode = 0;
    //format.map_bases.plane_a = _SVIN_NBG0_PNDR_START;

    vdp2_scrn_cell_format_set(&format,&normal_map);
    vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 3);
    vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0);
    vdp2_cram_mode_set(1);

    //setup nbg1
    /*format.scroll_screen = VDP2_SCRN_NBG1;
    format.cc_count = VDP2_SCRN_CCC_PALETTE_256;
    format.character_size = VDP2_SCRN_CHAR_SIZE_1X1;
    format.pnd_size = 2;
    format.auxiliary_mode = 1;
    format.cp_table = 0;
    format.color_palette = 0;
    format.plane_size = (2 * 1);
    format.sf_type = 0;//VDP2_SCRN_SF_TYPE_COLOR_CALCULATION;
    format.sf_code = VDP2_SCRN_SF_CODE_A;
    format.sf_mode = 0;
    format.map_bases.plane_a = _SVIN_NBG1_PNDR_START;

    vdp2_scrn_cell_format_set(&format);
    vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5);
    vdp2_scrn_display_set(VDP2_SCRN_NBG1);*/
    //vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1,0x80);

    //setup nbg2
    /*format.scroll_screen = VDP2_SCRN_NBG2;
    format.cc_count = VDP2_SCRN_CCC_PALETTE_256;
    format.character_size = (1 * 1);
    format.pnd_size = 2;
    format.auxiliary_mode = 1;
    format.cp_table = 0;
    format.color_palette = 0;
    format.plane_size = (2 * 1);
    format.sf_type = 0;//VDP2_SCRN_SF_TYPE_COLOR_CALCULATION;
    format.sf_code = VDP2_SCRN_SF_CODE_A;
    format.sf_mode = 0;
    format.map_bases.plane_a = _SVIN_NBG2_PNDR_START;

    vdp2_scrn_cell_format_set(&format);
    vdp2_scrn_priority_set(VDP2_SCRN_NBG2, 6);
    vdp2_scrn_display_set(VDP2_SCRN_NBG2);*/

    vdp2_tvmd_interlace_t interlace = (_SVIN_SCANMODE_240P == screen_mode.scanmode) ? VDP2_TVMD_INTERLACE_NONE :
                                            (_SVIN_SCANMODE_240I == screen_mode.scanmode)  ? VDP2_TVMD_INTERLACE_SINGLE : VDP2_TVMD_INTERLACE_DOUBLE;

    vdp2_tvmd_horz_t horz = (_SVIN_X_RESOLUTION_320 == screen_mode.x_res) ? 
                                    screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_A : VDP2_TVMD_HORZ_NORMAL_A :
                                    screen_mode.x_res_doubled ? VDP2_TVMD_HORZ_HIRESO_B : VDP2_TVMD_HORZ_NORMAL_B;
                                    
    vdp2_tvmd_vert_t vert = (_SVIN_Y_RESOLUTION_224 == screen_mode.y_res) ? VDP2_TVMD_VERT_224 :
                                (_SVIN_Y_RESOLUTION_240 == screen_mode.y_res) ? VDP2_TVMD_VERT_240 : VDP2_TVMD_VERT_256;

    vdp2_tvmd_display_res_set(interlace, horz, vert);

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

    //setting cycle patterns for cpu access
    _svin_set_cycle_patterns_cpu();

    vdp_sync_vblank_out_set(_svin_vblank_out_handler, NULL);
    
    vdp2_tvmd_display_set();
    //cpu_intc_mask_set(0); //?????
    vdp2_sync();

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
    
    vdp1_cmdt_system_clip_coord_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX]);
    vdp1_cmdt_t *cmdt_system_clip_coords;
    cmdt_system_clip_coords = &_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX];
    cmdt_system_clip_coords->cmd_xc = (_SVIN_X_RESOLUTION_320 == screen_mode.x_res) ? 319 : 351;
    cmdt_system_clip_coords->cmd_yc = (_SVIN_Y_RESOLUTION_224 == screen_mode.y_res) ? 224 : 
                                                (_SVIN_Y_RESOLUTION_240 == screen_mode.y_res) ? 240 : 256;

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
    _svin_cmdt_list->cmdts[index].cmd_ya= (_SVIN_Y_RESOLUTION_224 == screen_mode.y_res) ? 0 : 
                                                (_SVIN_Y_RESOLUTION_240 == screen_mode.y_res) ? 8 : 16;
    vdp1_cmdt_char_base_set(&_svin_cmdt_list->cmdts[index],vdp1_vram_partitions.texture_base);
    //_svin_cmdt_list->cmdts[index].cmd_srca = (uint16_t)vdp1_vram_partitions.texture_base;
    _svin_cmdt_list->cmdts[index].cmd_size=((320/8)<<8)|(224);

    vdp1_cmdt_end_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LIMIT]);

    vdp1_sync_cmdt_list_put(_svin_cmdt_list, 0);

#define VDP1_FBCR_DIE (0x0008)
    if (_SVIN_SCANMODE_240P == screen_mode.scanmode)
        MEMORY_WRITE(16, VDP1(FBCR), 0);
    else
        MEMORY_WRITE(16, VDP1(FBCR), VDP1_FBCR_DIE);

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
                .color_mode = VDP1_ENV_COLOR_MODE_RGB_PALETTE,
                .sprite_type = 0x0
    };
    vdp1_env.erase_points[1].x = screen_mode.x_res_doubled ? 703 : 351;
    vdp1_env.erase_points[1].y = (_SVIN_SCANMODE_480I == screen_mode.scanmode) ? 511 : 255;

    vdp1_env_set(&vdp1_env);

    vdp1_sync();
    vdp1_sync_wait();

    //-------------- setup pattern names -------------------

    //writing pattern names for nbg0
    //starting with plane 0
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00000000 + _SVIN_NBG0_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
    }

    /*//writing pattern names for nbg1
    //nbg1  is mostly transparent, so fill with that one
    _pointer32 = (int *)_SVIN_NBG1_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG1_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00000000 + _SVIN_NBG1_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
    }

    //writing pattern names for nbg2
    //nbg2  is mostly transparent, so fill with that one
    _pointer32 = (int *)_SVIN_NBG2_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG2_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00000000 + _SVIN_NBG2_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
    }*/

    //-------------- setup character pattern names -------------------

    //clearing character pattern names data for nbg0
    _pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_CHPNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0;
    }

    /*//clearing character pattern names data for nbg1
    _pointer32 = (int *)_SVIN_NBG1_CHPNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG1_CHPNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0;
    }

    //clearing character pattern names data for nbg2
    _pointer32 = (int *)_SVIN_NBG2_CHPNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG2_CHPNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0;
    }*/

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

    /*//setting up "transparent" character for nbg1
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

    //setting up "transparent" character for nbg1
    _pointer32 = (int *)_SVIN_NBG2_CHPNDR_SPECIALS_ADDR;
    for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
    {
        _pointer32[i] = 0;
    }

    //setting up "semi-transparent" character for nbg1
    _pointer32 = (int *)(_SVIN_NBG2_CHPNDR_SPECIALS_ADDR + _SVIN_CHARACTER_BYTES);
    for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
    {
        _pointer32[i] = 0x7F7F7F7F;
    }*/

    //-------------- setup palettes -------------------

    //setup default palettes
    uint8_t temp_pal[3 * 256];
    //grayscale gradient
    for (int i = 0; i < 256; i++)
    {
        temp_pal[i * 3] = i;     
        temp_pal[i * 3 + 1] = i; 
        temp_pal[i * 3 + 2] = i; 
    }
    _svin_set_palette(0, temp_pal);
    //red
    memset(temp_pal,0,sizeof(temp_pal));
    for (int i = 0; i < 256; i++)
    {
        temp_pal[i * 3] = i;      
    }
    _svin_set_palette(1, temp_pal);
    //green
    memset(temp_pal,0,sizeof(temp_pal));
    for (int i = 0; i < 256; i++)
    {
       temp_pal[i * 3 + 1] = i;      
    }
    _svin_set_palette(2, temp_pal);
    //blue
    memset(temp_pal,0,sizeof(temp_pal));
    for (int i = 0; i < 256; i++)
    {
       temp_pal[i * 3 + 2] = i;      
    }
    _svin_set_palette(3, temp_pal);
    //cyan
    memset(temp_pal,0,sizeof(temp_pal));
    for (int i = 0; i < 256; i++)
    {
        temp_pal[i * 3 + 1] = i; 
        temp_pal[i * 3 + 2] = i;    
    }
    _svin_set_palette(4, temp_pal);
    //magenta
    memset(temp_pal,0,sizeof(temp_pal));
    for (int i = 0; i < 256; i++)
    {
        temp_pal[i * 3] = i;     
        temp_pal[i * 3 + 2] = i;    
    }
    _svin_set_palette(5, temp_pal);
    //yellow
    memset(temp_pal,0,sizeof(temp_pal));
    for (int i = 0; i < 256; i++)
    {
        temp_pal[i * 3] = i;     
        temp_pal[i * 3 + 1] = i; 
    }
    _svin_set_palette(6, temp_pal);  

    //setting cycle patterns for nbg access
    _svin_set_cycle_patterns_nbg();

    smpc_peripheral_init();

    _svin_debug_init();


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
void _svin_set_palette(int number, uint8_t *pointer)
{
    _svin_set_palette_part(number,pointer,0,255);
}

void _svin_set_palette_part(int number, uint8_t *pointer, int start, int end)
{
    if (_svin_cram_24bpp)
    {
        uint8_t *my_vdp2_cram8 = (uint8_t *)VDP2_VRAM_ADDR(8, 0x400 * number);
        for (int i = start; i <= end; i++)
        {
            my_vdp2_cram8[i*4+0] = 0;
            my_vdp2_cram8[i*4+1] = pointer[i * 3 + 2];
            my_vdp2_cram8[i*4+2] = pointer[i * 3 + 1];
            my_vdp2_cram8[i*4+3] = pointer[i * 3 + 0];;
        }
    }
    else
    {
        uint16_t *my_vdp2_cram = (uint16_t *)VDP2_VRAM_ADDR(8, 0x200 * number);
        for (int i = start; i <= end; i++)
        {
            my_vdp2_cram[i] = (((pointer[i * 3 + 2] & 0xF8) << 7) |
                            ((pointer[i * 3 + 1] & 0xF8) << 2) |
                            ((pointer[i * 3 + 0] & 0xF8) >> 3));
        }        
    }
}

void _svin_clear_palette(int number)
{
    _svin_clear_palette_part(number,0,255);
}

void _svin_clear_palette_part(int number, int start, int end)
{
    if (_svin_cram_24bpp)
    {
        uint32_t *my_vdp2_cram32 = (uint32_t *)VDP2_VRAM_ADDR(8, 0x400 * number);
        for (int i = start; i <= end; i++)
        {
            my_vdp2_cram32[i] = 0;
        }
    }
    else
    {
        uint16_t *my_vdp2_cram = (uint16_t *)VDP2_VRAM_ADDR(8, 0x200 * number);
        for (int i = start; i <=  end; i++)
        {
            my_vdp2_cram[i] = 0;
        }
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

int _svin_wait_for_key_press_and_release()
{
    bool bPressed = false;
    int iCode = 0;
    while (false == bPressed)
    {
        smpc_peripheral_process();
        smpc_peripheral_digital_port(1, &_digital);
        if (_digital.pressed.raw != 0)
        {
            bPressed = true;
            iCode = _digital.pressed.raw;
        }
    }
    _svin_delay(15);//debounce
    while (true == bPressed)
    {
        smpc_peripheral_process();
        smpc_peripheral_digital_port(1, &_digital);
        if (_digital.pressed.raw == 0)
            bPressed = false;
    }
    return iCode;
}

int _svin_get_keys_state()
{
    smpc_peripheral_process();
    smpc_peripheral_digital_port(1, &_digital);
    return _digital.pressed.raw;
}

void _svin_set_palette_24bpp()
{
    _svin_cram_24bpp = true;
    vdp2_cram_mode_set(2);
}
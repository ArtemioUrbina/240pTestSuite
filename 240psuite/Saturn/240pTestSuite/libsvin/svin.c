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
int _svin_videomode_x_res;
int _svin_videomode_y_res;
bool _svin_videomode_progressive;
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

    // D0 D0 D2 D2
    // D0 D0 D1 D1
    // i0 i1 D1 D1
    // -- i2 D2 D2

    struct vdp2_vram_cycp vram_cycp;

    vram_cycp.pt[0].t0 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[0].t1 = VDP2_VRAM_CYCP_CHPNDR_NBG0;
    vram_cycp.pt[0].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG2;
    vram_cycp.pt[0].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG2;
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

    vram_cycp.pt[2].t0 = VDP2_VRAM_CYCP_PNDR_NBG0;
    vram_cycp.pt[2].t1 = VDP2_VRAM_CYCP_PNDR_NBG1;
    vram_cycp.pt[2].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
    vram_cycp.pt[2].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG1;
    vram_cycp.pt[2].t4 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t5 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t6 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[2].t7 = VDP2_VRAM_CYCP_NO_ACCESS;

    vram_cycp.pt[3].t0 = VDP2_VRAM_CYCP_NO_ACCESS;
    vram_cycp.pt[3].t1 = VDP2_VRAM_CYCP_PNDR_NBG2;
    vram_cycp.pt[3].t2 = VDP2_VRAM_CYCP_CHPNDR_NBG2;
    vram_cycp.pt[3].t3 = VDP2_VRAM_CYCP_CHPNDR_NBG2;
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

void _svin_init(_svin_x_resolution_t x, _svin_y_resolution_t y, bool progressive)
{
    int *_pointer32;
    _svin_init_done = 0;
    _svin_videomode_progressive = progressive;
    _svin_frame_count = 0;
    _svin_vdp1_cmdlist_toggle_at_vblank = true;
    _svin_cram_24bpp = false;

    switch (x)
    {
        case _SVIN_X_RESOLUTION_320 :
            _svin_videomode_x_res = 320;
            break;
        case _SVIN_X_RESOLUTION_352 :
            _svin_videomode_x_res = 352;
            break;
        case _SVIN_X_RESOLUTION_640 :
            _svin_videomode_x_res = 640;
            break;
        case _SVIN_X_RESOLUTION_704 :
            _svin_videomode_x_res = 704;
            break;
    }
    switch (y)
    {
        case _SVIN_Y_RESOLUTION_224 :
            _svin_videomode_y_res = 224;
            break;
        case _SVIN_Y_RESOLUTION_240 :
            _svin_videomode_y_res = 240;
            break;
        case _SVIN_Y_RESOLUTION_256 :
            _svin_videomode_y_res = 256;
            break;
        case _SVIN_Y_RESOLUTION_448 :
            _svin_videomode_y_res = 448;
            break;
        case _SVIN_Y_RESOLUTION_480 :
            _svin_videomode_y_res = 480;
            break;
        case _SVIN_Y_RESOLUTION_512 :
            _svin_videomode_y_res = 512;
            break;
    }

    //-------------- setup VDP2 -------------------

    //clearing vdp2
    vdp2_tvmd_display_clear();

    //setup nbg0
    struct vdp2_scrn_cell_format format;
    memset(&format, 0x00, sizeof(format));

    format.scroll_screen = VDP2_SCRN_NBG0;
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
    format.map_bases.plane_a = _SVIN_NBG0_PNDR_START;

    vdp2_scrn_cell_format_set(&format);
    vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 3);
    vdp2_scrn_display_set(VDP2_SCRN_NBG0);
    vdp2_cram_mode_set(1);

    //setup nbg1
    format.scroll_screen = VDP2_SCRN_NBG1;
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
    format.map_bases.plane_a = _SVIN_NBG1_PNDR_START;

    vdp2_scrn_cell_format_set(&format);
    vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5);
    vdp2_scrn_display_set(VDP2_SCRN_NBG1);
    //vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1,0x80);

    //setup nbg2
    format.scroll_screen = VDP2_SCRN_NBG2;
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
    vdp2_scrn_display_set(VDP2_SCRN_NBG2);

    vdp2_tvmd_interlace_t interlace = VDP2_TVMD_INTERLACE_SINGLE;
    if (_svin_videomode_y_res > 256) 
        interlace = VDP2_TVMD_INTERLACE_DOUBLE;
    else if (_svin_videomode_progressive) 
        interlace = VDP2_TVMD_INTERLACE_NONE;
    vdp2_tvmd_horz_t horz = VDP2_TVMD_HORZ_NORMAL_A;
    switch (_svin_videomode_x_res)
    {
        case 352:
            horz = VDP2_TVMD_HORZ_NORMAL_B;
            break;
        case 640:
            horz = VDP2_TVMD_HORZ_HIRESO_A;
            break;
        case 704:
            horz = VDP2_TVMD_HORZ_HIRESO_B;
            break;
    }
    vdp2_tvmd_vert_t vert = VDP2_TVMD_VERT_224;
    switch (_svin_videomode_y_res)
    {
        case 240:
        case 480:
            vert = VDP2_TVMD_VERT_240;
            break;
        case 256:
        case 512:
            vert = VDP2_TVMD_VERT_256;
            break;
    }

    vdp2_tvmd_display_res_set(interlace, horz, vert);

    color_rgb1555_t bs_color;
    bs_color = COLOR_RGB1555(1, 0, 0, 0);
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
    _svin_cmdt_list = vdp1_cmdt_list_alloc(_SVIN_VDP1_ORDER_LIMIT);

    static const int16_vec2_t local_coord_ul =
        INT16_VEC2_INITIALIZER(0,
                               0);

    static const vdp1_cmdt_draw_mode_t sprite_draw_mode = {
        .raw = 0x0000,
        .bits.pre_clipping_disable = true};

    vdp1_cmdt_color_bank_t font_color_bank;
    font_color_bank.raw = 0;

    assert(_svin_cmdt_list != NULL);

    (void)memset(_svin_cmdt_list->cmdts, 0x00, sizeof(vdp1_cmdt_t) * _SVIN_VDP1_ORDER_LIMIT);

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
    cmdt_system_clip_coords->cmd_xc = _svin_videomode_x_res - 1;
    cmdt_system_clip_coords->cmd_yc = _svin_videomode_y_res - 1;

    vdp1_cmdt_local_coord_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LOCAL_COORDS_INDEX]);
    vdp1_cmdt_param_vertex_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LOCAL_COORDS_INDEX],
                               CMDT_VTX_LOCAL_COORD, &local_coord_ul);
    
    //setting up VDP1 text layer quads
    //there are from 1 to 4 quads required depending on X and Y resolution configuration
    //VDP1 quads can't be bigger than 508, thus for high-res X (640 or 704) we use 2 quads along X axis
    //for 480p VDP1 outputs 1 half-resolution progressive frame at 50/60 fps, using 2 quads along Y axis
    //for 480i VDP1 outputs 2 different fields at 25/30 fps each, using 2 quads along Y axis
    //for 240p VDP1 outputs 1 progressive frame at 50/60 fps, using 1 quad along Y axis

    //quad 0 is always used, and is always at 0,0
    int index = _SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_svin_cmdt_list->cmdts[index]);
    vdp1_cmdt_param_draw_mode_set(&_svin_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_param_color_mode4_set(&_svin_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _svin_cmdt_list->cmdts[index].cmd_xa=0;
    _svin_cmdt_list->cmdts[index].cmd_ya=0;
    _svin_cmdt_list->cmdts[index].cmd_srca = vdp1_vram_partitions.texture_base;
    if (_svin_videomode_x_res > 512) {
        if (_svin_videomode_y_res > 256) 
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/16)<<8)|(_svin_videomode_y_res/2);
        else
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/16)<<8)|(_svin_videomode_y_res);
    }
    else {
        if (_svin_videomode_y_res > 256)
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/8)<<8)|(_svin_videomode_y_res/2);
        else
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/8)<<8)|(_svin_videomode_y_res);     
    }
    
    //quad 1 is used for high X resolution
    index = _SVIN_VDP1_ORDER_TEXT_SPRITE_1_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_svin_cmdt_list->cmdts[index]);
    vdp1_cmdt_param_draw_mode_set(&_svin_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_param_color_mode4_set(&_svin_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _svin_cmdt_list->cmdts[index].cmd_ya=0;
    if (_svin_videomode_x_res > 512) {
        _svin_cmdt_list->cmdts[index].cmd_xa=(_svin_videomode_x_res/2);
        if (_svin_videomode_y_res > 256) 
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/16)<<8)|(_svin_videomode_y_res/2);
        else
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/16)<<8)|(_svin_videomode_y_res);
    }
    else {
        _svin_cmdt_list->cmdts[index].cmd_xa=0;
        _svin_cmdt_list->cmdts[index].cmd_size=0;
    }

    //quad 2 is used for high Y resolution, i.e. 480i/480p modes
    index = _SVIN_VDP1_ORDER_TEXT_SPRITE_2_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_svin_cmdt_list->cmdts[index]);
    vdp1_cmdt_param_draw_mode_set(&_svin_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_param_color_mode4_set(&_svin_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _svin_cmdt_list->cmdts[index].cmd_xa=0;
    if (_svin_videomode_y_res > 256) {
        _svin_cmdt_list->cmdts[index].cmd_ya=(_svin_videomode_y_res/2);
        if (_svin_videomode_x_res > 512) 
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/16)<<8)|(_svin_videomode_y_res/2);
        else
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/8)<<8)|(_svin_videomode_y_res/2);
    }
    else {
        _svin_cmdt_list->cmdts[index].cmd_ya=0;
        _svin_cmdt_list->cmdts[index].cmd_size=0;
    }

    //quad 3 is used for high X and high Y resolution
    index = _SVIN_VDP1_ORDER_TEXT_SPRITE_3_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_svin_cmdt_list->cmdts[index]);
    vdp1_cmdt_param_draw_mode_set(&_svin_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_param_color_mode4_set(&_svin_cmdt_list->cmdts[index],font_color_bank);//8bpp
    if ( (_svin_videomode_x_res > 512) && (_svin_videomode_y_res > 256) ) {
            _svin_cmdt_list->cmdts[index].cmd_xa=(_svin_videomode_x_res/2);
            _svin_cmdt_list->cmdts[index].cmd_ya=(_svin_videomode_y_res/2);
            _svin_cmdt_list->cmdts[index].cmd_size=((_svin_videomode_x_res/16)<<8)|(_svin_videomode_y_res/2);
    }
    else {
        _svin_cmdt_list->cmdts[index].cmd_xa=0;
        _svin_cmdt_list->cmdts[index].cmd_ya=0;
        _svin_cmdt_list->cmdts[index].cmd_size=0;
    }

    vdp1_cmdt_end_set(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_LIMIT]);

    vdp1_sync_cmdt_list_put(_svin_cmdt_list, 0);

#define VDP1_FBCR_DIE (0x0008)
    if (_svin_videomode_progressive) 
        MEMORY_WRITE(16, VDP1(FBCR), 0);
    else
        MEMORY_WRITE(16, VDP1(FBCR), VDP1_FBCR_DIE);

    static vdp1_env_t vdp1_env = {
                .erase_color = COLOR_RGB1555(0, 0, 0, 0),
                .erase_points[0] = {
                        .x = 0,
                        .y = 0
                },
                .erase_points[1] = {
                        .x = 1,
                        .y = 1
                },
                .bpp = VDP1_ENV_BPP_8,
                .rotation = VDP1_ENV_ROTATION_0,
                .color_mode = VDP1_ENV_COLOR_MODE_PALETTE,
                .sprite_type = 0xC
    };

    if (_svin_videomode_x_res < 512)
    {
        vdp1_env.bpp = VDP1_ENV_BPP_16;
        vdp1_env.color_mode = VDP1_ENV_COLOR_MODE_RGB_PALETTE;
        vdp1_env.sprite_type = 0x0;
    }
    else
    {
        vdp1_env.bpp = VDP1_ENV_BPP_8;
        vdp1_env.color_mode = VDP1_ENV_COLOR_MODE_PALETTE;
        vdp1_env.sprite_type = 0xC;
    }

    vdp1_env.erase_points[1].x = _svin_videomode_x_res - 1;
    vdp1_env.erase_points[1].y = _svin_videomode_y_res - 1;

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

    //writing pattern names for nbg1
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

    //clearing character pattern names data for nbg2
    _pointer32 = (int *)_SVIN_NBG2_CHPNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG2_CHPNDR_SIZE / sizeof(int); i++)
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
    }

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

    _svin_text_init();
    _svin_textbox_init();
    _svin_textbox_disable();

    _svin_sprite_init();

    //setting cycle patterns for nbg access
    _svin_set_cycle_patterns_nbg();

    smpc_peripheral_init();

    _svin_debug_init();

    //_svin_menu_init();  //not now, because it requires filelist

    //-------------- init end -------------------  
    //vdp1_cmdt_jump_assign(&_svin_cmdt_list->cmdts[_SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX], _SVIN_VDP1_ORDER_LOCAL_COORDS_B_INDEX * 4);
    _svin_init_done = 1;
}

void _svin_deinit()
{

    free(_svin_cmdt_list);
    _svin_sprite_deinit();
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
        uint8_t *my_vdp2_cram8 = (uint16_t *)VDP2_VRAM_ADDR(8, 0x400 * number);
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
        uint32_t *my_vdp2_cram32 = (uint16_t *)VDP2_VRAM_ADDR(8, 0x400 * number);
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
#include <yaul.h>
#include <stdlib.h>
#include "video.h"
#include "video_vdp1.h"

vdp1_cmdt_list_t *_cmdt_list;

void video_vdp1_init(video_screen_mode_t screen_mode)
{
    int *_pointer32;

    //-------------- setup VDP1 -------------------
    //setting up a small VDP1 list with 3 commands: sys clip, local coords, end
    //everything else will be appended to this list later
    _cmdt_list = vdp1_cmdt_list_alloc(VIDEO_VDP1_ORDER_LIMIT+1);

    static const int16_vec2_t local_coord_ul =
        INT16_VEC2_INITIALIZER(0,
                               0);

    static const vdp1_cmdt_draw_mode_t sprite_draw_mode = {
        .raw = 0x0000,
        .pre_clipping_disable = true};

    vdp1_cmdt_color_bank_t font_color_bank;
    font_color_bank.raw = 0;

    assert(_cmdt_list != NULL);

    (void)memset(_cmdt_list->cmdts, 0x00, sizeof(vdp1_cmdt_t) * (VIDEO_VDP1_ORDER_LIMIT+1));

    vdp1_vram_partitions_set(64,//VDP1_VRAM_DEFAULT_CMDT_COUNT,
                              0x7F000, //  VDP1_VRAM_DEFAULT_TEXTURE_SIZE,
                               0,//  VDP1_VRAM_DEFAULT_GOURAUD_COUNT,
                               0);//  VDP1_VRAM_DEFAULT_CLUT_COUNT);

    vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);

    _cmdt_list->count = VIDEO_VDP1_ORDER_LIMIT+1;
    
    //setting clipping coordinates
    vdp1_cmdt_system_clip_coord_set(&_cmdt_list->cmdts[VIDEO_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX]);
    vdp1_cmdt_t *cmdt_system_clip_coords;
    cmdt_system_clip_coords = &_cmdt_list->cmdts[VIDEO_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX];
    cmdt_system_clip_coords->cmd_xc = (VIDEO_X_RESOLUTION_320 == screen_mode.x_res) ? 319 : 351;
    cmdt_system_clip_coords->cmd_yc = (VDP2_TVMD_VERT_224 == screen_mode.y_res) ? 224 : 
                                                (VDP2_TVMD_VERT_240 == screen_mode.y_res) ? 240 : 256;

    vdp1_cmdt_local_coord_set(&_cmdt_list->cmdts[VIDEO_VDP1_ORDER_LOCAL_COORDS_INDEX]);
    vdp1_cmdt_vtx_local_coord_set(&_cmdt_list->cmdts[VIDEO_VDP1_ORDER_LOCAL_COORDS_INDEX], local_coord_ul);
    
    //setting up VDP1 text layer quads
    //there are from 1 to 4 quads required depending on X and Y resolution configuration
    //VDP1 quads can't be bigger than 508, thus for high-res X (640 or 704) we use 2 quads along X axis
    //for 480p VDP1 outputs 1 half-resolution progressive frame at 50/60 fps, using 2 quads along Y axis
    //for 480i VDP1 outputs 2 different fields at 25/30 fps each, using 2 quads along Y axis
    //for 240p VDP1 outputs 1 progressive frame at 50/60 fps, using 1 quad along Y axis

    //quad for text, always 320x224 and always centered 
    int index = VIDEO_VDP1_ORDER_TEXT_SPRITE_0_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_cmdt_list->cmdts[index]);
    vdp1_cmdt_draw_mode_set(&_cmdt_list->cmdts[index], sprite_draw_mode);
    _cmdt_list->cmdts[index].cmd_draw_mode.trans_pixel_disable = 1;
    vdp1_cmdt_color_mode4_set(&_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _cmdt_list->cmdts[index].cmd_xa= (VIDEO_X_RESOLUTION_320 == screen_mode.x_res) ? 0 : 16;
    _cmdt_list->cmdts[index].cmd_ya= (VDP2_TVMD_VERT_224 == screen_mode.y_res) ? 0 : 
                                                (VDP2_TVMD_VERT_240 == screen_mode.y_res) ? 8 : 16;
    vdp1_cmdt_char_base_set(&_cmdt_list->cmdts[index],vdp1_vram_partitions.texture_base);
    _cmdt_list->cmdts[index].cmd_size=((320/8)<<8)|(224);

    //quad for shadowdrop test, 32x32, out-of-screen, storing after text quad at 0x11800
    index = VIDEO_VDP1_ORDER_TEXT_SPRITE_1_INDEX; 
    vdp1_cmdt_normal_sprite_set(&_cmdt_list->cmdts[index]);
    vdp1_cmdt_draw_mode_set(&_cmdt_list->cmdts[index], sprite_draw_mode);
    vdp1_cmdt_color_mode4_set(&_cmdt_list->cmdts[index],font_color_bank);//8bpp
    _cmdt_list->cmdts[index].cmd_xa = -40;
    _cmdt_list->cmdts[index].cmd_ya = -40;
    vdp1_cmdt_char_base_set(&_cmdt_list->cmdts[index],vdp1_vram_partitions.texture_base+0x11800);
    _cmdt_list->cmdts[index].cmd_size=((32/8)<<8)|(32);

    //end of commands list
    vdp1_cmdt_end_set(&_cmdt_list->cmdts[VIDEO_VDP1_ORDER_LIMIT]);

    vdp1_sync_cmdt_list_put(_cmdt_list, 0);

/*#define VDP1_FBCR_DIE (0x0008)
    if ( (VIDEO_SCANMODE_240P == screen_mode.scanmode) || (VIDEO_SCANMODE_480P == screen_mode.scanmode) )
        MEMORY_WRITE(16, VDP1(FBCR), 0);
    else
        MEMORY_WRITE(16, VDP1(FBCR), VDP1_FBCR_DIE);*/

/*#define VDP1_TVMR_VBE (0x0008)
    if (VIDEO_SCANMODE_480P == screen_mode.scanmode)
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

    if (VIDEO_SCANMODE_480P == screen_mode.scanmode)
        vdp1_env.hdtv = VDP1_ENV_HDTV_ON;
    else
        vdp1_env.hdtv = VDP1_ENV_HDTV_OFF;
    vdp1_env.erase_points[1].x = screen_mode.x_res_doubled ? 703 : 351;
    vdp1_env.erase_points[1].y = ( (VIDEO_SCANMODE_480I == screen_mode.scanmode) || (VIDEO_SCANMODE_480P == screen_mode.scanmode) ) ? 511 : 255;

    vdp1_env_set(&vdp1_env);

    vdp1_sync();
    vdp1_sync_wait();
}

void video_vdp1_deinit()
{
    free(_cmdt_list);
}
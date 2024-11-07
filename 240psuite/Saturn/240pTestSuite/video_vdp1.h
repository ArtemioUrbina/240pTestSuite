#ifndef VIDEO_VDP1_H
#define VIDEO_VDP1_H

#include "video.h"

//VDP1 command list order
#define VIDEO_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX  0
#define VIDEO_VDP1_ORDER_LOCAL_COORDS_INDEX        1
#define VIDEO_VDP1_ORDER_TEXT_SPRITE_0_INDEX       2
#define VIDEO_VDP1_ORDER_TEXT_SPRITE_1_INDEX       3
#define VIDEO_VDP1_ORDER_LIMIT                    4

void video_vdp1_init(video_screen_mode_t screen_mode);

#endif /* !VIDEO_VDP1_H */

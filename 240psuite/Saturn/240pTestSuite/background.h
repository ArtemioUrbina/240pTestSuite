#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "video_vdp2.h"

void background_fade_to_black_step();
void background_fade_to_black();
void background_set_from_assets(uint8_t *ptr, int size, int *names_ptr, int *data_ptr);
void draw_bg_with_expansion(video_screen_mode_t screenmode, bool bMascot);
void draw_bg_donna(video_screen_mode_t screenmode);

#endif /* !BACKGROUND_H */

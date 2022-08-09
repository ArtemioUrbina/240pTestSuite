#ifndef _SVIN_TEXTBOX_H_
#define _SVIN_TEXTBOX_H_

#include <yaul.h>

//#include <assert.h>
#include <stdlib.h>

#include "svin.h"

#include <mcufont.h>

/*typedef struct {
    const char *fontname;
    const char *filename;
    const char *text;
    bool justify;
    enum mf_align_t alignment;
    int width;
    int margin;
    int anchor;
    int scale;
} options_t;

typedef struct {
    options_t *options;
    uint8_t *buffer;
    uint16_t width;
    uint16_t height;
    uint16_t y;
    const struct mf_font_s *font;
} state_t;*/

void _svin_textbox_init();

void _svin_textbox_print(const char * speaker, const char * text, const char * fontname, int speaker_color, int text_color);

void _svin_textbox_clear();

void _svin_textbox_disable();

void _svin_textbox_init_palette();

#endif

#include <yaul.h>

//#include <assert.h>
#include <stdlib.h>

#include "svin.h"
#include "svin_text.h"

#include <mcufont.h>

#define UNUSED(x) (void)(x)

/* Callback to write to a memory buffer. */
static void pixel_callback(int16_t x, int16_t y, uint8_t count, uint8_t alpha,
                           void *state)
{
    state_t *s = (state_t*)state;
    uint32_t pos;
    int16_t value;
    
    if (y < 0 || y >= s->height) return;
    if (x < 0 || x + count >= s->width) return;
    
    while (count--)
    {
        pos = (uint32_t)s->width * y + x;
        value = s->buffer[pos];
        value -= alpha;
        if (value < 0) value = 0;
        s->buffer[pos] = value;
        
        x++;
    }
}

/* Callback to render characters. */
static uint8_t character_callback(int16_t x, int16_t y, mf_char character,
                                  void *state)
{
    state_t *s = (state_t*)state;
    return mf_render_character(s->font, x, y, character, pixel_callback, state);
}

/* Callback to render lines. */
static bool line_callback(const char *line, uint16_t count, void *state)
{
    state_t *s = (state_t*)state;
    
    if (s->options->justify)
    {
        mf_render_justified(s->font, s->options->anchor, s->y,
                            s->width - s->options->margin * 2,
                            line, count, character_callback, state);
    }
    else
    {
        mf_render_aligned(s->font, s->options->anchor, s->y,
                          s->options->alignment, line, count,
                          character_callback, state);
    }
    s->y += s->font->line_height;
    return true;
}

/* Callback to just count the lines.
 * Used to decide the image height */
static bool count_lines(const char *line, uint16_t count, void *state)
{
        UNUSED(line);
        UNUSED(count);
        int *linecount = (int*)state;
        (*linecount)++;
        return true;
}


void 
_svin_text_init()
{
    //-------------- setup palette 7 specifically for text  -------------------
    _svin_text_init_palette(); 
}

void 
_svin_text_init_palette()
{
    //filling pallete with valid colors to enable text layer
   //setup default palettes
    uint8_t temp_pal[3 * 256];

    //pallete 7 is special. it's 16 gradients from solid colors to "backgroung frame" color  
    //reverse grayscale
    int iBaseR,iBaseG,iBaseB;
    int iStepR,iStepG,iStepB;
    for (int iColor=0; iColor<16; iColor++)
    {
        iBaseR = 0x7F; iBaseG = 0x7F; iBaseB = 0x7F; 
        switch(iColor)
        {
            case 0: //quasi-black
                iStepR = -8; iStepG = -8; iStepB = -8;
                break;
            case 1: //red
                iStepR = 8; iStepG = -7; iStepB = -7;
                break;
            case 2: //green
                iStepR = -7; iStepG = 8; iStepB = -7;
                break;
            case 3: //blue
                iStepR = -7; iStepG = -7; iStepB = 8;
                break;
            case 4: //cyan
                iStepR =-7; iStepG = 8; iStepB = 8;
                break;
            case 5: //magenta
                iStepR = 8; iStepG = -7; iStepB = 8;
                break;
            case 6: //yellow
                iStepR = 8; iStepG = 8; iStepB = -7;
                break;
            case 7: //white
                iStepR = 8; iStepG = 8; iStepB = 8;
                break;
            case 8: //orange
                iStepR = 6; iStepG = 0; iStepB = -6;
                break;
            case 9: //olive
                iStepR = 0; iStepG = 4; iStepB = -4;
                break;
            case 10: //brown
                iStepR = 2; iStepG = 0; iStepB = -7;
                break;
            case 11: //lightblue
                iStepR = 0; iStepG = 4; iStepB = 8;
                break;
            case 12: //meaty
                iStepR = 6; iStepG = 0; iStepB = 0;
                break;
            case 13: //khaki
                iStepR = -7; iStepG = -2; iStepB = 0;
                break;
            case 14: //grey
                iStepR = 2; iStepG = 2; iStepB = 2;
                break;
            case 15: //coffee
                iStepR = 6; iStepG = 4; iStepB = -2;
                break;
        }

        for (int i = 15; i >= 0; i--)
        {
            temp_pal[iColor*48 + i * 3] = iBaseR;     
            temp_pal[iColor*48 + i * 3 + 1] = iBaseG; 
            temp_pal[iColor*48 + i * 3 + 2] = iBaseB; 
            iBaseR += iStepR;
            iBaseG += iStepG;
            iBaseB += iStepB;
        }
    }

    _svin_set_palette(7, temp_pal);
}

int
_svin_text_render(uint8_t * buffer, int width, const char * text, const char * fontname)
{
        int height;
        const struct mf_font_s *font;
        options_t options;
        state_t state = {};

        // rendering the text

        //first fill state
        memset(&options, 0, sizeof(options_t));
        options.fontname = fontname;
        options.text = text;
        options.filename = NULL;
        options.width = width;
        options.margin = 0;
        options.scale = 1;
        options.alignment = MF_ALIGN_LEFT;
        options.anchor = options.margin;

        font = mf_find_font(options.fontname);

        assert(font != NULL);
        
        //Count the number of lines that we need. 
        height = 0;
        mf_wordwrap(font, options.width - 2 * options.margin, options.text, count_lines, &height);
        height *= font->height;
        //height += 4;
        //height = (((height-1)/8)+1)*8;

        // Setup the image buffer
        state.options = &options;
        state.width = options.width;
        state.height = height;
        state.buffer = buffer;
        state.y = 0;
        state.font = font;

        // Initialize image to white
        memset(state.buffer, 255, options.width * height); 

        // Render the text
        mf_wordwrap(font, options.width - 2 * options.margin, options.text, line_callback, &state);

        return height;
}


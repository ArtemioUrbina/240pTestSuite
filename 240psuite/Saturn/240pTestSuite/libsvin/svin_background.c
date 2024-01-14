#include <yaul.h>
#include <vdp1/vram.h>
#include <bcl.h>
#include <svin.h>
//#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern uint8_t _svin_init_done;

void _svin_background_fade_to_black_step()
{
    uint8_t *my_vdp2_cram = (uint8_t *)VDP2_CRAM_ADDR(0x200);
    uint8_t r, g, b;
    for (int i = 0; i < 256; i++)
    {
        b = my_vdp2_cram[i*4+1];
        g = my_vdp2_cram[i*4+2];
        r = my_vdp2_cram[i*4+3];
        if (r > 4) r-=4; else r=0;
        if (g > 4) g-=4; else g=0;
        if (b > 4) b-=4; else b=0;
        my_vdp2_cram[i*4+1] = b;
        my_vdp2_cram[i*4+2] = g;
        my_vdp2_cram[i*4+3] = r;
    }
}

void _svin_background_fade_to_black()
{
    for (int fade = 0; fade < 64; fade++)
    {
        _svin_background_fade_to_black_step();
        //wait for 1 frame
        vdp2_tvmd_vblank_in_next_wait(1);
    }
}

void 
_svin_background_set_from_assets(uint8_t *ptr, int size)
{
    bool bVDP2 = false;
    uint16_t *ptr16 = (uint16_t*)ptr;
    uint16_t size_x = ptr16[2]/8;
    uint16_t size_y = ptr16[3]/8;
    if ( (ptr[2044] == 'V') && (ptr[2045] == 'D') && (ptr[2046] == 'P') && (ptr[2047] == '2') )
        bVDP2 = true; //VDP2 mode
    else 
        return; //not supporting old VDP1 backgrounds

    //clear background palette to hide loading
    _svin_clear_palette(1);

    //reading 2nd block to check if compressed
    if ( (ptr[2048] == 'L') && (ptr[2048+1] == 'Z') && (ptr[2048+2] == '7') && (ptr[2048+3] == '7') )
    {
        //compressed, decompressing
        //getting size
        int * p32 = (int *)(&ptr[2048]);
        int compressed_size = p32[1];
        assert(compressed_size > 0);
        assert(compressed_size < 0x1000000);
        int compressed_size_sectors = ((compressed_size-1)/2048)+1;
    
        _svin_set_cycle_patterns_cpu();
        //writing pattern names for nbg0
        int *_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
        //starting with plane 0
        for (unsigned int x = 0; ((x < 64)&&(x<size_x)) ; x++)
        {
            for (unsigned int y = 0; y  < size_y; y++)
            {   
                _pointer32[y*64+x] = 0x00100000  + y*size_x*2+x*2; //palette 0, transparency on
            }
        }
        //now plane 1
        for (unsigned int x = 64; x < size_x ; x++)
        {
            for (unsigned int y = 0; y  < size_y; y++)
            {   
                _pointer32[64*64+y*64+x-64] = 0x00100000  + y*size_x*2+x*2; //palette 0, transparency on
            }
        }
        
        //decompress
        bcl_lz_decompress(&(ptr[2048+8]),(char*)_SVIN_NBG0_CHPNDR_START,compressed_size);
        _svin_set_cycle_patterns_nbg();

        //set palette, using palette 1 for VDP2 backgrounds
        //palette in file is 24-bit, setting it color-by-color
        rgb888_t _color  = {0,0,0,0};
        for (int i = 0; i<256; i++)
        {
            _color.r = ptr[2048*compressed_size_sectors+2048+i*3+0];//palette[i*3+0];
            _color.g = ptr[2048*compressed_size_sectors+2048+i*3+1];//palette[i*3+1];
            _color.b = ptr[2048*compressed_size_sectors+2048+i*3+2];//palette[i*3+2];
            _svin_set_palette_part(1, &_color, i, i);
        }
    }
    else
    {
        //uncompressed, not supported anymore
    }
}

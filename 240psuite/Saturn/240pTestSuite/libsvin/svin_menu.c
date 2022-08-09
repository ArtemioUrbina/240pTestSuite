#include <yaul.h>

//#include <assert.h>
#include <stdlib.h>

#include "svin.h"
#include "svin_text.h"
#include "svin_menu.h"

#include <mcufont.h>

#define UNUSED(x) (void)(x)

_svin_menu_item_type _svin_menu_items[10];
uint8_t _svin_menu_items_count;

uint32_t MenuLinks[512];

void 
_svin_menu_init(char * filename)
{
        int i;
        _svin_menu_items_count = 0; //starting unpopulated
        fad_t _menulinks_fad;
        bool b = _svin_filelist_search(filename,&_menulinks_fad,&i);
        assert(true == b);
        _svin_cd_block_sector_read(_menulinks_fad, (uint8_t*)MenuLinks);
}

void 
_svin_menu_clear()
{
    _svin_set_cycle_patterns_cpu();

    int * _pointer32 = (int *)(_SVIN_NBG2_CHPNDR_TEXTBOX_ADDR);
    for (unsigned int i = 0; i < (_SVIN_NBG2_CHPNDR_TEXTBOX_SIZE) / sizeof(int); i+=4)
    {
        _pointer32[i] = 0x0F000F00;
        _pointer32[i+1] = 0x0F000F00;
        _pointer32[i+2] = 0x000F000F;
        _pointer32[i+3] = 0x000F000F;
    }

    _svin_set_cycle_patterns_nbg();
}

void 
_svin_menu_populate(int jump, const char * text)
{
        //just fill internal structures, do not draw anything yet
        if (_svin_menu_items_count >= 10) return;
        _svin_menu_items[_svin_menu_items_count].id = _svin_menu_items_count;
        _svin_menu_items[_svin_menu_items_count].jump = jump;
        _svin_menu_items[_svin_menu_items_count].line = malloc(2048);
        strcpy(_svin_menu_items[_svin_menu_items_count].line,text);
        _svin_menu_items_count++;
}

void 
_svin_menu_depopulate()
{
        for (int i =0; i<_svin_menu_items_count; i++)
        {
            free(_svin_menu_items[i].line);
        }
        _svin_menu_items_count = 0;
}

int 
_svin_menu_activate()
{
        //render text on up to 10 40x2 stripes of text 
        int *_pointer32;

        assert(_svin_menu_items_count>0);
        assert(_svin_menu_items_count<11);

        int8_t SelectorPosition = 0;

        _svin_set_cycle_patterns_cpu();

        //-------------- setup pattern names -------------------
        _pointer32 = (int *)_SVIN_NBG2_PNDR_START;

        //menu is 40x20 tiles, to fit into a single plane, and into the same tilespace as dialog 
        int index = 0;
        int iOffset;
        int iStartCell = 28-_svin_menu_items_count;
        for (int y = iStartCell; y < iStartCell + _svin_menu_items_count*2; y++)
        {
                //plane 0 only
                iOffset = y * 64;
                for (int x = 24; x < 64; x++)
                {
                _pointer32[iOffset + x] = 0x10700000 + _SVIN_NBG2_CHPNDR_TEXTBOX_INDEX + _SVIN_CHARACTER_UNITS * index; //palette 7, transparency on
                index++;
                }
        }

        _svin_set_cycle_patterns_nbg();

        _svin_menu_clear();

        int height;

        uint8_t *buffer = malloc(8 * 2048);
        int16_t _x,_y;
        uint8_t * _p;
        uint8_t _buf;
        //char dummy[16];

        int iKeycode;
        

        //menu loop here
        bool bChosen = false;
        while (false == bChosen)

        {

                //draw menu lines
                _svin_set_cycle_patterns_cpu();
        
                for (int iLine = 0; iLine < _svin_menu_items_count; iLine++)
                {
                        if (strlen(_svin_menu_items[iLine].line)>0)
                        {
                                // Render the speaker name 
                                //sprintf(dummy,"%i",_svin_menu_items[iLine].jump);
                                height = _svin_text_render(buffer,320,_svin_menu_items[iLine].line,"Lato_Black12");
                                
                                //copy menu line
                                _p = (uint8_t *)(_SVIN_NBG2_CHPNDR_TEXTBOX_ADDR);
                                for (int cellX = 0; cellX < 40; cellX++)
                                {
                                        for (int cellY = 0; cellY < 2; cellY++)
                                        {
                                                for (int x=0;x<8;x++)
                                                {
                                                        for (int y=0;y<8;y++)
                                                        {
                                                                _y = (cellY)*8+y+2;
                                                                if ( (_y >= 0) && (_y < height) )
                                                                {
                                                                        _x = cellX*8+x-8;
                                                                        if ( (_x >= 0) && (_x < 320) )
                                                                        {
                                                                                _buf = buffer[_y*320+_x];
                                                                                if (_buf!=0xFF) 
                                                                                {
                                                                                        if (SelectorPosition == iLine)
                                                                                                _buf = 7*16 + _buf/16;
                                                                                        else
                                                                                                _buf = 14*16 + _buf/16;
                                                                                        if (0==_buf)
                                                                                                _buf = 1;//0 is a transparency color, using close value
                                                                                        _p[((cellY+iLine*2) * 40 + cellX)*_SVIN_CHARACTER_BYTES + y*8 + x] = _buf;
                                                                                }
                                                                        }
                                                                }

                                                        }
                                                }
                                        }
                                }
                        }
                }
                
                _svin_set_cycle_patterns_nbg();

                //menu drawn, check keyboard
                
                //wait for keypress
                iKeycode = _svin_wait_for_key_press_and_release();

                if (PERIPHERAL_DIGITAL_UP & iKeycode)
                        SelectorPosition--;
                else if  (PERIPHERAL_ANALOG_UP & iKeycode)
                        SelectorPosition--;
                else if (PERIPHERAL_DIGITAL_DOWN & iKeycode)
                        SelectorPosition++;
                else if  (PERIPHERAL_ANALOG_DOWN & iKeycode)
                        SelectorPosition++;
                else if (PERIPHERAL_DIGITAL_A & iKeycode)
                        bChosen = true;
                else if  (PERIPHERAL_ANALOG_A & iKeycode)
                        bChosen = true;
                else if (PERIPHERAL_DIGITAL_C & iKeycode)
                        bChosen = true;
                else if  (PERIPHERAL_ANALOG_C & iKeycode)
                        bChosen = true;

                if (SelectorPosition<0)
                        SelectorPosition = _svin_menu_items_count-1;

                if (SelectorPosition>=_svin_menu_items_count)
                        SelectorPosition =0;


        }

        free (buffer);

        int iJump = _svin_menu_items[SelectorPosition].jump;

        //clear NBG2 names
        //menu is 40x20 tiles, to fit into a single plane, and into the same tilespace as dialog 
        for (int y = iStartCell; y < iStartCell + _svin_menu_items_count*2; y++)
        {
                //plane 0 only
                iOffset = y * 64;
                for (int x = 24; x < 64; x++)
                {
                _pointer32[iOffset + x] = 0x10000000 + _SVIN_NBG2_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
                }
        }

        _svin_menu_depopulate();

        assert (iJump >= 0);
        assert (iJump < 512);

        return MenuLinks[iJump];//it's an index from .MNU array, should get it here
}

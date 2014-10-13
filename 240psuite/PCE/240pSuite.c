#include "huc.h"

#include "res.h"

setupFont()
{	
    load_font(my_font, 96);
    set_font_pal(14);        
    set_color_rgb(225, 255, 255, 255);
    set_color_rgb(226, 0, 0, 0);
    set_color_rgb(241, 255, 0, 0);
    set_color_rgb(242, 0, 0, 0);
}

main()
{
    int OldButtonsInternal;
    int controller;   
    int read; 
    int redraw;
    int sel;

    /* 0428 to HDR */
    vreg(11, 0x0428);

    init_satb();
    satb_update();

    load_background(MB_bg, MB_pal, MB_bat, 32, 28);    
    setupFont();

    redraw = 1;
    sel = 0;
    OldButtonsInternal = 0;

    for (;;)
    {   
        if(redraw)
        {
            int row;

            row = 8;
            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Test Patterns", 5, row++);
            set_font_pal(sel == 1 ? 15 : 14);
            put_string("Drop Shadow Test", 5, row++);
            set_font_pal(sel == 2 ? 15 : 14);
            put_string("Striped Sprite Test", 5, row++);
            set_font_pal(sel == 3 ? 15 : 14);
            put_string("Lag Test", 5, row++);
            set_font_pal(sel == 4 ? 15 : 14);
            put_string("Manual Lag Test", 5, row++);
            set_font_pal(sel == 5 ? 15 : 14);
            put_string("Scroll Test", 5, row++);
            set_font_pal(sel == 6 ? 15 : 14);
            put_string("Grid Scroll Test", 5, row++);
            set_font_pal(sel == 7 ? 15 : 14);
            put_string("Horizontal Stripes", 5, row++);
            set_font_pal(sel == 8 ? 15 : 14);
            put_string("Checkerboard", 5, row++);
            set_font_pal(sel == 9 ? 15 : 14);
            put_string("Backlit Zone Test", 5, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("Sound Test", 5, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("Help", 5, row++);

            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 11)
                sel = 0;
            redraw = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 11;
            redraw = 1;
        }

        vsync();
    }
}

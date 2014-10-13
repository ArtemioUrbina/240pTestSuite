/*
            DEFINES
*/

#define FONT            "graphics/font.pcx"
#define MAIN_BACKGROUND "graphics/mainbg.pcx"

/*
            FONT
*/

#incchr(my_font, FONT, 32, 3);

/*
            GRAPHICS
*/

#incchr(MB_bg,MAIN_BACKGROUND,32,28);
#incpal(MB_pal,MAIN_BACKGROUND);
#incbat(MB_bat,MAIN_BACKGROUND,0x1000,32,28);

#include <stdint.h>

int ire_level_0 = 0;
int ire_level_100 = 255;

int dirty_round(double x)
{
    if (x < 0.0)
        return (int)(x - 0.5);
    else
        return (int)(x + 0.5);
}

uint8_t Get_IRE_Level(double level)
{
    return ire_level_0 + dirty_round(((ire_level_100-ire_level_0)*level)/100);
}
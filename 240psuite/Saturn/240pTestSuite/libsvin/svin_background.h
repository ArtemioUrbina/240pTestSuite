#ifndef _SVIN_BACKGROUND_H_
#define _SVIN_BACKGROUND_H_

#include <yaul.h>
#include <svin.h>
#include <svin_filelist.h>

void _svin_background_fade_to_black_step();
void _svin_background_fade_to_black();
void _svin_background_set_no_filelist(char *name);
void _svin_background_set_by_fad(fad_t fad, int size);
void _svin_background_fade_to_black();

#endif

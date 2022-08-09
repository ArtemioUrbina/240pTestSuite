#ifndef _SVIN_BACKGROUND_H_
#define _SVIN_BACKGROUND_H_

#include <yaul.h>
#include <svin.h>

void _svin_background_fade_to_black_step();
void _svin_background_fade_to_black();
void _svin_background_set_by_index(int index);
void _svin_background_set(char *name);
void _svin_background_set_no_filelist(char *name);
void _svin_background_set_by_fad(fad_t fad, int size);
void _svin_background_update_by_index(int index);
void _svin_background_update(char *name);
void _svin_background_clear();
void _svin_background_fade_to_black();
//void _svin_background_load_index(char * filename);

#endif

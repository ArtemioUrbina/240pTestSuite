#include <libdragon.h>

extern int current_resolution;
extern int current_bitdepth;
extern int current_buffers;
extern int current_gamma;
extern int current_antialias;
extern display_context_t disp;

void GetDisplay();
int isNTSC();
void set_video();
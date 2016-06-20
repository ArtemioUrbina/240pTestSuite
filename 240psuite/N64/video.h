#include <libdragon.h>

extern int current_resolution;
extern int current_bitdepth;
extern int current_buffers;
extern int current_gamma;
extern int current_antialias;
extern display_context_t disp;

extern int dW;
extern int dH;

void GetDisplay();
int isNTSC();
void init_video();
void set_video();
void WaitVsync();
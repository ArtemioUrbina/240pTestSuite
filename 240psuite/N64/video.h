#include <libdragon.h>

extern int current_resolution;
extern int current_bitdepth;
extern int current_buffers;
extern int current_gamma;
extern int current_antialias;

extern display_context_t __dc;

extern int dW;
extern int dH;

extern void *__safe_buffer[];
extern unsigned char *__screen_buffer;

void GetDisplay();
int isNTSC();
void init_video();
void set_video();
void WaitVsync();

int getBitDepthSize();

void FreeScreenBuffer();
void CreateScreenBuffer();
void CopyScreen();
void FillScreenFromBuffer();
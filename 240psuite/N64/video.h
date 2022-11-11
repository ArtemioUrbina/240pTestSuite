#include <libdragon.h>

extern int current_resolution;
extern int current_bitdepth;
extern int current_buffers;
extern int current_gamma;
extern int current_antialias;

extern volatile display_context_t __dc;

extern int dW;
extern int dH;
extern int bD;

extern int EnablePAL;
extern int EnableDivot;
extern int EnableDither;
extern int useNTSC;

extern void *__safe_buffer[];
extern unsigned char *__screen_buffer;

void GetDisplay();
int isNTSC();
void init_video();
void set_video();
void WaitVsync();

void FreeScreenBuffer();
void CreateScreenBuffer();
void CopyScreen();
void FillScreenFromBuffer();
void DarkenScreenBuffer(int amount);
void GetVideoModeStr(char *res, int shortdesc);
int GetFrameCount();
void reset_video();
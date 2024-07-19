#ifndef VIDEO_H
#define VIDEO_H

#define N64_FRAME_LEN 16.715f

extern surface_t* __disp;

extern unsigned int dW;
extern unsigned int dH;

void getDisplay();
void waitVsync();
void vblCallback();
void resetVideo();

#endif
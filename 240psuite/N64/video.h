#ifndef VIDEO_H
#define VIDEO_H

extern volatile display_context_t __dc;

extern unsigned int dW;
extern unsigned int dH;

void getDisplay();
void waitVsync();
void vblCallback();
void resetVideo();

#endif
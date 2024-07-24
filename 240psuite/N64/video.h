#ifndef VIDEO_H
#define VIDEO_H

// Measured with scope and get_ticks_us() 
#define N64_NTSC_FRAME_LEN 16.715f

#define SUITE_320x240	0
#define SUITE_640x480	1
#define SUITE_512x240	2
#define SUITE_512x480	3
#define SUITE_256x240	4
#define SUITE_NONE		-1

extern surface_t* __disp;

extern resolution_t		current_resolution;
extern unsigned int		current_bitdepth;
extern unsigned int		current_buffers;
extern unsigned int		current_gamma;
extern filter_options_t	current_antialias;
extern unsigned int		EnablePAL;
extern unsigned int		vMode;

extern int useNTSC;

extern unsigned int dW;
extern unsigned int dH;

void getDisplay();
void waitVsync();
void vblCallback();

void initVideo();
void setVideo(resolution_t newRes);

bool isSameRes(resolution_t *res1, const resolution_t *res2);
int videoModeToInt(resolution_t *res);

#ifdef DEBUG_BENCHMARK
#define	IDLE_WARN_MS	2.0f
void resetIdle();
#endif

#endif
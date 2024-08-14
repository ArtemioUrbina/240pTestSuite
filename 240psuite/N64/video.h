#ifndef VIDEO_H
#define VIDEO_H

// Measured with scope and get_ticks_us() 
#define N64_NTSC_FRAME_LEN 16.715f

#define SUITE_320x240	0
#define SUITE_640x480	1
#define SUITE_256x240	3
#define SUITE_512x480	4
#define SUITE_512x240	5

#define SUITE_NONE		-1

extern surface_t* __disp;

extern resolution_t		current_resolution;
extern bitdepth_t		current_bitdepth;
extern unsigned int		current_buffers;
extern unsigned int		current_gamma;
extern filter_options_t	current_antialias;
extern rdpq_antialias_t current_rdp_aa_filter;
extern unsigned int		enablePAL;
extern unsigned int		vMode;

void getDisplay();
void waitVsync();
void waitVsyncWithAudio();
void drawNoVsyncWithAudio();

int getHardWidth();
int getHardHeight();

void initVideo();
void changeVMode(resolution_t newRes);

int isSameRes(resolution_t *res1, const resolution_t *res2);
int videoModeToInt(resolution_t *res);
void getVideoModeStr(char *res, int shortdesc);
void changeToH256onVBlank();
void changeToH320onVBlank();
void changeBitDepthOnVBlank(int use32bits);
int isVMode256();
int isVMode480();

void setVideoInternal(resolution_t newRes);

#ifdef DEBUG_BENCHMARK
#define	IDLE_WARN_MS	2.0f
void resetIdle();
#endif

#endif
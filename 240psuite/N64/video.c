#include "video.h"

int current_resolution = RESOLUTION_320x240;
int current_bitdepth = DEPTH_32_BPP;
int current_buffers = 2;
int current_gamma = GAMMA_NONE;
int current_antialias = ANTIALIAS_OFF;
display_context_t disp = 0;

void set_video()
{
	display_init(current_resolution, current_bitdepth, current_buffers, current_gamma, current_antialias);
}

void GetDisplay()
{
	while( !(disp = display_lock()) );
}

int isNTSC()
{
	int tv; 
	
	tv = *(int *) 0x80000300;
	return tv;
}
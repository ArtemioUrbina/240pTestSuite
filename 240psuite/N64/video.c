#include "video.h"

int current_resolution = 0;
int current_bitdepth = 0;
int current_buffers = 0;
int current_gamma = 0;
int current_antialias = 0;

display_context_t disp = 0;
int dW = 0;
int dH = 0;

int video_set = 0;

void init_video()
{
	current_resolution = RESOLUTION_320x240;
	current_bitdepth = DEPTH_32_BPP;
	current_buffers = 2;
	current_gamma = GAMMA_NONE;
	current_antialias = ANTIALIAS_OFF;

	video_set = 0;
}

void set_video()
{
	if(video_set)
	{
		rdp_close();
		display_close();
		video_set = 0;
	}
	
	rdp_init();
	display_init(current_resolution, current_bitdepth, current_buffers, current_gamma, current_antialias);
	
	switch(current_resolution)
	{
		case RESOLUTION_320x240:
			dW = 320;
			dH = 240;
			break;
		case RESOLUTION_640x480:
			dW = 640;
			dH = 480;
			break;
		case RESOLUTION_256x240:
			dW = 256;
			dH = 240;
			break;
		case RESOLUTION_512x480:
			dW = 512;
			dH = 480;
			break;
	}
	video_set = 1;
}

void GetDisplay()
{
	if(disp)
		disp = 0;
		
	while(!(disp = display_lock()));
}

int isNTSC()
{
	int tv; 
	
	tv = *(int *) 0x80000300;
	return tv;
}

void WaitVsync()
{
	if(disp)
		display_show(disp);
}
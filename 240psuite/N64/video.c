#include "video.h"
#include "utils.h"

int current_resolution = 0;
int current_bitdepth = 0;
int current_buffers = 0;
int current_gamma = 0;
int current_antialias = 0;

display_context_t __dc = 0;
int dW = 0;
int dH = 0;

int video_set = 0;

unsigned char *__screen_buffer = NULL;

void init_video()
{
	current_resolution = RESOLUTION_320x240;
	current_bitdepth = DEPTH_16_BPP;
	current_buffers = 2;
	current_gamma = GAMMA_NONE;
	current_antialias = ANTIALIAS_RESAMPLE; // ANTIALIAS_OFF doesn't work in 16BPP
	__screen_buffer = NULL;
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
	
	display_init(current_resolution, current_bitdepth, current_buffers, current_gamma, current_antialias);
	rdp_init();
	
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
	
	CreateScreenBuffer();
	video_set = 1;
}

int getBitDepthSize()
{
	int size = 0;
	
	if(current_bitdepth == DEPTH_16_BPP)
		size = 2;
		
	if(current_bitdepth == DEPTH_32_BPP)
		size = 4;
		
	return size;
}

void GetDisplay()
{	
	while(!(__dc = display_lock()));
}

int isNTSC()
{
	int tv; 
	
	tv = *(int *) 0x80000300;
	return tv;
}

void WaitVsync()
{
	display_show(__dc);
}

void CreateScreenBuffer()
{	
	FreeScreenBuffer();
		
#ifdef USE_N64MEM		
	__screen_buffer = n64_malloc(dW*dH*getBitDepthSize());
#else
	__screen_buffer = malloc(dW*dH*getBitDepthSize());
#endif
}

void FreeScreenBuffer()
{
	if(__screen_buffer)
	{
#ifdef USE_N64MEM	
		n64_free(__screen_buffer);
#else
		free(__screen_buffer);
#endif
		__screen_buffer = NULL;
	}
}


void CopyScreen()
{	
	if(!__screen_buffer || !__dc)
		return;

#ifdef USE_N64MEM
    n64_memcpy(__screen_buffer, __safe_buffer[(__dc)-1], dW*dH*getBitDepthSize());
#else
	memcpy(__screen_buffer, __safe_buffer[(__dc)-1], dW*dH*getBitDepthSize());
#endif
}

void FillScreenFromBuffer()
{
	if(!__screen_buffer || !__dc)
		return;

#ifdef USE_N64MEM
    n64_memcpy(__safe_buffer[(__dc)-1], __screen_buffer, dW*dH*getBitDepthSize());
#else
	memcpy(__safe_buffer[(__dc)-1], __screen_buffer, dW*dH*getBitDepthSize());
#endif
}



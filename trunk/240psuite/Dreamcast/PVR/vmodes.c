#include <kos.h>
#include "vmodes.h"
#include "menu.h"

int vmode 	= -1;
int vcable	= CT_RGB;
int W		= 0;
int H		= 0;
int dW		= 0;
int dH		= 0;

/* 640x480 VGA 60Hz */
/* DM_640x480_VGA */
vid_mode_t custom_vga =
{
	DM_640x480,
	640, 480,
	VID_INTERLACE,
	CT_VGA,
	PM_RGB565,
	526, 855,
	175, 36,
	21, 260,
	135, 855,
	36, 516,
	0, 1,
	{ 0, 0, 0, 0 }
};

ImagePtr   scanlines = NULL;

void LoadScanlines()
{
	if(!scanlines && vmode == VIDEO_480P_SL)
	{
		scanlines = LoadKMG("/rd/scanlines.kmg.gz", 0);
		scanlines->layer = 5.0;
		scanlines->alpha = 0.5f;
		scanlines->scale = 0;
		CalculateUV(0, 0, 640, 480, scanlines);
	}
}

void ToggleScanlineEvenOdd()
{
	if(scanlines)
	{
		if(scanlines->y == 0)
			scanlines->y = -1;
		else
			scanlines->y = 0;
	}
}

void RaiseScanlineIntensity()
{
	if(scanlines)
	{
		scanlines->alpha += 0.05f;
		if(scanlines->alpha > 1.0f)
			scanlines->alpha = 1.0f;
	}
}

void LowerScanlineIntensity()
{
	if(scanlines)
	{
		scanlines->alpha -= 0.05f;
		if(scanlines->alpha < 0.0f)
			scanlines->alpha = 0.0f;
	}
}

int ScanlinesEven()
{
	if(scanlines)
		return(scanlines->y == 0);
	return 0;
}

double GetScanlineIntensity()
{
	if(scanlines)
		return((double)scanlines->alpha*100);
	else
		return 0;
}

inline void DrawScanlines()
{
	if(vmode == VIDEO_480P_SL && scanlines)
		DrawImage(scanlines);
}

inline void ReleaseScanlines()
{
	FreeImage(&scanlines);
}

void ChangeResolution(int nvmode)
{
	int lastw;

	lastw = W;

	// Skip useless video modes when in VGA
	vcable = vid_check_cable();
	if(vcable == CT_VGA && nvmode == VIDEO_240P)
		nvmode = VIDEO_480P;

	if(nvmode < VIDEO_240P)
		nvmode = VIDEO_240P;
	if(nvmode > VIDEO_480P)
		nvmode = VIDEO_480P_SL;
	if(nvmode > VIDEO_480P)
	{
		if(vcable != CT_VGA)
			nvmode = VIDEO_240P;
	}
	if(vmode == nvmode)
		return;

	vmode = nvmode;

	switch(vmode)
	{
		case VIDEO_240P:
			W = 320;
			H = 240;
			dW = 320;
			dH = 240;
			break;
		case VIDEO_480I_A240:
		case VIDEO_480P_SL:
			W = 640;
			H = 480;
			dW = 320;
			dH = 240;
			break;
		case VIDEO_480I:
		case VIDEO_480P:
			W = 640;
			H = 480;
			dW = 640;
			dH = 480;
			break;
	}

	if(lastw != W)
	{
		ReleaseTextures();

		pvr_shutdown();
		switch(vmode)
		{
			case VIDEO_240P:
						vid_set_mode(DM_320x240_NTSC, PM_RGB565); 
				break;
			case VIDEO_480I_A240:
			case VIDEO_480P_SL:
			case VIDEO_480I:
			case VIDEO_480P:
					if(vcable == CT_VGA)
						vid_set_mode(DM_640x480_VGA, PM_RGB565); 
						//vid_set_mode_ex(&custom_vga);
					else
						vid_set_mode(DM_640x480_NTSC_IL, PM_RGB565); 
				break;
		}

		pvr_init_defaults();

		RefreshLoadedImages();

		// Disable deflicker filter, 
		if(PVR_GET(PVR_SCALER_CFG) != 0x400)
		{
			dbglog(DBG_KDEBUG, "Disabling pvr deflicker filter for 240p tests\n");
			PVR_SET(PVR_SCALER_CFG, 0x400);
		}

		// Turn off texture dithering
		if(PVR_GET(PVR_FB_CFG_2) != 0x00000001)
		{
			dbglog(DBG_KDEBUG, "Disabling pvr dithering for 240p tests\n");
			PVR_SET(PVR_FB_CFG_2, 0x00000001);
		}

		if(!settings.drawborder) // Draw back video signal limits?
			vid_border_color(0, 0, 0);
		else
			vid_border_color(255, 255, 255);
	
		if(!settings.drawpvrbg)
			pvr_set_bg_color(0.0f, 0.0f, 0.0f);
		else
			pvr_set_bg_color(0.0f, 1.0f, 0.0f);
		
	}
}

void Toggle240p480i(int mode)
{
	// Skip useless video modes when in VGA
	if(vcable == CT_VGA)
		return;

	ReleaseTextures();
	pvr_shutdown();

	if(mode == 0)
	{
		ReleaseTextures();

		W = 320;
		H = 240;
		dW = 320;
		dH = 240;
		vmode = VIDEO_240P;

		vid_set_mode(DM_320x240_NTSC, PM_RGB565); 

	}
	else
	{

		W = 640;
		H = 480;
		dW = 320;
		dH = 240;
		vmode = VIDEO_480I_A240;

		vid_set_mode(DM_640x480_NTSC_IL, PM_RGB565); 
	}

	pvr_init_defaults();

	RefreshLoadedImages();

	// Disable deflicker filter, 
	if(PVR_GET(PVR_SCALER_CFG) != 0x400)
	{
		dbglog(DBG_KDEBUG, "Disabling pvr deflicker filter for 240p tests\n");
		PVR_SET(PVR_SCALER_CFG, 0x400);
	}

	// Turn off texture dithering
	if(PVR_GET(PVR_FB_CFG_2) != 0x00000001)
	{
		dbglog(DBG_KDEBUG, "Disabling pvr dithering for 240p tests\n");
		PVR_SET(PVR_FB_CFG_2, 0x00000001);
	}
}

void PVRStats(char *msg)
{
	pvr_stats_t stats;

	pvr_get_stats(&stats);
	if(msg)
		printf("%s: VBlank count %u  Frame last time: %u  Frame Rate: %f  Rendering Time: %u\n",
			msg, (unsigned int)stats.vbl_count, (unsigned int)stats.frame_last_time, (double)stats.frame_rate, (unsigned int)stats.rnd_last_time);
	else
		printf("VBlank count %u  Frame last time: %u  Frame Rate: %f  Rendering Time: %u\n",
			(unsigned int)stats.vbl_count, (unsigned int)stats.frame_last_time, (double)stats.frame_rate, (unsigned int)stats.rnd_last_time);
}


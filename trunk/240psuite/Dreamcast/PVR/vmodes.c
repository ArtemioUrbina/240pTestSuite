#include <kos.h>
#include "vmodes.h"

int vmode 	= NATIVE_320;
int vcable	= CT_RGB;
int W				= 320;
int H				= 240;
int dW			= 320;
int dH			= 240;

ImagePtr   scanlines = NULL;

void LoadScanlines()
{
	if(!scanlines && vmode == FAKE_640_SL)
	{
		scanlines = LoadKMG("/rd/scanlines.kmg.gz", 0);
		scanlines->layer = 5.0;
		scanlines->alpha = 0.7f;
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
	if(vmode == FAKE_640_SL && scanlines)
		DrawImage(scanlines);
}

inline void ReleaseScanlines()
{
	FreeImage(&scanlines);
}

void ChangeResolution()
{
	int lastw;

	lastw = W;
	// Skip useless video modes when in VGA
	if(vcable == CT_VGA && vmode == NATIVE_320)
		vmode = NATIVE_640;
	vmode ++;

	if(vmode > FAKE_640_SL)
		vmode = NATIVE_320;

	if(vmode > NATIVE_640_FS)
	{
		if(vcable != CT_VGA)
			vmode = NATIVE_320;
	}

	switch(vmode)
	{
		case NATIVE_320:
			W = 320;
			H = 240;
			dW = 320;
			dH = 240;
			break;
		case NATIVE_640:
		case FAKE_640:
		case FAKE_640_SL:
			W = 640;
			H = 480;
			dW = 320;
			dH = 240;
			break;
		case NATIVE_640_FS:
			W = 640;
			H = 480;
			dW = 640;
			dH = 480;
			break;
	}

	if(lastw != W)
	{
		pvr_shutdown();
		switch(vmode)
		{
			case NATIVE_320:
						vid_set_mode(DM_320x240_NTSC, PM_RGB565); 
				break;
			case FAKE_640:
			case FAKE_640_SL:
			case NATIVE_640:
			case NATIVE_640_FS:
						vid_set_mode(DM_640x480_NTSC_IL, PM_RGB565); 
				break;
		}

		pvr_init_defaults();

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

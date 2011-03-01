#include <kos.h>
#include "vmodes.h"

int vmode 	= NATIVE_320;
int region	= FLASHROM_REGION_US;
int vcable	= CT_RGB;
int W				= 320;
int H				= 240;
int dW			= 320;
int dH			= 240;

void InitVideo()
{
	// PM_RGB555 PM_RGB565 PM_RGB888
	vcable = vid_check_cable();
	region = flashrom_get_region();

	vid_set_mode(DM_320x240_NTSC, PM_RGB565); 
	pvr_init_defaults();

 	ChangePVRDefaults();

	vid_border_color(0, 0, 0);
	pvr_set_bg_color(0.0f, 0.0f, 0.0f);  
}

void ChangePVRDefaults()
{
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

void ChangeResolution()
{
	int lastw;

	lastw = W;
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
		case FAKE_640:
		case FAKE_640_SL:
		case NATIVE_640:
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

		ChangePVRDefaults();
	}
}





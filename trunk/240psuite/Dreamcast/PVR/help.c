#include <kos.h>
#include <stdio.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"

#define LINESPERPAGE	15

extern ImagePtr    scanlines;

uint16	showhelp = 0;

static inline void DrawScanlines()
{
	if(vmode == FAKE_640_SL && scanlines)
		DrawImage(scanlines);
}

char *LoadHelpFile(char *filename, char ***pages, int *npages)
{
	int		i = 0, lines = 0, linecount = 0, currpage = 0;
	long 	size = 0;
	FILE 	*fp = NULL;
	char 	*buffer = NULL;

	fp = fopen(filename, "r");
	if(!fp)
	{
		fprintf(stderr, "Could not load %s help file\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp)+1;
	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(fp);
		fprintf(stderr, "Could not load %s help file to RAM\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_SET);
	memset(buffer, 0x0, sizeof(char)*size);
	fread(buffer, sizeof(char), size-1, fp);
	fclose(fp);

	for(i = 0; i < size; i++)
		if(buffer[i] == '\n')
			lines ++;

	*npages = (lines / LINESPERPAGE);
	if(lines % LINESPERPAGE && lines > LINESPERPAGE)
		(*npages) ++;
	if(*npages < 1)
		*npages = 1;

	*pages = (char**)malloc(sizeof(char*)*(*npages));
	if(!*pages)
	{
		fprintf(stderr, "Could not malloc array of pages %s\n", filename);
		free(buffer);
		buffer = NULL;
		return NULL;
	}

	*pages[0] = buffer;
	currpage = 1;
	if(*npages > 1)
	{
		for(i = 0; i < size; i++)
		{
			if(buffer[i] == '\n')
			{
				linecount ++;
				if(linecount == LINESPERPAGE)
				{
					linecount = 0;
					buffer[i] = 0x0;
					if(currpage < *npages)
						(*pages)[currpage++] = buffer+i+1;
				}
			}
		}
	}

	return buffer;
}

uint16 HelpWindow(char *filename, ImagePtr screen, uint16 main)
{
	int         done = 0, npages = 0, page = 0;
	uint16      oldbuttons = 0xffff, pressed;    
	ImagePtr    back;
	char				*buffer = NULL, **pages = NULL;

	if(!showhelp && !main)
		return 1;

	buffer = LoadHelpFile(filename, &pages, &npages);
	if(!buffer)
		return 1;

	back = LoadImage("/rd/help.png", 1);
	back->alpha = 0.75f;
    
	updateVMU("   Help  ", "", 1);
	if(main)
		showhelp = 1;
	while(!done) 
	{
		pvr_wait_ready();

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			    
			if (pressed & CONT_START)
				done =  1;                
			if (pressed & CONT_DPAD_LEFT)
				page --;
			if (pressed & CONT_DPAD_RIGHT)
				page ++;
			if (pressed & CONT_B)
				done =  2;                
			if (pressed & CONT_Y)
				showhelp =  !showhelp;                

		MAPLE_FOREACH_END()

		if(page > npages - 1)
			page = npages - 1;

		if(page < 0)
			page = 0;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		if(screen)
			DrawImage(screen);
		DrawImage(back);
		DrawStringS(34, 42, 1.0f, 1.0f, 1.0f, pages[page]); 

		if(main)
			DrawStringS(80, 200, 0.9f, 0.9f, 0.9f, "Press START to return to Menu"); 
		else
			DrawStringS(30, 200, 0.9f, 0.9f, 0.9f, "Press START to return to Menu, or B to close help"); 
		DrawStringS(170, 190, 0.7f, 0.7f, 0.7f, "Show help (Press Y):"); 

		if(showhelp)
			DrawStringS(270, 190, 0.0f, 0.7f, 0.7f, "Yes"); 
		else
			DrawStringS(270, 190, 0.0f, 0.7f, 0.7f, "No"); 
		DrawScanlines();
		pvr_list_finish();        

		pvr_scene_finish();
	}
	if(buffer)
		free(buffer);
	if(pages)
		free(pages);
	FreeImage(&back);
	if(done == 2)
		return 0;
	return 1;
}



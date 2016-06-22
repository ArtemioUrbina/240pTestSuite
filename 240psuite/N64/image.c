#include "image.h"
#include "utils.h"

sprite_t *LoadImage(char *name)
{
	int fp = 0;
	sprite_t *image = NULL;

	fp = dfs_open(name);
	if(fp == DFS_ESUCCESS)
		return NULL;
#ifdef USE_N64MEM
    image = n64_malloc(dfs_size( fp ));
#else
	image = malloc(dfs_size( fp ));
#endif
	if(!image)
		return NULL;
    if(dfs_read(image, 1, dfs_size(fp), fp) < 0)
	{
#ifdef USE_N64MEM
		n64_free(image);
#else
		free(image);
#endif
		return NULL;
	}
    dfs_close(fp);
	return image;
}

void FreeImage(sprite_t **image)
{
	if(*image)
	{
#ifdef USE_N64MEM
		n64_free(*image);
#else
		free(*image);
#endif
		*image = NULL;
	}
}

void SoftDrawImage(int x, int y, sprite_t *image)
{
	if(!image)
		return;
		
	graphics_draw_sprite_trans(__dc, x, y, image);
}

void SoftDrawImageSolid(int x, int y, sprite_t *image)
{
	if(!image)
		return;
		
	graphics_draw_sprite(__dc, x, y, image);
}

void rdp_start()
{
	rdp_sync(SYNC_PIPE);
	rdp_set_default_clipping();
	rdp_enable_texture_copy();
	rdp_attach_display(__dc);
}

void HardDrawImage(int x, int y, sprite_t *image)
{	
	if(!image)
		return;
		
	if(getBitDepthSize() == 2 && image->width < 20 && image->height < 20)
	{
		rdp_sync(SYNC_PIPE);
		rdp_load_texture(0, 0, MIRROR_DISABLED, image);
		rdp_draw_sprite(0, x, y);
	}
	else
		SoftDrawImage(x, y, image);
}

void rdp_end()
{
	rdp_detach_display();
}

void FillScreenWithTexture(sprite_t *image)
{
	rdp_sync(SYNC_PIPE);
	rdp_load_texture(0, 0, MIRROR_DISABLED, image);
	rdp_draw_textured_rectangle (0, 0, 0, dW, dH);
}

void drawPatchBackgroundFromCapture(int x, int y, sprite_t *sprite) 
{ 
    unsigned char*	target, *src; 
	int 	size = 0, width = 0, height = 0;
	
	if(!sprite || !__screen_buffer)
		return;
		
	width = sprite->width+2;
	height = sprite->height+2;
	
	x = x - 1;
	y = y - 1;
	
	if(x < 0)
		x = 0;
	if(x+width > dW)
		x = dW - width;
	if(y < 0)
		y = 0;
	if(y+height > dH)
		y = dH - height;
 
	size = getBitDepthSize();
		
	src = __screen_buffer + y*dW*size + x*size; 
    target = __safe_buffer[(__dc)-1] + y*dW*size + x*size; 

    while (height--) 
    { 
#ifdef USE_N64MEM
		n64_memcpy(target, src, width*size); 
#else
		memcpy(target, src, width*size); 
#endif
		target += dW*size; 
		src += dW*size; 
    } 
} 

void drawPatchBackground(int x, int y, sprite_t *sprite, sprite_t *backgd) 
{ 
    unsigned char*	target, *src; 
	int 	size = 0, width = 0, height = 0;
	
	if(!sprite || !backgd)
		return;
		
	width = sprite->width+2;
	height = sprite->height+2;
	
	x = x - 1;
	y = y - 1;
	
	if(x < 0)
		x = 0;
	if(x+width > dW)
		x = dW - width;
	if(y < 0)
		y = 0;
	if(y+height > dH)
		y = dH - height;
 
	size = getBitDepthSize();
		
	src = (unsigned char*)backgd->data + y*backgd->width*size + x*size;
    target = __safe_buffer[(__dc)-1] + y*dW*size + x*size; 

    while (height--) 
    { 
#ifdef USE_N64MEM
		n64_memcpy(target, src, width*size); 
#else
		memcpy(target, src, width*size); 
#endif
		target += dW*size; 
		src += backgd->width*size; 
    } 
} 

void drawBackground(sprite_t *backgd) 
{ 
    unsigned char*	target, *src; 
	int 	size = 0, width = 0, height = 0;
	
	if(!backgd)
		return;
		
	width = backgd->width;
	height = backgd->height;
	
	if(width > dW)
		width = dW;
	if(height > dH)
		height = dH;
	
	size = getBitDepthSize();
		
	src = (unsigned char*)backgd->data;
    target = __safe_buffer[(__dc)-1]; 

    while (height--) 
    { 
#ifdef USE_N64MEM
		n64_memcpy(target, src, width*size); 
#else
		memcpy(target, src, width*size); 
#endif
		target += dW*size; 
		src += backgd->width*size; 
    } 
}

void FillScreen(int r, int g, int b)
{
	for(int i = 0; i < current_buffers; i++)
	{
		GetDisplay();
		
		rdp_sync(SYNC_PIPE);
		rdp_set_default_clipping();
		rdp_enable_primitive_fill();
		rdp_attach_display(__dc);
		rdp_sync(SYNC_PIPE);
		rdp_set_primitive_color(graphics_make_color(r, g, b, 0xff));
		rdp_draw_filled_rectangle(0, 0, dW, dH);
		rdp_detach_display();
		
		WaitVsync();
	}
}

void ClearScreen()
{
	FillScreen(0, 0, 0);
}

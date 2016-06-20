#include "image.h"

sprite_t *LoadImage(char *name)
{
	int fp = 0;
	sprite_t *image = NULL;

	fp = dfs_open(name);
	if(fp == DFS_ESUCCESS)
		return NULL;
    image = malloc(dfs_size( fp ));
	if(!image)
		return NULL;
    if(dfs_read(image, 1, dfs_size(fp), fp) < 0)
	{
		free(image);
		return NULL;
	}
    dfs_close(fp);
	return image;
}

void FreeImage(sprite_t **image)
{
	if(*image)
	{
		free(*image);
		*image = NULL;
	}
}

void SoftDrawImage(int x, int y, sprite_t *image)
{
	graphics_draw_sprite_trans(disp, x, y, image);
}

void rdp_start()
{
	rdp_sync(SYNC_PIPE);
	rdp_set_default_clipping();
	rdp_enable_texture_copy();
	rdp_attach_display(disp);
}

void HardDrawImage(int x, int y, sprite_t *image)
{	
	rdp_sync(SYNC_PIPE);
	rdp_load_texture(0, 0, MIRROR_DISABLED, image);
	rdp_draw_sprite(0, x, y);
}

void rdp_end()
{
	rdp_detach_display();
}

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>
#include <malloc.h>
#include <ogc/tpl.h>


#define TPL_FILE_TYPE_DISC			0
#define TPL_FILE_TYPE_MEM			1

#define TPL_HDR_VERSION_FIELD		0
#define TPL_HDR_NTEXTURE_FIELD		4
#define TPL_HDR_HDRSIZE_FIELD		8
#define TPL_HDR_DESCR_FIELD		   12

// texture header
typedef struct _tplimgheader TPLImgHeader;

struct _tplimgheader {
	u16 height;
	u16 width;
	u32 fmt;
	void *data;
	u32 wraps;
	u32 wrapt;
	u32 minfilter;
	u32 magfilter;
	f32 lodbias;
	u8 edgelod;
	u8 minlod;
	u8 maxlod;
	u8 unpacked;
} ATTRIBUTE_PACKED;

// texture palette header
typedef struct _tplpalheader TPLPalHeader;

struct _tplpalheader {
	u16 nitems;
	u8 unpacked;
	u8 pad;
	u32 fmt;
	void *data;
} ATTRIBUTE_PACKED;

// texture descriptor
typedef struct _tpldesc TPLDescHeader;

struct _tpldesc {
	TPLImgHeader *imghead;
	TPLPalHeader *palhead;
} ATTRIBUTE_PACKED;

static u32 TPL_GetTextureSize(u32 width,u32 height,u32 fmt)
{
	u32 size = 0;

	switch(fmt) {
			case GX_TF_I4:
			case GX_TF_CI4:
			case GX_TF_CMPR:
				size = ((width+7)>>3)*((height+7)>>3)*32;
				break;
			case GX_TF_I8:
			case GX_TF_IA4:
			case GX_TF_CI8:
				size = ((width+7)>>3)*((height+7)>>2)*32;
				break;
			case GX_TF_IA8:
			case GX_TF_CI14:
			case GX_TF_RGB565:
			case GX_TF_RGB5A3:
				size = ((width+3)>>2)*((height+3)>>2)*32;
				break;
			case GX_TF_RGBA8:
				size = ((width+3)>>2)*((height+3)>>2)*32*2;
				break;
			default:
				break;
	}
	return size;
}

// Creates a RAM copy from a TPL texture
s32 TPL_GetTextureMEMCopy(TPLFile *tdf,s32 id,GXTexObj *texObj, void **texture)
{
	u32 size;
	TPLDescHeader *deschead = NULL;
	TPLImgHeader *imghead = NULL, lclhead;
	s32 bMipMap = 0;
	u8 biasclamp = GX_DISABLE;

	if(!tdf) return -1;
	if(!texObj) return -1;
	if(id<0 || id>=tdf->ntextures) return -1;

	deschead = (TPLDescHeader*)tdf->texdesc;
	if(!deschead) return -1;

	imghead = deschead[id].imghead;
	if(!imghead) return -1;
	
	lclhead = *imghead;
	lclhead.data = NULL;

	size = TPL_GetTextureSize(lclhead.width,lclhead.height,lclhead.fmt);
	
	lclhead.data = memalign(32,size);
	if(!lclhead.data) return -1;
	memcpy(lclhead.data,imghead->data, size);
	
	if(lclhead.maxlod>0) bMipMap = 1;
	if(lclhead.lodbias>0.0f) biasclamp = GX_ENABLE;

	DCFlushRange(lclhead.data,size);
	GX_InitTexObj(texObj,lclhead.data,lclhead.width,lclhead.height,lclhead.fmt,lclhead.wraps,lclhead.wrapt,bMipMap);
	if(bMipMap) GX_InitTexObjLOD(texObj,lclhead.minfilter,lclhead.magfilter,lclhead.minlod,lclhead.maxlod,
								 lclhead.lodbias,biasclamp,biasclamp,lclhead.edgelod);

	*texture = lclhead.data;
	return 0;
}

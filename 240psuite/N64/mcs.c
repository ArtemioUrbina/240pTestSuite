/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */
 
 /*
  * Code in this file is based on the 01_model example from tiny 3D
  * https://github.com/HailToDodongo/tiny3d
  * © 2023-2024 - Max Bebök (HailToDodongo)
  * Tiny3D is licensed under the MIT License
  */
 
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

#include "menu.h"
#include "controller.h"
#include "video.h"

#define AMBIENT		135
#define BGCOLOR		0xbb
#define FADE_FRAMES	30
#define DISTANCE	300.0f

#define ZOOMIN_3D	0
#define ROTATE_3D	1
#define HOLD_3D		2
#define FADE_3D		3

#define MCS_FONT	1

// #8969FF
#define FONT_R	0x89
#define FONT_G	0x69
#define FONT_B	0xFF

typedef struct scenaData_st {
	surface_t depthBuffer;
	T3DViewport viewport;
	T3DMat4 modelMat;			// matrix for our model, this is a "normal" float matrix
	T3DMat4FP* modelMatFP;
	T3DVec3 camPos;
	T3DVec3 camTarget;
	uint8_t colorAmbient[4];
	uint8_t colorDir[4];
	uint8_t colorBG;
	T3DVec3 lightDirVec;
	T3DModel *modelMCS;
	rdpq_font_t *font; 
	uint8_t colorFont[4];
	float rotAngle;
} SceneData;

void freeSceneData(SceneData *scene) {
	if(scene->depthBuffer.buffer)
		surface_free(&scene->depthBuffer);
	
	if(scene->modelMatFP) {
		free_uncached(scene->modelMatFP);
		scene->modelMatFP = NULL;
	}
	
	if(scene->modelMCS) {
		t3d_model_free(scene->modelMCS);
		scene->modelMCS = NULL;
	}
	
	if(scene->font) {
		rdpq_text_unregister_font(MCS_FONT);
		rdpq_font_free(scene->font);
		scene->font = NULL;
	}
}

int load3DScene(SceneData *scene) {
	memset(scene, 0, sizeof(SceneData));
	
	scene->depthBuffer = surface_alloc(FMT_RGBA16, getHardWidth(), getHardHeight());
	if(!scene->depthBuffer.buffer)
		return 0;

	scene->viewport = t3d_viewport_create();

	t3d_mat4_identity(&scene->modelMat);

	scene->modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
	if(!scene->modelMatFP) {
		freeSceneData(scene);
		return 0;
	}

	scene->camPos.v[0] = 0.0f;
	scene->camPos.v[1] = 4.0f;
	scene->camPos.v[2] = 50.0f;
	
	scene->camTarget.v[0] = 0.0f;
	scene->camTarget.v[1] = 0.0f;
	scene->camTarget.v[2] = 0.0f;

	scene->colorAmbient[0] = 0;
	scene->colorAmbient[1] = 0;
	scene->colorAmbient[2] = 0;
	scene->colorAmbient[3] = 0xff;
	
	scene->colorDir[0] = 0xAA;
	scene->colorDir[1] = 0xAA;
	scene->colorDir[2] = 0xAA;
	scene->colorDir[3] = 0xff;
	
	scene->lightDirVec.v[0] = -1.0f;
	scene->lightDirVec.v[1] = 1.0f;
	scene->lightDirVec.v[2] = 1.0f;
	
	scene->colorBG = 0;
	
	t3d_vec3_norm(&scene->lightDirVec);

	// Load a model-file, this contains the geometry and some metadata
	scene->modelMCS = t3d_model_load("rom:/mcs.t3dm");
	if(!scene->modelMCS) {
		freeSceneData(scene);
		return 0;
	}
	
	scene->font = rdpq_font_load("rom:/BebasNeue-Regular.font64");
	if(!scene->font) {
		freeSceneData(scene);
		return 0;
	}
	
	scene->colorFont[0] = FONT_R;
	scene->colorFont[1] = FONT_G;
	scene->colorFont[2] = FONT_B;
	scene->colorFont[3] = 0;
	
    rdpq_text_register_font(MCS_FONT, scene->font);
	
	rdpq_font_style(scene->font, 0, &(rdpq_fontstyle_t){
        .color = RGBA32(scene->colorFont[0], scene->colorFont[1], scene->colorFont[2], scene->colorFont[3]),
    });
	
	return 1;
}

void fadeColor(uint8_t *color, uint8_t thrshld) {
	if(*color > thrshld)
		*color -= thrshld;
	else
		*color = 0;
}

void raiseColor(uint8_t *color, uint8_t thrshld, uint8_t limit) {
	if(*color < limit - thrshld)
		*color += thrshld;
	else
		*color = limit;
}

int draw3DScene(SceneData *scene, int frames, int type, int controls) {
	int cancel = 0, origFrames = frames;
	float modelScale = 0.08f, step = 0;
	float rotX = 0.0f, rotZ = 0.0f;
	joypad_buttons_t keys;

	if(type == ZOOMIN_3D) {
		step = (DISTANCE - scene->camPos.v[2])/frames;
		scene->camPos.v[2] = DISTANCE;
	}
	
	if(type == FADE_3D) {
		step = (DISTANCE - scene->camPos.v[2])/frames;
	}
	
	while(frames) {
		switch(type) {
			case ZOOMIN_3D:
				scene->camPos.v[2] -= step;
				raiseColor(&scene->colorBG, ceil(BGCOLOR/frames), BGCOLOR);
				raiseColor(&scene->colorAmbient[0], ceil(AMBIENT/frames), AMBIENT);
				raiseColor(&scene->colorAmbient[1], ceil(AMBIENT/frames), AMBIENT);
				raiseColor(&scene->colorAmbient[2], ceil(AMBIENT/frames), AMBIENT);
				break;
			case ROTATE_3D:
				if(!controls) {
					scene->rotAngle -= 0.03f;
					raiseColor(&scene->colorFont[3], ceil(0xff/frames), 0xff);
				}
				break;
			case FADE_3D:
				scene->camPos.v[2] += step;
				fadeColor(&scene->colorBG, ceil(BGCOLOR/frames));
				fadeColor(&scene->colorAmbient[0], ceil(AMBIENT/frames));
				fadeColor(&scene->colorAmbient[1], ceil(AMBIENT/frames));
				fadeColor(&scene->colorAmbient[2], ceil(AMBIENT/frames));
				fadeColor(&scene->colorFont[3], ceil(0xff/frames));
				scene->rotAngle -= 0.05f;
				break;
			case HOLD_3D:
				break;
			default:
				break;
		}
		
		t3d_viewport_set_projection(&scene->viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, DISTANCE);
		t3d_viewport_look_at(&scene->viewport, &scene->camPos, &scene->camTarget, &(T3DVec3){{0,1,0}});

		// slowly rotate model
		t3d_mat4_from_srt_euler(&scene->modelMat,
		  (float[3]){modelScale, modelScale, modelScale},
		  (float[3]){rotX, scene->rotAngle, rotZ},
		  (float[3]){0, 0, 0}
		);
		t3d_mat4_to_fixed(scene->modelMatFP, &scene->modelMat);

		// ======== Draw ======== //
		rdpq_attach(display_get(), &scene->depthBuffer);
		t3d_frame_start();
		t3d_viewport_attach(&scene->viewport);

		t3d_screen_clear_color(RGBA32(scene->colorBG, scene->colorBG, scene->colorBG, 0xFF));
		t3d_screen_clear_depth();

		t3d_light_set_ambient(scene->colorAmbient);
		t3d_light_set_directional(0, scene->colorDir, &scene->lightDirVec);
		t3d_light_set_count(1);

		t3d_matrix_push(scene->modelMatFP);
		t3d_model_draw(scene->modelMCS);
		t3d_matrix_pop(1);
		
		if(!controls) {
			rdpq_font_style(scene->font, 0, &(rdpq_fontstyle_t){
				.color = RGBA32(scene->colorFont[0], scene->colorFont[1], scene->colorFont[2], scene->colorFont[3]),
			});
			rdpq_text_printf(&(rdpq_textparms_t){
				.width = 300,
				.height = 100,
				.align  = ALIGN_CENTER,
				}, MCS_FONT, 10, 10, "Mega Cat Studios");
			rdpq_text_printf(&(rdpq_textparms_t){
				.width = 300,
				.height = 100,
				.align  = ALIGN_CENTER,
				}, MCS_FONT, 10, 210, "Proud to contribute to the community we love");
		}
		
		rdpq_detach_show();
		frames --;
		
		joypad_poll();
		keys = controllerButtonsDown();

		if(controls && type == ROTATE_3D) {
			if(keys.b) {
				frames = 0;
				cancel = 1;
			}
			
			keys = controllerButtonsHeld();
			if(keys.l) {
				scene->rotAngle += 0.01f;
				frames = origFrames;
			}
			if(keys.r) {
				scene->rotAngle -= 0.01f;
				frames = origFrames;
			}
			if(keys.c_up) {
				rotX += 0.01f;
				frames = origFrames;
			}
			if(keys.c_down) {
				rotX -= 0.01f;
				frames = origFrames;
			}
			if(keys.d_up) {
				scene->camPos.v[2] -= 0.5f;
				frames = origFrames;
			}
			if(keys.d_down) {
				scene->camPos.v[2] += 0.5f;
				frames = origFrames;
			}
		
			if(scene->camPos.v[2] < 45.0)
				scene->camPos.v[2] = 45.0f;
			if(scene->camPos.v[2] > 590.0)
				scene->camPos.v[2] = 590.0;
		}
		else
		{
			if(keys.a || keys.b || keys.start) {
				frames = 0;
				cancel = 1;
			}
		}
	}
	
	if(cancel) {
		scene->colorBG = BGCOLOR;
		scene->colorAmbient[0] = AMBIENT;
		scene->colorAmbient[1] = AMBIENT;
		scene->colorAmbient[2] = AMBIENT;
		scene->colorFont[3] = 0xff;
	}
	return cancel;
}

int drawMCSScreen(int frames, int controls) {
	int cancel = 0;
	SceneData scene;
		
	if(!load3DScene(&scene))
		return 0;
		
	t3d_init((T3DInitParams){});

	// Zoom in
	cancel = draw3DScene(&scene, FADE_FRAMES, ZOOMIN_3D, controls);
	// Regular scene
	if(!cancel)
		cancel = draw3DScene(&scene, frames+4, ROTATE_3D, controls);
	if(draw3DScene(&scene, !cancel ? ceil(frames/2) : ceil(frames/8), HOLD_3D, controls))
		cancel = true;
	// Fade and zoom out
	(void)draw3DScene(&scene, FADE_FRAMES, FADE_3D, controls);
	
	setClearScreen();
	waitVsync();

	freeSceneData(&scene);
	t3d_destroy();
	
	return cancel;
}

 
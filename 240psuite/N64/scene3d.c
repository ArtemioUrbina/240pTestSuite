/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2026 Mauricio Garrido/Artemio Urbina
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
  
  * Original Demo by Mauricio Garrido
  * Inserted in the Suite by Artemio
  */
 
 
#include <libdragon.h>

#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3ddebug.h>
#include <stdarg.h>

#include "menu.h"
#include "controller.h"
#include "video.h"

typedef struct {
    T3DVec3 pos;
    float strength;
    color_t color;
} PointLight;

typedef enum { 
    POINT_LIGHTS = 0,
    DIR_LIGHTS,
    FLASHLIGHT,
    LIGHTS_OFF 
} lights_type_t;

typedef enum {
    DITHER_NONE = 0,
    DITHER_SQUARE,
    DITHER_NOISE
} dither_mode_t;

void draw_text(int x, int y, const char *fmt, ...) {
    char buffer[512];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    rdpq_text_printf(&(rdpq_textparms_t){
        .width  = 300,
        .height = 100,
        .align  = ALIGN_CENTER,
    }, FONT_BUILTIN_DEBUG_MONO, x, y, "%s", buffer);
}

 void scene3d() {
  int exit = 0;
  
  rdpq_font_t* font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
  rdpq_text_register_font(FONT_BUILTIN_DEBUG_MONO, font);
  
  t3d_init((T3DInitParams){});
  T3DViewport viewport = t3d_viewport_create();

  #define MODEL_COUNT 2
  T3DModel *models[MODEL_COUNT] = {
    t3d_model_load("rom://scene3d.t3dm"),
    t3d_model_load("rom://scene3d_cutout.t3dm")
  };

  T3DMat4FP* modelMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));

  T3DVec3 camPos = {{70.0f, 35.0f, 15.0f}};
  T3DVec3 camPosTarget = camPos;
  T3DVec3 camDir = {{0,1,0}};

  float camRotX = T3D_DEG_TO_RAD(265);
  float camRotY = T3D_DEG_TO_RAD(-30);
  float camRotXTarget = camRotX;
  float camRotYTarget = camRotY;
  
  bool showInfoScreen = true;
  bool fpsLimit = false;

  int ditherMode = DITHER_SQUARE;
  
  lights_type_t lightsType = POINT_LIGHTS;
  const char *lightsName[] = { "Point", "Directional", "Flashlight", "Off" };
  const char *ditherName[] = { "None", "Square", "Noise" };
  
  PointLight pointLights[4] = { // XYZ, strength, color
    {{{  4.0f, 35.0f, 10.0f}}, 18.0f, {0xFF, 0xFA, 0xF4, 0xFF}}, // 1st Cabinet
    {{{ 66.0f, 35.0f, 10.0f}}, 18.0f, {0xFF, 0xFA, 0xF4, 0xFF}}, // 3rd Cabinet
    {{{ 18.0f, 50.0f, 45.0f}}, 25.0f, {0xFF, 0xFA, 0xF4, 0xFF}}, // Desk Lamp
    // Using four point lights considerably decreases performance
    //{{{ -2.0f, 59.0f, -5.0f}},  0.05f,  {0xFF, 0xF5, 0xB6, 0xFF}}, // Wall Lamp
  };

  PointLight flashlight[3] = {
      {{{ -8.0f, 40.0f, 22.0f}}, 32.5f,       {0xFF, 0xFA, 0xF4, 0xFF}},
      {{{ -8.0f, 40.0f, 22.0f}}, 21.9375f,    {0xFF, 0xFA, 0xF4, 0xFF}},
      {{{ -8.0f, 40.0f, 22.0f}}, 14.8078125f, {0xFF, 0xFA, 0xF4, 0xFF}},
  };

  // In order to cull, we must either not record the entire mesh, or do so with individual objects.
  // Here we do the latter. To still take advantage cross-material optimizations, we only record objects
  for(int m=0; m<MODEL_COUNT; ++m) {
    T3DModelIter it = t3d_model_iter_create(models[m], T3D_CHUNK_TYPE_OBJECT);
    while(t3d_model_iter_next(&it)) {
      rspq_block_begin();
      t3d_model_draw_object(it.object, NULL);
      // the object struct offers a 'userBlock' for recording, this is automatically freed when the t3dm object is freed
      // if you need to manually free it, make sure to set it back to NULL afterward
      it.object->userBlock = rspq_block_end();
    }
  }
  
  float modelScale = 0.5f;
  t3d_mat4fp_from_srt_euler(modelMatFP,
    (float[]){modelScale,modelScale,modelScale},
    (float[]){0,0,0},
    (float[]){0,0,0}
  );

  uint64_t ticks = 0;
  for(uint32_t frame=1; !exit; ++frame)
  {
    int visibleObjects = 0, triCount = 0;

    joypad_poll();
    joypad_inputs_t joypad = joypad_get_inputs(JOYPAD_PORT_1);
    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if(joypad.stick_x < 10 && joypad.stick_x > -10)joypad.stick_x = 0;
    if(joypad.stick_y < 10 && joypad.stick_y > -10)joypad.stick_y = 0;

    if(pressed.l) showInfoScreen = !showInfoScreen;
    if(pressed.a)  {
        fpsLimit = !fpsLimit;
        
        if (fpsLimit) display_set_fps_limit(30.0f);
        else display_set_fps_limit(0);
    }

    if(pressed.start) {
        lightsType++;
        if (lightsType > LIGHTS_OFF) lightsType = POINT_LIGHTS;
    }

    if(pressed.r) {
        ditherMode++;
        if (ditherMode > DITHER_NOISE) ditherMode = DITHER_NONE;
    }
    
    if(frame > 60 || pressed.a ) { ticks = 0; frame = 1; }
	
	if(pressed.b) exit = 1;

    // Camera controls:
    float camSpeed = display_get_delta_time() * 0.95f;
    float camRotSpeed = display_get_delta_time() * 0.025f;

    camDir.v[0] = fm_cosf(camRotX) * fm_cosf(camRotY);
    camDir.v[1] = fm_sinf(camRotY);
    camDir.v[2] = fm_sinf(camRotX) * fm_cosf(camRotY);
    t3d_vec3_norm(&camDir);

    if(joypad.btn.z) {
      camRotXTarget += (float)joypad.stick_x * camRotSpeed;
      camRotYTarget -= (float)joypad.stick_y * camRotSpeed;
    } else {
      camPosTarget.v[0] += camDir.v[0] * (float)joypad.stick_y * camSpeed;
      camPosTarget.v[1] += camDir.v[1] * (float)joypad.stick_y * camSpeed;
      camPosTarget.v[2] += camDir.v[2] * (float)joypad.stick_y * camSpeed;
      camPosTarget.v[0] += camDir.v[2] * (float)joypad.stick_x * -camSpeed;
      camPosTarget.v[2] -= camDir.v[0] * (float)joypad.stick_x * -camSpeed;
    }   

    if(joypad.btn.c_up)   camPosTarget.v[1] += camSpeed * 35.0f;
    if(joypad.btn.c_down) camPosTarget.v[1] -= camSpeed * 35.0f;

    t3d_vec3_lerp(&camPos, &camPos, &camPosTarget, 0.8f);
    camRotX = t3d_lerp(camRotX, camRotXTarget, 0.8f);
    camRotY = t3d_lerp(camRotY, camRotYTarget, 0.8f);

    // A very rudimentary flashlight simulation.  It works by using three
    // point lights with different size and distance values, roughly resembling
    // a flashlight light's cone.
    if (lightsType == FLASHLIGHT) {
      float lightDistance = 100.0f;
      for(int i=0; i<3; ++i) {
        flashlight[i].pos.v[0] = camPos.v[0] + camDir.v[0] * lightDistance;
        flashlight[i].pos.v[1] = camPos.v[1] + camDir.v[1] * lightDistance;
        flashlight[i].pos.v[2] = camPos.v[2] + camDir.v[2] * lightDistance;

        if(flashlight[i].pos.v[0] < -20.f) flashlight[i].pos.v[0] = -20.f;
        if(flashlight[i].pos.v[0] > 80.f)  flashlight[i].pos.v[0] = 80.f;
        if(flashlight[i].pos.v[1] < 0.0f)  flashlight[i].pos.v[1] = 0.0f;
        if(flashlight[i].pos.v[2] < 0.0f)  flashlight[i].pos.v[2] = 0.0f;
        if(flashlight[i].pos.v[2] > 100.f) flashlight[i].pos.v[2] = 100.f;

        lightDistance *= 0.675f;
      }
    }

    // Flashlight mode needs to use a model with the cutout render
    // value in all it's materials.
    const T3DModel *model = lightsType == FLASHLIGHT ? models[1] : models[0];

    T3DVec3 camTarget;
    t3d_vec3_add(&camTarget, &camPos, &camDir);

    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 5.0f, 165.0f);
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0,1,0}});

    // since we want to avoid transforming individual AABBs, we transform the frustum
    // to match our map instead (model space). In this case we only have to scale it
    T3DFrustum frustum = viewport.viewFrustum;
    t3d_frustum_scale(&frustum, modelScale);

    // before we do any finer checks with BVH, we test if the entire model is visible
    // note that this data is always available in all models, even without a BVH
    bool modelIsVisible = t3d_frustum_vs_aabb_s16(&frustum, model->aabbMin, model->aabbMax);

    uint64_t ticksStart = get_ticks();
    if(modelIsVisible) {
      // If visible, perform more detailed checks with the BVH (if present in the file)
      // you can also iterate over all AABBs directly (always present) and perform a manual frustum check
      // Note that it might be worth measuring the performance difference between the two methods
      // since at lower object counts the BVH might not be as efficient as a simple linear check

      const T3DBvh *bvh = t3d_model_bvh_get(model); // BVHs are optional, use '--bvh' in the gltf importer (see Makefile)
      if(bvh) {
        t3d_model_bvh_query_frustum(bvh, &frustum);
      } else {
        // without BVH, you can still iterate over all objects and perform a manual frustum checks
        T3DModelIter it = t3d_model_iter_create(model, T3D_CHUNK_TYPE_OBJECT);
        while(t3d_model_iter_next(&it)) {
          it.object->isVisible = t3d_frustum_vs_aabb_s16(&frustum, it.object->aabbMin, it.object->aabbMax);
        }
      }
    }

    ticks += get_ticks() - ticksStart;

    // ----------- DRAW ------------ //
    surface_t *surface = display_get();
    rdpq_attach(surface, display_get_zbuf());

    t3d_frame_start();

    if(lightsType == FLASHLIGHT) {
      rdpq_mode_antialias(AA_NONE);
    } else rdpq_mode_antialias(AA_STANDARD);

    if(ditherMode == 0) rdpq_mode_dithering(DITHER_NONE_NONE);
    if(ditherMode == 1) rdpq_mode_dithering(DITHER_SQUARE_SQUARE);
    if(ditherMode == 2) rdpq_mode_dithering(DITHER_NOISE_NOISE);

    t3d_viewport_attach(&viewport);
    t3d_fog_set_enabled(false);
    t3d_screen_clear_color(RGBA32(0x10, 0x10, 0x10, 0xFF));
    t3d_screen_clear_depth();

    switch(lightsType) {
    case DIR_LIGHTS:
        t3d_light_set_ambient((uint8_t[]){0x00, 0x00, 0x00, 0x00});
        t3d_light_set_directional(0, (uint8_t[]){0xCF, 0xC1, 0xB0, 0xFF}, &(T3DVec3){{-camDir.v[0], -camDir.v[1], -camDir.v[2]}});
        t3d_light_set_count(1);
    break;
    case POINT_LIGHTS:
        t3d_light_set_ambient((uint8_t[]){0x00, 0x00, 0x00, 0x00});
        for(int i=0; i<3; ++i) {
            // Sets the actual point light
            t3d_light_set_point(i, &pointLights[i].color.r,
                &(T3DVec3){{
                    pointLights[i].pos.v[0],
                    pointLights[i].pos.v[1],
                    pointLights[i].pos.v[2]
                }},
                pointLights[i].strength, false);
        }
        t3d_light_set_count(3);
    break;
    case FLASHLIGHT:
        t3d_light_set_ambient((uint8_t[]){0x0B, 0x0C, 0x0B, 0x00});
        for(int i=0; i<3; ++i) {
            t3d_light_set_point(i, (uint8_t[]){0xFF, 0xFA, 0xF4, 0xFF},
                &(T3DVec3){{
                    flashlight[i].pos.v[0],
                    flashlight[i].pos.v[1],
                    flashlight[i].pos.v[2]
                }},
                flashlight[i].strength, true);
        }
        t3d_light_set_count(3);
    break;
    case LIGHTS_OFF:
        t3d_light_set_ambient((uint8_t[]){0x0B, 0x0C, 0x0B, 0xFF});
        t3d_light_set_count(0);
    break;
    };

    t3d_matrix_push(modelMatFP);

    // Now draw all objects that we determined to be visible
    // we still want to optimize materials, so we create a state here and draw them directly
    // the objects (so vertex loads + triangle draws) are recorded since they don't depend on visibility
    int totalObjects = 0;
    if(modelIsVisible)
    {
      T3DModelState state = t3d_model_state_create();
      T3DModelIter it = t3d_model_iter_create(model, T3D_CHUNK_TYPE_OBJECT);
      while(t3d_model_iter_next(&it)) {
        if(it.object->isVisible) {
          // draw material and object
          t3d_model_draw_material(it.object->material, &state);
          rspq_block_run(it.object->userBlock);
          it.object->isVisible = false; // BVH only sets visible objects, so we need to reset this

          // collect some metrics
          ++visibleObjects;
          triCount += it.object->triCount;
        }
        ++totalObjects;
      }
    }

    t3d_matrix_pop(1);

    // ----------- DRAW (2D) ------------ //
	
    if(showInfoScreen) {
      draw_text(18, 18, "Tris: %d", triCount);
      draw_text(18, 32, "Visible Objects: %d", visibleObjects);
      draw_text(18, 46, "%.2f FPS %s", display_get_fps(), fpsLimit ? "Limited to 30" : " ");
      const char* INFO[] = {
        "Start: Toggle lights",
        "       (%s)",
        "A: Toggle fps limit",
        "Z + Stick: Rotate", "Stick: Move",
        "C-U/D: Move up/down",
        "L: closes HELP", "B: Exit"
      };
      int y_offset = getHardWidth() == 640 ? 100 : 0;
      draw_text(18, y_offset+70, INFO[0]);
      draw_text(18, y_offset+84, INFO[1], lightsName[lightsType]);
      for(int i=2; i<8; ++i) {
        draw_text(18, y_offset+70+(i*14), INFO[i]);
      }
      if (lightsType == FLASHLIGHT) {
        draw_text(18, y_offset+182, "R: Toggle dither");
        draw_text(18, y_offset+196, "       (%s)", ditherName[ditherMode]);
      }
    }

    rdpq_detach_show();
  }
  setClearScreen();
  waitVsync();

  free_uncached(modelMatFP);
  t3d_destroy();
  
  rdpq_text_unregister_font(FONT_BUILTIN_DEBUG_MONO);
  rdpq_font_free(font);
  
  if (fpsLimit) display_set_fps_limit(0);
}


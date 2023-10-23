/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
void DrawCheck();
void DrawStripes();
void DropShadow();
void scrollTest();
void LEDZoneTest();
void StripedSprite();
void LagTest();
void VScrollTest();
void SoundTest();
void AudioSyncTest();
void ReflexTest();
void DrawPhase();

void RedrawCheck(int res);
void RedrawDropShadow();
void SwapPalette(int pal, int index);
void DrawPalm();
void MovePalm(int x);
void DrawNumber(int x, int y, int sprite, int number, int palette);
void ChangeNumber(int sprite, int number);
void DrawCircle(int pos);
void LoadNumbers();
void VScrollTest();

void RedrawReflexTest();
void ReflexTestSprites();
void ReflexTestText();
void ReflexTestResults();
void ReflexTestResultsBack();

void PSG_LoadWave(unsigned char chan, unsigned char *wave);
void PSG_SetWaveFreq(unsigned char chan, unsigned int freq);
void PSG_SetNoiseFreq(unsigned int chan, unsigned int freq);
void PSG_StopNoise(unsigned int chann);
void PSG_PlayLeft(unsigned char chan);
void PSG_PlayRight(unsigned char chan);
void PSG_PlayCenter(unsigned char chan);
void PSG_StopAudio(unsigned char chan);
void PSG_StopAllAudio();
void PSG_SetBalance(unsigned char chan, unsigned char left, unsigned char right, unsigned char vol);
void PSG_SetVolume(unsigned char chan, unsigned char vol);
void MDFourier(int boot);
void ConstrInterf();

#ifdef CDPLAYER
void CDDAPlayer();
#endif
			

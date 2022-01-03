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

void display_audiosync_00()
{
#asm
	p_string	audiosync_00_00,24,4
	__ldwi		14
	call		_set_font_pal
	p_string	audiosync_00_01,6,6
	p_string	audiosync_00_02,6,7
	p_string	audiosync_00_03,6,9
	p_string	audiosync_00_04,6,10
	p_string	audiosync_00_05,6,12
	rts

audiosync_00_00:	.db	"AUDIO SYNC"
			.db	0
audiosync_00_01:	.db	"This test flashes the whole screen white for 2"
			.db	0
audiosync_00_02:	.db	"frames, along with a 1khz test tone."
			.db	0
audiosync_00_03:	.db	"You can verify the sync between audio and video"
			.db	0
audiosync_00_04:	.db	"with recording equipment or specialized hardware."
			.db	0
audiosync_00_05:	.db	"- Press I to start/stop the test."
			.db	0
#endasm
}


void display_check_00()
{
#asm
	p_string	check_00_00,24,4
	__ldwi		14
	call		_set_font_pal
	p_string	check_00_01,6,6
	p_string	check_00_02,6,7
	p_string	check_00_03,6,9
	p_string	check_00_04,6,10
	p_string	check_00_05,6,12
	p_string	check_00_06,6,13
	p_string	check_00_07,6,15
	p_string	check_00_08,6,16
	p_string	check_00_09,6,17
	p_string	check_00_10,6,18
	p_string	check_00_11,6,20
	p_string	check_00_12,6,21
	rts

check_00_00:	.db	"CHECKERBOARD"
			.db	0
check_00_01:	.db	"This pattern shows all the visible pixels in an"
			.db	0
check_00_02:	.db	"alternating b&w grid array."
			.db	0
check_00_03:	.db	"- You can switch between all supported resolutions"
			.db	0
check_00_04:	.db	"with SELECT."
			.db	0
check_00_05:	.db	"- You can toggle the pattern with L/R, or turn"
			.db	0
check_00_06:	.db	"on auto-toggle each frame with button I."
			.db	0
check_00_07:	.db	"When auto-toggle is set, you should see it"
			.db	0
check_00_08:	.db	"alternating rapidly. On some setups, the pattern"
			.db	0
check_00_09:	.db	"doesn't change at all. This means that the signal"
			.db	0
check_00_10:	.db	"is being treated as interlaced."
			.db	0
check_00_11:	.db	"- A frame counter can be displayed on screen by"
			.db	0
check_00_12:	.db	"pressing up on the d-pad."
			.db	0
#endasm
}


void display_color601_00()
{
#asm
	p_string	color601_00_00,18,4
	__ldwi		14
	call		_set_font_pal
	p_string	color601_00_01,6,8
	p_string	color601_00_02,6,9
	p_string	color601_00_03,6,11
	p_string	color601_00_04,6,12
	p_string	color601_00_05,6,13
	p_string	color601_00_06,6,14
	p_string	color601_00_07,6,15
	rts

color601_00_00:	.db	"COLOR BARS WITH GRAY SCALE"
			.db	0
color601_00_01:	.db	"This pattern idea and layout borrows from the"
			.db	0
color601_00_02:	.db	"Digital Video Essentials color adjust pattern."
			.db	0
color601_00_03:	.db	"It has a gray background as reference, and can"
			.db	0
color601_00_04:	.db	"be used with color filters in order to adjust"
			.db	0
color601_00_05:	.db	"color accuracy. If your display set has a blue"
			.db	0
color601_00_06:	.db	"only mode, you can also use that to correct"
			.db	0
color601_00_07:	.db	"color balance."
			.db	0
#endasm
}


void display_colorbleed_00()
{
#asm
	p_string	colorbleed_00_00,24,4
	__ldwi		14
	call		_set_font_pal
	p_string	colorbleed_00_01,6,7
	p_string	colorbleed_00_02,6,8
	p_string	colorbleed_00_03,6,10
	p_string	colorbleed_00_04,6,11
	p_string	colorbleed_00_05,6,13
	p_string	colorbleed_00_06,6,14
	p_string	colorbleed_00_07,6,16
	rts

colorbleed_00_00:	.db	"COLOR BLEED"
			.db	0
colorbleed_00_01:	.db	"This pattern helps diagnose color subsampling"
			.db	0
colorbleed_00_02:	.db	"(4:4:4 to 4:2:2, etc). "
			.db	0
colorbleed_00_03:	.db	"It consists of one pixel width color bars "
			.db	0
colorbleed_00_04:	.db	"alternating with one pixel width black bars."
			.db	0
colorbleed_00_05:	.db	"- You can change the vertical bars to a checker"
			.db	0
colorbleed_00_06:	.db	"board with button I."
			.db	0
colorbleed_00_07:	.db	"- You can change the resolution with SELECT."
			.db	0
#endasm
}


void display_colors_00()
{
#asm
	p_string	colors_00_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	colors_00_01,6,8
	p_string	colors_00_02,6,9
	p_string	colors_00_03,6,11
	p_string	colors_00_04,6,12
	p_string	colors_00_05,6,13
	p_string	colors_00_06,6,14
	p_string	colors_00_07,6,16
	p_string	colors_00_08,6,17
	p_string	colors_00_09,6,18
	p_string	colors_00_10,6,19
	p_string	colors_00_11,6,20
	rts

colors_00_00:	.db	"COLORBARS (1/2)"
			.db	0
colors_00_01:	.db	"This pattern allows you to calibrate each color:"
			.db	0
colors_00_02:	.db	"Red, Green and Blue; as  well as the white level."
			.db	0
colors_00_03:	.db	"This is adjusted in the same way black levels"
			.db	0
colors_00_04:	.db	"are set with the PLUGE. If you adjusted the black"
			.db	0
colors_00_05:	.db	"level correctly with the PLUGE, the rectangles"
			.db	0
colors_00_06:	.db	"under the 1 column should be barely visible."
			.db	0
colors_00_07:	.db	"Adjust the white level first, this is done using"
			.db	0
colors_00_08:	.db	"the Contrast control on your TV set. Raise it"
			.db	0
colors_00_09:	.db	"until you cannot distinguish between the two"
			.db	0
colors_00_10:	.db	"blocks under F, and lower it slowly until you"
			.db	0
colors_00_11:	.db	"can clearly tell them apart."
			.db	0
#endasm
}


void display_colors_01()
{
#asm
	p_string	colors_01_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	colors_01_01,6,8
	p_string	colors_01_02,6,9
	p_string	colors_01_03,6,10
	p_string	colors_01_04,6,11
	p_string	colors_01_05,6,12
	p_string	colors_01_06,6,13
	p_string	colors_01_07,6,15
	rts

colors_01_00:	.db	"COLORBARS (2/2)"
			.db	0
colors_01_01:	.db	"The same procedure should be used with each"
			.db	0
colors_01_02:	.db	"color, raise the control that adjusts the"
			.db	0
colors_01_03:	.db	"individual color level (not the color/tint one)"
			.db	0
colors_01_04:	.db	"until you can't tell the rightmost box under F"
			.db	0
colors_01_05:	.db	"from the one to its left, and then lower it until"
			.db	0
colors_01_06:	.db	"you can distinguish them clearly."
			.db	0
colors_01_07:	.db	"Repeat for each color and you are set."
			.db	0
#endasm
}


void display_convergence_00()
{
#asm
	p_string	convergence_00_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	convergence_00_01,6,6
	p_string	convergence_00_02,6,7
	p_string	convergence_00_03,6,9
	p_string	convergence_00_04,6,10
	p_string	convergence_00_05,6,11
	p_string	convergence_00_06,6,13
	p_string	convergence_00_07,6,14
	p_string	convergence_00_08,6,16
	rts

convergence_00_00:	.db	"CONVERGENCE TESTS"
			.db	0
convergence_00_01:	.db	"These are used to adjust color convergence in CRT "
			.db	0
convergence_00_02:	.db	"displays."
			.db	0
convergence_00_03:	.db	"- Button 'I' changes the pattern between cross hatch"
			.db	0
convergence_00_04:	.db	"lines, crosses, dots and a color pattern with or"
			.db	0
convergence_00_05:	.db	"without a black grid, for transition boundary check."
			.db	0
convergence_00_06:	.db	"- Left and Right on the D-Pad allow cycling between"
			.db	0
convergence_00_07:	.db	"patterns."
			.db	0
convergence_00_08:	.db	"- You can change the resolution with SELECT."
			.db	0
#endasm
}


void display_dshadow_00()
{
#asm
	p_string	dshadow_00_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	dshadow_00_01,6,7
	p_string	dshadow_00_02,6,8
	p_string	dshadow_00_03,6,9
	p_string	dshadow_00_04,6,10
	p_string	dshadow_00_05,6,12
	p_string	dshadow_00_06,6,13
	p_string	dshadow_00_07,6,14
	p_string	dshadow_00_08,6,15
	p_string	dshadow_00_09,6,17
	p_string	dshadow_00_10,6,18
	p_string	dshadow_00_11,6,20
	p_string	dshadow_00_12,6,22
	rts

dshadow_00_00:	.db	"DROP SHADOW TEST"
			.db	0
dshadow_00_01:	.db	"This is a crucial test for 240p upscan converters."
			.db	0
dshadow_00_02:	.db	"It displays a simple sprite shadow (32x32 pixels)"
			.db	0
dshadow_00_03:	.db	"against a background, but the shadow is shown only"
			.db	0
dshadow_00_04:	.db	"on each other frame. "
			.db	0
dshadow_00_05:	.db	"On a CRT this achieves a transparency effect,"
			.db	0
dshadow_00_06:	.db	"since you are watching a 30hz shadow on a 60hz"
			.db	0
dshadow_00_07:	.db	"signal. No background detail should be lost and"
			.db	0
dshadow_00_08:	.db	"the shadow should be visible."
			.db	0
dshadow_00_09:	.db	"- The user can toggle the frame used to draw the"
			.db	0
dshadow_00_10:	.db	"shadow with SELECT. "
			.db	0
dshadow_00_11:	.db	"- Backgrounds can be switched with button I."
			.db	0
dshadow_00_12:	.db	"Donna art by Jose Salot"
			.db	0
#endasm
}


void display_ebucb_00()
{
#asm
	p_string	ebucb_00_00,22,4
	__ldwi		14
	call		_set_font_pal
	p_string	ebucb_00_01,6,7
	p_string	ebucb_00_02,6,9
	p_string	ebucb_00_03,6,10
	rts

ebucb_00_00:	.db	"EBU COLOR BARS"
			.db	0
ebucb_00_01:	.db	"- This pattern can be used to calibrate colors"
			.db	0
ebucb_00_02:	.db	"- You can toggle between 75% and 100% saturation"
			.db	0
ebucb_00_03:	.db	"by pressing I."
			.db	0
#endasm
}


void display_gray_00()
{
#asm
	p_string	gray_00_00,26,4
	__ldwi		14
	call		_set_font_pal
	p_string	gray_00_01,6,7
	p_string	gray_00_02,6,8
	p_string	gray_00_03,6,10
	p_string	gray_00_04,6,11
	p_string	gray_00_05,6,12
	p_string	gray_00_06,6,13
	rts

gray_00_00:	.db	"GRAY RAMP"
			.db	0
gray_00_01:	.db	"This gray ramp pattern can be used to check color"
			.db	0
gray_00_02:	.db	"balance and to adjust the white level."
			.db	0
gray_00_03:	.db	"You should make sure the bars are gray, with no"
			.db	0
gray_00_04:	.db	"red or blue bias. This can be adjusted with the"
			.db	0
gray_00_05:	.db	"individual color settings, or the tint control"
			.db	0
gray_00_06:	.db	"in NTSC displays."
			.db	0
#endasm
}


void display_grid_00()
{
#asm
	p_string	grid_00_00,29,4
	__ldwi		14
	call		_set_font_pal
	p_string	grid_00_01,6,6
	p_string	grid_00_02,6,7
	p_string	grid_00_03,6,8
	p_string	grid_00_04,6,9
	p_string	grid_00_05,6,11
	p_string	grid_00_06,6,13
	p_string	grid_00_07,6,14
	p_string	grid_00_08,6,15
	p_string	grid_00_09,6,17
	p_string	grid_00_10,6,19
	p_string	grid_00_11,6,20
	p_string	grid_00_12,6,22
	rts

grid_00_00:	.db	"GRID"
			.db	0
grid_00_01:	.db	"These grid tests change horizontal resolution between"
			.db	0
grid_00_02:	.db	"256, 320, 352, and 512. You can vary the vertical"
			.db	0
grid_00_03:	.db	"resolution from the options menu. The pattern should"
			.db	0
grid_00_04:	.db	"fill the 4:3 screen area."
			.db	0
grid_00_05:	.db	"Consumer TVs may not show red squares due to overscan."
			.db	0
grid_00_06:	.db	"The Grids are not perfectly centered within the NTSC"
			.db	0
grid_00_07:	.db	"signal, and that is normal. It is the way Hudson/NEC"
			.db	0
grid_00_08:	.db	"used each resolution."
			.db	0
grid_00_09:	.db	"- You can change the resolution with SELECT."
			.db	0
grid_00_10:	.db	"- You can press button I to fill all the video signal"
			.db	0
grid_00_11:	.db	"background area."
			.db	0
grid_00_12:	.db	"- The 352x240 resolution is used by R-Type"
			.db	0
#endasm
}


void display_gridscroll_00()
{
#asm
	p_string	gridscroll_00_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	gridscroll_00_01,6,7
	p_string	gridscroll_00_02,6,8
	p_string	gridscroll_00_03,6,9
	p_string	gridscroll_00_04,6,10
	p_string	gridscroll_00_05,6,12
	p_string	gridscroll_00_06,6,13
	p_string	gridscroll_00_07,6,14
	p_string	gridscroll_00_08,6,16
	p_string	gridscroll_00_09,6,18
	rts

gridscroll_00_00:	.db	"GRID SCROLL TEST"
			.db	0
gridscroll_00_01:	.db	"A grid is scrolled vertically or horizontally,"
			.db	0
gridscroll_00_02:	.db	"which can be used to test linearity of the signal"
			.db	0
gridscroll_00_03:	.db	"and how well the display or video processor copes"
			.db	0
gridscroll_00_04:	.db	"with scrolling and framerate."
			.db	0
gridscroll_00_05:	.db	"- Button I can be used to toggle between horizontal"
			.db	0
gridscroll_00_06:	.db	"and vertical scroll, while the d-pad up or down"
			.db	0
gridscroll_00_07:	.db	"regulates speed."
			.db	0
gridscroll_00_08:	.db	"- D-pad left or right changes direction."
			.db	0
gridscroll_00_09:	.db	"- You can change the resolution with SELECT."
			.db	0
#endasm
}


void display_mdfourier_00()
{
#asm
	p_string	mdfourier_00_00,24,4
	__ldwi		14
	call		_set_font_pal
	p_string	mdfourier_00_01,6,6
	p_string	mdfourier_00_02,6,7
	p_string	mdfourier_00_03,6,8
	p_string	mdfourier_00_04,6,10
	p_string	mdfourier_00_05,6,11
	p_string	mdfourier_00_06,6,13
	rts

mdfourier_00_00:	.db	"MDFourier"
			.db	0
mdfourier_00_01:	.db	"This test generates a series of audio signals that"
			.db	0
mdfourier_00_02:	.db	"are to be recorded and analyzed for comparison"
			.db	0
mdfourier_00_03:	.db	"with the MDFourier tool on a computer."
			.db	0
mdfourier_00_04:	.db	"This allows detailed comparison of the signal"
			.db	0
mdfourier_00_05:	.db	"as generated by this system."
			.db	0
mdfourier_00_06:	.db	"Visit http://junkerhq.net/MDFourier for details."
			.db	0
#endasm
}


void display_monoscope_00()
{
#asm
	p_string	monoscope_00_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	monoscope_00_01,6,6
	p_string	monoscope_00_02,6,7
	p_string	monoscope_00_03,6,9
	p_string	monoscope_00_04,6,10
	p_string	monoscope_00_05,6,12
	p_string	monoscope_00_06,6,13
	p_string	monoscope_00_07,6,14
	p_string	monoscope_00_08,6,16
	p_string	monoscope_00_09,6,17
	p_string	monoscope_00_10,6,18
	p_string	monoscope_00_11,6,20
	p_string	monoscope_00_12,6,21
	rts

monoscope_00_00:	.db	"MONOSCOPE (1/2)"
			.db	0
monoscope_00_01:	.db	"This pattern contains elements to calibrate multiple"
			.db	0
monoscope_00_02:	.db	"aspects of a CRT."
			.db	0
monoscope_00_03:	.db	"Read your monitor's service manual to learn how, and"
			.db	0
monoscope_00_04:	.db	"use Up/Down to change IRE."
			.db	0
monoscope_00_05:	.db	"Brightness adjustment: Adjust convergence at low"
			.db	0
monoscope_00_06:	.db	"brightness (13/25 IRE). An overly bright pattern can"
			.db	0
monoscope_00_07:	.db	"mask convergence issues."
			.db	0
monoscope_00_08:	.db	"Convergence: Use the center crosshair to check static"
			.db	0
monoscope_00_09:	.db	"(center of screen) convergence. Use the patterns at"
			.db	0
monoscope_00_10:	.db	"the sides to check dynamic (edge) convergence."
			.db	0
monoscope_00_11:	.db	"Corners: After setting center and edge convergence,"
			.db	0
monoscope_00_12:	.db	"use magnets to adjust corner purity and geometry."
			.db	0
#endasm
}


void display_monoscope_01()
{
#asm
	p_string	monoscope_01_00,23,4
	__ldwi		14
	call		_set_font_pal
	p_string	monoscope_01_01,6,6
	p_string	monoscope_01_02,6,7
	p_string	monoscope_01_03,6,8
	p_string	monoscope_01_04,6,9
	p_string	monoscope_01_05,6,11
	p_string	monoscope_01_06,6,12
	p_string	monoscope_01_07,6,13
	p_string	monoscope_01_08,6,15
	rts

monoscope_01_00:	.db	"MONOSCOPE (2/2)"
			.db	0
monoscope_01_01:	.db	"Size and aspect ratio: If vertical and horizontal size"
			.db	0
monoscope_01_02:	.db	"are correct, the red squares in the pattern will be"
			.db	0
monoscope_01_03:	.db	"perfect squares. After setting H size, use a tape"
			.db	0
monoscope_01_04:	.db	"measure to adjust V size to match it."
			.db	0
monoscope_01_05:	.db	"Linearity: The squares in each corner should get you"
			.db	0
monoscope_01_06:	.db	"started. Confirm your adjustment using the scroll"
			.db	0
monoscope_01_07:	.db	"tests."
			.db	0
monoscope_01_08:	.db	"Designed by Keith Raney."
			.db	0
#endasm
}


void display_options_00()
{
#asm
	p_string	options_00_00,27,4
	__ldwi		14
	call		_set_font_pal
	p_string	options_00_01,6,6
	p_string	options_00_02,6,7
	p_string	options_00_03,6,8
	p_string	options_00_04,6,9
	p_string	options_00_05,6,11
	p_string	options_00_06,6,12
	p_string	options_00_07,6,13
	p_string	options_00_08,6,14
	p_string	options_00_09,6,16
	p_string	options_00_10,6,17
	p_string	options_00_11,6,18
	p_string	options_00_12,6,19
	p_string	options_00_13,6,20
	p_string	options_00_14,6,22
	p_string	options_00_15,6,23
	rts

options_00_00:	.db	"OPTIONS"
			.db	0
options_00_01:	.db	"Vertical resolution: This system outputs either 224"
			.db	0
options_00_02:	.db	"or 240p video. Although capable of displaying all 240"
			.db	0
options_00_03:	.db	"vertical lines games tend to use only 239; probably"
			.db	0
options_00_04:	.db	"to gain the CPU time of 4 extra lines during vblank."
			.db	0
options_00_05:	.db	"The suite uses the full 240 mode by default, but you"
			.db	0
options_00_06:	.db	"can enable the one games use by selecting to start at"
			.db	0
options_00_07:	.db	"line 24 instead of 22. This will clip the last line"
			.db	0
options_00_08:	.db	"in patterns, but show the real area used by games."
			.db	0
options_00_09:	.db	"Composite Filter: The system outputs an extra video"
			.db	0
options_00_10:	.db	"line (266) and creates a comb filter with every other"
			.db	0
options_00_11:	.db	"frame to reduce artifacts in the composite signal."
			.db	0
options_00_12:	.db	"This extra line contains no data; but since the"
			.db	0
options_00_13:	.db	"video signal is taller, CRTs shift vertically."
			.db	0
options_00_14:	.db	"Composite B&W: This option works only in composite."
			.db	0
options_00_15:	.db	"It disables the NTSC color burst, eliminating color."
			.db	0
#endasm
}


void display_overscan_00()
{
#asm
	p_string	overscan_00_00,27,4
	__ldwi		14
	call		_set_font_pal
	p_string	overscan_00_01,6,6
	p_string	overscan_00_02,6,7
	p_string	overscan_00_03,6,8
	p_string	overscan_00_04,6,10
	p_string	overscan_00_05,6,11
	p_string	overscan_00_06,6,12
	p_string	overscan_00_07,6,13
	p_string	overscan_00_08,6,15
	p_string	overscan_00_09,6,17
	p_string	overscan_00_10,6,18
	p_string	overscan_00_11,6,19
	p_string	overscan_00_12,6,20
	p_string	overscan_00_13,6,22
	p_string	overscan_00_14,6,23
	rts

overscan_00_00:	.db	"OVERSCAN"
			.db	0
overscan_00_01:	.db	"With this pattern you can interactively find out"
			.db	0
overscan_00_02:	.db	"the overscan in pixels and percentage of each"
			.db	0
overscan_00_03:	.db	"edge in a display."
			.db	0
overscan_00_04:	.db	"- Increment the overscan with left and right until"
			.db	0
overscan_00_05:	.db	"you see the white border, then go back one pixel."
			.db	0
overscan_00_06:	.db	"The resulting number is the amount of overscan"
			.db	0
overscan_00_07:	.db	"in pixels in each direction."
			.db	0
overscan_00_08:	.db	"- Button I allows faster movement of the borders."
			.db	0
overscan_00_09:	.db	"This pattern follows the current vertical option,"
			.db	0
overscan_00_10:	.db	"so you can check how it handles in 239, 240, and"
			.db	0
overscan_00_11:	.db	"224 modes. Keep in mind the horizontal resolution"
			.db	0
overscan_00_12:	.db	"is fixed at 256 pixels."
			.db	0
overscan_00_13:	.db	"- The rows colored in yellow rows are never shown"
			.db	0
overscan_00_14:	.db	"by original hardware."
			.db	0
#endasm
}


void display_pluge_00()
{
#asm
	p_string	pluge_00_00,25,4
	__ldwi		14
	call		_set_font_pal
	p_string	pluge_00_01,6,7
	p_string	pluge_00_02,6,8
	p_string	pluge_00_03,6,9
	p_string	pluge_00_04,6,11
	p_string	pluge_00_05,6,12
	p_string	pluge_00_06,6,13
	p_string	pluge_00_07,6,15
	p_string	pluge_00_08,6,17
	p_string	pluge_00_09,6,18
	p_string	pluge_00_10,6,19
	rts

pluge_00_00:	.db	"PLUGE (1/2)"
			.db	0
pluge_00_01:	.db	"The picture line-up generation equipment (PLUGE)"
			.db	0
pluge_00_02:	.db	"is a test pattern used to adjust the black level"
			.db	0
pluge_00_03:	.db	"and contrast of a video signal."
			.db	0
pluge_00_04:	.db	"The pattern in this suite is mainly designed to"
			.db	0
pluge_00_05:	.db	"adjust the black level. The control for setting"
			.db	0
pluge_00_06:	.db	"this on your TV is usually named Brightness."
			.db	0
pluge_00_07:	.db	"This allows all detail to be seen on dark scenes."
			.db	0
pluge_00_08:	.db	"- Button I changes the inner bars between the"
			.db	0
pluge_00_09:	.db	"lowest B,G,R values and gray. You should use the"
			.db	0
pluge_00_10:	.db	"blue one in order not to crush detail."
			.db	0
#endasm
}


void display_pluge_01()
{
#asm
	p_string	pluge_01_00,25,4
	__ldwi		14
	call		_set_font_pal
	p_string	pluge_01_01,6,7
	p_string	pluge_01_02,6,8
	p_string	pluge_01_03,6,9
	p_string	pluge_01_04,6,10
	p_string	pluge_01_05,6,12
	p_string	pluge_01_06,6,13
	p_string	pluge_01_07,6,14
	rts

pluge_01_00:	.db	"PLUGE (2/2)"
			.db	0
pluge_01_01:	.db	"Reduce the Brightness until the inner bars"
			.db	0
pluge_01_02:	.db	"are no longer visible, and then slowly raise it"
			.db	0
pluge_01_03:	.db	"until they are just visible, and go back one"
			.db	0
pluge_01_04:	.db	"notch where they blend with the background."
			.db	0
pluge_01_05:	.db	"Please keep in mind that black levels are lower"
			.db	0
pluge_01_06:	.db	"in other game consoles, these are just the lowest"
			.db	0
pluge_01_07:	.db	"values available in this system."
			.db	0
#endasm
}


void display_reflex_00()
{
#asm
	p_string	reflex_00_00,16,4
	__ldwi		14
	call		_set_font_pal
	p_string	reflex_00_01,6,6
	p_string	reflex_00_02,6,7
	p_string	reflex_00_03,6,8
	p_string	reflex_00_04,6,9
	p_string	reflex_00_05,6,10
	p_string	reflex_00_06,6,11
	p_string	reflex_00_07,6,13
	p_string	reflex_00_08,6,14
	p_string	reflex_00_09,6,15
	p_string	reflex_00_10,6,16
	p_string	reflex_00_11,6,18
	p_string	reflex_00_12,6,19
	p_string	reflex_00_13,6,20
	p_string	reflex_00_14,6,21
	rts

reflex_00_00:	.db	"Timing & Reflex Test (1/2)"
			.db	0
reflex_00_01:	.db	"WARNING: This evaluation is dependent on reflexes"
			.db	0
reflex_00_02:	.db	"and/or rhythm more than anything. The visual and"
			.db	0
reflex_00_03:	.db	"audio cues are the more revealing aspects which"
			.db	0
reflex_00_04:	.db	"the user should consider, of course the interactive"
			.db	0
reflex_00_05:	.db	"factor can give an experienced player the hang of"
			.db	0
reflex_00_06:	.db	"the system when testing via different connections."
			.db	0
reflex_00_07:	.db	"The main intention is to show a changing pattern"
			.db	0
reflex_00_08:	.db	"on the screen, which can be complemented with"
			.db	0
reflex_00_09:	.db	"audio. This should show to some degree any lag"
			.db	0
reflex_00_10:	.db	"when processing the signal."
			.db	0
reflex_00_11:	.db	"As an added feature, the user can click button I"
			.db	0
reflex_00_12:	.db	"when the sprite is aligned with the one on the"
			.db	0
reflex_00_13:	.db	"background, and the offset in frames from the"
			.db	0
reflex_00_14:	.db	"actual intersection will be shown on screen. "
			.db	0
#endasm
}


void display_reflex_01()
{
#asm
	p_string	reflex_01_00,16,4
	__ldwi		14
	call		_set_font_pal
	p_string	reflex_01_01,6,6
	p_string	reflex_01_02,6,7
	p_string	reflex_01_03,6,8
	p_string	reflex_01_04,6,9
	p_string	reflex_01_05,6,10
	p_string	reflex_01_06,6,11
	p_string	reflex_01_07,6,12
	p_string	reflex_01_08,6,14
	p_string	reflex_01_09,6,15
	p_string	reflex_01_10,6,16
	p_string	reflex_01_11,6,18
	p_string	reflex_01_12,6,19
	p_string	reflex_01_13,6,20
	rts

reflex_01_00:	.db	"Timing & Reflex Test (2/2)"
			.db	0
reflex_01_01:	.db	"A one frame 1khz tone is played when audio is"
			.db	0
reflex_01_02:	.db	"enabled and the sprites are aligned, and a 500hz"
			.db	0
reflex_01_03:	.db	"tone is played when button I is pressed. This"
			.db	0
reflex_01_04:	.db	"can be repeated ten times and the software will"
			.db	0
reflex_01_05:	.db	"calculate the average. Whenever the button was"
			.db	0
reflex_01_06:	.db	"pressed before the actual intersection frame, the"
			.db	0
reflex_01_07:	.db	"result will be ignored (but still shown on screen)."
			.db	0
reflex_01_08:	.db	"- SELECT can be used to change the direction"
			.db	0
reflex_01_09:	.db	"of the sprite from vertical to horizontal, or"
			.db	0
reflex_01_10:	.db	"display both at the same time"
			.db	0
reflex_01_11:	.db	"Since a frame is around 16 ms (1000/60) in NTSC"
			.db	0
reflex_01_12:	.db	"that value must be considered the general error"
			.db	0
reflex_01_13:	.db	"when using the test results."
			.db	0
#endasm
}


void display_scroll_00()
{
#asm
	p_string	scroll_00_00,26,4
	__ldwi		14
	call		_set_font_pal
	p_string	scroll_00_01,6,7
	p_string	scroll_00_02,6,8
	p_string	scroll_00_03,6,10
	p_string	scroll_00_04,6,11
	p_string	scroll_00_05,6,13
	p_string	scroll_00_06,6,15
	p_string	scroll_00_07,6,16
	p_string	scroll_00_08,6,18
	p_string	scroll_00_09,6,19
	p_string	scroll_00_10,6,21
	p_string	scroll_00_11,6,22
	rts

scroll_00_00:	.db	"SCROLL TEST"
			.db	0
scroll_00_01:	.db	"This test shows either an horizontal background"
			.db	0
scroll_00_02:	.db	"from Sonic or a vertical one from Kiki Kaikai."
			.db	0
scroll_00_03:	.db	"- Speed can be varied with up & down on the"
			.db	0
scroll_00_04:	.db	"d-pad and scroll direction with left or right."
			.db	0
scroll_00_05:	.db	"- SELECT changes between 320 and 256 resolutions."
			.db	0
scroll_00_06:	.db	"- Button I toggles between vertical and horizontal"
			.db	0
scroll_00_07:	.db	"scroll."
			.db	0
scroll_00_08:	.db	"This can be used to notice any drops in"
			.db	0
scroll_00_09:	.db	"framerate, or pixel width inconsistencies."
			.db	0
scroll_00_10:	.db	"Sonic is a trademark of Sega Enterprises Ltd."
			.db	0
scroll_00_11:	.db	"Kiki Kaikai is a trademark of Taito/Square."
			.db	0
#endasm
}


void display_sharpness_00()
{
#asm
	p_string	sharpness_00_00,26,4
	__ldwi		14
	call		_set_font_pal
	p_string	sharpness_00_01,6,7
	p_string	sharpness_00_02,6,8
	p_string	sharpness_00_03,6,9
	p_string	sharpness_00_04,6,11
	p_string	sharpness_00_05,6,12
	p_string	sharpness_00_06,6,13
	p_string	sharpness_00_07,6,15
	p_string	sharpness_00_08,6,16
	p_string	sharpness_00_09,6,18
	p_string	sharpness_00_10,6,19
	p_string	sharpness_00_11,6,20
	rts

sharpness_00_00:	.db	"SHARPNESS"
			.db	0
sharpness_00_01:	.db	"You should set the sharpness of your CRT to a"
			.db	0
sharpness_00_02:	.db	"value that shows clean black and gray transitions,"
			.db	0
sharpness_00_03:	.db	"with no white ghosting in between."
			.db	0
sharpness_00_04:	.db	"On most modern displays, the sharpness control is"
			.db	0
sharpness_00_05:	.db	"an edge-enhancement control, and most probably"
			.db	0
sharpness_00_06:	.db	"should be set to zero, or in the middle."
			.db	0
sharpness_00_07:	.db	"In some PVM/BVM displays this is also referred"
			.db	0
sharpness_00_08:	.db	"to as aperture."
			.db	0
sharpness_00_09:	.db	"- You can change the pattern with I. When using "
			.db	0
sharpness_00_10:	.db	"the brick pattern, resolutions can be changed with "
			.db	0
sharpness_00_11:	.db	"SELECT."
			.db	0
#endasm
}


#ifndef CDROM
void display_sound_00()
{
#asm
	p_string	sound_00_00,28,4
	__ldwi		14
	call		_set_font_pal
	p_string	sound_00_01,6,8
	p_string	sound_00_02,6,9
	p_string	sound_00_03,6,11
	p_string	sound_00_04,6,12
	rts

sound_00_00:	.db	"SOUND"
			.db	0
sound_00_01:	.db	"This simple test plays a sound in either or both"
			.db	0
sound_00_02:	.db	"channels."
			.db	0
sound_00_03:	.db	"It can be used to determine if the audio chain is"
			.db	0
sound_00_04:	.db	"working properly."
			.db	0
#endasm
}
#endif


#ifdef CDROM
void display_soundCD_00()
{
#asm
	p_string	soundCD_00_00,28,4
	__ldwi		14
	call		_set_font_pal
	p_string	soundCD_00_01,6,8
	p_string	soundCD_00_02,6,9
	p_string	soundCD_00_03,6,11
	p_string	soundCD_00_04,6,12
	p_string	soundCD_00_05,6,14
	p_string	soundCD_00_06,6,15
	rts

soundCD_00_00:	.db	"SOUND"
			.db	0
soundCD_00_01:	.db	"This simple test plays a PSG sound in either"
			.db	0
soundCD_00_02:	.db	"or both channels."
			.db	0
soundCD_00_03:	.db	"Aside from PSG, it can also play CDDA and"
			.db	0
soundCD_00_04:	.db	"ADPCM audio."
			.db	0
soundCD_00_05:	.db	"It can be used to determine if the audio chain is"
			.db	0
soundCD_00_06:	.db	"working properly."
			.db	0
#endasm
}
#endif


void display_striped_00()
{
#asm
	p_string	striped_00_00,21,4
	__ldwi		14
	call		_set_font_pal
	p_string	striped_00_01,6,7
	p_string	striped_00_02,6,8
	p_string	striped_00_03,6,9
	p_string	striped_00_04,6,10
	p_string	striped_00_05,6,11
	p_string	striped_00_06,6,13
	p_string	striped_00_07,6,15
	rts

striped_00_00:	.db	"STRIPED SPRITE TEST"
			.db	0
striped_00_01:	.db	"There actually are deinterlacers out there which"
			.db	0
striped_00_02:	.db	"can display the drop shadows correctly and still"
			.db	0
striped_00_03:	.db	"interpret 240p as 480i. With a striped sprite it"
			.db	0
striped_00_04:	.db	"should be easy to tell if a processor tries to"
			.db	0
striped_00_05:	.db	"deinterlace (plus interpolate) or not."
			.db	0
striped_00_06:	.db	"- You can change backgrounds with I."
			.db	0
striped_00_07:	.db	"Donna art by Jose Salot"
			.db	0
#endasm
}


void display_white_00()
{
#asm
	p_string	white_00_00,24,4
	__ldwi		14
	call		_set_font_pal
	p_string	white_00_01,6,7
	p_string	white_00_02,6,8
	p_string	white_00_03,6,9
	p_string	white_00_04,6,11
	p_string	white_00_05,6,12
	p_string	white_00_06,6,13
	p_string	white_00_07,6,15
	p_string	white_00_08,6,16
	p_string	white_00_09,6,18
	p_string	white_00_10,6,19
	rts

white_00_00:	.db	"WHITE SCREEN"
			.db	0
white_00_01:	.db	"- This pattern can be changed between white, black,"
			.db	0
white_00_02:	.db	"red, green, blue and custom color full screens by"
			.db	0
white_00_03:	.db	"using button I."
			.db	0
white_00_04:	.db	"- Pressing Select while displaying the white screen"
			.db	0
white_00_05:	.db	"will enter color edit mode, to select the RGB"
			.db	0
white_00_06:	.db	"color to fill the screen with."
			.db	0
white_00_07:	.db	"Some displays and scalers have issues when"
			.db	0
white_00_08:	.db	"changing between a black 0 IRE and a white screen."
			.db	0
white_00_09:	.db	"This pattern can also be used to check color"
			.db	0
white_00_10:	.db	"purity, and for dead pixels in digital displays."
			.db	0
#endasm
}



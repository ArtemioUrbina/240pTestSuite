void display_SMPTEColor_00()
{
#asm
	p_string	SMPTEColor_00_00,22,5
	__ldwi		14
	call		_set_font_pal
	p_string	SMPTEColor_00_01,6,9
	p_string	SMPTEColor_00_02,6,10
	p_string	SMPTEColor_00_03,6,11
	p_string	SMPTEColor_00_04,6,13
	p_string	SMPTEColor_00_05,6,14
	rts

SMPTEColor_00_00:	.db	"SMPTE COLOR BARS"
			.db	0
SMPTEColor_00_01:	.db	"This pattern can be used to calibrate colors, but"
			.db	0
SMPTEColor_00_02:	.db	"black level does not comply to the standard due"
			.db	0
SMPTEColor_00_03:	.db	"to console capabilities."
			.db	0
SMPTEColor_00_04:	.db	"You can toggle between 75% and 100% SMPTE color"
			.db	0
SMPTEColor_00_05:	.db	"bars by pressing I."
			.db	0
#endasm
}


void display_audiosync_00()
{
#asm
	p_string	audiosync_00_00,24,5
	__ldwi		14
	call		_set_font_pal
	p_string	audiosync_00_01,6,7
	p_string	audiosync_00_02,6,8
	p_string	audiosync_00_03,6,10
	p_string	audiosync_00_04,6,11
	p_string	audiosync_00_05,6,13
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
audiosync_00_05:	.db	"Press I to start/stop the test."
			.db	0
#endasm
}


void display_color601_00()
{
#asm
	p_string	color601_00_00,18,5
	__ldwi		14
	call		_set_font_pal
	p_string	color601_00_01,6,9
	p_string	color601_00_02,6,10
	p_string	color601_00_03,6,12
	p_string	color601_00_04,6,13
	p_string	color601_00_05,6,14
	p_string	color601_00_06,6,15
	p_string	color601_00_07,6,16
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


void display_colors_00()
{
#asm
	p_string	colors_00_00,23,5
	__ldwi		14
	call		_set_font_pal
	p_string	colors_00_01,6,9
	p_string	colors_00_02,6,10
	p_string	colors_00_03,6,12
	p_string	colors_00_04,6,13
	p_string	colors_00_05,6,14
	p_string	colors_00_06,6,15
	p_string	colors_00_07,6,17
	p_string	colors_00_08,6,18
	p_string	colors_00_09,6,19
	p_string	colors_00_10,6,20
	p_string	colors_00_11,6,21
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
	p_string	colors_01_00,23,5
	__ldwi		14
	call		_set_font_pal
	p_string	colors_01_01,6,9
	p_string	colors_01_02,6,10
	p_string	colors_01_03,6,11
	p_string	colors_01_04,6,12
	p_string	colors_01_05,6,13
	p_string	colors_01_06,6,14
	p_string	colors_01_07,6,16
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


void display_dshadow_00()
{
#asm
	p_string	dshadow_00_00,23,5
	__ldwi		14
	call		_set_font_pal
	p_string	dshadow_00_01,6,8
	p_string	dshadow_00_02,6,9
	p_string	dshadow_00_03,6,10
	p_string	dshadow_00_04,6,11
	p_string	dshadow_00_05,6,12
	p_string	dshadow_00_06,6,13
	p_string	dshadow_00_07,6,14
	p_string	dshadow_00_08,6,16
	p_string	dshadow_00_09,6,17
	p_string	dshadow_00_10,6,18
	rts

dshadow_00_00:	.db	"DROP SHADOW TEST"
			.db	0
dshadow_00_01:	.db	"This is a crucial test for 240p upscan converters."
			.db	0
dshadow_00_02:	.db	"It displays a simple sprite shadow (32x32 pixels)"
			.db	0
dshadow_00_03:	.db	"against a background, but the shadow is shown only"
			.db	0
dshadow_00_04:	.db	"on each other frame. On a CRT this achieves a"
			.db	0
dshadow_00_05:	.db	"transparency effect, since you are watching a 30hz"
			.db	0
dshadow_00_06:	.db	"shadow on a 60hz signal. No background detail"
			.db	0
dshadow_00_07:	.db	"should be lost and the shadow should be visible."
			.db	0
dshadow_00_08:	.db	"The user can toggle the frame used to draw the"
			.db	0
dshadow_00_09:	.db	"shadow with SELECT. Backgrounds can be"
			.db	0
dshadow_00_10:	.db	"switched with button I."
			.db	0
#endasm
}


void display_gray_00()
{
#asm
	p_string	gray_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	gray_00_01,6,8
	p_string	gray_00_02,6,9
	p_string	gray_00_03,6,11
	p_string	gray_00_04,6,12
	p_string	gray_00_05,6,13
	p_string	gray_00_06,6,14
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
	p_string	grid_00_00,29,5
	__ldwi		14
	call		_set_font_pal
	p_string	grid_00_01,6,7
	p_string	grid_00_02,6,8
	p_string	grid_00_03,6,9
	p_string	grid_00_04,6,10
	p_string	grid_00_05,6,12
	p_string	grid_00_06,6,13
	p_string	grid_00_07,6,14
	p_string	grid_00_08,6,16
	p_string	grid_00_09,6,17
	p_string	grid_00_10,6,18
	p_string	grid_00_11,6,20
	p_string	grid_00_12,6,21
	rts

grid_00_00:	.db	"GRID"
			.db	0
grid_00_01:	.db	"These grid tests change horizontal resolution"
			.db	0
grid_00_02:	.db	"between 256, 320 and 512. You can also vary the"
			.db	0
grid_00_03:	.db	"vertical resolution from the options menu. The"
			.db	0
grid_00_04:	.db	"pattern should fill all of the 4:3 screen area."
			.db	0
grid_00_05:	.db	"You can use it to verify that all the visible area"
			.db	0
grid_00_06:	.db	"is being displayed, and that there is no distortion"
			.db	0
grid_00_07:	.db	"present. Consumer TVs may not show the red squares."
			.db	0
grid_00_08:	.db	"The Grids are not perfectly centered within the"
			.db	0
grid_00_09:	.db	"NTSC signal, and that is normal. It is the way"
			.db	0
grid_00_10:	.db	"Hudson uses each resolution."
			.db	0
grid_00_11:	.db	"You can press button I to fill all the video signal"
			.db	0
grid_00_12:	.db	"background area."
			.db	0
#endasm
}


void display_gridscroll_00()
{
#asm
	p_string	gridscroll_00_00,23,5
	__ldwi		14
	call		_set_font_pal
	p_string	gridscroll_00_01,6,8
	p_string	gridscroll_00_02,6,9
	p_string	gridscroll_00_03,6,10
	p_string	gridscroll_00_04,6,11
	p_string	gridscroll_00_05,6,13
	p_string	gridscroll_00_06,6,14
	p_string	gridscroll_00_07,6,15
	p_string	gridscroll_00_08,6,16
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
gridscroll_00_05:	.db	"Select can be used to toggle between horizontal"
			.db	0
gridscroll_00_06:	.db	"and vertical, while the d-pad up or down"
			.db	0
gridscroll_00_07:	.db	"regulates speed. Button I stops the scroll and"
			.db	0
gridscroll_00_08:	.db	"d-pad left or right changes direction."
			.db	0
#endasm
}


void display_linearity_00()
{
#asm
	p_string	linearity_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	linearity_00_01,6,8
	p_string	linearity_00_02,6,9
	p_string	linearity_00_03,6,11
	p_string	linearity_00_04,6,12
	p_string	linearity_00_05,6,13
	p_string	linearity_00_06,6,14
	rts

linearity_00_00:	.db	"LINEARITY"
			.db	0
linearity_00_01:	.db	"This pattern shows 5 circles, that are matched"
			.db	0
linearity_00_02:	.db	"to the 10:11 NTSC pixel aspect ratio."
			.db	0
linearity_00_03:	.db	"The linearity of the display or upscaler can be"
			.db	0
linearity_00_04:	.db	"verified by measuring the diameter of the circles."
			.db	0
linearity_00_05:	.db	"Of course the linearity should be kept in all"
			.db	0
linearity_00_06:	.db	"directions."
			.db	0
#endasm
}


void display_linearity256_00()
{
#asm
	p_string	linearity256_00_00,22,5
	__ldwi		14
	call		_set_font_pal
	p_string	linearity256_00_01,6,8
	p_string	linearity256_00_02,6,9
	p_string	linearity256_00_03,6,10
	p_string	linearity256_00_04,6,12
	p_string	linearity256_00_05,6,13
	p_string	linearity256_00_06,6,14
	p_string	linearity256_00_07,6,15
	rts

linearity256_00_00:	.db	"LINEARITY 256"
			.db	0
linearity256_00_01:	.db	"This pattern shows 5 circles, that are matched"
			.db	0
linearity256_00_02:	.db	"to the 10:11 NTSC, but using the 256 horizontal"
			.db	0
linearity256_00_03:	.db	"resolution."
			.db	0
linearity256_00_04:	.db	"The linearity of the display or upscaler can be "
			.db	0
linearity256_00_05:	.db	"verified by measuring the diameter of the circles."
			.db	0
linearity256_00_06:	.db	"Of course the linearity should be kept in all"
			.db	0
linearity256_00_07:	.db	"directions, no distortion should be present."
			.db	0
#endasm
}


void display_manuallag_00()
{
#asm
	p_string	manuallag_00_00,20,5
	__ldwi		14
	call		_set_font_pal
	p_string	manuallag_00_01,6,7
	p_string	manuallag_00_02,6,8
	p_string	manuallag_00_03,6,9
	p_string	manuallag_00_04,6,10
	p_string	manuallag_00_05,6,12
	p_string	manuallag_00_06,6,13
	p_string	manuallag_00_07,6,14
	p_string	manuallag_00_08,6,15
	p_string	manuallag_00_09,6,17
	p_string	manuallag_00_10,6,18
	p_string	manuallag_00_11,6,19
	p_string	manuallag_00_12,6,20
	p_string	manuallag_00_13,6,21
	p_string	manuallag_00_14,6,22
	rts

manuallag_00_00:	.db	"MANUAL LAG TEST (1/2)"
			.db	0
manuallag_00_01:	.db	"The main intention is to show a changing pattern"
			.db	0
manuallag_00_02:	.db	"on the screen, which can be complemented with"
			.db	0
manuallag_00_03:	.db	"audio. This should show to some degree any lag"
			.db	0
manuallag_00_04:	.db	"when processing the signal."
			.db	0
manuallag_00_05:	.db	"As an added feature, the user can click button I"
			.db	0
manuallag_00_06:	.db	"when the sprite is aligned with the one on the"
			.db	0
manuallag_00_07:	.db	"background, and the offset in frames from the"
			.db	0
manuallag_00_08:	.db	"actual intersection will be shown on screen. "
			.db	0
manuallag_00_09:	.db	"A one frame 1khz tone is played when audio is"
			.db	0
manuallag_00_10:	.db	"enabled and the sprites are aligned, and a 500hz"
			.db	0
manuallag_00_11:	.db	"tone is played when button I is pressed. This"
			.db	0
manuallag_00_12:	.db	"can be repeated ten times and the software will"
			.db	0
manuallag_00_13:	.db	"calculate the average. Whenever the button was"
			.db	0
manuallag_00_14:	.db	"pressed before the actual intersection frame, the"
			.db	0
#endasm
}


void display_manuallag_01()
{
#asm
	p_string	manuallag_01_00,20,5
	__ldwi		14
	call		_set_font_pal
	p_string	manuallag_01_01,6,7
	p_string	manuallag_01_02,6,9
	p_string	manuallag_01_03,6,10
	p_string	manuallag_01_04,6,11
	p_string	manuallag_01_05,6,13
	p_string	manuallag_01_06,6,14
	p_string	manuallag_01_07,6,15
	p_string	manuallag_01_08,6,16
	p_string	manuallag_01_09,6,17
	p_string	manuallag_01_10,6,18
	p_string	manuallag_01_11,6,20
	p_string	manuallag_01_12,6,21
	p_string	manuallag_01_13,6,22
	rts

manuallag_01_00:	.db	"MANUAL LAG TEST (2/2)"
			.db	0
manuallag_01_01:	.db	"result will be ignored (but still shown on screen)."
			.db	0
manuallag_01_02:	.db	"SELECT can be used to change the direction"
			.db	0
manuallag_01_03:	.db	"of the sprite from vertical to horizontal, or"
			.db	0
manuallag_01_04:	.db	"display both at the same time"
			.db	0
manuallag_01_05:	.db	"Of course the evaluation is dependent on reflexes"
			.db	0
manuallag_01_06:	.db	"and/or rhythm more than anything. The visual and"
			.db	0
manuallag_01_07:	.db	"audio cues are the more revealing aspects which"
			.db	0
manuallag_01_08:	.db	"the user should consider, of course the interactive"
			.db	0
manuallag_01_09:	.db	"factor can give an experienced player the hang of"
			.db	0
manuallag_01_10:	.db	"the system when testing via different connections."
			.db	0
manuallag_01_11:	.db	"Since a frame is around 16 ms (1000/60) in NTSC"
			.db	0
manuallag_01_12:	.db	"that value must be considered the general error"
			.db	0
manuallag_01_13:	.db	"when using the test results."
			.db	0
#endasm
}


void display_options_00()
{
#asm
	p_string	options_00_00,27,5
	__ldwi		14
	call		_set_font_pal
	p_string	options_00_01,6,7
	p_string	options_00_02,6,8
	p_string	options_00_03,6,9
	p_string	options_00_04,6,10
	p_string	options_00_05,6,12
	p_string	options_00_06,6,13
	p_string	options_00_07,6,14
	p_string	options_00_08,6,15
	p_string	options_00_09,6,17
	p_string	options_00_10,6,18
	p_string	options_00_11,6,19
	p_string	options_00_12,6,20
	p_string	options_00_13,6,21
	p_string	options_00_14,6,23
	p_string	options_00_15,6,24
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
	p_string	overscan_00_00,27,5
	__ldwi		14
	call		_set_font_pal
	p_string	overscan_00_01,6,8
	p_string	overscan_00_02,6,9
	p_string	overscan_00_03,6,10
	p_string	overscan_00_04,6,12
	p_string	overscan_00_05,6,13
	p_string	overscan_00_06,6,14
	p_string	overscan_00_07,6,15
	p_string	overscan_00_08,6,17
	p_string	overscan_00_09,6,18
	p_string	overscan_00_10,6,20
	p_string	overscan_00_11,6,21
	p_string	overscan_00_12,6,22
	p_string	overscan_00_13,6,23
	rts

overscan_00_00:	.db	"OVERSCAN"
			.db	0
overscan_00_01:	.db	"With this pattern you can interactively find out"
			.db	0
overscan_00_02:	.db	"the overscan in pixels and percentage of each"
			.db	0
overscan_00_03:	.db	"edge in a display."
			.db	0
overscan_00_04:	.db	"Increment the overscan with left and right until"
			.db	0
overscan_00_05:	.db	"you see the white border, then go back one pixel."
			.db	0
overscan_00_06:	.db	"The resulting number is the amount of overscan"
			.db	0
overscan_00_07:	.db	"in pixels in each direction."
			.db	0
overscan_00_08:	.db	"The SELECT button allows faster movement of the"
			.db	0
overscan_00_09:	.db	"borders."
			.db	0
overscan_00_10:	.db	"This pattern follows the current video options,"
			.db	0
overscan_00_11:	.db	"so you can check how it handles in 239, 240, and"
			.db	0
overscan_00_12:	.db	"224 modes. Keep in mind the horizontal resolution"
			.db	0
overscan_00_13:	.db	"is fixed at 256 pixels."
			.db	0
#endasm
}


void display_pluge_00()
{
#asm
	p_string	pluge_00_00,25,5
	__ldwi		14
	call		_set_font_pal
	p_string	pluge_00_01,6,8
	p_string	pluge_00_02,6,9
	p_string	pluge_00_03,6,10
	p_string	pluge_00_04,6,12
	p_string	pluge_00_05,6,13
	p_string	pluge_00_06,6,14
	p_string	pluge_00_07,6,16
	p_string	pluge_00_08,6,17
	p_string	pluge_00_09,6,18
	p_string	pluge_00_10,6,20
	p_string	pluge_00_11,6,21
	p_string	pluge_00_12,6,22
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
pluge_00_07:	.db	"This adjustment ensures that anything intended"
			.db	0
pluge_00_08:	.db	"to be displayed is actually shown, and allows all"
			.db	0
pluge_00_09:	.db	"detail to be seen on dark scenes."
			.db	0
pluge_00_10:	.db	"Button I changes the inner bars between the"
			.db	0
pluge_00_11:	.db	"lowest B,G,R values and gray. You should use the"
			.db	0
pluge_00_12:	.db	"blue one in order not to crush detail."
			.db	0
#endasm
}


void display_pluge_01()
{
#asm
	p_string	pluge_01_00,25,5
	__ldwi		14
	call		_set_font_pal
	p_string	pluge_01_01,6,8
	p_string	pluge_01_02,6,9
	p_string	pluge_01_03,6,10
	p_string	pluge_01_04,6,11
	p_string	pluge_01_05,6,13
	p_string	pluge_01_06,6,14
	p_string	pluge_01_07,6,15
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
pluge_01_06:	.db	"in other game consoles, the are just the lowest"
			.db	0
pluge_01_07:	.db	"values available in this system."
			.db	0
#endasm
}


void display_scroll_00()
{
#asm
	p_string	scroll_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	scroll_00_01,6,8
	p_string	scroll_00_02,6,9
	p_string	scroll_00_03,6,11
	p_string	scroll_00_04,6,12
	p_string	scroll_00_05,6,13
	p_string	scroll_00_06,6,14
	p_string	scroll_00_07,6,16
	p_string	scroll_00_08,6,17
	p_string	scroll_00_09,6,19
	p_string	scroll_00_10,6,20
	rts

scroll_00_00:	.db	"SCROLL TEST"
			.db	0
scroll_00_01:	.db	"This test shows either an horizontal background"
			.db	0
scroll_00_02:	.db	"from Sonic or a vertical one from Kiki Kaikai."
			.db	0
scroll_00_03:	.db	"Speed can be varied with up & down on the"
			.db	0
scroll_00_04:	.db	"d-pad and scroll direction with left or right."
			.db	0
scroll_00_05:	.db	"Button I stops the screen and SELECT toggles"
			.db	0
scroll_00_06:	.db	"between vertical and horizontal scroll."
			.db	0
scroll_00_07:	.db	"This can be used to notice any drops in"
			.db	0
scroll_00_08:	.db	"framerate, or pixel width inconsistencies."
			.db	0
scroll_00_09:	.db	"Sonic is a trademark of Sega Enterprises Ltd."
			.db	0
scroll_00_10:	.db	"Kiki Kaikai is a trademark of Namco Bandai."
			.db	0
#endasm
}


void display_sharpness_00()
{
#asm
	p_string	sharpness_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	sharpness_00_01,6,8
	p_string	sharpness_00_02,6,9
	p_string	sharpness_00_03,6,10
	p_string	sharpness_00_04,6,12
	p_string	sharpness_00_05,6,13
	p_string	sharpness_00_06,6,14
	p_string	sharpness_00_07,6,16
	p_string	sharpness_00_08,6,17
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
#endasm
}


void display_sound_00()
{
#asm
	p_string	sound_00_00,28,5
	__ldwi		14
	call		_set_font_pal
	p_string	sound_00_01,6,9
	p_string	sound_00_02,6,10
	p_string	sound_00_03,6,12
	p_string	sound_00_04,6,13
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


void display_striped_00()
{
#asm
	p_string	striped_00_00,21,5
	__ldwi		14
	call		_set_font_pal
	p_string	striped_00_01,6,8
	p_string	striped_00_02,6,9
	p_string	striped_00_03,6,10
	p_string	striped_00_04,6,11
	p_string	striped_00_05,6,12
	p_string	striped_00_06,6,14
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
striped_00_06:	.db	"You can change backgrounds with I."
			.db	0
#endasm
}


void display_white_00()
{
#asm
	p_string	white_00_00,24,5
	__ldwi		14
	call		_set_font_pal
	p_string	white_00_01,6,8
	p_string	white_00_02,6,9
	p_string	white_00_03,6,10
	p_string	white_00_04,6,12
	p_string	white_00_05,6,13
	p_string	white_00_06,6,14
	p_string	white_00_07,6,16
	p_string	white_00_08,6,17
	p_string	white_00_09,6,19
	p_string	white_00_10,6,20
	rts

white_00_00:	.db	"WHITE SCREEN"
			.db	0
white_00_01:	.db	"This pattern can be changed between white, black,"
			.db	0
white_00_02:	.db	"red, green, blue and custom color full screens by"
			.db	0
white_00_03:	.db	"using button I."
			.db	0
white_00_04:	.db	"Pressing Select while displaying the white screen"
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
white_00_10:	.db	"purity, and for dead pixels in some displays."
			.db	0
#endasm
}



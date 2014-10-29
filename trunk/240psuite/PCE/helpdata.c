void display_backlit_00()
{
#asm
	p_string	backlit_00_00,24,5
	__ldwi		14
	call		_set_font_pal
	p_string	backlit_00_01,6,7
	p_string	backlit_00_02,6,8
	p_string	backlit_00_03,6,9
	p_string	backlit_00_04,6,10
	p_string	backlit_00_05,6,11
	p_string	backlit_00_06,6,13
	p_string	backlit_00_07,6,14
	p_string	backlit_00_08,6,15
	p_string	backlit_00_09,6,16
	p_string	backlit_00_10,6,17
	p_string	backlit_00_11,6,19
	p_string	backlit_00_12,6,20
	p_string	backlit_00_13,6,21
	p_string	backlit_00_14,6,22
	rts

backlit_00_00:	.db	"BACKLIT TEST"
			.db	0
backlit_00_01:	.db	"This test allows you to check how the display's"
			.db	0
backlit_00_02:	.db	"backlit works when only a small array of pixels"
			.db	0
backlit_00_03:	.db	"is shown. This can be very revealing, and can"
			.db	0
backlit_00_04:	.db	"be used to detect the zones in LED backlit"
			.db	0
backlit_00_05:	.db	"displays, or to evaluate global and local dimming."
			.db	0
backlit_00_06:	.db	"The user can move around the white pixel arrays"
			.db	0
backlit_00_07:	.db	"with the d-pad or joystiq, and change the size"
			.db	0
backlit_00_08:	.db	"of the pixel array with button I. The SELECT"
			.db	0
backlit_00_09:	.db	"button allows the user to hide the pixel array"
			.db	0
backlit_00_10:	.db	"in order to alternate a fully black screen."
			.db	0
backlit_00_11:	.db	"It might be necessary to have a fully-off panel"
			.db	0
backlit_00_12:	.db	"(if possible) when displaying the black"
			.db	0
backlit_00_13:	.db	"background,  in order to evaluate full-backlit"
			.db	0
backlit_00_14:	.db	"displays by adjusting brightness accordingly"
			.db	0
backlit_00_15:	.db	"for this test."
			.db	0
#endasm
}


void display_bleed_00()
{
#asm
	p_string	bleed_00_00,25,5
	__ldwi		14
	call		_set_font_pal
	p_string	bleed_00_01,6,7
	p_string	bleed_00_02,6,8
	p_string	bleed_00_03,6,9
	p_string	bleed_00_04,6,11
	p_string	bleed_00_05,6,12
	rts

bleed_00_00:	.db	"COLOR BLEED "
			.db	0
bleed_00_01:	.db	"This pattern helps diagnose unneeded color"
			.db	0
bleed_00_02:	.db	"upsampling. It consists of one pixel width color"
			.db	0
bleed_00_03:	.db	"bars alternating with one pixel width black bars."
			.db	0
bleed_00_04:	.db	"You can change the vertical bars to a checker"
			.db	0
bleed_00_05:	.db	"board with button I."
			.db	0
#endasm
}


void display_check_00()
{
#asm
	p_string	check_00_00,24,5
	__ldwi		14
	call		_set_font_pal
	p_string	check_00_01,6,7
	p_string	check_00_02,6,8
	p_string	check_00_03,6,10
	p_string	check_00_04,6,11
	p_string	check_00_05,6,13
	p_string	check_00_06,6,14
	p_string	check_00_07,6,15
	p_string	check_00_08,6,16
	p_string	check_00_09,6,17
	p_string	check_00_10,6,19
	rts

check_00_00:	.db	"CHECKERBOARD"
			.db	0
check_00_01:	.db	"This pattern shows all the visible pixels in an"
			.db	0
check_00_02:	.db	"alternating white and black grid array."
			.db	0
check_00_03:	.db	"You can toggle the pattern with SELECT, or turn"
			.db	0
check_00_04:	.db	"on auto-toggle each frame with the I button."
			.db	0
check_00_05:	.db	"When auto-toggle is set, you should see it"
			.db	0
check_00_06:	.db	"alternating rapidly. On some setups, the pattern"
			.db	0
check_00_07:	.db	"doesn't change at all. This means that the signal"
			.db	0
check_00_08:	.db	"is being treated as 480i/576i and odd or even "
			.db	0
check_00_09:	.db	"frames are being discarded completely."
			.db	0
check_00_10:	.db	"A frame counter can be displayed on screen by"
			.db	0
check_00_11:	.db	"pressing left on the d-pad."
			.db	0
#endasm
}


void display_color601_00()
{
#asm
	p_string	color601_00_00,18,5
	__ldwi		14
	call		_set_font_pal
	p_string	color601_00_01,6,7
	p_string	color601_00_02,6,8
	p_string	color601_00_03,6,10
	p_string	color601_00_04,6,11
	p_string	color601_00_05,6,12
	p_string	color601_00_06,6,13
	rts

color601_00_00:	.db	"COLOR BARS WITH GRAY SCALE"
			.db	0
color601_00_01:	.db	"This pattern idea and layout borrows from the "
			.db	0
color601_00_02:	.db	"Digital Video Essentials color adjust pattern."
			.db	0
color601_00_03:	.db	"It has a gray background as reference, and can "
			.db	0
color601_00_04:	.db	"be used with color filters in order to adjust "
			.db	0
color601_00_05:	.db	"color accuracy. If your display set has a blue "
			.db	0
color601_00_06:	.db	"only mode, you can also use that to correct "
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
	p_string	colors_00_01,6,7
	p_string	colors_00_02,6,8
	p_string	colors_00_03,6,10
	p_string	colors_00_04,6,11
	p_string	colors_00_05,6,12
	p_string	colors_00_06,6,13
	p_string	colors_00_07,6,15
	p_string	colors_00_08,6,16
	p_string	colors_00_09,6,17
	p_string	colors_00_10,6,18
	p_string	colors_00_11,6,19
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
colors_00_05:	.db	"level correctly with the PLUGE, the rectangles "
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
	p_string	colors_01_01,6,7
	p_string	colors_01_02,6,8
	p_string	colors_01_03,6,9
	p_string	colors_01_04,6,10
	p_string	colors_01_05,6,11
	p_string	colors_01_06,6,12
	p_string	colors_01_07,6,14
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
colors_01_06:	.db	"you can distinguish them clearly. "
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
	p_string	dshadow_00_01,6,7
	p_string	dshadow_00_02,6,8
	p_string	dshadow_00_03,6,9
	p_string	dshadow_00_04,6,10
	p_string	dshadow_00_05,6,11
	p_string	dshadow_00_06,6,12
	p_string	dshadow_00_07,6,13
	p_string	dshadow_00_08,6,15
	p_string	dshadow_00_09,6,16
	rts

dshadow_00_00:	.db	"DROP SHADOW TEST "
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
dshadow_00_07:	.db	"should be lost and the shadow should be visible. "
			.db	0
dshadow_00_08:	.db	"The user can toggle the frame used to draw the"
			.db	0
dshadow_00_09:	.db	"shadow with SELECT. Backgrounds can be"
			.db	0
dshadow_00_10:	.db	"switched with button I."
			.db	0
#endasm
}


void display_general_00()
{
#asm
	p_string	general_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	general_00_01,6,7
	p_string	general_00_02,6,8
	p_string	general_00_03,6,10
	p_string	general_00_04,6,11
	p_string	general_00_05,6,13
	p_string	general_00_06,6,14
	p_string	general_00_07,6,15
	p_string	general_00_08,6,17
	p_string	general_00_09,6,18
	p_string	general_00_10,6,19
	p_string	general_00_11,6,20
	p_string	general_00_12,6,22
	p_string	general_00_13,6,23
	rts

general_00_00:	.db	"HELP (1/2)"
			.db	0
general_00_01:	.db	"The 240p Test Suite was designed with two goals"
			.db	0
general_00_02:	.db	"in mind:"
			.db	0
general_00_03:	.db	"1) Evaluate 240p signal processing on modern TV"
			.db	0
general_00_04:	.db	"sets and video processing equipment; and "
			.db	0
general_00_05:	.db	"2) Provide calibration patterns generated on your"
			.db	0
general_00_06:	.db	"game console to help in properly calibrating the"
			.db	0
general_00_07:	.db	"display's black, white and color levels."
			.db	0
general_00_08:	.db	"Help is available everywhere by pressing RUN."
			.db	0
general_00_09:	.db	"Use right and left in the d-pad to navigate between"
			.db	0
general_00_10:	.db	"help pages. Options can be changed via SELECT"
			.db	0
general_00_11:	.db	"when in the main menus."
			.db	0
general_00_12:	.db	"The PC Engine version of the suite is"
			.db	0
general_00_13:	.db	"320x240 by default, but can be changed to 320x224p."
			.db	0
general_00_14:	.db	"Grids and other screens are also available in "
			.db	0
#endasm
}


void display_general_01()
{
#asm
	p_string	general_01_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	general_01_01,6,7
	p_string	general_01_02,6,8
	p_string	general_01_03,6,9
	p_string	general_01_04,6,10
	p_string	general_01_05,6,12
	p_string	general_01_06,6,13
	p_string	general_01_07,6,14
	p_string	general_01_08,6,15
	p_string	general_01_09,6,16
	p_string	general_01_10,6,18
	p_string	general_01_11,6,19
	p_string	general_01_12,6,20
	rts

general_01_00:	.db	"HELP (2/2)"
			.db	0
general_01_01:	.db	"256x224, 256x240, 512x224 and 512x240 resolutions."
			.db	0
general_01_02:	.db	"When in 240p mode the system can start drawing"
			.db	0
general_01_03:	.db	"from video line 22, which is the default of this"
			.db	0
general_01_04:	.db	"suite in order to display all 240 lines."
			.db	0
general_01_05:	.db	"However all games start from line 24 when in 240p"
			.db	0
general_01_06:	.db	"mode, showing only 239 visible lines. You can"
			.db	0
general_01_07:	.db	"select that mode from the options menu in order to"
			.db	0
general_01_08:	.db	"display the regular signal you'll encounter when"
			.db	0
general_01_09:	.db	"using this console."
			.db	0
general_01_10:	.db	"The 240p suite is also available in other systems,"
			.db	0
general_01_11:	.db	"including: Sega Genesis/Mega Drive, Sega CD/"
			.db	0
general_01_12:	.db	"Mega CD, Super Nintendo, Nintendo GameCube,"
			.db	0
general_01_13:	.db	"Nintendo Wii and Sega Dreamcast."
			.db	0
#endasm
}


void display_gray_00()
{
#asm
	p_string	gray_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	gray_00_01,6,7
	p_string	gray_00_02,6,8
	p_string	gray_00_03,6,10
	p_string	gray_00_04,6,11
	p_string	gray_00_05,6,12
	p_string	gray_00_06,6,13
	p_string	gray_00_07,6,15
	p_string	gray_00_08,6,16
	p_string	gray_00_09,6,17
	p_string	gray_00_10,6,18
	p_string	gray_00_11,6,19
	rts

gray_00_00:	.db	"GRAY RAMP "
			.db	0
gray_00_01:	.db	"This gray ramp pattern can be used to check color"
			.db	0
gray_00_02:	.db	"balance and to adjust the white level. "
			.db	0
gray_00_03:	.db	"You should make sure the bars are gray, with no"
			.db	0
gray_00_04:	.db	"red or blue bias. This can be adjusted with the"
			.db	0
gray_00_05:	.db	"individual color settings, or the tint control"
			.db	0
gray_00_06:	.db	"in NTSC displays."
			.db	0
gray_00_07:	.db	"In order to adjust white level, you need to use"
			.db	0
gray_00_08:	.db	"the contrast control of your TV set. Raise the"
			.db	0
gray_00_09:	.db	"contrast to the point where the white bars on the"
			.db	0
gray_00_10:	.db	"sides are undistinguishable from each other, and "
			.db	0
gray_00_11:	.db	"then lower it to the point where you can clearly"
			.db	0
gray_00_12:	.db	"identify both."
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
	p_string	grid_00_03,6,10
	p_string	grid_00_04,6,11
	p_string	grid_00_05,6,12
	p_string	grid_00_06,6,14
	p_string	grid_00_07,6,15
	p_string	grid_00_08,6,17
	p_string	grid_00_09,6,18
	rts

grid_00_00:	.db	"GRID"
			.db	0
grid_00_01:	.db	"This grid uses a 320x240 pattern, and should fill"
			.db	0
grid_00_02:	.db	"all of the 4:3 visible area. "
			.db	0
grid_00_03:	.db	"You can use it to verify that all the visible area"
			.db	0
grid_00_04:	.db	"is being displayed, and that there is no distortion"
			.db	0
grid_00_05:	.db	"present. Consumer TVs may not show the red squares."
			.db	0
grid_00_06:	.db	"The 240 test suite draws from line 22 onwards from "
			.db	0
grid_00_07:	.db	"the blanking interval of NTSC signals"
			.db	0
grid_00_08:	.db	"You can press button I to fill all the video signal"
			.db	0
grid_00_09:	.db	"background area."
			.db	0
#endasm
}


void display_grid224_00()
{
#asm
	p_string	grid224_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	grid224_00_01,6,7
	p_string	grid224_00_02,6,8
	p_string	grid224_00_03,6,9
	p_string	grid224_00_04,6,10
	p_string	grid224_00_05,6,12
	p_string	grid224_00_06,6,13
	p_string	grid224_00_07,6,14
	p_string	grid224_00_08,6,16
	rts

grid224_00_00:	.db	"GRID 224p"
			.db	0
grid224_00_01:	.db	"This grid uses a 320x224 pattern in a 224p signal."
			.db	0
grid224_00_02:	.db	"This vertical resolution is also used by other"
			.db	0
grid224_00_03:	.db	"consoles, such as the SNES, Sega Genesis and"
			.db	0
grid224_00_04:	.db	"Playstation."
			.db	0
grid224_00_05:	.db	"You can use it to verify that all the visible area"
			.db	0
grid224_00_06:	.db	"is being displayed, and that there is no distortion"
			.db	0
grid224_00_07:	.db	"present. Consumer TVs may not show the red squares."
			.db	0
grid224_00_08:	.db	"You can press button I to fill all the video signal"
			.db	0
grid224_00_09:	.db	"background area."
			.db	0
#endasm
}


void display_gridscroll_00()
{
#asm
	p_string	gridscroll_00_00,23,5
	__ldwi		14
	call		_set_font_pal
	p_string	gridscroll_00_01,6,7
	p_string	gridscroll_00_02,6,8
	p_string	gridscroll_00_03,6,9
	p_string	gridscroll_00_04,6,10
	p_string	gridscroll_00_05,6,12
	p_string	gridscroll_00_06,6,13
	p_string	gridscroll_00_07,6,14
	p_string	gridscroll_00_08,6,15
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
gridscroll_00_08:	.db	"d-pad left or right changes direction. "
			.db	0
#endasm
}


void display_ire100_00()
{
#asm
	p_string	ire100_00_00,27,5
	__ldwi		14
	call		_set_font_pal
	p_string	ire100_00_01,6,7
	p_string	ire100_00_02,6,8
	p_string	ire100_00_03,6,9
	p_string	ire100_00_04,6,11
	p_string	ire100_00_05,6,12
	p_string	ire100_00_06,6,14
	p_string	ire100_00_07,6,15
	p_string	ire100_00_08,6,16
	p_string	ire100_00_09,6,17
	p_string	ire100_00_10,6,18
	p_string	ire100_00_11,6,20
	p_string	ire100_00_12,6,21
	p_string	ire100_00_13,6,23
	rts

ire100_00_00:	.db	"100 IRE"
			.db	0
ire100_00_01:	.db	"An IRE is a unit used in the measurement of"
			.db	0
ire100_00_02:	.db	"composite video signals. Its name is derived from"
			.db	0
ire100_00_03:	.db	"the initials of the Institute of Radio Engineers."
			.db	0
ire100_00_04:	.db	"A value of 100 IRE was originally defined to be"
			.db	0
ire100_00_05:	.db	"the range from black to white in a video signal."
			.db	0
ire100_00_06:	.db	"A value of 0 IRE corresponds to the zero voltage"
			.db	0
ire100_00_07:	.db	"value during the blanking period. The sync pulse"
			.db	0
ire100_00_08:	.db	"is normally 40 IRE below the zero value, so, peak"
			.db	0
ire100_00_09:	.db	"to peak, an all white signal should be equal to"
			.db	0
ire100_00_10:	.db	"140 IRE."
			.db	0
ire100_00_11:	.db	"You can press left and right to advance in IRE"
			.db	0
ire100_00_12:	.db	"steps in the selected range."
			.db	0
ire100_00_13:	.db	"Button I toggles the range between [0-100] and"
			.db	0
ire100_00_14:	.db	"[100-140] IRE."
			.db	0
#endasm
}


void display_linearity_00()
{
#asm
	p_string	linearity_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	linearity_00_01,6,7
	p_string	linearity_00_02,6,8
	p_string	linearity_00_03,6,10
	p_string	linearity_00_04,6,11
	p_string	linearity_00_05,6,12
	rts

linearity_00_00:	.db	"LINEARITY"
			.db	0
linearity_00_01:	.db	"This pattern shows 5 circles, that are matched"
			.db	0
linearity_00_02:	.db	"to the 10:11 NTSC pixel aspect ratio."
			.db	0
linearity_00_03:	.db	"The linearity of the display or upscaler can be "
			.db	0
linearity_00_04:	.db	"verified by measuring the diameter of the circles."
			.db	0
linearity_00_05:	.db	"Of course the linearity should be kept in all "
			.db	0
linearity_00_06:	.db	"directions."
			.db	0
#endasm
}


void display_linearity224_00()
{
#asm
	p_string	linearity224_00_00,22,5
	__ldwi		14
	call		_set_font_pal
	p_string	linearity224_00_01,6,7
	p_string	linearity224_00_02,6,8
	p_string	linearity224_00_03,6,10
	p_string	linearity224_00_04,6,11
	p_string	linearity224_00_05,6,12
	rts

linearity224_00_00:	.db	"LINEARITY 224p"
			.db	0
linearity224_00_01:	.db	"This pattern shows 5 circles, that are matched"
			.db	0
linearity224_00_02:	.db	"to the 10:11 NTSC, but limited to a 320x224 mode."
			.db	0
linearity224_00_03:	.db	"The linearity of the display or upscaler can be "
			.db	0
linearity224_00_04:	.db	"verified by measuring the diameter of the circles."
			.db	0
linearity224_00_05:	.db	"Of course the linearity should be kept in all "
			.db	0
linearity224_00_06:	.db	"directions."
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
	p_string	manuallag_00_05,6,11
	p_string	manuallag_00_06,6,13
	p_string	manuallag_00_07,6,14
	p_string	manuallag_00_08,6,15
	p_string	manuallag_00_09,6,16
	p_string	manuallag_00_10,6,17
	p_string	manuallag_00_11,6,18
	p_string	manuallag_00_12,6,19
	p_string	manuallag_00_13,6,20
	p_string	manuallag_00_14,6,21
	p_string	manuallag_00_15,6,22
	p_string	manuallag_00_16,6,23
	rts

manuallag_00_00:	.db	"MANUAL LAG TEST (1/2)"
			.db	0
manuallag_00_01:	.db	"The main intention is to show a changing pattern"
			.db	0
manuallag_00_02:	.db	"on the screen, which can be complemented with "
			.db	0
manuallag_00_03:	.db	"audio and vibration on the controller if set. "
			.db	0
manuallag_00_04:	.db	"This should show to some degree any lag when  "
			.db	0
manuallag_00_05:	.db	"processing the signal."
			.db	0
manuallag_00_06:	.db	"As an added feature, the user can click the $A"
			.db	0
manuallag_00_07:	.db	"button when the sprite is aligned with the one on"
			.db	0
manuallag_00_08:	.db	"the background, and the offset in frames from the"
			.db	0
manuallag_00_09:	.db	"actual intersection will be shown on screen. This"
			.db	0
manuallag_00_10:	.db	"can be repeated ten times and the software will"
			.db	0
manuallag_00_11:	.db	"calculate the average. Whenever the button was"
			.db	0
manuallag_00_12:	.db	"pressed before the actual intersection frame, the"
			.db	0
manuallag_00_13:	.db	"result will be ignored (but still shown onscreen)."
			.db	0
manuallag_00_14:	.db	"Button $X can be used to change the direction"
			.db	0
manuallag_00_15:	.db	"of the sprite from vertical to horizontal, or"
			.db	0
manuallag_00_16:	.db	"display both at the same time."
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
	p_string	manuallag_01_02,6,8
	p_string	manuallag_01_03,6,9
	p_string	manuallag_01_04,6,10
	p_string	manuallag_01_05,6,11
	p_string	manuallag_01_06,6,12
	p_string	manuallag_01_07,6,14
	p_string	manuallag_01_08,6,15
	p_string	manuallag_01_09,6,16
	rts

manuallag_01_00:	.db	"MANUAL LAG TEST (2/2)"
			.db	0
manuallag_01_01:	.db	"Of course the evaluation is dependent on reflexes"
			.db	0
manuallag_01_02:	.db	"and/or rhythm more than anything. The visual and"
			.db	0
manuallag_01_03:	.db	"audio cues are the more revealing aspects which "
			.db	0
manuallag_01_04:	.db	"the user should consider, of course the interactive"
			.db	0
manuallag_01_05:	.db	"factor can give an experienced player the hang of"
			.db	0
manuallag_01_06:	.db	"the system when testing via different connections."
			.db	0
manuallag_01_07:	.db	"Since a frame is around 16 ms (1000/60) in NTSC "
			.db	0
manuallag_01_08:	.db	"that value must be considered the general error"
			.db	0
manuallag_01_09:	.db	"when using the test results. "
			.db	0
#endasm
}


void display_options_00()
{
#asm
	p_string	options_00_00,24,5
	__ldwi		14
	call		_set_font_pal
	p_string	options_00_01,6,7
	p_string	options_00_02,10,8
	p_string	options_00_03,10,9
	p_string	options_00_04,10,10
	p_string	options_00_05,10,11
	p_string	options_00_06,10,12
	p_string	options_00_07,10,13
	p_string	options_00_08,10,14
	p_string	options_00_09,10,15
	p_string	options_00_10,10,16
	p_string	options_00_11,10,17
	p_string	options_00_12,10,18
	p_string	options_00_13,10,19
	p_string	options_00_14,6,21
	p_string	options_00_15,10,22
	rts

options_00_00:	.db	"OPTIONS (1/2)"
			.db	0
options_00_01:	.db	"#GVI Trap Filter (Composite):#G A Trap filter is"
			.db	0
options_00_02:	.db	"generally used to improve #YLuma#Y/#YChroma#Y"
			.db	0
options_00_03:	.db	"separation in the #Ycomposite#Y video signal. When "
			.db	0
options_00_04:	.db	"disabled, the video signal is unfiltered, "
			.db	0
options_00_05:	.db	"which sometimes produces visual artefacts such "
			.db	0
options_00_06:	.db	"as color bleeding. A notch filter centered at "
			.db	0
options_00_07:	.db	"the color subcarrier frequency is used to "
			.db	0
options_00_08:	.db	"extract color from 2.5MHz to 4.5MHz.This method "
			.db	0
options_00_09:	.db	"has two drawbacks. First, it limits the color "
			.db	0
options_00_10:	.db	"bandwidth (which extends to 2.1MHz) cutting out"
			.db	0
options_00_11:	.db	"fine color detail. Second, luminance info in "
			.db	0
options_00_12:	.db	"the notch frequency range contaminates color "
			.db	0
options_00_13:	.db	"and generates unwanted color or rainbows."
			.db	0
options_00_14:	.db	"#GGX Deflickering Filter:#G A filter that removes"
			.db	0
options_00_15:	.db	"ficker in interlaced modes by softening the"
			.db	0
options_00_16:	.db	"rendered image."
			.db	0
#endasm
}


void display_overscan_00()
{
#asm
	p_string	overscan_00_00,27,5
	__ldwi		14
	call		_set_font_pal
	p_string	overscan_00_01,6,7
	p_string	overscan_00_02,6,8
	p_string	overscan_00_03,6,9
	p_string	overscan_00_04,6,11
	p_string	overscan_00_05,6,12
	p_string	overscan_00_06,6,13
	p_string	overscan_00_07,6,14
	rts

overscan_00_00:	.db	"OVERSCAN"
			.db	0
overscan_00_01:	.db	"With this pattern you can interactively find out"
			.db	0
overscan_00_02:	.db	"the overscan in pixels and percentage of each"
			.db	0
overscan_00_03:	.db	"edge in a display. "
			.db	0
overscan_00_04:	.db	"Increment the overscan with left and right until"
			.db	0
overscan_00_05:	.db	"you see the white border, then go back one pixel."
			.db	0
overscan_00_06:	.db	"The resulting number is the amount of overscan"
			.db	0
overscan_00_07:	.db	"in pixels in each direction."
			.db	0
overscan_00_08:	.db	"This pattern follows the current video options."
			.db	0
#endasm
}


void display_passivelag_00()
{
#asm
	p_string	passivelag_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	passivelag_00_01,6,7
	p_string	passivelag_00_02,6,8
	p_string	passivelag_00_03,6,9
	p_string	passivelag_00_04,6,10
	p_string	passivelag_00_05,6,12
	p_string	passivelag_00_06,6,13
	p_string	passivelag_00_07,6,14
	p_string	passivelag_00_08,6,16
	p_string	passivelag_00_09,6,17
	p_string	passivelag_00_10,6,19
	p_string	passivelag_00_11,6,20
	p_string	passivelag_00_12,6,22
	p_string	passivelag_00_13,6,23
	rts

passivelag_00_00:	.db	"LAG TEST"
			.db	0
passivelag_00_01:	.db	"This test is designed to be used with two displays"
			.db	0
passivelag_00_02:	.db	"conected at the same time. One being a CRT, or a"
			.db	0
passivelag_00_03:	.db	"display with a known lag as reference, and the"
			.db	0
passivelag_00_04:	.db	"other the display to test."
			.db	0
passivelag_00_05:	.db	"Using a camera, a picture should be taken of both"
			.db	0
passivelag_00_06:	.db	"screens at the same time. The picture will show "
			.db	0
passivelag_00_07:	.db	"the frame discrepancy between them. "
			.db	0
passivelag_00_08:	.db	"The circles in the bottom help determine the frame"
			.db	0
passivelag_00_09:	.db	"even when the numbers are blurry."
			.db	0
passivelag_00_10:	.db	"The Sega Genesis version of the test can be used"
			.db	0
passivelag_00_11:	.db	"with a Nomad as the reference display."
			.db	0
passivelag_00_12:	.db	"You can also chain the output ports of a PVM/BVM"
			.db	0
passivelag_00_13:	.db	"to an upscaler and use the CRT as reference, or"
			.db	0
passivelag_00_14:	.db	"split the video signal."
			.db	0
#endasm
}


void display_pluge_00()
{
#asm
	p_string	pluge_00_00,25,5
	__ldwi		14
	call		_set_font_pal
	p_string	pluge_00_01,6,7
	p_string	pluge_00_02,6,8
	p_string	pluge_00_03,6,9
	p_string	pluge_00_04,6,11
	p_string	pluge_00_05,6,12
	p_string	pluge_00_06,6,13
	p_string	pluge_00_07,6,15
	p_string	pluge_00_08,6,16
	p_string	pluge_00_09,6,17
	p_string	pluge_00_10,6,19
	p_string	pluge_00_11,6,20
	p_string	pluge_00_12,6,21
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
pluge_00_05:	.db	"adjust the black level. The control for setting "
			.db	0
pluge_00_06:	.db	"this on your TV is usually named Brightness."
			.db	0
pluge_00_07:	.db	"This adjustment ensures that anything intended"
			.db	0
pluge_00_08:	.db	"to be black is actually black, and allows all"
			.db	0
pluge_00_09:	.db	"detail to be seen on dark scenes."
			.db	0
pluge_00_10:	.db	"The button I changes the inner bars between gray"
			.db	0
pluge_00_11:	.db	"and the lowest R,G,B values, which are lower in"
			.db	0
pluge_00_12:	.db	"luminance in order not to crush detail."
			.db	0
#endasm
}


void display_pluge_01()
{
#asm
	p_string	pluge_01_00,25,5
	__ldwi		14
	call		_set_font_pal
	p_string	pluge_01_01,6,6
	p_string	pluge_01_02,6,7
	p_string	pluge_01_03,6,8
	p_string	pluge_01_04,6,9
	p_string	pluge_01_05,6,11
	p_string	pluge_01_06,6,12
	rts

pluge_01_00:	.db	"PLUGE (2/2)                                         "
			.db	0
pluge_01_01:	.db	"Reduce the Brightness until the inner bars"
			.db	0
pluge_01_02:	.db	"are no longer visible, and then slowly raise it"
			.db	0
pluge_01_03:	.db	"until they are just visible, and go back one"
			.db	0
pluge_01_04:	.db	"notch where they blend with the background. "
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
	p_string	scroll_00_01,6,7
	p_string	scroll_00_02,6,8
	p_string	scroll_00_03,6,9
	p_string	scroll_00_04,6,10
	p_string	scroll_00_05,6,12
	p_string	scroll_00_06,6,13
	p_string	scroll_00_07,6,15
	rts

scroll_00_00:	.db	"SCROLL TEST"
			.db	0
scroll_00_01:	.db	"This test shows a two layer background from Sonic"
			.db	0
scroll_00_02:	.db	"the Hedgehog. The speed can be varied with the"
			.db	0
scroll_00_03:	.db	"d-pad, and change direction with the Select"
			.db	0
scroll_00_04:	.db	"Button I stops the scroll. "
			.db	0
scroll_00_05:	.db	"This can be used to notice any drops in framerate"
			.db	0
scroll_00_06:	.db	"when using a video processor."
			.db	0
scroll_00_07:	.db	"Sonic The Hedgehog is a trademark of "
			.db	0
scroll_00_08:	.db	"Sega Enterprises Ltd."
			.db	0
#endasm
}


void display_sharpness_00()
{
#asm
	p_string	sharpness_00_00,26,5
	__ldwi		14
	call		_set_font_pal
	p_string	sharpness_00_01,6,7
	p_string	sharpness_00_02,6,8
	p_string	sharpness_00_03,6,9
	p_string	sharpness_00_04,6,11
	p_string	sharpness_00_05,6,12
	p_string	sharpness_00_06,6,13
	p_string	sharpness_00_07,6,15
	rts

sharpness_00_00:	.db	"SHARPNESS"
			.db	0
sharpness_00_01:	.db	"You should set the sharpness of your CRT to a"
			.db	0
sharpness_00_02:	.db	"value that shows clean black and gray transitions,"
			.db	0
sharpness_00_03:	.db	"with no white ghosting in between."
			.db	0
sharpness_00_04:	.db	"On most modern displays, the sharpness control is "
			.db	0
sharpness_00_05:	.db	"an edge-enhancement control, and most probably"
			.db	0
sharpness_00_06:	.db	"should be set to zero, or in the middle."
			.db	0
sharpness_00_07:	.db	"In some PVM/BVM displays this is also referred "
			.db	0
sharpness_00_08:	.db	"to as aperture."
			.db	0
#endasm
}


void display_SMPTEColor_00()
{
#asm
	p_string	SMPTEColor_00_00,22,5
	__ldwi		14
	call		_set_font_pal
	p_string	SMPTEColor_00_01,6,7
	p_string	SMPTEColor_00_02,6,8
	p_string	SMPTEColor_00_03,6,9
	p_string	SMPTEColor_00_04,6,11
	rts

SMPTEColor_00_00:	.db	"SMPTE COLOR BARS"
			.db	0
SMPTEColor_00_01:	.db	"This pattern can be used to calibrate for NTSC"
			.db	0
SMPTEColor_00_02:	.db	"levels regarding contrast and brightness, and "
			.db	0
SMPTEColor_00_03:	.db	"appropiate colors as well. "
			.db	0
SMPTEColor_00_04:	.db	"You can toggle between 75% and 100% SMPTE color "
			.db	0
SMPTEColor_00_05:	.db	"bars by pressing I."
			.db	0
#endasm
}


void display_sound_00()
{
#asm
	p_string	sound_00_00,28,5
	__ldwi		14
	call		_set_font_pal
	p_string	sound_00_01,6,7
	p_string	sound_00_02,6,8
	p_string	sound_00_03,6,10
	p_string	sound_00_04,6,11
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
	p_string	striped_00_01,6,7
	p_string	striped_00_02,6,8
	p_string	striped_00_03,6,9
	p_string	striped_00_04,6,10
	p_string	striped_00_05,6,11
	p_string	striped_00_06,6,13
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


void display_stripes_00()
{
#asm
	p_string	stripes_00_00,19,5
	__ldwi		14
	call		_set_font_pal
	p_string	stripes_00_01,6,7
	p_string	stripes_00_02,6,8
	p_string	stripes_00_03,6,9
	p_string	stripes_00_04,6,11
	p_string	stripes_00_05,6,12
	p_string	stripes_00_06,6,13
	p_string	stripes_00_07,6,14
	p_string	stripes_00_08,6,16
	p_string	stripes_00_09,6,17
	p_string	stripes_00_10,6,18
	p_string	stripes_00_11,6,19
	p_string	stripes_00_12,6,20
	p_string	stripes_00_13,6,22
	p_string	stripes_00_14,6,23
	rts

stripes_00_00:	.db	"HORIZONTAL STRIPES (1/2)"
			.db	0
stripes_00_01:	.db	"This pattern is designed to show if all lines are"
			.db	0
stripes_00_02:	.db	"visible in your setup, and how your video"
			.db	0
stripes_00_03:	.db	"processor is handling 240p video. "
			.db	0
stripes_00_04:	.db	"You should see a pattern of lines, each one pixel"
			.db	0
stripes_00_05:	.db	"in height, starting with a white one at the top of"
			.db	0
stripes_00_06:	.db	"the screen. You can toggle the pattern with "
			.db	0
stripes_00_07:	.db	"Select, or turn on auto-toggle each frame with I."
			.db	0
stripes_00_08:	.db	"When auto-toggle is set, you should see the lines"
			.db	0
stripes_00_09:	.db	"alternating rapidly. On some setups, the pattern"
			.db	0
stripes_00_10:	.db	"doesn't change at all. This means that the signal"
			.db	0
stripes_00_11:	.db	"is being treated as 480i/576i and odd or even"
			.db	0
stripes_00_12:	.db	"frames are being discarded completely."
			.db	0
stripes_00_13:	.db	"A frame counter can be displayed on screen by "
			.db	0
stripes_00_14:	.db	"pressing left."
			.db	0
#endasm
}


void display_stripes_01()
{
#asm
	p_string	stripes_01_00,19,5
	__ldwi		14
	call		_set_font_pal
	p_string	stripes_01_01,6,7
	p_string	stripes_01_02,6,8
	p_string	stripes_01_03,6,9
	rts

stripes_01_00:	.db	"HORIZONTAL STRIPES (2/2)"
			.db	0
stripes_01_01:	.db	"You can also display vertical bars by pressing up,"
			.db	0
stripes_01_02:	.db	"that pattern will help you evaluate if the signal"
			.db	0
stripes_01_03:	.db	"is not distorted horizontaly, since all lines "
			.db	0
stripes_01_04:	.db	"should be one pixel wide."
			.db	0
#endasm
}


void display_white_00()
{
#asm
	p_string	white_00_00,24,5
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
	rts

white_00_00:	.db	"WHITE SCREEN"
			.db	0
white_00_01:	.db	"This pattern can be changed between white, black,"
			.db	0
white_00_02:	.db	"red, green, blue and custom color full screens by "
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
white_00_09:	.db	"This pattern can also be used to check color "
			.db	0
white_00_10:	.db	"purity, and for dead pixels in some displays."
			.db	0
#endasm
}



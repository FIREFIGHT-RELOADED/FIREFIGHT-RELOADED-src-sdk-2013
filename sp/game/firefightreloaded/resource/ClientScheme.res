///////////////////////////////////////////////////////////
// Tracker scheme resource file
//
// sections:
//		Colors			- all the colors used by the scheme
//		BaseSettings	- contains settings for app to use to draw controls
//		Fonts			- list of all the fonts used by app
//		Borders			- description of all the borders
//
///////////////////////////////////////////////////////////
Scheme
{
	//////////////////////// COLORS ///////////////////////////
	// color details
	// this is a list of all the colors used by the scheme
	Colors
	{
		"White"				"255 255 255 255"
		"HL2Yellowish"			"255 160 0 255"
		"HL2Normal"				"255 208 64 255"
		"HL2Caution"			"255 48 0 255"
		"SMODBlueish"			"84 84 255 255"
		"SMODNormal"			"64 64 255 255"
		"SMODNormalDim"			"64 64 255 120"
		"SMODBright"			"128 128 255 255"
		"SMODCaution"			"255 84 255 255"
		
		// base colors
		"Orange"			"255 176 0 255"
		"OrangeDim"			"255 176 0 120"
		"LightOrange"		"188 112 0 128"
		
		"Red"				"192 28 0 140"
		"Black"				"0 0 0 196"
		"TransparentBlack"	"0 0 0 196"
		"TransparentLightBlack"	"0 0 0 90"

		"Blank"				"0 0 0 0"
		"ForTesting"		"255 0 0 32"
		"ForTesting_Magenta"	"255 0 255 255"
		"ForTesting_MagentaDim"	"255 0 255 120"
	}
	
	///////////////////// BASE SETTINGS ////////////////////////
	//
	// default settings for all panels
	// controls use these to determine their settings
	BaseSettings
	{
		"FgColor"			"SMODBlueish"
		"BgColor"			"0 0 0 120"

		"Panel.FgColor"			"SMODBlueish"
		"Panel.BgColor"			"0 0 0 120"
		
		"BrightFg"		"SMODBright"
		"BrightBg"		"SMODBright"

		"DamagedBg"			"180 0 0 200"
		"DamagedFg"			"180 0 0 230"
		"BrightDamagedFg"		"255 0 0 255"

		// weapon selection colors
		"SelectionNumberFg"		"SMODBlueish"
		"SelectionTextFg"		"SMODBlueish"
		"SelectionEmptyBoxBg" 	"0 0 0 120"
		"SelectionBoxBg" 		"0 0 0 120"
		"SelectionSelectedBoxBg" "0 0 0 120"
		
		"ZoomReticleColor"	"SMODBlueish"

		// HL1-style HUD colors
		"Yellowish"			"SMODBlueish"
		"Normal"			"SMODNormal"
		"Caution"			"SMODCaution"

		// Top-left corner of the "Half-Life 2" on the main screen
		"Main.Title1.X"				"76"
		"Main.Title1.Y"				"145"
		"Main.Title1.Y_hidef"		"130"
		"Main.Title1.Color"	"255 255 255 255"

		// Top-left corner of secondary title e.g. "DEMO" on the main screen
		"Main.Title2.X"				"76"
		"Main.Title2.Y"				"190"
		"Main.Title2.Y_hidef"		"174"
		"Main.Title2.Color"	"255 255 255 200"

		// Top-left corner of the menu on the main screen
		"Main.Menu.X"			"53"
		"Main.Menu.X_hidef"		"76"
		"Main.Menu.Y"			"240"

		// Blank space to leave beneath the menu on the main screen
		"Main.BottomBorder"	"32"
		
		///HERE
				// vgui_controls color specifications
		Border.Bright					"SMODBlueish"		// the lit side of a control
		Border.Dark						"SMODBlueish"		// the dark/unlit side of a control
		Border.Selection				"Blank"				// the additional border color for displaying the default/selected button

		Button.TextColor				"SMODNormal"
		Button.BgColor					"TransparentBlack"
		Button.ArmedTextColor			"SMODNormal"
		Button.ArmedBgColor				"SMODBlueish"
		Button.DepressedTextColor		"SMODNormal"
		Button.DepressedBgColor			"SMODBlueish"

		CheckButton.TextColor			"SMODNormal"
		CheckButton.SelectedTextColor	"SMODNormal"
		CheckButton.BgColor				"TransparentBlack"
		CheckButton.Border1  			"Border.Dark" 		// the left checkbutton border
		CheckButton.Border2  			"Border.Bright"		// the right checkbutton border
		CheckButton.Check				"SMODNormal"				// color of the check itself

		ComboBoxButton.ArrowColor		"SMODNormal"
		ComboBoxButton.ArmedArrowColor	"SMODNormal"
		ComboBoxButton.BgColor			"TransparentBlack"
		ComboBoxButton.DisabledBgColor	"Blank"

		Frame.BgColor					"TransparentBlack"
		Frame.OutOfFocusBgColor			"TransparentBlack"
		Frame.FocusTransitionEffectTime	"0.0"	// time it takes for a window to fade in/out on focus/out of focus
		Frame.TransitionEffectTime		"0.0"	// time it takes for a window to fade in/out on open/close
		Frame.AutoSnapRange				"0"
		FrameGrip.Color1				"Blank"
		FrameGrip.Color2				"Blank"
		FrameTitleButton.FgColor		"Blank"
		FrameTitleButton.BgColor		"Blank"
		FrameTitleButton.DisabledFgColor	"Blank"
		FrameTitleButton.DisabledBgColor	"Blank"
		FrameSystemButton.FgColor		"Blank"
		FrameSystemButton.BgColor		"Blank"
		FrameSystemButton.Icon			""
		FrameSystemButton.DisabledIcon	""
		FrameTitleBar.TextColor			"SMODNormal"
		FrameTitleBar.BgColor			"Blank"
		FrameTitleBar.DisabledTextColor	"SMODNormal"
		FrameTitleBar.DisabledBgColor	"Blank"

		GraphPanel.FgColor				"SMODNormal"
		GraphPanel.BgColor				"TransparentBlack"

		Label.TextDullColor				"SMODNormal"
		Label.TextColor					"SMODNormal"
		Label.TextBrightColor			"SMODNormal"
		Label.SelectedTextColor			"SMODNormal"
		Label.BgColor					"Blank"
		Label.DisabledFgColor1			"Blank"
		Label.DisabledFgColor2			"SMODBlueish"

		ListPanel.TextColor					"SMODNormal"
		ListPanel.BgColor					"TransparentBlack"
		ListPanel.SelectedTextColor			"Black"
		ListPanel.SelectedBgColor			"SMODBlueish"
		ListPanel.SelectedOutOfFocusBgColor	"SMODBlueish"
		ListPanel.EmptyListInfoTextColor	"SMODNormal"

		Menu.TextColor					"SMODNormal"
		Menu.BgColor					"TransparentBlack"
		Menu.ArmedTextColor				"SMODNormal"
		Menu.ArmedBgColor				"SMODBlueish"
		Menu.TextInset					"6"

		Chat.TypingText					"SMODNormal"

		Panel.FgColor					"SMODNormalDim"
		Panel.BgColor					"blank"

		ProgressBar.FgColor				"SMODNormal"
		ProgressBar.BgColor				"TransparentBlack"

		PropertySheet.TextColor			"SMODNormal"
		PropertySheet.SelectedTextColor	"SMODNormal"
		PropertySheet.TransitionEffectTime	"0.25"	// time to change from one tab to another

		RadioButton.TextColor			"SMODNormal"
		RadioButton.SelectedTextColor	"SMODNormal"

		RichText.TextColor				"SMODNormal"
		RichText.BgColor				"Blank"
		RichText.SelectedTextColor		"SMODNormal"
		RichText.SelectedBgColor		"Blank"

		ScrollBarButton.FgColor				"SMODNormal"
		ScrollBarButton.BgColor				"Blank"
		ScrollBarButton.ArmedFgColor		"SMODNormal"
		ScrollBarButton.ArmedBgColor		"Blank"
		ScrollBarButton.DepressedFgColor	"SMODNormal"
		ScrollBarButton.DepressedBgColor	"Blank"

		ScrollBarSlider.FgColor				"SMODNormal"		// nob color
		ScrollBarSlider.BgColor				"Blank"		// slider background color

		SectionedListPanel.HeaderTextColor	"SMODNormal"
		SectionedListPanel.HeaderBgColor	"Blank"
		SectionedListPanel.DividerColor		"Black"
		SectionedListPanel.TextColor		"SMODNormal"
		SectionedListPanel.BrightTextColor	"SMODNormal"
		SectionedListPanel.BgColor			"TransparentLightBlack"
		SectionedListPanel.SelectedTextColor			"SMODNormal"
		SectionedListPanel.SelectedBgColor				"SMODBlueish"
		SectionedListPanel.OutOfFocusSelectedTextColor	"SMODNormal"
		SectionedListPanel.OutOfFocusSelectedBgColor	"255 255 255 32"

		Slider.NobColor				"SMODNormal"
		Slider.TextColor			"SMODNormal"
		Slider.TrackColor			"SMODNormal"
		Slider.DisabledTextColor1	"SMODBlueish"
		Slider.DisabledTextColor2	"SMODBlueish"

		TextEntry.TextColor			"SMODNormal"
		TextEntry.BgColor			"TransparentBlack"
		TextEntry.CursorColor		"SMODNormal"
		TextEntry.DisabledTextColor	"SMODNormal"
		TextEntry.DisabledBgColor	"Blank"
		TextEntry.SelectedTextColor	"Black"
		TextEntry.SelectedBgColor	"SMODBlueish"
		TextEntry.OutOfFocusSelectedBgColor	"SMODBlueish"
		TextEntry.FocusEdgeColor	"TransparentBlack"

		ToggleButton.SelectedTextColor	"SMODNormal"

		Tooltip.TextColor			"TransparentBlack"
		Tooltip.BgColor				"SMODBlueish"

		TreeView.BgColor			"TransparentBlack"

		WizardSubPanel.BgColor		"Blank"
		
		"FgColor"		"SMODNormal"
		"BgColor"		"TransparentBlack"
	}

	//////////////////////// BITMAP FONT FILES /////////////////////////////
	//
	// Bitmap Fonts are ****VERY*** expensive static memory resources so they are purposely sparse
	BitmapFontFiles
	{
		// UI buttons, custom font, (256x64)
		"Buttons"		"materials/vgui/fonts/buttons_32.vbf"
	}

	
	//////////////////////// FONTS /////////////////////////////
	//
	// describes all the fonts
	Fonts
	{
		// fonts are used in order that they are listed
		// fonts are used in order that they are listed
		"DebugFixed"
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"antialias" "1"
			}
		}
		// fonts are used in order that they are listed
		"DebugFixedSmall"
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"antialias" "1"
			}
		}
		// fonts listed later in the order will only be used if they fulfill a range not already filled
		// if a font fails to load then the subsequent fonts will replace
		Default
		{
			"1"	[$X360]
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
			"1"	[$WIN32]
			{
				"name"		"Verdana"
				"tall"		"9"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1200 10000"
				"additive"	"1"
			}
		}
		"DefaultSmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Arial"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
				"scanlines" "0"
			}
		}
		"DefaultVerySmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
				"scanlines" "0"
			}
			"7"
			{
				"name"		"Arial"
				"tall"		"11"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
				"scanlines" "0"
			}
		}
		DefaultKillFeed
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"15"
				"weight"	"500"
				"antialias" "1"
			}
		}
		WeaponIconsKillFeed
		{
			"1"
			{
				"name"		"HL2MP"
				"tall"		"30"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsKillFeed2
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"30"
				"weight"	"2000"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsKillFeed3
		{
			"1"
			{
				"name"		"csd"
				"tall"		"30"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		AmmoIconsKillFeed
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"30"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		"MenuTitle"
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"22"
				"weight"	"500"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		"MenuTitleSmall"
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"15"
				"weight"	"500"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		WeaponIcons
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelected
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"antialias" "1"
				"blur"		"4"
				"scanlines"	"2"
				"custom"	"1"
			}
		}
		WeaponIconsSmall
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"32"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		WeaponIconsCS
		{
			"1"
			{
				"name"		"cs"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelectedCS
		{
			"1"
			{
				"name"		"cs"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"antialias" "1"
				"blur"		"4"
				"scanlines"	"2"
				"custom"	"1"
			}
		}
		WeaponIconsSmallCS
		{
			"1"
			{
				"name"		"cs"
				"tall"		"32"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		Crosshairs
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"40" [!$OSX]
				"tall"		"41" [$OSX]
				"weight"	"0"
				"scanlines" "0"
				"antialias" "0"
				"custom"	"1"
				"yres"		"1 10000"
			}
		}
		QuickInfo
		{
			"1"	[$X360]
			{
				"name"		"HL2cross"
				"tall"		"57"
				"weight"	"0"
				"scanlines" "0"
				"antialias" 	"1"
				"custom"	"1"
			}
			"1"	[$WIN32]
			{
				"name"		"HL2cross"
				"tall"		"28" [!$OSX]
				"tall"		"50" [$OSX]
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1" [!$OSX]
			}
		}
		HudNumbers
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"26"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		SquadIcon
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"50"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlow
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"26"
				"weight"	"0"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersSmall
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"14"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowSmall
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"14"
				"weight"	"1000"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersSmallMoney
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"16"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowSmallMoney
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"16"
				"weight"	"1000"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersTimer
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"25"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowTimer
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"25"
				"weight"	"0"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersSmallTimer
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"16"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudSelectionNumbers
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"11"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		HudHintTextLarge
		{
			"1"	[$X360]
			{
				"bitmap"	"1"
				"name"		"Buttons"
				"scalex"	"1.0"
				"scaley"	"1.0"
				"scanlines" "0"
			}
			"1"	[$WIN32]
			{
				"name"		"Verdana" [!$OSX]
				"name"		"Helvetica Bold" [$OSX]
				"tall"		"14"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		HudHintTextSmall
		{
			"1"	[$WIN32]
			{
				"name"		"Verdana" [!$OSX]
				"name"		"Helvetica" [$OSX]
				"tall"		"11"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
			}
			"1"	[$X360]
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		HudSelectionText
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"8"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"10"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"17"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1200 10000"
			}
		}
		GameUIButtons
		{
			"1"	[$X360]
			{
				"bitmap"	"1"
				"name"		"Buttons"
				"scalex"	"0.63"
				"scaley"	"0.63"
				"scalex_hidef"	"1.0"
				"scaley_hidef"	"1.0"
			}
		}
		BudgetLabel
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"outline"	"1"
			}
		}
		DebugOverlay
		{
			"1"	[$WIN32]
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"outline"	"1"
			}
			"1"	[$X360]
			{
				"name"		"Tahoma"
				"tall"		"18"
				"weight"	"200"
				"outline"	"1"
			}
		}
		"CloseCaption_Normal"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"500"
			}
		}
		"CloseCaption_Italic"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana Italic" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"500"
				"italic"	"1"
			}
		}
		"CloseCaption_Bold"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana Bold" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"900"
			}
		}
		"CloseCaption_BoldItalic"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana Bold Italic" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"900"
				"italic"	"1"
			}
		}
		"CloseCaption_Small"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana" [$OSX]
				"tall"		"16" [!$OSX]
				"tall"		"14" [$OSX]
				"tall_hidef"	"24"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
			}
		}
		// this is the symbol font
		"Marlett"
		{
			"1"
			{
				"name"		"Marlett"
				"tall"		"14"
				"weight"	"0"
				"symbol"	"1"
			}
		}
		"Trebuchet24"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
				"range"		"0x0000 0x007F"	//	Basic Latin
				"antialias" "1"
				"additive"	"1"
			}
		}
		"Trebuchet18"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"18"
				"weight"	"900"
			}
		}
		ClientTitleFont
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"34"
				"tall_hidef"	"48"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		ClientTitleFontMedium
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"24"
				"tall_hidef"	"38"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		ClientTitleFontSmall
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"15"
				"tall_hidef"	"27"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		ClientTitleFontHint
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"20"
				"tall_hidef"	"34"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		MenuTitleFont
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"34"
				"tall_hidef"	"48"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		MenuTitleFontMedium
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"20"
				"tall_hidef"	"38"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		CreditsLogo
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"128"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		CreditsText
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"20"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		CreditsOutroLogos
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"48"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		CreditsOutroText
		{
			"1"
			{
				"name"		"Verdana" [!$OSX]
				"name"		"Courier Bold" [$OSX]
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
			}
		}
		CenterPrintText
		{
			// note that this scales with the screen resolution
			"1"
			{
				"name"		"Trebuchet MS" [!$OSX]
				"name"		"Helvetica" [$OSX]
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HDRDemoText
		{
			// note that this scales with the screen resolution
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		"AchievementNotification"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
			}
		}
		"CommentaryDefault"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"	[$WIN32]
				"tall"		"20"	[$X360]
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"900"
			}
			"7"
			{
				"name"		"Arial"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"800"
			}
			
		}
	}
	
	//////////////////////// CUSTOM FONT FILES /////////////////////////////
	//
	// specifies all the custom (non-system) font files that need to be loaded to service the above described fonts
	CustomFontFiles
	{
		"1"		"resource/HALFLIFE2.ttf"
		"2"		"resource/HL2crosshairs.ttf"
		"3"		"resource/HL2EP2.ttf"
		"4"		"resource/smodgui.ttf"
		"5"		"resource/hl2mp.ttf"
		"6"		"resource/cs.ttf"
		"7"		"resource/csd.ttf"
	}

}

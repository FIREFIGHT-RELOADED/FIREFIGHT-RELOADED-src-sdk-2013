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
// 
// Mapbase scheme file.
// Use it when we figure out how to get Hammer to mount it but not the game.
// 
///////////////////////////////////////////////////////////
#base "SourceSchemeBase.res"

Scheme
{
	//////////////////////// COLORS ///////////////////////////
	// color details
	// this is a list of all the colors used by the scheme
	Colors
	{
		// base colors
		"White"				"255 255 255 255"
		"OffWhite"			"221 221 221 255"
		"DullWhite"			"211 211 211 255"
		
		"TransparentBlack"	"0 0 0 128"
		"Black"				"0 0 0 255"

		"Blank"				"0 0 0 0"

		// base colors
		"BaseText"			"216 222 211 255"	// used in text windows, lists
		"BrightBaseText"	"255 255 255 255"	// brightest text
		"SelectedText"		"255 255 255 255"	// selected text
		"DimBaseText"		"160 170 149 255"	// dim base text
		"LabelDimText"		"160 170 149 255"	// used for info text
		"ControlText"		"216 222 211 255"	// used in all text controls
		"BrightControlText"	"196 181 80 255"	// use for selected controls
		"DisabledText1"		"117 128 111 255"	// disabled text
		"DisabledText2"		"40 46 34 255"		// overlay color for disabled text (to give that inset look)
		"DimListText"		"117 134 102 255"	// offline friends, unsubscribed games, etc.

		// background colors
		"ControlBG"			"5 0 12 255"		// background color of controls
		"ControlDarkBG"		"14 12 15 255"		// darker background color; used for background of scrollbars
		"WindowBG"			"10 0 17 255"		// background color of text edit panes (chat, text entries, etc.)
		"SelectionBG"		"188 132 0 255"	// background color of any selected text or menu item
		"SelectionBG2"		"40 46 34 255"		// selection background in window w/o focus
		"ListBG"			"8 0 16 255"		// background of server browser, buddy list, etc.

		// titlebar colors
		"TitleText"			"255 255 255 255"
		"TitleDimText"		"136 145 128 255"
		"TitleBG"			"76 88 68 0"
		"TitleDimBG"		"76 88 68 0"
		
		// slider tick colors
		"SliderTickColor"	"127 140 127 255"
		"SliderTrackColor"	"31 31 31 255"

		// border colors
		"BorderBright"		"136 145 128 255"	// the lit side of a control
		"BorderDark"		"40 46 34 255"		// the dark/unlit side of a control
		"BorderSelection"	"0 0 0 255"			// the additional border color for displaying the default/selected button

		"TestColor"			"255 0 255 255"
	}

	///////////////////// BASE SETTINGS ////////////////////////
	//
	// default settings for all panels
	// controls use these to determine their settings
	BaseSettings
	{
		// vgui_controls color specifications
		Border.Bright					"BorderBright"	// the lit side of a control
		Border.Dark						"BorderDark"		// the dark/unlit side of a control
		Border.Selection				"BorderSelection"			// the additional border color for displaying the default/selected button

		Button.TextColor				"ControlText"
		Button.BgColor					"ControlBG"
//		Button.ArmedTextColor			"BrightBaseText"
//		Button.ArmedBgColor				"SelectionBG"
//		Button.DepressedTextColor		"DimBaseText"
//		Button.DepressedBgColor			"ControlDarkBG"	
		Button.FocusBorderColor			"TransparentBlack"
		
		CheckButton.TextColor			"BaseText"
		CheckButton.SelectedTextColor	"BrightControlText"
		CheckButton.BgColor				"ListBG"
		CheckButton.Border1  			"Border.Dark" 		// the left checkbutton border
		CheckButton.Border2  			"Border.Bright"		// the right checkbutton border
		CheckButton.Check				"BrightControlText"	// color of the check itself

		ComboBoxButton.ArrowColor		"DimBaseText"
		ComboBoxButton.ArmedArrowColor	"BrightBaseText"
		ComboBoxButton.BgColor			"ListBG"
		ComboBoxButton.DisabledBgColor	"ControlBG"

		Frame.TitleTextInsetX			30
		Frame.ClientInsetX				20
		Frame.ClientInsetY				6
		Frame.BgColor					"ControlBG"
		Frame.OutOfFocusBgColor			"ControlBG"
		Frame.FocusTransitionEffectTime	"0.3"	// time it takes for a window to fade in/out on focus/out of focus
		Frame.TransitionEffectTime		"0.3"	// time it takes for a window to fade in/out on open/close
		Frame.AutoSnapRange				"0"
		FrameGrip.Color1				"BorderBright"
		FrameGrip.Color2				"BorderSelection"
		FrameTitleButton.FgColor		"BorderBright"
		FrameTitleButton.BgColor		"ControlBG"
		FrameTitleButton.DisabledFgColor	"TitleDimText"
		FrameTitleButton.DisabledBgColor	"TitleDimBG"
		FrameSystemButton.FgColor		"TitleBG"
		FrameSystemButton.BgColor		"TitleBG"
		FrameSystemButton.Icon			"resource/icon_steam"
		FrameSystemButton.DisabledIcon	"resource/icon_steam_disabled"
		FrameTitleBar.TextColor			"TitleText"
		FrameTitleBar.BgColor			"TitleBG"
		FrameTitleBar.DisabledTextColor	"TitleText"
		FrameTitleBar.DisabledBgColor	"TitleBG"

		GraphPanel.FgColor				"White"
		GraphPanel.BgColor				"TransparentBlack"

		Label.TextDullColor				"DimBaseText"
		Label.TextColor					"ControlText"
		Label.TextBrightColor			"BrightBaseText"
		Label.SelectedTextColor			"BrightControlText"
		Label.BgColor					"ControlBG"
		Label.DisabledFgColor1			"DisabledText1"	
		Label.DisabledFgColor2			"DisabledText2"	

		ListPanel.TextColor					"BaseText"
		ListPanel.BgColor					"ListBG"
		ListPanel.SelectedTextColor			"BrightBaseText"
		ListPanel.SelectedBgColor			"SelectionBG"
		ListPanel.SelectedOutOfFocusBgColor	"SelectionBG2"
		ListPanel.EmptyListInfoTextColor	"DimBaseText"

		Menu.TextInset					"6"
		Menu.FgColor			"DimBaseText"
		Menu.BgColor			"ControlBG"
		Menu.ArmedFgColor		"BrightBaseText"
		Menu.ArmedBgColor		"SelectionBG"
		Menu.DividerColor		"BorderDark"

		Panel.FgColor					"BorderSelection"
		Panel.BgColor					"ControlBG"

		ProgressBar.FgColor				"Label.FgColor"
		ProgressBar.BgColor				"Label.BgColor"

		PropertySheet.TextColor			"ControlText"
		PropertySheet.SelectedTextColor		"BrightControlText"
		PropertySheet.TransitionEffectTime	"0"	// time to change from one tab to another

		RadioButton.TextColor			"ToggleButton.TextColor"
		RadioButton.SelectedTextColor		"ToggleButton.SelectedTextColor"

		RichText.TextColor				"TextEntry.TextColor"
		RichText.BgColor				"TextEntry.BgColor"
		RichText.SelectedTextColor		"TextEntry.SelectedTextColor"
		RichText.SelectedBgColor		"SelectionBG"

		ScrollBar.Wide					17

		ScrollBarButton.FgColor				"ControlText"
		ScrollBarButton.BgColor				"ControlDarkBG"
		ScrollBarButton.ArmedFgColor		"BrightBaseText"
		ScrollBarButton.ArmedBgColor		"ControlDarkBG"
		ScrollBarButton.DepressedFgColor	"BrightBaseText"
		ScrollBarButton.DepressedBgColor	"ControlDarkBG"

		ScrollBarSlider.BgColor				"ControlDarkBG"		// this isn't really used
		ScrollBarSlider.FgColor				"ControlBG"		// handle with which the slider is grabbed

//		SectionedListPanel.HeaderTextColor	"White"
//		SectionedListPanel.HeaderBgColor	"Blank"
//		SectionedListPanel.DividerColor		"Black"
//		SectionedListPanel.TextColor		"DullWhite"
//		SectionedListPanel.BrightTextColor	"White"
//		SectionedListPanel.BgColor			"TransparentBlack"
//		SectionedListPanel.SelectedTextColor			"Black"
//		SectionedListPanel.SelectedBgColor				"255 155 0 255"
//		SectionedListPanel.OutOfFocusSelectedTextColor	"Black"
//		SectionedListPanel.OutOfFocusSelectedBgColor	"255 155 0 128"

		Slider.NobColor				"ControlDarkBG"		
		Slider.TextColor			"ControlBG"
		Slider.TrackColor			"ControlDarkBG"
//		Slider.DisabledTextColor1	"117 117 117 255"
//		Slider.DisabledTextColor2	"30 30 30 255"

		TextEntry.TextColor			"ControlText"
		TextEntry.BgColor			"ListBG"
		TextEntry.CursorColor		"Label.CursoColor"
		TextEntry.DisabledTextColor	"DimBaseText"
		TextEntry.DisabledBgColor	"ControlBG"
		TextEntry.SelectedTextColor	"SelectedText"
		TextEntry.SelectedBgColor	"SelectionBG"
		TextEntry.OutOfFocusSelectedBgColor	"SelectionBG2"
		TextEntry.FocusEdgeColor	"SelectionBG"

		ToggleButton.SelectedTextColor	"Label.SelectedTextColor"

//		Tooltip.TextColor			"0 0 0 196"
//		Tooltip.BgColor				"255 155 0 255"

		TreeView.BgColor			"WindowBG"

		WizardSubPanel.BgColor		"ControlBG"
	}
}
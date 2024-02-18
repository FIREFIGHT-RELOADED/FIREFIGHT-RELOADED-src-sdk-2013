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
	//////////////////////// FONTS /////////////////////////////
	//
	// describes all the fonts
	Fonts
	{
		// ========================================================================================================================
		// New conditionals cannot be added for ClientScheme.res without engine access, so Steam Deck fonts use their own definitions.
		// ========================================================================================================================
		Default_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"22"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"26"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"30"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5" // Proportional - Josh
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		WeaponIcons_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"70"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelected_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"70"
				"weight"	"0"
				"antialias" "1"
				"blur"		"5"
				"scanlines"	"2"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSmall_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"36"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		FlashlightDeck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"46"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		Crosshairs_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"40" [!$OSX]
				"weight"	"0"
				"antialias" "0"
				"additive"	"1"
				"custom"	"1"
				"yres"		"1 1439"
			}
			"2"
			{
				"name"		"HalfLife2"
				"tall"		"80"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
				"yres"		"1440 3199"
			}
			"3"
			{
				"name"		"HalfLife2"
				"tall"		"120"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
				"yres"		"3200 4799"
			}
			"4"
			{
				"name"		"HalfLife2"
				"tall"		"17"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		QuickInfo_Deck
		{
			"1"
			{
				"name"		"HL2cross"
				"tall"		"36"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbers_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"40"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersGlow_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"40"
				"weight"	"0"
				"blur"		"4"
				"scanlines" "2"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersSmall_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"26"
				"weight"	"1000"
				"additive"	"1"
				"antialias" "1"
				"custom"	"1"
			}
		}
		SquadIcon_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"40"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudSelectionNumbers_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"700"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudHintTextLarge_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"22"
				"weight"	"1000"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudHintTextSmall_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"18"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudSelectionText_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"10"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
				"additive"	"1"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
				"additive"	"1"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
				"additive"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"22"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
				"additive"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"9"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		"CloseCaption_Normal_Deck"
		{
			"1"
			{
				"name"		"Sans Regular"
				"tall"		"21" // 15
				"weight"	"500"
				"antialias"	"1"
			}
		}
		"CloseCaption_Italic_Deck"
		{
			"1"
			{
				"name"		"Sans Italic"
				"tall"		"21" // 15
				"weight"	"500"
				"italic"	"1"
				"antialias"	"1"
			}
		}
		"CloseCaption_Bold_Deck"
		{
			"1"
			{
				"name"		"Sans Bold"
				"tall"		"21" // 15
				"weight"	"900"
				"antialias"	"1"
			}
		}
		"CloseCaption_BoldItalic_Deck"
		{
			"1"
			{
				"name"		"Sand Bold Italic"
				"tall"		"21" // 15
				"weight"	"900"
				"italic"	"1"
				"antialias"	"1"
			}
		}
		//"CloseCaption_Small_Deck"
		//{
		//	"1"
		//	{
		//		"name"		"Tahoma"
		//		"tall"		"15"
		//		"weight"	"900"
		//		"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
		//		"antialias"	"1"
		//	}
		//}
	}
}

"Resource/HudLayout.res"
{
	HudHealth
	{
		"fieldName"		"HudHealth"
		"xpos"	"16"
		"ypos"	"424"
		"wide"	"50"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
		
		"text_xpos" "8"
		"text_ypos" "5"
		"digit_xpos" "4"
		"digit_ypos" "16"
	}
	
	HudSuit
	{
		"fieldName"		"HudSuit"
		"xpos"	"68"
		"ypos"	"424"
		"wide"	"50"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "5"
		"digit_xpos" "4"
		"digit_ypos" "16"
	}

	HudAmmo
	{
		"fieldName" "HudAmmo"
		"xpos"	"r90"
		"ypos"	"424"
		"wide"	"75"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "40"
		"text_ypos" "5"
		"digit_xpos" "6"
		"digit_ypos" "16"
		"digit2_xpos" "36"
		"digit2_ypos" "16"
		"primaryammoicon_xpos" "5"
		"primaryammoicon_ypos" "-7"
	}

	HudAmmoSecondary
	{
		"fieldName" "HudAmmoSecondary"
		"xpos"	"r78"
		"ypos"	"424"
		"wide"	"55"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
		
		"text_xpos" "36"
		"text_ypos" "5"
		"digit_xpos" "4"
		"digit_ypos" "16"
		"secondaryammoicon_xpos" "5"
		"secondaryammoicon_ypos" "-7"
	}

	HudPosture
	{
		"fieldName" 		"HudPosture"
		"visible" 		"1"
		"PaintBackgroundType"	"2"
		"xpos"	"16"
		"ypos"	"316"
		"tall"  "36"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		"icon_xpos"	"10"
		"icon_ypos" 	"0"
	}

	HudSuitPower
	{
		"fieldName" "HudSuitPower"
		"visible" "1"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"395"
		"wide"	"102"
		"tall"	"26"
		
		"AuxPowerColor"	"84 84 255 255"
		"AuxPowerLowColor" "255 84 255 255"
		"AuxPowerHighColor" "255 220 0 220"
		"AuxPowerDisabledAlpha" "70"

		"BarInsetX" "8"
		"BarInsetY" "15"
		"BarWidth" "92"
		"BarHeight" "4"
		"BarChunkWidth" "6"
		"BarChunkGap" "1"

		"text_xpos" "8"
		"text_ypos" "4"
		"text2_xpos" "8"
		"text2_ypos" "22"
		"text2_gap" "10"

		"PaintBackgroundType"	"2"
	}
	
	HudFlashlight
	{
		"fieldName" "HudFlashlight"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"135"
		"ypos"	"397"		
		"tall"  "24"
		"wide"	"40"
		"font"	"WeaponIconsSmall"
		
		"icon_xpos"	"4"
		"icon_ypos" "-8"
		
		"BarInsetX" "4"
		"BarInsetY" "18"
		"BarWidth" "28"
		"BarHeight" "2"
		"BarChunkWidth" "2"
		"BarChunkGap" "1"
	}

	HudLocator
	{
		"fieldName" "HudLocator"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"c8"	[$WIN32]
		"ypos"	"r36"	[$WIN32]
		"xpos"	"c-32"	[$X360]
		"ypos_hidef"	"r52"	[$X360]
		"ypos_lodef"	"r95"	[$X360]		// 52 is aligned to bottom of HudSuit
		"wide"	"64"
		"tall"  "24"
	}

	HudDamageIndicator
	{
		"fieldName" "HudDamageIndicator"
		"visible" "1"
		"enabled" "1"
		"DmgColorLeft" "255 0 128 0"
		"DmgColorRight" "255 0 128 0"
		
		"dmg_xpos" "30"
		"dmg_ypos" "100"
		"dmg_wide" "36"
		"dmg_tall1" "240"
		"dmg_tall2" "200"
	}

	HudZoom
	{
		"fieldName" "HudZoom"
		"visible" "1"
		"enabled" "1"
		"Circle1Radius" "66"
		"Circle2Radius"	"74"
		"DashGap"	"16"
		"DashHeight" "4"	[$WIN32]
		"DashHeight" "6"	[$X360]		
		"BorderThickness" "88"
	}

	HudWeaponSelection
	{
		"fieldName" "HudWeaponSelection"
		"ypos" 	"16"
		"visible" "1"
		"enabled" "1"
		"SmallBoxSize" "32"
		"LargeBoxWide" "178"
		"LargeBoxTall" "72"
		"BoxGap" "4"
		"SelectionNumberXPos" "4"
		"SelectionNumberYPos" "4"
		"SelectionGrowTime"	"0.4"
		"TextYPos" "54"
	}

	HudCrosshair
	{
		"fieldName" "HudCrosshair"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudDeathNotice
	{
		"fieldName" "HudDeathNotice"
		"visible" "1"
		"enabled" "1"
		"xpos"	 "r640"
		"ypos"	 "5"
		"wide"	 "628"
		"tall"	 "468"

		"MaxDeathNotices" "4"
		"LineHeight"	  "28"
		"RightJustify"	  "1"	// If 1, draw notices from the right
		"IconOffsetWepY"  "-18"
		"IconOffsetAmmoY"  "-18"
		"IconOffsetTexWepY"	"-3"

		"TextFont"				"DefaultKillFeed"
		
		"PaintBackgroundType"	"2"
	}

	HudVehicle
	{
		"fieldName" "HudVehicle"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	ScorePanel
	{
		"fieldName" "ScorePanel"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudTrain
	{
		"fieldName" "HudTrain"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMOTD
	{
		"fieldName" "HudMOTD"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMessage
	{
		"fieldName" "HudMessage"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMenu
	{
		"fieldName" "HudMenu"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudCloseCaption
	{
		"fieldName" "HudCloseCaption"
		"visible"	"0"
		"enabled"	"1"
		"xpos"		"c-250"
		"ypos"		"276"	[$WIN32]
		"ypos_hidef"	"236"	[$X360]
		"ypos_lodef"	"206"	[$X360]			//236
		"wide"		"500"
		"tall"		"136"	[$WIN32]
		"tall"		"176"	[$X360]

		"BgAlpha"	"128"

		"GrowTime"		"0.25"
		"ItemHiddenTime"	"0.2"  // Nearly same as grow time so that the item doesn't start to show until growth is finished
		"ItemFadeInTime"	"0.15"	// Once ItemHiddenTime is finished, takes this much longer to fade in
		"ItemFadeOutTime"	"0.3"
		"topoffset"		"0"		[$WIN32]
		"topoffset"		"40"	[$X360]
	}

	HudChat
	{
		"fieldName" "HudChat"
		"visible" "0"
		"enabled" "1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	 "4"
		"tall"	 "4"
	}

	HudHistoryResource	[$WIN32]
	{
		"fieldName" "HudHistoryResource"
		"visible" "1"
		"enabled" "1"
		"xpos"	"r252"
		//"ypos"	"50"
		"ypos"	"100"
		"wide"	 "248"
		//"tall"	 "370"
		"tall"	 "270"

		"history_gap"	"35"
		"icon_inset"	"28"
		"text_inset"	"26"
		"NumberFont"	"HudNumbersSmall"
		"PaintBackgroundType"	"2"
	}

	HudGeiger
	{
		"fieldName" "HudGeiger"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HUDQuickInfo
	{
		"fieldName" "HUDQuickInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudWeapon
	{
		"fieldName" "HudWeapon"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}
	HudAnimationInfo
	{
		"fieldName" "HudAnimationInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudPredictionDump
	{
		"fieldName" "HudPredictionDump"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudHintDisplay
	{
		"fieldName"	"HudHintDisplay"
		"visible"	"0"
		"enabled" "1"
		"Alpha"		"0"		// Remove this to enable hint hud element
		"xpos"	"r120"	[$WIN32]
		"ypos"	"r340"	[$WIN32]
		"xpos"	"r148"	[$X360]
		"ypos"	"r338"	[$X360]
		"wide"	"100"
		"tall"	"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}

	HudHintKeyDisplay
	{
		"fieldName"	"HudHintKeyDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"	[$WIN32]
		"ypos"		"r380"	[$WIN32]
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}
	
	HudAchievementHintDisplay
	{
		"fieldName"	"HudAchievementHintDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"
		"ypos"		"r260"
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}
	
	HudLevelHintDisplay
	{
		"fieldName"	"HudLevelHintDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"
		"ypos"		"r340"
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}
	
	HudPerkHintDisplay
	{
		"fieldName"	"HudPerkHintDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"
		"ypos"		"r480"
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}

	HudSquadStatus	[$WIN32]
	{
		"fieldName"	"HudSquadStatus"
		"visible"	"1"
		"enabled" "1"
		"xpos"	"r127"
		"ypos"	"375"
		"wide"	"104"
		"tall"	"46"
		"text_xpos"	"8"
		"text_ypos"	"34"
		"SquadIconColor"	"84 84 255 160"
		"IconInsetX"	"8"
		"IconInsetY"	"-10"
		"IconGap"		"24"

		"PaintBackgroundType"	"2"
	}

	HudPoisonDamageIndicator	[$WIN32]
	{
		"fieldName"	"HudPoisonDamageIndicator"
		"visible"	"0"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"326"
		"wide"	"136"
		"tall"	"38"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_ygap" "14"
		"TextColor"	"255 170 0 220"
		"PaintBackgroundType"	"2"
	}

	HudCredits
	{
		"fieldName"	"HudCredits"
		"TextFont"	"Default"
		"visible"	"1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"	"480"
		"TextColor"	"255 255 255 192"

	}
	
	HUDAutoAim
	{
		"fieldName" "HUDAutoAim"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"	[$WIN32]
		"tall"	 "480"	[$WIN32]
		"wide"	 "960"	[$X360]
		"tall"	 "720"	[$X360]
	}

	HudCommentary
	{
		"fieldName" "HudCommentary"
		"xpos"	"c-190"
		"ypos"	"350"
		"wide"	"380"
		"tall"  "40"
		"visible" "1"
		"enabled" "1"
		
		"PaintBackgroundType"	"2"
		
		"bar_xpos"		"50"
		"bar_ypos"		"20"
		"bar_height"	"8"
		"bar_width"		"320"
		"speaker_xpos"	"50"
		"speaker_ypos"	"8"
		"count_xpos_from_right"	"10"	// Counts from the right side
		"count_ypos"	"8"
		
		"icon_texture"	"vgui/hud/icon_commentary"
		"icon_xpos"		"0"
		"icon_ypos"		"0"		
		"icon_width"	"40"
		"icon_height"	"40"
	}
	
	HudHDRDemo
	{
		"fieldName" "HudHDRDemo"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"
		
		"Alpha"	"255"
		"PaintBackgroundType"	"2"
		
		"BorderColor"	"0 0 0 255"
		"BorderLeft"	"16"
		"BorderRight"	"16"
		"BorderTop"		"16"
		"BorderBottom"	"64"
		"BorderCenter"	"0"
		
		"TextColor"		"255 255 255 255"
		"LeftTitleY"	"422"
		"RightTitleY"	"422"
	}
	"HudChatHistory"
	{
		"ControlName"		"RichText"
		"fieldName"		"HudChatHistory"
		"xpos"			"10"
		"ypos"			"290"
		"wide"	 		"300"
		"tall"			 "75"
		"wrap"			"1"
		"autoResize"		"1"
		"pinCorner"		"1"
		"visible"		"0"
		"enabled"		"1"
		"labelText"		""
		"textAlignment"		"south-west"
		"font"			"Default"
		"maxchars"		"-1"
	}

	AchievementNotificationPanel	
	{
		"fieldName"				"AchievementNotificationPanel"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"180"
		"wide"					"f10"	[$WIN32]
		"wide"					"f60"	[$X360]
		"tall"					"100"
	}
	
	HudLevel
	{
		"fieldName"		"HudLevel"
		"xpos"	"184"
		"ypos"	"424"
		"wide"	"60"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
 		
		"text_xpos" "8"
		"text_ypos" "5"
		"digit_xpos" "4"
		"digit_ypos" "16"
	}
	
	HudKills
	{
		"fieldName"		"HudKills"
		"xpos"	"120"
		"ypos"	"424"
		"wide"	"60"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
 		
		"text_xpos" "8"
		"text_ypos" "5"
		"digit_xpos" "4"
		"digit_ypos" "16"
	}
	
	HudEXP
	{
		"fieldName" "HudEXP"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"250"
		"ypos"	"443"		
		"tall"  "25"
		"wide"	"150"
		
		//"ypos"	"424"
		//"wide"	"50"
		//"tall"  "44"
		
		"text_xpos" "5"
		"text_ypos" "3"
		"text_xpos2" "5"
		"text_ypos2" "13"
		
		"BarInsetX" "5"
		"BarInsetY" "15"
		"BarWidth" "140"
		"BarHeight" "5"
		"BarChunkWidth" "2"
		"BarChunkGap" "1"
	}
	
	HudMoney
	{
		"fieldName"		"HudMoney"
		"xpos"	"250"
		"ypos"	"413"
		"wide"	"150"
		"tall"  "27"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
 		
		"text_xpos" "5"
		"text_ypos" "3"
		"digit_xpos" "7"
		"digit_ypos" "10"
	}
	
	HudMoneyClassic
	{
		"fieldName"		"HudMoneyClassic"
		"xpos"	"184"
		"ypos"	"424"
		"wide"	"150"
		"tall"  "44"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"
 		
		"text_xpos" "8"
		"text_ypos" "5"
		"digit_xpos" "4"
		"digit_ypos" "16"
	}
	
	HudTimer
	{
		"fieldName"   "HudTimer"
		"xpos"	"180"
		"ypos"	"394"
		"wide"  "60"
		"tall"  "27"
		"visible" "1"
		"enabled" "1"
		"PaintBackgroundType" "2"
		"digit_xpos" "2"
		"digit_ypos" "2"
	}
}

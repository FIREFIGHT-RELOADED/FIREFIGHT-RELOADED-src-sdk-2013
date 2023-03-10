"Options"
{
	"Difficulty"
	{
		"title"			"#GameUI_Difficulty"
		"alternating"   "0"
		"horizontal"	"1"
		"items"
		{
			"Easy"
			{
				"text"			"#GameUI_Easy"
				"description"	"#GameUI_EasyDescription"
				"type"			"skillyskill"
				"image"			"gamepadui/easy"
				"skill"			"0"
			}

			"Medium"
			{
				"text"			"#GameUI_Medium"
				"description"	"#GameUI_NormalDescription"
				"type"			"skillyskill"
				"image"			"gamepadui/medium"
				"skill"			"1"
			}

			"Hard"
			{
				"text"			"#GameUI_Hard"
				"description"	"#GameUI_HardDescription"
				"type"			"skillyskill"
				"image"			"gamepadui/hard"
				"skill"			"2"
			}

			"VeryHard"
			{
				"text"			"#GameUI_VeryHard"
				"description"	"#GameUI_VeryHardDescription"
				"type"			"skillyskill"
				"image"			"gamepadui/veryhard"
				"skill"			"3"
			}

			"Nightmare"
			{
				"text"			"#GameUI_Nightmare"
				"description"	"#GameUI_NightmareDescription"
				"type"			"skillyskill"
				"image"			"gamepadui/nightmare"
				"skill"			"4"
			}
		}
	}
	"Gameplay"
	{
		"title"			"#GameUI_Game"
		"alternating"   "1"
		"items"
		{
			"AutoTarget"
			{
				"text"			"#GameUI_AutoaimController"
				"type"			"wheelywheel"
				"convar"		"sv_player_autoaimcrosshair"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"FastSwitch"
			{
				"text"			"#GameUI_FastSwitchCheck"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"hud_fastswitch"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"FasterSwitch"
			{
				"text"			"#GameUI_FasterWeaponSwitch"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"weapon_quickswitch"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Grapple"
			{
				"text"			"#GameUI_GrappleToggle"
				"type"			"wheelywheel"
				"convar"		"sv_player_grapple"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"RocketJumping"
			{
				"text"			"#GameUI_RocketJumping"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"sv_player_rocketjumping"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ClassicModifier"
			{
				"text"			"#GameUI_ClassicMode"
				"type"			"wheelywheel"
				"convar"		"g_fr_classic"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"HardcoreModifier"
			{
				"text"			"#GameUI_HardcoreMode"
				"type"			"wheelywheel"
				"convar"		"g_fr_hardcore"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"LoneWolfModifier"
			{
				"text"			"#GameUI_LoneWolfMode"
				"type"			"wheelywheel"
				"convar"		"g_fr_lonewolf"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"IronKickModifier"
			{
				"text"			"#GameUI_IronKickMode"
				"type"			"wheelywheel"
				"convar"		"g_fr_ironkick"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Attributes"
			{
				"text"			"#GameUI_ToggleAttributes"
				"type"			"wheelywheel"
				"convar"		"entity_attributes"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AdvancedHeader"
			{
				"text"			"#GameUI_AdvancedNoEllipsis"
				"type"			"headeryheader"
				"advanced"		"1"
			}

			"Console"
			{
				"text"			"#GameUI_DeveloperConsoleCheck"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"con_enable"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"OldSpawner"
			{
				"text"			"#FR_OldSpawner"
				"type"			"wheelywheel"
				"convar"		"g_fr_spawneroldfunctionality"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Gamemodes"
			{
				"text"			"#GameUI_GamemodeSelect"
				"type"			"wheelywheel"
				"convar"		"g_gamemode"
				"advanced"		"1"

				"options"
				{
					"0"	"#GameUI_GamemodeRandom"
					"1"	"#GameUI_GamemodeCF"
					"2"	"#GameUI_GamemodeXI"
					"3"	"#GameUI_GamemodeAA"
					"4"	"#GameUI_GamemodeZS"
					"5"	"#GameUI_GamemodeFR"
				}
			}

			"HealthRegenPerk"
			{
				"text"			"#GameUI_MakeHealthRegenPerk"
				"type"			"wheelywheel"
				"convar"		"sv_fr_perks_healthregeneration_perkmode"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PerkModeInMutators"
			{
				"text"			"#GameUI_MakeHealthRegenPerkInMutators"
				"type"			"wheelywheel"
				"convar"		"sv_fr_perks_healthregeneration_perkmode_inmutators"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AttributesChance"
			{
				"text"			"#GameUI_AttributesChance"
				"type"			"slideyslide"
				"convar"		"entity_attributes_chance"
				"advanced"		"1"

				"min"			"1"
				"max"			"15"
				"step"			"1"

				"textprecision" "0"
			}

			"AttributesNumPresets"
			{
				"text"			"#GameUI_AttributesPresetCount"
				"type"			"slideyslide"
				"convar"		"entity_attributes_numpresets"
				"advanced"		"1"

				"min"			"1"
				"max"			"30"
				"step"			"1"

				"textprecision" "0"
			}
		}
	}
	"Keyboard"
	{
		"title"			"#GameUI_Keyboard"
		"items_from"	"keyboard"
		"alternating"   "1"
	}
	"Mouse"
	{
		"title"			"#GameUI_Mouse"
		"alternating"   "1"
		"items"
		{
			"ReverseMouse"
			{
				"text"			"#GameUI_ReverseMouse"
				"type"			"wheelywheel"
				"convar"		"m_pitch"
				"signonly"		"1"

				"options"
				{
					"1"		"#gameui_disabled"
					"-1"	"#gameui_enabled"
				}
			}

			"MouseFilter"
			{
				"text"			"#GameUI_MouseFilter"
				"type"			"wheelywheel"
				"convar"		"m_filter"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"MouseSensitivity"
			{
				"text"			"#GameUI_MouseSensitivity"
				"type"			"slideyslide"
				"convar"		"sensitivity"

				"min"			"0.1"
				"max"			"6.0"
				"step"			"0.1"

				"textprecision" "2"
			}

			"RawInput"
			{
				"text"			"#GameUI_MouseRaw"
				"type"			"wheelywheel"
				"convar"		"m_rawinput"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"MouseAcceleration"
			{
				"text"			"#GameUI_MouseAcceleration"
				"type"			"wheelywheel"
				"convar"		"m_customaccel"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"MouseAccelerationExponent"
			{
				"text"			""
				"type"			"slideyslide"
				"convar"		"m_customaccel_exponent"

				"min"			"1.0"
				"max"			"1.4"
				"step"			"0.05"

				"depends_on"	"m_customaccel"

				"textprecision" "2"
			}
		}
	}
	"Audio"
	{
		"title"			"#GameUI_Audio"
		"alternating"   "1"
		"items"
		{
			"SFXSlider"
			{
				"text"			"#GameUI_SoundEffectVolume"
				"type"			"slideyslide"
				"convar"		"volume"

				"min"			"0.0"
				"max"			"1.0"
				"step"			"0.1"
			}

			"MusicSlider"
			{
				"text"			"#GameUI_MusicVolume"
				"type"			"slideyslide"
				"convar"		"snd_musicvolume"

				"min"			"0.0"
				"max"			"1.0"
				"step"			"0.1"
			}

			"SpeakerConfiguration"
			{
				"text"			"#GameUI_SpeakerConfiguration"
				"type"			"wheelywheel"
				"convar"		"snd_surround_speakers"

				"options"
				{
					"0"	"#GameUI_Headphones" [!$POSIX]
					"2"	"#GameUI_2Speakers"
					"4"	"#GameUI_4Speakers" [!$POSIX]
					"5"	"#GameUI_5Speakers" [!$POSIX]
					"7"	"#GameUI_7Speakers" [!$POSIX]
				}
			}

			"SoundQuality"
			{
				"text"			"#GameUI_SoundQuality"
				"type"			"wheelywheel"
				"convar"		"_gamepadui_sound_quality"

				"options"
				{
					"0"	"#GameUI_Low"
					"1"	"#GameUI_Medium"
					"2"	"#GameUI_High"
				}
			}

			"CloseCaption"
			{
				"text"			"#GameUI_CloseCaptions_Checkbox"
				"type"			"wheelywheel"
				"convar"		"_gamepadui_closecaptions"

				"options"
				{
					"0"	"#GameUI_NoClosedCaptions"
					"1"	"#GameUI_Subtitles"
					"2"	"#GameUI_SubtitlesAndSoundEffects"
				}
			}

			"LoseFocus"
			{
				"text"			"#GameUI_SndMuteLoseFocus"
				"type"			"wheelywheel"
				"convar"		"snd_mute_losefocus"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"OpenTechCredits"
			{
				"text"			"#GameUI_ThirdPartyTechCredits"
				"type"			"button"
				"command"		"open_techcredits"
			}
		}
	}
	"Video"
	{
		"title"			"#GameUI_Video"
		"alternating"   "1"
		"items"
		{
			"Resolution"
			{
				"text"			"#GameUI_Resolution"
				"type"			"wheelywheel"
				"convar"		"_gamepadui_resolution"

				"options_from"	"resolutions"
			}

			"AspectRatio"
			{
				"text"			"#GameUI_AspectRatio"
				"type"			"wheelywheel"

				"convar"		"_gamepadui_aspectratio"
				"instantapply"	"1"
				"options"
				{
					"0"			"#GameUI_AspectNormal"
					"1"			"#GameUI_AspectWide16x9"
					"2"			"#GameUI_AspectWide16x10"
				}
			}

			"DisplayMode"
			{
				"text"			"#GameUI_DisplayMode"
				"type"			"wheelywheel"
				"convar"		"_gamepadui_displaymode"
				"instantapply"	"1"

				"options_from"	"displaymode"
			}

			"AdvancedHeader"
			{
				"text"			"#GameUI_AdvancedNoEllipsis"
				"type"			"headeryheader"
				"advanced"		"1"
			}

			"DrawHUD"
			{
				"text"			"#GameUI_ToggleHUD"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"cl_drawhud"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"DrawViewModel"
			{
				"text"			"#GameUI_ToggleViewmodel"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"r_drawviewmodel"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Crosshair"
			{
				"text"			"#GameUI_Crosshair"
				"type"			"wheelywheel"
				"convar"		"crosshair"

				"options"
				{
					"0"		"#GameUI_CrosshairOff"
					"1"		"#GameUI_CrosshairOn"
				}
			}

			"RollAngle"
			{
				"text"			"#GameUI_CameraViewRollAngle"
				"type"			"slideyslide"
				"convar"		"sv_rollangle"
				"advanced"		"1"

				"min"			"0.0"
				"max"			"14.0"
				"step"			"0.1"

				"textprecision" "0"
			}

			"RollSpeed"
			{
				"text"			"#GameUI_CameraViewRollSpeed"
				"type"			"slideyslide"
				"convar"		"sv_rollspeed"
				"advanced"		"1"

				"min"			"0"
				"max"			"200"
				"step"			"1"

				"textprecision" "0"
			}

			"ModelDetail"
			{
				"text"			"#GameUI_Model_Detail"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"r_rootlod"

				"options"
				{
					"2"	"#gameui_low"
					"1"	"#gameui_medium"
					"0"	"#gameui_high"
				}
			}

			"TextureDetail"
			{
				"text"			"#GameUI_Texture_Detail"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_picmip"

				"options"
				{
					"2"		"#gameui_low"
					"1"		"#gameui_medium"
					"0"		"#gameui_high"
				}
			}

			"ShaderDetail"
			{
				"text"			"#GameUI_Shader_Detail"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_reducefillrate"

				"options"
				{
					"1"		"#gameui_low"
					"0"		"#gameui_high"
				}
			}

			"WaterDetail"
			{
				"text"			"#GameUI_Water_Detail"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"_gamepadui_water_detail"

				"options"
				{
					"0"		"#gameui_noreflections"
					"1"		"#gameui_reflectonlyworld"
					"2"		"#gameui_reflectall"
				}
			}

			"ShadowDetail"
			{
				"text"			"#GameUI_Shadow_Detail"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"_gamepadui_shadow_detail"

				"options"
				{
					"0"		"#gameui_low"
					"1"		"#gameui_medium"
					"2"		"#gameui_high"
				}
			}

			"MuzzleFlashDLight"
			{
				"text"			"#GameUI_MuzzleFlashDLight"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"cl_muzzleflash_dlight"

				"options"
				{
					"0"		"#GameUI_MuzzleDefault"
					"1"		"#GameUI_MuzzleYellowLight"
					"2"		"#GameUI_MuzzleWashedOut"
					"3"		"#GameUI_MuzzleValveStyled"
				}
			}

			"ColorCorrection"
			{
				"text"			"#GameUI_Color_Correction"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_colorcorrection"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Antialiasing"
			{
				"text"			"#GameUI_Antialiasing_Mode"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"_gamepadui_antialiasing"

				"options_from"	"antialiasing"
			}

			"FilteringMode"
			{
				"text"			"#GameUI_Filtering_Mode"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_forceaniso"

				"options"
				{
					"0"		"#GameUI_Bilinear"
					"1"		"#GameUI_Trilinear"
					"2"		"#GameUI_Anisotropic2X"
					"4"		"#GameUI_Anisotropic4X"
					"8"		"#GameUI_Anisotropic8X"
					"16"	"#GameUI_Anisotropic16X"
				}
			}

			"VerticalSync"
			{
				"text"			"#GameUI_Wait_For_VSync"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_vsync"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"MotionBlur"
			{
				"text"			"#GameUI_MotionBlur"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_motion_blur_enabled"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"FOV"
			{
				"text"			"#GameUI_FOV"
				"type"			"slideyslide"
				"convar"		"fov_desired"
				"advanced"		"1"

				"min"			"54"
				"max"			"150"
				"step"			"1"

				"textprecision" "0"
			}

			"ViewmodelFOV"
			{
				"text"			"#GameUI_FOVViewmodel"
				"type"			"slideyslide"
				"convar"		"viewmodel_fov"
				"advanced"		"1"

				"min"			"54"
				"max"			"75"
				"step"			"1"

				"textprecision" "0"
			}

			"FlipViewModels"
			{
				"text"			"#GameUI_FlipViewModels"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"cl_righthand"

				"options"
				{
					"0"		"#GameUI_FlipLeft"
					"1"		"#GameUI_FlipRight"
				}
			}

			"MulticoreRendering"
			{
				"text"			"#GameUI_MulticoreRendering"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_queue_mode"

				"options"
				{
					"0"		"#gameui_disabled"
					"-1"	"#gameui_enabled"
				}
			}

			"HDR"
			{
				"text"			"#GameUI_HDR"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_hdr_level"

				"options"
				{
					"0"		"#GameUI_hdr_level0"
					"1"		"#GameUI_hdr_level1"
					"2"		"#GameUI_hdr_level2"
				}
			}

			"HDRForceBloom"
			{
				"text"			"#GameUI_ForceBloom"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"mat_force_bloom"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
		}
	}
}
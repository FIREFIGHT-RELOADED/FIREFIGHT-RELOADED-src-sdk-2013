"Options"
{
	"Difficulty"
	{
		"title"	"#GameUI_Difficulty"
		"alternating"	"1"
		"items"
		{
			"BasicSkillSelection"
			{
				"text"	"#GameUI_SelectDifficulty"
				"type"	"wheelywheel"
				"convar"	"_gamepadui_skill"

				"options"
				{
					"0"	"#GameUI_Easy"
					"1"	"#GameUI_Medium"
					"2"	"#GameUI_Hard"
					"3"	"#GameUI_VeryHard"
					"4"	"#GameUI_Nightmare"
				}
			}

			"LoadoutSelection"
			{
				"text"	"#GameUI_SelectLoadout"
				"type"	"wheelywheel"
				"convar"	"sv_player_defaultloadout"
				"usesstring"	"1"

				"options"
				{
					"default"		        "#GameUI_LoadoutDefault"
					"default_nomelee"		"#GameUI_LoadoutDefaultNoMelee"
					"ironkick"		        "#GameUI_IronKickMode"
					"hardcore"		        "#GameUI_HardcoreMode"
					"default_hardcore"		        "#GameUI_LoadoutDefaultHardcore"
					"default_nomelee_hardcore"		        "#GameUI_LoadoutDefaultNoMeleeHardcore"
					"gaussrail"		        "#GameUI_LoadoutRailgun"
					"explosives"		        "#GameUI_LoadoutExplosives"
					"snipers"		        "#GameUI_LoadoutSnipers"
					"randomizer"            "#GameUI_LoadoutRandomizer"
					"randomizer_hardcore"	"#GameUI_LoadoutRandomizerPermadeath"
					"mapadd"		        "#FIREFIGHTRELOADED_ModMapping_Mapadd"
				}
			}

			"ClassicModifier"
			{
				"text"	"#GameUI_ClassicMode"
				"type"	"wheelywheel"
				"convar"	"g_fr_classic"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"LoneWolfModifier"
			{
				"text"	"#GameUI_LoneWolfMode"
				"type"	"wheelywheel"
				"convar"	"g_fr_lonewolf"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"SaveWeapons"
			{
				"text"	"#GameUI_SaveWeapons"
				"type"	"wheelywheel"
				"convar"	"sk_saveweapons"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"SavePurchasedWeapons"
			{
				"text"	"#GameUI_SaveWeapons_Store"
				"type"	"wheelywheel"
				"convar"	"sk_savepurchasedweapons"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"SaveDroppedWeapons"
			{
				"text"	"#GameUI_SaveWeapons_Drops"
				"type"	"wheelywheel"
				"convar"	"sk_savedroppedweapons"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}
			
			"Grapple"
			{
				"text"	"#GameUI_GrappleToggle"
				"type"	"wheelywheel"
				"convar"	"sv_player_grapple"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"Katana"
			{
				"text"	"#GameUI_KatanaToggle"
				"type"	"wheelywheel"
				"convar"	"sv_player_katana"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"Perks"
			{
				"text"	"#GameUI_Perks"
				"type"	"wheelywheel"
				"convar"	"sv_fr_perks"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}

			"AdvancedHeader"
			{
				"text"	"#GameUI_AdvancedNoEllipsis"
				"type"	"headeryheader"
				"advanced"	"1"
			}

			"ShowAdvancedGameOptions"
			{
				"text"	"#GameUI_ShowAdvancedOptions"
				"type"	"wheelywheel"
				"convar"	"gamepadui_showadvancedoptions"
				"advanced"	"1"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#gameui_enabled"
				}
			}
			
			"CustomPlayerOptions"
			{
				"text"			"#GameUI_Difficulty_CustomPlayer"
				"type"			"headeryheader"
				"advanced"		"1"
				"hidden"		"1"
			}

			"DeckOverride"
			{
				"text"			"#GameUI_DeckOverride"
				"type"			"wheelywheel"
				"convar"		"cl_deck_override_client_settings"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StartingKash"
			{
				"text"			"#GameUI_StartingKash"
				"type"			"wheelywheel"
				"convar"		"sv_player_startingmoney"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StartingKashAmount"
			{
				"text"			"#GameUI_StartingKashAmount"
				"type"			"slideyslide"
				"convar"		"sv_player_startingmoney_amount"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"9999"
				"step"			"5"
				"step_adjustment"	"10"

				"textprecision" "2"
			}

			"PlayerHealth"
			{
				"text"			"#GameUI_PlayerHealth"
				"type"			"slideyslide"
				"convar"		"player_defaulthealth"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"999"
				"step"			"5"

				"textprecision" "2"
			}

			"PlayerArmor"
			{
				"text"			"#GameUI_PlayerArmor"
				"type"			"slideyslide"
				"convar"		"player_defaultarmor"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"999"
				"step"			"5"

				"textprecision" "2"
			}

			"PlayerMaxArmor"
			{
				"text"			"#GameUI_PlayerMaxArmor"
				"type"			"slideyslide"
				"convar"		"player_maxarmor"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"999"
				"step"			"5"

				"textprecision" "2"
			}

			"PlayerBaseXP"
			{
				"text"			"#GameUI_PlayerBaseXP"
				"type"			"slideyslide"
				"convar"		"player_basexp"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"9999"
				"step"			"5"
				"step_adjustment"	"10"

				"textprecision" "2"
			}

			"ArmorMode"
			{
				"text"			"#GameUI_ArmorMode"
				"type"			"wheelywheel"
				"convar"		"player_armor_mode"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#GameUI_ArmorMode_HL1"
					"1"		"#GameUI_ArmorMode_HL2"
					"2"		"#GameUI_ArmorMode_FR"
				}
			}

			"GrappleRangeRestriction"
			{
				"text"			"#GameUI_GrappleRangeRestriction"
				"type"			"wheelywheel"
				"convar"		"sk_grapple_rangerestriction"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"GrappleBatteryDrain"
			{
				"text"			"#GameUI_GrappleBatteryDrain"
				"type"			"wheelywheel"
				"convar"		"sk_grapple_batterydrain"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"GrappleBatteryDrainTime"
			{
				"text"			"#GameUI_GrappleBatteryDrainTime"
				"type"			"slideyslide"
				"convar"		"sk_grapple_batterydrain_time"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"1"
				"step"			"0.01"

				"textprecision" "2"
			}

			"GrappleBatteryDrainAmount"
			{
				"text"			"#GameUI_GrappleBatteryDrainAmount"
				"type"			"slideyslide"
				"convar"		"sk_grapple_batterydrain_amount"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"100"
				"step"			"5"

				"textprecision" "2"
			}

			"GrappleMaxRange"
			{
				"text"			"#GameUI_GrappleMaxRange"
				"type"			"slideyslide"
				"convar"		"sk_grapple_rangerestriction_max"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"100000"
				"step"			"5"
				"step_adjustment"	"20"

				"textprecision" "2"
			}

			"RocketJumping"
			{
				"text"			"#GameUI_RocketJumping"
				"type"			"wheelywheel"
				"convar"		"sv_player_rocketjumping"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"MaxSpeed"
			{
				"text"			"#GameUI_ChangeMaxSpeed"
				"type"			"wheelywheel"
				"convar"		"sv_leagcy_maxspeed"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#GameUI_ChangeMaxSpeedFR"
					"1"		"#GameUI_ChangeMaxSpeedHL2"
				}
			}
			
			"BullettimeScale"
			{
				"text"			"#GameUI_BullettimeSpeed"
				"type"			"slideyslide"
				"convar"		"sv_player_bullettime_timescale"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"100"
				"step"			"1"

				"textprecision" "2"
			}
			
			"BullettimeScaleShop"
			{
				"text"			"#GameUI_BullettimeSpeedStore"
				"type"			"slideyslide"
				"convar"		"sv_player_bullettime_shop_timescale"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"100"
				"step"			"1"

				"textprecision" "2"
			}

			"HealthChargerRecharge"
			{
				"text"			"#GameUI_HealthCharger_Recharge"
				"type"			"wheelywheel"
				"convar"		"sv_healthcharger_recharge"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"HealthChargerRechargeTime"
			{
				"text"			"#GameUI_HealthCharger_Recharge_Time"
				"type"			"slideyslide"
				"convar"		"sv_healthcharger_recharge_time"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"300"
				"step"			"5"

				"textprecision" "2"
			}

			"SuitChargerRecharge"
			{
				"text"			"#GameUI_SuitCharger_Recharge"
				"type"			"wheelywheel"
				"convar"		"sv_suitcharger_recharge"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SuitChargerRechargeTime"
			{
				"text"			"#GameUI_SuitCharger_Recharge_Time"
				"type"			"slideyslide"
				"convar"		"sv_suitcharger_recharge_time"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"300"
				"step"			"5"

				"textprecision" "2"
			}

			"HealthChargerUsesMoney"
			{
				"text"			"#GameUI_HealthChargerUsesMoney"
				"type"			"wheelywheel"
				"convar"		"sk_healthcharger_usesmoney"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SuitChargerUsesMoney"
			{
				"text"			"#GameUI_SuitChargerUsesMoney"
				"type"			"wheelywheel"
				"convar"		"sk_suitcharger_usesmoney"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ToggleEconomy"
			{
				"text"			"#GameUI_ToggleEconomy"
				"type"			"wheelywheel"
				"convar"		"g_fr_economy"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PerksInfiniteAuxPower"
			{
				"text"			"#GameUI_Perks_InfiniteAuxPower"
				"type"			"wheelywheel"
				"convar"		"sv_fr_perks_infiniteauxpower"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PerksInfiniteAmmo"
			{
				"text"			"#GameUI_Perks_InfiniteAmmo"
				"type"			"wheelywheel"
				"convar"		"sv_fr_perks_infiniteammo"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"HealthRegenPerk"
			{
				"text"			"#GameUI_MakeHealthRegenPerk"
				"type"			"wheelywheel"
				"convar"		"sv_fr_perks_healthregeneration_perkmode"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"PerksHealthRegenRate"
			{
				"text"			"#GameUI_Perks_HelthRegenRate"
				"type"			"wheelywheel"
				"convar"		"sv_fr_perks_healthregenerationrate"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"HealthRegenInGeneral"
			{
				"text"			"#GameUI_HealthRegen"
				"type"			"wheelywheel"
				"convar"		"sv_regeneration"
				"advanced"		"1"
				"hidden"		"1"

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
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"RewardAttempts"
			{
				"text"			"#GameUI_RewardAttempts"
				"type"			"slideyslide"
				"convar"		"sv_fr_reward_attemptcount"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"15"
				"step"			"1"

				"textprecision" "2"
			}

			"PlayerAllyRegenTime"
			{
				"text"			"#GameUI_PlayerAllyRegenTime"
				"type"			"slideyslide"
				"convar"		"sk_ally_regen_time"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.1"
				"max"			"2"
				"step"			"0.1"

				"textprecision" "2"
			}

			"CustomWeaponOptions"
			{
				"text"			"#GameUI_Difficulty_CustomWep"
				"type"			"headeryheader"
				"advanced"		"1"
				"hidden"		"1"
			}

			"RailgunOverchargeLimit"
			{
				"text"			"#GameUI_RailgunOverchargeLimit"
				"type"			"slideyslide"
				"convar"		"sk_weapon_railgun_overcharge_limit"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"999"
				"step"			"5"

				"textprecision" "2"
			}

			"PumpSecondaryFire"
			{
				"text"			"#GameUI_PumpSecondaryFire"
				"type"			"wheelywheel"
				"convar"		"sv_shotgun_pumpsecondaryfire"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"InfiniteKnives"
			{
				"text"			"#GameUI_InfiniteKnives"
				"type"			"wheelywheel"
				"convar"		"sv_infinite_knives"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"InfiniteTurrets"
			{
				"text"			"#GameUI_InfiniteTurrets"
				"type"			"wheelywheel"
				"convar"		"sv_infinite_turrets"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ShootInZoom"
			{
				"text"			"#GameUI_ShootInZoom"
				"type"			"wheelywheel"
				"convar"		"sv_player_shootinzoom"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SMG1GrenadeGlassPassthrough"
			{
				"text"			"#GameUI_SMG1GrenadeGlassPassthrough"
				"type"			"wheelywheel"
				"convar"		"smg1_grenade_glass_passthrough"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"CrossbowGlassPassthrough"
			{
				"text"			"#GameUI_CrossbowGlassPassthrough"
				"type"			"wheelywheel"
				"convar"		"crossbow_new_glass_passthrough"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"MagStyledReloads"
			{
				"text"			"#GameUI_MagazineStyledReloads"
				"type"			"wheelywheel"
				"convar"		"weapon_magazinestyledreloads"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ItemRespawn"
			{
				"text"			"#GameUI_Item_Respawn"
				"type"			"wheelywheel"
				"convar"		"sv_item_respawn"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"WeaponRespawn"
			{
				"text"			"#GameUI_Weapon_Respawn"
				"type"			"wheelywheel"
				"convar"		"sv_weapon_respawn"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"CombineBallSeekAngle"
			{
				"text"			"#GameUI_CombineBallSeekAngle"
				"type"			"slideyslide"
				"convar"		"sk_combineball_seek_angle"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"5"
				"max"			"30"
				"step"			"5"

				"textprecision" "2"
			}

			"CombineBallGuideFactor"
			{
				"text"			"#GameUI_CombineBallGuideFactor"
				"type"			"slideyslide"
				"convar"		"sk_combineball_guidefactor"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.1"
				"max"			"4"
				"step"			"0.1"

				"textprecision" "2"
			}

			"CombineBallSearchRadius"
			{
				"text"			"#GameUI_CombineBallSearchRadius"
				"type"			"slideyslide"
				"convar"		"sk_combine_ball_search_radius"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"16"
				"max"			"2048"
				"step"			"8"

				"textprecision" "2"
			}

			"CombineBallSeekKill"
			{
				"text"			"#GameUI_CombineBallSeekKill"
				"type"			"wheelywheel"
				"convar"		"sk_combineball_seek_kill"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"CustomEnemyOptions"
			{
				"text"			"#GameUI_Difficulty_CustomEnemy"
				"type"			"headeryheader"
				"advanced"		"1"
				"hidden"		"1"
			}

			"SpawnerHideFromPlayer"
			{
				"text"			"#GameUI_SpawnerHideFromPlayer"
				"type"			"wheelywheel"
				"convar"		"sk_spawnerhidefromplayer"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SpawnerDifficultyAdjust"
			{
				"text"			"#GameUI_SpawnerDifficultyAdjust"
				"type"			"wheelywheel"
				"convar"		"sk_spawner_difficultyadjust"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SpawnerDeckAdjust"
			{
				"text"			"#GameUI_SpawnerDeckAdjust"
				"type"			"slideyslide"
				"convar"		"sk_spawner_deckadjustfactor"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"3"
				"step"			"0.1"

				"textprecision" "2"
			}
			
			"SpawnRareEnemies"
			{
				"text"			"#GameUI_SpawnRareEnemies"
				"type"			"wheelywheel"
				"convar"		"sk_spawnrareenemies"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"RumbleEnemyAttack"
			{
				"text"			"#GameUI_FIREFIGHTRUMBLE_EnemyAttack"
				"type"			"wheelywheel"
				"convar"		"firefightrumble_enemyattack"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AIFear"
			{
				"text"			"#GameUI_AIFear"
				"type"			"wheelywheel"
				"convar"		"ai_enable_fear_behavior"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"SecondaryFireShotgunner"
			{
				"text"			"#GameUI_CombineSecondary"
				"type"			"wheelywheel"
				"convar"		"sv_combine_shotgunner_secondaryfire"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SecondaryFireShotgunnerChance"
			{
				"text"			"#GameUI_CombineSecondaryChance"
				"type"			"slideyslide"
				"convar"		"sv_combine_shotgunner_secondaryfire_chance"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"20"
				"step"			"5"

				"textprecision" "2"
			}

			"CombineSoldierGrenades"
			{
				"text"			"#GameUI_CombineSoldierGrenades"
				"type"			"wheelywheel"
				"convar"		"combine_soldier_spawnwithgrenades"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"CombineAceBulletResistance"
			{
				"text"			"#GameUI_CombineAceBulletResistance"
				"type"			"wheelywheel"
				"convar"		"combine_ace_disablebulletresistance"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"CombineTacticalVariants"
			{
				"text"			"#GameUI_CombineTacticalVariants"
				"type"			"wheelywheel"
				"convar"		"combine_spawnwithtacticalvariant"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AceShieldMode"
			{
				"text"			"#GameUI_AceShieldMode"
				"type"			"wheelywheel"
				"convar"		"combine_ace_shieldspawnmode"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#GameUI_ShieldMode_Ace_None"
					"1"		"#GameUI_ShieldMode_Ace_All"
					"2"		"#GameUI_ShieldMode_Ace_Random"
				}
			}

			"MetropoliceManhacks"
			{
				"text"			"#GameUI_MetropoliceManhacks"
				"type"			"wheelywheel"
				"convar"		"metropolice_spawnwithmanhacks"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"MetrocopReactionTime"
			{
				"text"			"#GameUI_MetrocopReactionTime"
				"type"			"slideyslide"
				"convar"		"sk_metropolice_stitch_reaction"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.1"
				"max"			"2"
				"step"			"0.1"

				"textprecision" "2"
			}

			"HeadcrablessDamage"
			{
				"text"			"#GameUI_HeadcrablessDamage"
				"type"			"wheelywheel"
				"convar"		"zombie_headcrabless_damage"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Armored"
			{
				"text"			"#GameUI_ZombieArmor"
				"type"			"wheelywheel"
				"convar"		"sk_zombie_armored"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ArmoredRarity"
			{
				"text"			"#GameUI_ZombieArmorRarity"
				"type"			"slideyslide"
				"convar"		"sk_zombie_armored_rarity"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"20"
				"step"			"1"

				"textprecision" "2"
			}

			"SayCheese"
			{
				"text"			"#GameUI_ScannersBlindPlayer"
				"type"			"wheelywheel"
				"convar"		"sv_cscanner_blindplayer"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SayCheeseOnTheXbox360"
			{
				"text"			"#GameUI_ScannerBlindMode"
				"type"			"wheelywheel"
				"convar"		"sv_cscanner_blindtime"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"1"		"#GameUI_ScannerBlindMode_HL2"
					"2"		"#GameUI_ScannerBlindMode_OB360"
				}
			}

			"SayCheeseWithBackjackAndHookers"
			{
				"text"			"#GameUI_ScannerBlindOverride"
				"type"			"slideyslide"
				"convar"		"sv_cscanner_blindtime_custom"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"15"
				"step"			"5"

				"textprecision" "2"
			}

			"SayCheeseHoldOnTheXbox360"
			{
				"text"			"#GameUI_ScannerHoldBlindMode"
				"type"			"wheelywheel"
				"convar"		"sv_cscanner_blindholdtime"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"1"		"#GameUI_ScannerHoldBlindMode_HL2"
					"2"		"#GameUI_ScannerHoldBlindMode_FR"
				}
			}

			"SayCheeseHoldWithBackjackAndHookers"
			{
				"text"			"#GameUI_ScannerHoldBlindOverride"
				"type"			"slideyslide"
				"convar"		"sv_cscanner_blindholdtime_custom"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"15"
				"step"			"5"

				"textprecision" "2"
			}

			"RollermineStunDelay"
			{
				"text"			"#GameUI_RollermineStunDelay"
				"type"			"slideyslide"
				"convar"		"sk_rollermine_stun_delay"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.1"
				"max"			"3.5"
				"step"			"0.1"

				"textprecision" "2"
			}

			"StalkerLasers"
			{
				"text"			"#GameUI_StalkerLasers"
				"type"			"wheelywheel"
				"convar"		"stalker_skilllevellasers"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ControllerBallSpeed"
			{
				"text"			"#GameUI_ControllerBallSpeed"
				"type"			"slideyslide"
				"convar"		"sk_controller_speedball"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"100"
				"max"			"1500"
				"step"			"5"

				"textprecision" "2"
			}

			"BossGoTo_DroneKill"
			{
				"text"			"#GameUI_BossGoTo_DroneKill"
				"type"			"wheelywheel"
				"convar"		"sk_gotoboss_ondronekill"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"BossGoTo_OnDeath"
			{
				"text"			"#GameUI_BossGoTo_OnDeath"
				"type"			"wheelywheel"
				"convar"		"sk_gotoboss_ondeath"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"BossGoTo_Level"
			{
				"text"			"#GameUI_BossGoTo_Level"
				"type"			"slideyslide"
				"convar"		"sk_gotoboss_onlevel"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"20"
				"step"			"5"

				"textprecision" "2"
			}

			"BossGoTo_ClassicKillCount"
			{
				"text"			"#GameUI_BossGoTo_ClassicKillCount"
				"type"			"slideyslide"
				"convar"		"sk_gotoboss_classic_onkillcount"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"1000"
				"step"			"10"

				"textprecision" "2"
			}

			"AdvisorSpeed"
			{
				"text"			"#GameUI_AdvisorSpeed"
				"type"			"slideyslide"
				"convar"		"advisor_speed"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"250"
				"step"			"5"

				"textprecision" "2"
			}

			"AdvisorThrowVelocity"
			{
				"text"			"#GameUI_AdvisorThrowVelocity"
				"type"			"slideyslide"
				"convar"		"advisor_throw_velocity"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"750"
				"max"			"20000"
				"step"			"5"

				"textprecision" "2"
			}

			"AdvisorThrowRate"
			{
				"text"			"#GameUI_AdvisorThrowRate"
				"type"			"slideyslide"
				"convar"		"advisor_throw_rate"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"10"
				"step"			"1"

				"textprecision" "2"
			}

			"AdvisorThrowWarnTime"
			{
				"text"			"#GameUI_AdvisorThrowWarnTime"
				"type"			"slideyslide"
				"convar"		"advisor_throw_warn_time"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.1"
				"max"			"2"
				"step"			"0.1"

				"textprecision" "2"
			}

			"AdvisorThrowLeadPrefetchTime"
			{
				"text"			"#GameUI_AdvisorThrowLeadPrefetchTime"
				"type"			"slideyslide"
				"convar"		"advisor_throw_lead_prefetch_time"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.1"
				"max"			"2"
				"step"			"0.1"

				"textprecision" "2"
			}

			"AdvisorStagingNum"
			{
				"text"			"#GameUI_AdvisorStagingNum"
				"type"			"slideyslide"
				"convar"		"advisor_staging_num"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"8"
				"step"			"1"

				"textprecision" "2"
			}

			"AdvisorBulletResistThrowVelocity"
			{
				"text"			"#GameUI_AdvisorBulletResistThrowVelocity"
				"type"			"slideyslide"
				"convar"		"advisor_bulletresistance_throw_velocity"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"750"
				"max"			"20000"
				"step"			"10"

				"textprecision" "2"
			}

			"AdvisorBulletResistThrowRate"
			{
				"text"			"#GameUI_AdvisorBulletResistThrowRate"
				"type"			"slideyslide"
				"convar"		"advisor_bulletresistance_throw_rate"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"10"
				"step"			"1"

				"textprecision" "2"
			}

			"AdvisorBulletResistStagingNum"
			{
				"text"			"#GameUI_AdvisorBulletResistStagingNum"
				"type"			"slideyslide"
				"convar"		"advisor_bulletresistance_staging_num"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"8"
				"step"			"1"

				"textprecision" "2"
			}

			"AdvisorBulletResistSpeed"
			{
				"text"			"#GameUI_AdvisorBulletResistSpeed"
				"type"			"slideyslide"
				"convar"		"advisor_bulletresistance_speed"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"150"
				"step"			"10"

				"textprecision" "2"
			}

			"AdvisorBulletResistance"
			{
				"text"			"#GameUI_AdvisorBulletResistance"
				"type"			"wheelywheel"
				"convar"		"advisor_disablebulletresistance"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AdvisorPrematureDroning"
			{
				"text"			"#GameUI_AdvisorPrematureDroning"
				"type"			"wheelywheel"
				"convar"		"advisor_enable_premature_droning"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AdvisorDroning"
			{
				"text"			"#GameUI_AdvisorDroning"
				"type"			"wheelywheel"
				"convar"		"advisor_enable_droning"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AIEnhancedPerseption"
			{
				"text"			"#GameUI_AIEnhancedPerseption"
				"type"			"wheelywheel"
				"convar"		"ai_enhanced_perseption"
				"advanced"		"1"
				"hidden"		"1"

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
				"advanced"		"1"
				"hidden"		"1"

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
				"hidden"		"1"

				"min"			"1"
				"max"			"15"
				"step"			"1"

				"textprecision" "2"
			}

			"AttributesNumPresets"
			{
				"text"			"#GameUI_AttributesPresetCount"
				"type"			"slideyslide"
				"convar"		"entity_attributes_numpresets"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1"
				"max"			"30"
				"step"			"1"

				"textprecision" "2"
			}
		}
	}
	"Gameplay"
	{
		"title"			"#GameUI_Game"
		"alternating"	"1"
		"items"
		{
			"FasterSwitch"
			{
				"text"			"#GameUI_FasterWeaponSwitch"
				"type"			"wheelywheel"
				"convar"		"weapon_quickswitch"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"AutoswitchOnPickup"
			{
				"text"			"#GameUI_Autoswitch"
				"type"			"wheelywheel"
				"convar"		"sv_player_autoswitch"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"AutoswitchOnReward"
			{
				"text"			"#GameUI_AutoswitchOnReward"
				"type"			"wheelywheel"
				"convar"		"sv_player_autoswitchonreward"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AutoAim"
			{
				"text"			"#GameUI_Autoaim"
				"type"			"wheelywheel"
				"convar"		"sk_allow_autoaim"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
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

			"AutoaimMode"
			{
				"text"			"#GameUI_AutoaimMode"
				"type"			"wheelywheel"
				"convar"		"sk_autoaim_mode"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
					"2"		"#gameui_enabled_controller"
				}
			}

			"AutosaveLevelUp"
			{
				"text"			"#GameUI_AutosaveLevelUp"
				"type"			"wheelywheel"
				"convar"		"sv_autosave_levelup"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Headshots"
			{
				"text"			"#GameUI_Headshots"
				"type"			"wheelywheel"
				"convar"		"g_fr_headshotgore"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"AllyPlayerModel"
			{
				"text"			"#GameUI_AlliesUseModel"
				"type"			"wheelywheel"
				"convar"		"npc_playerbot_useplayersmodel"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AllyTalk"
			{
				"text"			"#GameUI_AlliesTalk"
				"type"			"wheelywheel"
				"convar"		"npc_playerbot_talk"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"KillingSprees"
			{
				"text"			"#GameUI_KillingSprees"
				"type"			"wheelywheel"
				"convar"		"sv_killingspree"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"UseTimer"
			{
				"text"			"#GameUI_UseTimer"
				"type"			"wheelywheel"
				"convar"		"cl_fr_usetimer"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Flashlight"
			{
				"text"			"#GameUI_UseLeagcyFlashlight"
				"type"			"wheelywheel"
				"convar"		"sv_legacy_flashlight"

				"options"
				{
					"0"		"#GameUI_EP2Flashlight"
					"1"		"#GameUI_HL2Flashlight"
				}
			}

			"MobilityHeader"
			{
				"text"			"#GameUI_MobilityHeader"
				"type"			"headeryheader"
			}

			"ToggleBunnyhop"
			{
				"text"			"#GameUI_ToggleBunnyhop"
				"type"			"wheelywheel"
				"convar"		"fr_enable_bunnyhop"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"TogglePowerslide"
			{
				"text"			"#GameUI_TogglePowerslide"
				"type"			"wheelywheel"
				"convar"		"sv_slide"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ToggleWallrun"
			{
				"text"			"#GameUI_ToggleWallrun"
				"type"			"wheelywheel"
				"convar"		"sv_wallrun"

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

			"ShowAdvancedGameOptions2"
			{
				"text"			"#GameUI_ShowAdvancedOptions"
				"type"			"wheelywheel"
				"convar"		"gamepadui_showadvancedoptions"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"LowerVMOnSprint"
			{
				"text"			"#GameUI_LowerOnSprint"
				"type"			"wheelywheel"
				"convar"		"viewmodel_lower_on_sprint"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ShowNPCNames"
			{
				"text"			"#GameUI_ShowNPCNames"
				"type"			"wheelywheel"
				"convar"		"sv_killog_shownpcnames"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"NPCvNPCKills"
			{
				"text"			"#GameUI_NPCvNPCKills"
				"type"			"wheelywheel"
				"convar"		"ai_allow_npcvnpc_killlog"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PlayerDropWeaponsOnDeath"
			{
				"text"			"#GameUI_PlayerDropWeaponsOnDeath"
				"type"			"wheelywheel"
				"convar"		"sv_player_dropweaponsondeath"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StoreNotificationBuy"
			{
				"text"			"#GameUI_StoreNotificationBuy"
				"type"			"wheelywheel"
				"convar"		"sv_store_buynotifications"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StoreNotificationDeny"
			{
				"text"			"#GameUI_StoreNotificationDeny"
				"type"			"wheelywheel"
				"convar"		"sv_store_denynotifications"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ToggleFloatyMove"
			{
				"text"			"#GameUI_ToggleFloatyMove"
				"type"			"wheelywheel"
				"convar"		"fr_floatymove"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"GravityAdjust"
			{
				"text"			"#GameUI_GravityAdjust"
				"type"			"wheelywheel"
				"convar"		"sv_gravity"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"600"		"#GameUI_GravityAdjust_HL2"
					"800"		"#GameUI_GravityAdjust_TF2"
					"1100"		"#GameUI_GravityAdjust_ENDGAME"
				}
			}

			"JumpHeight"
			{
				"text"			"#GameUI_JumpHeight"
				"type"			"slideyslide"
				"convar"		"sv_jump_height"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"1.0"
				"max"			"99.0"
				"step"			"0.5"

				"textprecision" "2"
			}

			"ToggleWallrunAnticipation"
			{
				"text"			"#GameUI_ToggleWallrunAnticipation"
				"type"			"wheelywheel"
				"convar"		"sv_wallrun_anticipation"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#GameUI_ToggleWallrunAnticipation_None"
					"1"		"#GameUI_ToggleWallrunAnticipation_ViewRollOnly"
					"2"		"#GameUI_ToggleWallrunAnticipation_Full"
				}
			}

			"WallrunRollAngle"
			{
				"text"			"#GameUI_WallrunViewRollAngle"
				"type"			"slideyslide"
				"convar"		"sv_wallrun_roll"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.0"
				"max"			"14.0"
				"step"			"0.5"

				"textprecision" "2"
			}

			"WallrunDirectControl"
			{
				"text"			"#GameUI_WallrunDirectControl"
				"type"			"wheelywheel"
				"convar"		"sv_wallrun_requiredirectcontrol"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"IronsightSpeed"
			{
				"text"			"#GameUI_IronsightTime"
				"type"			"slideyslide"
				"convar"		"ironsight_speed"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"6.5"
				"max"			"50"
				"step"			"0.1"

				"textprecision" "2"
			}
			
			"RollAngle"
			{
				"text"			"#GameUI_CameraViewRollAngle"
				"type"			"slideyslide"
				"convar"		"sv_rollangle"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0.0"
				"max"			"14.0"
				"step"			"0.5"

				"textprecision" "2"
			}

			"RollSpeed"
			{
				"text"			"#GameUI_CameraViewRollSpeed"
				"type"			"slideyslide"
				"convar"		"sv_rollspeed"
				"advanced"		"1"
				"hidden"		"1"

				"min"			"0"
				"max"			"200"
				"step"			"5"

				"textprecision" "2"
			}

			"OldSpawner"
			{
				"text"			"#FR_OldSpawner"
				"type"			"wheelywheel"
				"convar"		"g_fr_spawneroldfunctionality"
				"advanced"		"1"
				"hidden"		"1"

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
				"hidden"		"1"

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
			
			"ServerRagdoll"
			{
				"text"			"#GameUI_NPCCorpseServer"
				"type"			"wheelywheel"
				"convar"		"ai_force_serverside_ragdoll"
				"advanced"		"1"
				"hidden"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
		}
	}
	"Keyboard"
	{
		"title"			"#GameUI_Keyboard"
		"items_from"	"keyboard"
		"alternating"	"1"
		"items"
		{
			"AdvancedHeader"
			{
				"text"			"#GameUI_AdvancedNoEllipsis"
				"type"			"headeryheader"
				"advanced"		"1"
			}

			"FastWeaponSwitch"
			{
				"text"			"#GameUI_FastSwitchCheck"
				"type"			"wheelywheel"
				"convar"		"hud_fastswitch"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"DeveloperConsole"
			{
				"text"			"#GameUI_DeveloperConsoleCheck"
				"type"			"wheelywheel"
				"convar"		"con_enable"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
		}
	}
	"Mouse"
	{
		"title"			"#GameUI_Mouse"
		"alternating"	"1"
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
		"alternating"	"1"
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

			"MusicSystem"
			{
				"text"			"#GameUI_MusicSystem"
				"type"			"wheelywheel"
				"convar"		"snd_fmod_musicsystem"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"MusicSystemHUD"
			{
				"text"			"#GameUI_MusicSystemHUD"
				"type"			"wheelywheel"
				"convar"		"snd_fmod_musicsystem_shownowplayinghud"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"MusicSystemForceShuffle"
			{
				"text"			"#GameUI_MusicSystemForceShuffle"
				"type"			"wheelywheel"
				"convar"		"snd_fmod_musicsystem_forceshuffle"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
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

			"LoseFocus2"
			{
				"text"			"#GameUI_SndPauseLoseFocus"
				"type"			"wheelywheel"
				"convar"		"snd_fmod_pause_losefocus"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PauseOnPause"
			{
				"text"			"#GameUI_SndPauseGamePaused"
				"type"			"wheelywheel"
				"convar"		"snd_fmod_pause_gamepaused"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"BullettimePitchControl"
			{
				"text"			"#GameUI_BullettimePitchControl"
				"type"			"wheelywheel"
				"convar"		"snd_timescale_pitchcontrol"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"BullettimePitchControlOverride"
			{
				"text"			"#GameUI_BullettimePitchControlOverride"
				"type"			"slideyslide"
				"convar"		"snd_timescale_pitchcontrol_pitchoverride"
				"advanced"		"1"

				"min"			"0"
				"max"			"1"
				"step"			"0.1"

				"textprecision" "2"
			}
			
			"ExplosionRinging"
			{
				"text"			"#GameUI_ExplosionRinging"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"sv_player_explosionringing"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"RailgunWarningBeepTime"
			{
				"text"			"#GameUI_RailgunWarningBeepTime"
				"type"			"slideyslide"
				"convar"		"sk_weapon_railgun_warning_beep_time"
				"advanced"		"1"

				"min"			"3"
				"max"			"30"
				"step"			"0.5"

				"textprecision" "2"
			}

			"SuitBootup"
			{
				"text"			"#GameUI_SuitBootupMode"
				"type"			"wheelywheel"
				"convar"		"sv_suitintro"

				"options"
				{
					"0"	"#gameui_disabled"
					"1"	"#GameUI_SuitBootupMode_HL1"
					"2"	"#GameUI_SuitBootupMode_HL1S"
					"3"	"#GameUI_SuitBootupMode_FR"
					"3"	"#GameUI_SuitBootupMode_FRS"
				}
			}
			
			"ManOfAFewWordsArentYou"
			{
				"text"			"#GameUI_PlayerVoice"
				"type"			"wheelywheel"
				"convar"		"sv_player_voice"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ManOfAFewWordsArentYouKill"
			{
				"text"			"#GameUI_ToggleKillSound"
				"type"			"wheelywheel"
				"convar"		"sv_player_voice_kill"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ManOfAFewWordsArentYouDeath"
			{
				"text"			"#GameUI_ToggleDeathSound"
				"type"			"wheelywheel"
				"convar"		"sv_player_voice_death"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ManOfAFewWordsArentYouPerk"
			{
				"text"			"#GameUI_TogglePerkSound"
				"type"			"wheelywheel"
				"convar"		"sv_player_voice_perk"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ManOfAFewWordsArentYouHit"
			{
				"text"			"#GameUI_ToggleHitSound"
				"type"			"wheelywheel"
				"convar"		"sv_player_voice_hit"
				"advanced"		"1"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"HunterGoesHaHaHaHaHaHaHa"
			{
				"text"			"#GameUI_HuntersLaugh"
				"type"			"wheelywheel"
				"convar"		"hunter_laugh"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PlayerDamageSounds"
			{
				"text"			"#GameUI_PlayerDamageSounds"
				"type"			"wheelywheel"
				"convar"		"sv_player_dmgsounds"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StoreMusic"
			{
				"text"			"#GameUI_StoreMusic"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"gamepadui_store_music"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StoreSoundBuy"
			{
				"text"			"#GameUI_StoreSoundBuy"
				"type"			"wheelywheel"
				"convar"		"sv_store_buysounds"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"StoreSoundDeny"
			{
				"text"			"#GameUI_StoreSoundDeny"
				"type"			"wheelywheel"
				"convar"		"sv_store_denysounds"

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
		"alternating"	"1"
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

			"Glyphs"
			{
				"text"			"#Deck_Glyphs"
				"type"			"wheelywheel"
				"convar"		"cl_glyphtype"

				"options"
				{
					"0"		"#Deck_Glyphs_Off"
					"1"		"#Deck_Glyphs_Xbone"
					"2"		"#Deck_Glyphs_PS"
					"3"		"#Deck_Glyphs_Switch"
					"4"		"#Deck_Glyphs_Generic"
					"5"		"#Deck_Glyphs_Keyboard"
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

				"textprecision" "2"
			}

			"ViewmodelFOV"
			{
				"text"			"#GameUI_FOVViewmodel"
				"type"			"slideyslide"
				"convar"		"viewmodel_fov"
				"advanced"		"1"

				"min"			"54"
				"max"			"90"
				"step"			"1"

				"textprecision" "2"
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

			"DeathcamView"
			{
				"text"			"#GameUI_DeathcamMode"
				"type"			"wheelywheel"
				"convar"		"cl_deathcam_mode"

				"options"
				{
					"0"		"#GameUI_DeathcamMode_HL2"
					"1"		"#GameUI_DeathcamMode_HL2MP"
					"2"		"#GameUI_DeathcamMode_FP"
				}
			}

			"IronsightVignette"
			{
				"text"			"#GameUI_IronsightVignette"
				"type"			"wheelywheel"
				"convar"		"sv_ironsightvignette"

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

			"CrosshairAppearanceHeader"
			{
				"text"			"#GameUI_CrosshairDescription"
				"type"			"headeryheader"
				"advanced"		"1"
			}

			"CrosshairRed"
			{
				"text"			"#GameUI_CrosshairRed"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"cl_crosshair_red"

				"min"			"0"
				"max"			"255"
				"step"			"5"

				"textprecision" "2"
			}

			"CrosshairGreen"
			{
				"text"			"#GameUI_CrosshairGreen"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"cl_crosshair_green"

				"min"			"0"
				"max"			"255"
				"step"			"5"

				"textprecision" "2"
			}

			"CrosshairRedBlue"
			{
				"text"			"#GameUI_CrosshairBlue"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"cl_crosshair_blue"

				"min"			"0"
				"max"			"255"
				"step"			"5"

				"textprecision" "2"
			}

			"AdvancedHeader"
			{
				"text"			"#GameUI_AdvancedNoEllipsis"
				"type"			"headeryheader"
				"advanced"		"1"
			}

			"DeathcamViewFPSwitch"
			{
				"text"			"#GameUI_DeathcamMode_FP_Switch"
				"type"			"wheelywheel"
				"convar"		"cl_deathcam_fp_autoswitch"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#GameUI_DeathcamMode_HL2"
					"2"		"#GameUI_DeathcamMode_HL2MP"
				}
			}

			"DeathcamViewFPSwitchMinDistance"
			{
				"text"			"#GameUI_DeathcamMode_FP_Switch_MinDist"
				"type"			"slideyslide"
				"convar"		"cl_deathcam_fp_autoswitch_mindistance"

				"min"			"1"
				"max"			"25"
				"step"			"0.5"

				"textprecision" "2"
			}

			"ClassicBlood"
			{
				"text"			"#GameUI_ClassicBlood"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"r_classic_blood"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"ClassicFire"
			{
				"text"			"#GameUI_ClassicFire"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"_gamepadui_classicfire"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"NPCMuzzleSmoke"
			{
				"text"			"#GameUI_NPCMuzzleSmoke"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"g_fr_npc_muzzlesmoke"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PLRMuzzleSmoke"
			{
				"text"			"#GameUI_PLRMuzzleSmoke"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"g_fr_plr_muzzlesmoke"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
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

			"ZoomDraw"
			{
				"text"			"#GameUI_ZoomDraw"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"cl_hud_zoom_draw"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"PlayerDamageFlash"
			{
				"text"			"#GameUI_PlayerDamageFlash"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sv_player_damageflash_time"

				"min"			"0"
				"max"			"3"
				"step"			"0.1"

				"textprecision" "2"
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

			"NPCGlow"
			{
				"text"			"#GameUI_GlowEffect"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"npc_gloweffect"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
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
			
			"SeamlessCubemaps"
			{
				"text"			"#GameUI_SeamlessCubemaps"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"r_seamless_cubemaps"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
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
			
			"WorldLightShadows"
			{
				"text"			"#GameUI_WorldLightShadows"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"r_worldlight_castshadows"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}
			
			"PlayerShadows"
			{
				"text"			"#GameUI_PlayerShadows"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"cl_player_castshadows"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
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

			"GaussBeam"
			{
				"text"			"#GameUI_GaussBeamOption"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"sv_gauss_jeep_beam"

				"options"
				{
					"0"		"#Song_Album_HalfLife"
					"1"		"#Song_Album_HalfLife2"
				}
			}

			"BullettimeBulletSpeed"
			{
				"text"			"#GameUI_Bullettime_Bullet_Speed"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sv_bullettime_bullet_speed"

				"min"			"1000"
				"max"			"20000"
				"step"			"100"

				"textprecision" "2"
			}

			"BullettimeBulletSpeedOffset"
			{
				"text"			"#GameUI_Bullettime_Bullet_Speed_Offset"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sv_bullettime_timescale_offset"

				"min"			"100"
				"max"			"300"
				"step"			"10"

				"textprecision" "2"
			}

			"BullettimeBulletSpeedOffsetPlayer"
			{
				"text"			"#GameUI_Bullettime_Bullet_Speed_Offset_Player"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sv_bullettime_timescale_offset_player"

				"min"			"100"
				"max"			"300"
				"step"			"10"

				"textprecision" "2"
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
			
			"MineCount"
			{
				"text"			"#GameUI_Performance_HopperCount"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"g_max_combine_mines"

				"min"			"32"
				"max"			"128"
				"step"			"5"

				"textprecision" "2"
			}
			
			"GibCount"
			{
				"text"			"#GameUI_Performance_GibsCount"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"g_max_gib_pieces"

				"min"			"32"
				"max"			"128"
				"step"			"5"

				"textprecision" "2"
			}
			
			"KnifeCount"
			{
				"text"			"#GameUI_Performance_KnifeCount"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"g_max_thrown_knives"

				"min"			"32"
				"max"			"128"
				"step"			"5"

				"textprecision" "2"
			}

			"DroppedWeaponCount"
			{
				"text"			"#GameUI_Performance_WeaponCount"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"g_max_dropped_weapons"

				"min"			"32"
				"max"			"128"
				"step"			"5"

				"textprecision" "2"
			}
			
			"RagdollCount"
			{
				"text"			"#GameUI_Performance_RagdollsCount"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"g_ragdoll_maxcount"

				"min"			"32"
				"max"			"128"
				"step"			"5"

				"textprecision" "2"
			}

			"EntTolerance"
			{
				"text"			"#GameUI_EdictTolerance"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"g_fr_entitytolerance"

				"min"			"8"
				"max"			"100"
				"step"			"5"

				"textprecision" "2"
			}

			"AIGoByeBye"
			{
				"text"			"#GameUI_NPCRemove"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"ai_disappear"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AIGoByeByeTime"
			{
				"text"			"#GameUI_NPCRemoveTime"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"ai_disappear_time"

				"min"			"0"
				"max"			"600"
				"step"			"5"

				"textprecision" "2"
			}

			"AIGoByeByeTimeRare"
			{
				"text"			"#GameUI_NPCRemoveTimeRare"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"ai_disappear_time_rare"

				"min"			"0"
				"max"			"600"
				"step"			"5"

				"textprecision" "2"
			}

			"AIGoByeByeMaxDistance"
			{
				"text"			"#GameUI_NPCRemoveMaxDistance"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"ai_disappear_max_distance"

				"min"			"128"
				"max"			"8192"
				"step"			"128"

				"textprecision" "2"
			}

			"AIGoByeByeFPSControl"
			{
				"text"			"#GameUI_NPCFPSControl"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"ai_fps_control"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"AIGoByeByeMinFPS"
			{
				"text"			"#GameUI_NPCFPSTarget"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"ai_fps_control_target"

				"min"			"10"
				"max"			"300"
				"step"			"5"

				"textprecision" "2"
			}

			"AIGoByeByeFPSMode"
			{
				"text"			"#GameUI_NPCFPSMode"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"ai_fps_control_mode"

				"options"
				{
					"0"		"#gameui_low"
					"1"		"#gameui_medium"
					"2"		"#gameui_high"
				}
			}

			"SpawnerFPSControl"
			{
				"text"			"#GameUI_SpawnerFPSControl"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"sk_spawner_fps_control"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"SpawnerFPS"
			{
				"text"			"#GameUI_SpawnerFPS"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sk_spawner_min_fps"

				"min"			"10"
				"max"			"60"
				"step"			"5"

				"textprecision" "2"
			}

			"SpawnerMaxDistance"
			{
				"text"			"#GameUI_SpawnerMaxDistance"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sk_spawner_max_distance"

				"min"			"128"
				"max"			"8192"
				"step"			"128"

				"textprecision" "2"
			}

			"SpawnerMaxDistanceOverrideFactor"
			{
				"text"			"#GameUI_SpawnerMaxDistanceOverrideFactor"
				"type"			"slideyslide"
				"advanced"		"1"
				"convar"		"sk_spawner_max_distance_override_factor"

				"min"			"0"
				"max"			"1"
				"step"			"0.1"

				"textprecision" "2"
			}
			
			"RagdollFade"
			{
				"text"			"#GameUI_RagdollsFadeOut"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"sk_spawner_npc_ragdoll_fade"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
				}
			}

			"Turbophysics"
			{
				"text"			"#GameUI_Turbophysics"
				"type"			"wheelywheel"
				"advanced"		"1"
				"convar"		"sv_turbophysics"

				"options"
				{
					"0"		"#gameui_disabled"
					"1"		"#gameui_enabled"
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
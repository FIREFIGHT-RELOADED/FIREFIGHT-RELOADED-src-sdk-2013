"MainMenu"
{	
	"ResumeGame"
	{
		"text"	"#GameUI_GameMenu_ResumeGame"
		"command"	"cmd gamemenucommand resumegame"
		"priority"	"13"
		"family"	"ingame"
	}
	
	"NewGame"
	{
		"text"	"#GameUI_GameMenu_NewGame"
		"command"	"cmd gamepadui_openmapchooser"
		"priority"	"12"
		"family"	"all"
	}
	
	"SaveGame"
	{
		"text"	"#GameUI_GameMenu_SaveGame"
		"command"	"cmd gamepadui_opensavegamedialog"
		"priority"	"11"
		"family"	"ingame"
	}

	"LoadGame"
	{
		"text"	"#GameUI_GameMenu_LoadGame"
		"command"	"cmd gamepadui_openloadgamedialog"
		"priority"	"10"
		"family"	"all"
	}

	"Options"
	{
		"text"	"#GameUI_GameMenu_Options"
		"command"	"cmd gamepadui_openoptionsdialog"
		"priority"	"9"
		"family"	"all"
	}
	"OptionsOriginal"
	{
		"text"	"#GameUI_GameMenu_LegacyOptions"
		"command"	"cmd gamemenucommand OpenOptionsDialog"
		"priority"	"8"
		"family"	"all"
	}

	"ModelOptions"
	{
		"text"	"#FRMP_PlayerModel_Title"
		"command"	"cmd gamepadui_openmodelchooser"
		"priority"	"7"
		"family"	"all"
	}

	"SpawnlistOptions"
	{
		"text"	"#FR_Spawnlist_Title"
		"command"	"cmd gamepadui_openspawnlistchooser"
		"priority"	"6"
		"family"	"all"
	}

	"PlaylistOptions"
	{
		"text"	"#FR_Playlist_Title"
		"command"	"cmd gamepadui_openplaylistchooser"
		"priority"	"5"
		"family"	"all"
	}

	"OpenWorkshop"
	{
		"text"	"#FIREFIGHTRELOADED_ModMenu_Title"
		"command"	"cmd showworkshop"
		"priority"	"4"
		"family"	"all"
	}
	
	"Achievements"
	{
		"text"	"#GameUI_GameMenu_Achievements"
		"command"	"cmd gamepadui_openachievementsdialog"
		"priority"	"3"
		"family"	"all"
	}

	"BackToMain"
	{
		"text"	"#GameUI_GameMenu_MainMenu"
		"command"	"cmd disconnect"
		"priority"	"2"
		"family"	"ingame"
	}

	"Quit"
	{
		"text"	"#GameUI_GameMenu_Quit"
		"command"	"cmd gamepadui_openquitgamedialog"
		"priority"	"1"
		"family"	"all"
	}
}
"MainMenu"
{	
	"ResumeGame"
	{
		"text"			"#GameUI_GameMenu_ResumeGame"
		"command"		"cmd gamemenucommand resumegame"
		"priority"		"10"
		"family"		"ingame"
	}
	
	"NewGame"
	{
		"text"			"#GameUI_GameMenu_NewGame"
		"command"		"cmd gamepadui_openmapchooser"
		"priority"		"9"
		"family"		"all"
	}
	
	"SaveGame"
	{
		"text"			"#GameUI_GameMenu_SaveGame"
		"command"		"cmd gamepadui_opensavegamedialog"
		"priority"		"8"
		"family"		"ingame"
	}

	"LoadGame"
	{
		"text"			"#GameUI_GameMenu_LoadGame"
		"command"		"cmd gamepadui_openloadgamedialog"
		"priority"		"7"
		"family"		"all"
	}

	"Options"
	{
		"text"			"#GameUI_GameMenu_Options"
		"command"		"cmd gamepadui_openoptionsdialog"
		"priority"		"6"
		"family"		"all"
	}

	"ModelOptions"
	{
		"text"			"#FRMP_PlayerModel_Title"
		"command"		"cmd gamepadui_openmodelchooser"
		"priority"		"5"
		"family"		"all"
	}

	"SpawnlistOptions"
	{
		"text"			"#FR_Spawnlist_Title"
		"command"		"cmd gamepadui_openspawnlistchooser"
		"priority"		"4"
		"family"		"all"
	}

	"OpenWorkshop"
	{
		"text"			"#FIREFIGHTRELOADED_ModMenu_Title"
		"command"		"cmd showworkshop"
		"priority"		"3"
		"family"		"all"
	}
	
	"Achievements"
	{
		"text"			"#GameUI_GameMenu_Achievements"
		"command"		"cmd gamepadui_openachievementsdialog"
		"priority"		"2"
		"family"		"all"
	}

	"Quit"
	{
		"text"			"#GameUI_GameMenu_Quit"
		"command"		"cmd gamepadui_openquitgamedialog"
		"priority"		"1"
		"family"		"all"
	}
}
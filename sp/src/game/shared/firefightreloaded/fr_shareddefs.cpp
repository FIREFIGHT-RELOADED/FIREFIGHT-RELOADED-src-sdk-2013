#include "cbase.h"
#include "fr_shareddefs.h"
#include "filesystem.h"
#include "gamestringpool.h"
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
#include "steam/steam_api.h"
#endif
#include "vgui_controls/MessageBox.h"
#if GAME_DLL
#include "globalstate.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar fr_kvloader_debug("fr_kvloader_debug", "0", FCVAR_REPLICATED);

const char* UTIL_FR_GetVersion(bool cmd)
{
	char verString[1024];
	const char* result = "";
	if (g_pFullFileSystem->FileExists("version.txt"))
	{
		FileHandle_t fh = filesystem->Open("version.txt", "r", "MOD");
		int file_len = filesystem->Size(fh);
		char* GameInfo = new char[file_len + 1];

		filesystem->Read((void*)GameInfo, file_len, fh);
		GameInfo[file_len] = 0; // null terminator

		filesystem->Close(fh);

		if (cmd)
		{
			Q_snprintf(verString, sizeof(verString), "Game Version: v%s (%s, %s)\n", GameInfo + 8, __DATE__, __TIME__);
		}
		else
		{
			Q_snprintf(verString, sizeof(verString), "v%s\n", GameInfo + 8);
		}

		result = verString;

		delete[] GameInfo;
	}

	return result;
}

// checks to see if the antlion bypass is available for this gamemode.
#if GAME_DLL
bool UTIL_FR_CanForceAntlionsAllied()
{
	if (g_pGameRules->GetGamemode() == FR_GAMEMODE_CAMPAIGN)
	{
		// campaign uses its own logic
		return false;
	}

	if (GlobalEntity_GetState("cannot_force_allied_antlions") == GLOBAL_ON)
	{
		return false;
	}

	return ((g_pGameRules->GetGamemode() != FR_GAMEMODE_ANTLIONASSAULT) && !g_fr_lonewolf.GetBool());
}

bool UTIL_FR_AreAntlionsAllied()
{
	if (!UTIL_FR_CanForceAntlionsAllied())
		return false;

	return (GlobalEntity_GetState("antlion_allied") == GLOBAL_ON);
}
#endif

#if CLIENT_DLL
CON_COMMAND(list_centities, "List all client entities.")
{
	for (auto iter = ClientEntityList().FirstHandle(); iter.IsValid(); iter = ClientEntityList().NextHandle(iter))
		ConMsg("%d: %s\n", iter.GetEntryIndex(), ((C_BaseEntity*)iter.Get())->GetClassname());
}

CON_COMMAND(report_centities, "Report all client entities.")
{
	CUtlStringMap<int> classnames_count;
	CUtlVector<CUtlString> sorted_classnames;

	for (auto iter = ClientEntityList().FirstBaseEntity(); iter != nullptr; iter = ClientEntityList().NextBaseEntity(iter))
	{
		auto classname = iter->GetClassname();
		if (classnames_count.Defined(classname))
			++classnames_count[classname];
		else
		{
			sorted_classnames.AddToTail(classname);
			classnames_count[classname] = 1;
		}
	}

	sorted_classnames.Sort(CUtlString::SortCaseSensitive);
	for (auto iter : sorted_classnames)
		ConMsg("Class: %s (%d)\n", (const char*)iter, classnames_count[iter]);
}

CON_COMMAND(showworkshop, "")
{
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
	if (steamapicontext && steamapicontext->SteamFriends())
	{
		char szWorkshopURL[1024];
		Q_snprintf(szWorkshopURL, sizeof(szWorkshopURL), "https://steamcommunity.com/app/%i/workshop/\n", engine->GetAppID());
		steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage(szWorkshopURL);
	}
#endif
}

CON_COMMAND(fr_version, "")
{
	Msg(UTIL_FR_GetVersion(true));
}

#ifdef _DEBUG
CON_COMMAND(leaksun, "Sets fullbright to 1 and spams \"THE SUN IS LEAKING\" in the console repeatedly.")
{
	engine->ClientCmd("mat_fullbright 1\n");
	Warning("THE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\n\n\n\n\n\n\nRestore the lighting with mat_fullbright 0\n");
}

void OpenMessageBox(const CCommand& args)
{
	vgui::MessageBox* pMessageBox = new vgui::MessageBox(args[1], args[2]);
	pMessageBox->DoModal();
}

ConCommand show_messagebox("show_messagebox", OpenMessageBox);

void OpenErrorBox(const CCommand& args)
{
	Error(args[1]);
}

ConCommand show_error("show_error", OpenErrorBox);
#endif
#endif

FRTeam_t CFRTeamLoader::FRTeamMap[] =
{
	//FR COLORS
	{ TEAM_RED,				COLOR_RED, "RED" },
	{ TEAM_BLUE,			COLOR_BLUE, "BLUE" },
	{ TEAM_YELLOW,			COLOR_YELLOW, "YELLOW" },
	{ TEAM_GREEN,			COLOR_GREEN, "GREEN"},
	{ TEAM_GREY,			COLOR_GREY, "GREY"},
	{ TEAM_WHITE,			COLOR_WHITE, "WHITE"},
	{ TEAM_BLACK,			COLOR_BLACK, "BLACK"},
	{ TEAM_PURPLE,			COLOR_PURPLE, "PURPLE"},
	//"yellow" is orange.....
	{ TEAM_ORANGE,			COLOR_YELLOW, "YELLOW"},
	{ TEAM_CYAN,			COLOR_CYAN, "CYAN"},
	{ TEAM_TURQUOISE,		COLOR_TURQUOISE, "TURQUOISE"},
	{ TEAM_PINK,			COLOR_PINK, "PINK"},
	{ TEAM_MAGENTA,			COLOR_MAGENTA, "MAGENTA"},
	//FR FUN COLORS
	{ TEAM_SMOD,			COLOR_FR_OLD, "SMOD_BLUE" },
	{ TEAM_FR,				COLOR_FR, "FR_BLUE" },
	//tf2 quality/skin colors
	//items
	{ TEAM_UNIQUE,			COLOR_UNIQUE, "TF_UNIQUE_QUALITY" },
	{ TEAM_VINTAGE,			COLOR_VINTAGE, "TF_VINTAGE_QUALITY" },
	{ TEAM_GENUINE,			COLOR_GENUINE, "TF_GENUINE_QUALITY" },
	{ TEAM_STRANGE,			COLOR_STRANGE, "TF_STRANGE_QUALITY" },
	{ TEAM_UNUSUAL,			COLOR_UNUSUAL, "TF_UNUSUAL_QUALITY" },
	{ TEAM_HAUNTED,			COLOR_HAUNTED, "TF_HAUNTED_QUALITY" },
	{ TEAM_COLLECTORS,		COLOR_COLLECTORS, "TF_COLLECTORS_QUALITY" },
	{ TEAM_COMMUNITY,		COLOR_COMMUNITY, "TF_COMMUNITY_QUALITY" },
	{ TEAM_VALVE,			COLOR_VALVE, "TF_VALVE_QUALITY" },
	//paints
	{ TEAM_CIVILIAN,		COLOR_CIVILIAN, "TF_CIVILIAN_GRADE" },
	{ TEAM_FREELANCE,		COLOR_FREELANCE, "TF_FREELANCE_GRADE" },
	{ TEAM_MERCENARY,		COLOR_MERCENARY, "TF_MERCENARY_GRADE" },
	{ TEAM_COMMANDO,		COLOR_COMMANDO, "TF_COMMANDO_GRADE" },
	{ TEAM_ASSASSIN,		COLOR_ASSASSIN, "TF_ASSASSIN_GRADE" },
	{ TEAM_ELITE,			COLOR_ELITE, "TF_ELITE_GRADE" },
};

Color CFRTeamLoader::GetColorForTeam(int iTeamNumber)
{
	Color col = Color(255, 255, 255, 255);

	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		if (FRTeamMap[i].id == iTeamNumber)
		{
			col = FRTeamMap[i].col;
			break;
		}
	}

	return col;
}

const char* CFRTeamLoader::GetNameForTeam(int iTeamNumber)
{
	const char* name = "NONE";

	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		if (FRTeamMap[i].id == iTeamNumber)
		{
			name = FRTeamMap[i].name;
			break;
		}
	}

	return name;
}

void CFRTeamLoader::LoadColors(Color* colorArray)
{
	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		colorArray[FRTeamMap[i].id] = FRTeamMap[i].col;
	}
}

void FRKeyValuesLoader::LoadEntries(const char* fileName, const char* kvHeader)
{
	m_storedVector.RemoveAll();

	KeyValues* pKV = new KeyValues(kvHeader);
	if (pKV->LoadFromFile(filesystem, fileName, "GAME"))
	{
		FOR_EACH_VALUE(pKV, pSubData)
		{
			if (FStrEq(pSubData->GetString(), ""))
				continue;

			string_t iName = AllocPooledString(pSubData->GetString());
			if (m_storedVector.Find(iName) == m_storedVector.InvalidIndex())
				m_storedVector[m_storedVector.AddToTail()] = iName;
		}
	}

	if (fr_kvloader_debug.GetBool())
	{
		KeyValuesDumpAsDevMsg(pKV, 1);
	}

	pKV->deleteThis();
}

bool FRKeyValuesLoader::FindEntry(string_t query)
{
	bool result = false;

	FOR_EACH_VEC(m_storedVector, i)
	{
		string_t iszName = m_storedVector[i];
		const char* pszName = STRING(iszName);
		const char* pszQuery = STRING(query);

		bool IsMatching = (!strcmp(pszName, pszQuery) ? true : false);
		if (IsMatching)
		{
			result = true;
			break;
		}
	}

	return result;
}

string_t FRKeyValuesLoader::GrabRandomEntryString(void)
{
	string_t result = m_storedVector.Random();
	return result;
}

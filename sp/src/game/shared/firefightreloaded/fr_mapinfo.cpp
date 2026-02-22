#include "fr_mapinfo.h"
#include "filesystem.h"
#ifndef GAMEPADUI_DLL
#include "cbase.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifndef GAMEPADUI_DLL
KeyValues* CMapInfo::GetMapInfoData()
{
	char mapname[256];
#if !defined( CLIENT_DLL )
	Q_snprintf(mapname, sizeof(mapname), "%s", STRING(gpGlobals->mapname));
#else
	const char* levelname = engine->GetLevelName();

	const char* str = Q_strstr(levelname, "maps");
	if (str)
	{
		Q_strncpy(mapname, str + 5, sizeof(mapname) - 1);	// maps + \\ = 5
	}
	else
	{
		Q_strncpy(mapname, levelname, sizeof(mapname) - 1);
	}

	char* ext = Q_strstr(mapname, ".bsp");
	if (ext)
	{
		*ext = 0;
	}
#endif

	return GetMapInfoData(mapname);
}
#endif

KeyValues* CMapInfo::GetMapInfoData(const char* pMapName)
{
	char szFullName[MAX_PATH];
	Q_snprintf(szFullName, sizeof(szFullName), "maps/%s_mapinfo.txt", pMapName);

	KeyValues* pKV = new KeyValues(pMapName);
	if (pKV->LoadFromFile(g_pFullFileSystem, szFullName))
	{
		//Msg("Loaded mapinfo file '%s'.\n", szFullName);
		return pKV;
	}

	//Warning("Failed to load mapinfo file '%s'.\n", szFullName);
	return NULL;
}

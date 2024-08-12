#include "cbase.h"
#include "mapinfo.h"
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

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

	char szFullName[512];
	Q_snprintf(szFullName, sizeof(szFullName), "scripts/mapinfo/%s.txt", mapname);

	char szFullKVName[512];
	Q_snprintf(szFullKVName, sizeof(szFullKVName), "%s", mapname);

	KeyValues* pKV = new KeyValues(mapname);
	if (pKV->LoadFromFile(filesystem, szFullName))
	{
		return pKV;
	}

	return NULL;
}
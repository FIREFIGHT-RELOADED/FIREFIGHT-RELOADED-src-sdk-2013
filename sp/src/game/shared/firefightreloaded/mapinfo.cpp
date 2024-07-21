#include "cbase.h"
#include "mapinfo.h"
#include "KeyValues.h"
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

CMapInfo::CMapInfo()
{
	char mapname[256];
#if !defined( CLIENT_DLL )
	Q_snprintf(mapname, sizeof(mapname), "%s", STRING(gpGlobals->mapname));
#else
	Q_strncpy(mapname, engine->GetLevelName(), sizeof(mapname));
#endif

	char szFullName[512];
	Q_snprintf(szFullName, sizeof(szFullName), "scripts/mapinfo/%s.txt", mapname);

	char szFullKVName[512];
	Q_snprintf(szFullKVName, sizeof(szFullKVName), "%s", mapname);

	KeyValues* pKV = new KeyValues(mapname);
	if (pKV->LoadFromFile(filesystem, szFullName))
	{
		data = pKV->MakeCopy();
	}

	pKV->deleteThis();
}

const char* CMapInfo::GetString(const char* szString, const char* defaultValue)
{
	return data->GetString(szString, defaultValue);
}

int CMapInfo::GetInt(const char* szString, int defaultValue)
{
	return data->GetInt(szString, defaultValue);
}

float CMapInfo::GetFloat(const char* szString, float defaultValue)
{
	return data->GetFloat(szString, defaultValue);
}

bool CMapInfo::GetBool(const char* szString, bool defaultValue)
{
	return data->GetBool(szString, defaultValue);
}

Color CMapInfo::GetColor(const char* szString)
{
	return data->GetColor(szString);
}

Vector CMapInfo::GetVector(const char* szString, Vector defaultValue)
{
	return data->GetVector(szString, defaultValue);
}
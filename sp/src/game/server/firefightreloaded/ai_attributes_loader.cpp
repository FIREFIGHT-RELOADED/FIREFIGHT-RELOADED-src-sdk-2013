#include "cbase.h"
#include "ai_attributes_loader.h"
#include "KeyValues.h"
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

ConVar ai_attributes_numpresets("ai_attributes_numpresets", "1", FCVAR_CHEAT);
ConVar ai_attributes_chance("ai_attributes_chance", "1", FCVAR_CHEAT);

CAIAttributesLoader *LoadNPCPresetFile(const char* className)
{
	int randPreset = random->RandomInt(1, ai_attributes_chance.GetInt());

	if (randPreset == ai_attributes_chance.GetInt())
	{
		int randPreset = random->RandomInt(1, ai_attributes_numpresets.GetInt());
		CAIAttributesLoader* loader = new CAIAttributesLoader(className, randPreset);

		if (loader->loadedAttributes == true)
		{
			return loader;
		}
		else
		{
			//load the first preset.
			loader = new CAIAttributesLoader(className, 1);
			return loader;
		}
	}
	else
	{
		return NULL;
	}
}

CAIAttributesLoader::CAIAttributesLoader(const char *className, int preset)
{
	Init(className, preset);
}

void CAIAttributesLoader::Init(const char *className, int preset)
{
	if (!className || className == NULL || strlen(className) == 0)
	{
		Warning("CAIAttributesLoader definition has no classname!\n");
		return;
	}
	
	char szFullName[512];
	Q_snprintf( szFullName, sizeof( szFullName ), "scripts/npc/%s/preset%i.txt", className, preset );

	char szFullKVName[512];
	Q_snprintf(szFullKVName, sizeof(szFullKVName), "%s_preset%i", className, preset);
	
	KeyValues* pKV = new KeyValues(className);
	if (pKV->LoadFromFile(filesystem, szFullName, "GAME"))
	{
		data = pKV->MakeCopy();
		loadedAttributes = true;
	}
	else
	{
		loadedAttributes = false;
	}
	
	pKV->deleteThis();
}

const char* CAIAttributesLoader::LoadStringVal(const char* szString, const char* defaultValue)
{
	char szStringVal[256];
	Q_strncpy(szStringVal, data->GetString(szString, defaultValue), sizeof(szStringVal));
	const char* finalVal = szStringVal;
	return finalVal;
}

void CAIAttributesLoader::Die()
{
	data->deleteThis();
	delete this;
}
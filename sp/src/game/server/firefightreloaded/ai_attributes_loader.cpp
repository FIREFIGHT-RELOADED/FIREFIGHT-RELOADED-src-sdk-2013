#include "cbase.h"
#include "ai_attributes_loader.h"
#include "KeyValues.h"
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

ConVar ai_attributes_numpresets("ai_attributes_numpresets", "10", FCVAR_CHEAT);
ConVar ai_attributes_chance("ai_attributes_chance", "10", FCVAR_CHEAT);
ConVar ai_attributes("ai_attributes", "1", FCVAR_CHEAT);

CAIAttributesLoader *LoadNPCPresetFile(const char* className)
{
	random->SetSeed(gpGlobals->curtime);
	int randPreset = random->RandomInt(1, ai_attributes_numpresets.GetInt());
	CAIAttributesLoader* loader = new CAIAttributesLoader(className, randPreset);

	if (!loader->loadedAttributes)
	{
		//load the first preset.
		loader = new CAIAttributesLoader(className, 1);

		if (!loader->loadedAttributes)
		{
			loader->Die();
			return NULL;
		}
	}

	int randChance = random->RandomInt(1, ai_attributes_chance.GetInt());

	if (loader->GetBool("persist") || randChance == ai_attributes_chance.GetInt())
	{
		return loader;
	}
	else
	{
		loader->Die();
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

const char* CAIAttributesLoader::GetString(const char* szString, const char* defaultValue)
{
	return STRING(AllocPooledString(data->GetString(szString, defaultValue)));
}

int CAIAttributesLoader::GetInt(const char* szString, int defaultValue)
{
	return data->GetInt(szString, defaultValue);
}

float CAIAttributesLoader::GetFloat(const char* szString, float defaultValue)
{
	return data->GetFloat(szString, defaultValue);
}

bool CAIAttributesLoader::GetBool(const char* szString, bool defaultValue)
{
	return data->GetBool(szString, defaultValue);
}

Color CAIAttributesLoader::GetColor(const char* szString)
{
	return data->GetColor(szString);
}

Vector CAIAttributesLoader::GetVector(const char* szString, Vector defaultValue)
{
	return data->GetVector(szString, defaultValue);
}

void CAIAttributesLoader::SwitchEntityModel(CBaseEntity* ent, const char* szString, const char* defaultValue)
{
	const char* newModelName = GetString(szString, defaultValue);

	if (strlen(newModelName) > 0)
	{
		ent->SetModelName(AllocPooledString(newModelName));
		CBaseEntity::PrecacheModel(STRING(ent->GetModelName()));
		ent->SetModel(STRING(ent->GetModelName()));
	}
}

void CAIAttributesLoader::SwitchEntityColor(CBaseEntity* ent, const char* szString)
{
	Color newColor = GetColor(szString);
	if (newColor.r() > 0 || newColor.g() > 0 || newColor.b() > 0 || newColor.a() > 0)
	{
		ent->SetRenderColor(newColor.r(), newColor.g(), newColor.b(), newColor.a());
	}
}

void CAIAttributesLoader::Die()
{
	data->deleteThis();
	delete this;
}
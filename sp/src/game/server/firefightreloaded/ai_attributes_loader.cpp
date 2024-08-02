#include "cbase.h"
#include "ai_attributes_loader.h"
#include "KeyValues.h"
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

ConVar entity_attributes_numpresets("entity_attributes_numpresets", "10", FCVAR_ARCHIVE);
ConVar entity_attributes_chance("entity_attributes_chance", "3", FCVAR_ARCHIVE);
ConVar entity_attributes("entity_attributes", "1", FCVAR_ARCHIVE);

CAttributesLoader *LoadRandomPresetFile(const char* className)
{
	random->SetSeed(gpGlobals->curtime);
	int randPreset = random->RandomInt(1, entity_attributes_numpresets.GetInt());
	CAttributesLoader* loader = new CAttributesLoader(className, randPreset);

	if (!loader->loadedAttributes)
	{
		bool shouldLoadFirstPreset = (random->RandomInt(0, entity_attributes_chance.GetInt()) == entity_attributes_chance.GetInt() ? true : false);
		
		if (shouldLoadFirstPreset)
		{
			//load the first preset.
			loader = new CAttributesLoader(className, 1);

			if (!loader->loadedAttributes)
			{
				DevWarning("CAttributesLoader: Cannot load random attribute preset %i for %s.\n", randPreset, className);
				return NULL;
			}
		}
		else
		{
			DevWarning("CAttributesLoader: Cannot load random attribute preset %i for %s.\n", randPreset, className);
			return NULL;
		}
	}

	if (!loader->GetBool("spawner_only"))
	{
		int randChance = random->RandomInt(1, entity_attributes_chance.GetInt());

		if (loader->GetBool("persist") || randChance == entity_attributes_chance.GetInt())
		{
			return loader;
		}
		else
		{
			loader->Die();
			return NULL;
		}
	}
	else
	{
		DevWarning("CAttributesLoader: Preset %i for %s can only be spawned-in manually. Disable \"spawner_only\" to allow this preset to spawn randomly.\n", randPreset, className);
		loader->Die();
		return NULL;
	}
}

CAttributesLoader* LoadPresetFile(const char* className, int preset)
{
	CAttributesLoader* loader = new CAttributesLoader(className, preset);

	if (!loader->loadedAttributes)
	{
		return NULL;
	}

	return loader;
}

CAttributesLoader::CAttributesLoader(const char *className, int preset)
{
	if (!className || className == NULL || strlen(className) == 0)
	{
		DevWarning("CAttributesLoader: Definition has no classname!\n");
		return;
	}

	if (FStrEq(className, "npc_any"))
	{
		wildcard = true;
	}
	else
	{
		wildcard = false;
	}

	char szFullName[512];
	Q_snprintf(szFullName, sizeof(szFullName), "scripts/entity_attributes/%s/preset%i.txt", className, preset);

	char szFullKVName[512];
	Q_snprintf(szFullKVName, sizeof(szFullKVName), "%s_preset%i", className, preset);

	KeyValues* pKV = new KeyValues(className);
	if (pKV->LoadFromFile(filesystem, szFullName))
	{
		data = pKV->MakeCopy();
		presetNum = preset;
		loadedAttributes = true;
	}
	else
	{
		DevWarning("CAttributesLoader: Failed to load attributes for %s on preset %i!! File may not exist.\n", className, preset);
		loadedAttributes = false;
	}

	pKV->deleteThis();
}

const char* CAttributesLoader::GetString(const char* szString, const char* defaultValue)
{
	return data->GetString(szString, defaultValue);
}

int CAttributesLoader::GetInt(const char* szString, int defaultValue)
{
	return data->GetInt(szString, defaultValue);
}

float CAttributesLoader::GetFloat(const char* szString, float defaultValue)
{
	return data->GetFloat(szString, defaultValue);
}

bool CAttributesLoader::GetBool(const char* szString, bool defaultValue)
{
	return data->GetBool(szString, defaultValue);
}

Color CAttributesLoader::GetColor(const char* szString)
{
	return data->GetColor(szString);
}

Vector CAttributesLoader::GetVector(const char* szString, Vector defaultValue)
{
	return data->GetVector(szString, defaultValue);
}

void CAttributesLoader::SwitchEntityModel(CBaseEntity* ent, const char* szString, const char* defaultValue)
{
	const char* newModelName = STRING(AllocPooledString(GetString(szString, defaultValue)));

	if (strlen(newModelName) > 0)
	{
		ent->SetModelName(AllocPooledString(newModelName));
		CBaseEntity::PrecacheModel(STRING(ent->GetModelName()));
		ent->SetModel(STRING(ent->GetModelName()));
	}
}

void CAttributesLoader::SwitchEntityColor(CBaseEntity* ent, const char* szString)
{
	Color newColor = GetColor(szString);
	if (newColor.GetRawColor() >= 0)
	{
		ent->SetRenderColor(newColor.r(), newColor.g(), newColor.b(), newColor.a());
	}
}

void CAttributesLoader::SwitchEntityRenderMode(CBaseEntity* ent, const char* szString, RenderMode_t defaultValue)
{
	int renderMode = GetInt(szString, defaultValue);
	if (renderMode >= kRenderNormal)
	{
		ent->SetRenderMode(defaultValue);
	}
}

void CAttributesLoader::SwitchEntityRenderFX(CBaseEntity* ent, const char* szString, RenderFx_t defaultValue)
{
	int renderFX = GetInt(szString, defaultValue);
	if (renderFX >= kRenderFxNone)
	{
		ent->m_nRenderFX = defaultValue;
	}
}

void CAttributesLoader::SwitchEntityBodygroup(CBaseAnimating* ent, const char* szNum, const char* szVal)
{
	int num, val;
	num = GetInt(szNum);
	val = GetInt(szVal);
	ent->SetBodygroup(num, val);
}

void CAttributesLoader::SwitchEntitySkin(CBaseAnimating* ent, const char* szVal)
{
	int val;
	val = GetInt(szVal);
	ent->m_nSkin = val;
}

void CAttributesLoader::Die()
{
	if (data)
	{
		data->deleteThis();
	}
}
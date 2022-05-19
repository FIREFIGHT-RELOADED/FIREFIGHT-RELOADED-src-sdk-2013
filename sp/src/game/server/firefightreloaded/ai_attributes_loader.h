#ifndef ATTRIBUTESLOADER_H
#define ATTRIBUTESLOADER_H
#ifdef _WIN32
#pragma once
#endif

class CAttributesLoader
{
public:
	CAttributesLoader(const char* className, int preset);
	virtual const char* GetString(const char* szString, const char* defaultValue = "");
	virtual int GetInt(const char* szString, int defaultValue = 0);
	virtual float GetFloat(const char* szString, float defaultValue = 0.0f);
	virtual bool GetBool(const char* szString, bool defaultValue = false);
	virtual Color GetColor(const char* szString);
	virtual Vector GetVector(const char* szString, Vector defaultValue = Vector(0,0,0));
	virtual void SwitchEntityModel(CBaseEntity *ent, const char* szString, const char* defaultValue);
	virtual void SwitchEntityColor(CBaseEntity* ent, const char* szString);
	virtual void SwitchEntityBodygroup(CBaseAnimating* ent, const char* szNum, const char* szVal);
	virtual void SwitchEntitySkin(CBaseAnimating* ent, const char* szVal);
	virtual void Die(void);
	
public:
	bool loadedAttributes;
	int presetNum;

private:
	KeyValues* data;
};

extern ConVar entity_attributes_numpresets;
extern ConVar entity_attributes_chance;
extern ConVar entity_attributes;
CAttributesLoader *LoadRandomPresetFile(const char* className);
CAttributesLoader *LoadPresetFile(const char* className, int preset);
#endif
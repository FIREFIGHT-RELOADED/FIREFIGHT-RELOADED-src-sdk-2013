#ifndef AIATTRIBUTESLOADER_H
#define AIATTRIBUTESLOADER_H
#ifdef _WIN32
#pragma once
#endif

class CAIAttributesLoader
{
public:
	CAIAttributesLoader(const char* className, int preset);
	virtual void Init(const char* className, int preset);
	virtual const char* GetString(const char* szString, const char* defaultValue = "");
	virtual int GetInt(const char* szString, int defaultValue = 0);
	virtual float GetFloat(const char* szString, float defaultValue = 0.0f);
	virtual bool GetBool(const char* szString, bool defaultValue = false);
	virtual Color GetColor(const char* szString);
	virtual Vector GetVector(const char* szString, Vector defaultValue = Vector(0,0,0));
	virtual void SwitchEntityModel(CBaseEntity *ent, const char* szString, const char* defaultValue);
	virtual void SwitchEntityColor(CBaseEntity* ent, const char* szString);
	virtual void Die(void);
	
public:
	bool loadedAttributes;

private:
	KeyValues* data;
};

extern ConVar ai_attributes_numpresets;
extern ConVar ai_attributes_chance;
extern ConVar ai_attributes;
CAIAttributesLoader *LoadRandomNPCPresetFile(const char* className);
CAIAttributesLoader *LoadNPCPresetFile(const char* className, int preset);
#endif
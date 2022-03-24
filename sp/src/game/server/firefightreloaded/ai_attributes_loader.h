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
	virtual const char* LoadStringVal(const char* szString, const char* defaultValue);
	virtual void Die(void);
	
public:
	bool loadedAttributes;
	KeyValues* data;
};

extern ConVar ai_attributes_numpresets;
extern ConVar ai_attributes_chance;
CAIAttributesLoader *LoadNPCPresetFile(const char* className);
#endif
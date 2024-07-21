#ifndef MAPINFO_H
#define MAPINFO_H
#ifdef _WIN32
#pragma once
#endif

class CMapInfo
{
public:
	CMapInfo();
	virtual const char* GetString(const char* szString, const char* defaultValue = "");
	virtual int GetInt(const char* szString, int defaultValue = 0);
	virtual float GetFloat(const char* szString, float defaultValue = 0.0f);
	virtual bool GetBool(const char* szString, bool defaultValue = false);
	virtual Color GetColor(const char* szString);
	virtual Vector GetVector(const char* szString, Vector defaultValue = Vector(0,0,0));

public:
	KeyValues* data;
};
#endif
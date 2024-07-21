#ifndef MAPINFO_H
#define MAPINFO_H
#ifdef _WIN32
#pragma once
#endif

#include "KeyValues.h"

class CMapInfo
{
public:
	static KeyValues* GetMapInfoData();
};
#endif
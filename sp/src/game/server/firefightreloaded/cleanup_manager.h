#ifndef CLEANUP
#define CLEANUP
#include "cbase.h"
#include "utlvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar g_max_combine_mines;
extern ConVar g_max_gib_pieces;
extern ConVar g_max_thrown_knives;
extern ConVar g_ragdoll_maxcount;

class CCleanupManager : public CBaseEntity
{
	DECLARE_CLASS(CCleanupManager, CBaseEntity)
	DECLARE_DATADESC()

	typedef void cleanupFunc(EHANDLE);

	typedef CUtlVector<EHANDLE> Handles;
	Handles m_CombineMines;
	Handles m_Gibs;
	Handles m_Ragdolls;
	Handles m_ThrownKnives;

	static CCleanupManager* pManager;
	static CCleanupManager* GetManager();

	static void Add( Handles& handles, EHANDLE handle, const ConVar& var, cleanupFunc* func );
	static bool Remove( Handles& handles, EHANDLE handle );

public:
	static void AddCombineMine( EHANDLE mine );
	static void AddGib( EHANDLE gib );
	static void AddRagdoll( EHANDLE ragdoll );
	static void AddThrownKnife( EHANDLE knife );

	static bool RemoveCombineMine( EHANDLE mine );
	static bool RemoveThrownKnife( EHANDLE knife );

	static void Shutdown();
};

#endif
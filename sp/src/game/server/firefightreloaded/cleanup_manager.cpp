#include "cbase.h"
#include "saverestore_utlvector.h"
#include "cleanup_manager.h"
#include "combine_mine.h"

ConVar g_max_combine_mines( "g_max_combine_mines", "32", FCVAR_REPLICATED, "Maximum number of hoppers dropped by scanners." );
ConVar g_max_gib_pieces( "g_max_gib_pieces", "32", FCVAR_REPLICATED, "Maximum number of gibs." );
ConVar g_max_thrown_knives( "g_max_thrown_knives", "32", FCVAR_REPLICATED, "Maximum number of thrown knives." );
ConVar g_ragdoll_maxcount( "g_ragdoll_maxcount", "32", FCVAR_REPLICATED, "Maximum number of ragdolls." );

BEGIN_DATADESC( CCleanupManager )
DEFINE_UTLVECTOR( m_CombineMines, FIELD_EHANDLE ),
DEFINE_UTLVECTOR( m_Gibs, FIELD_EHANDLE ),
DEFINE_UTLVECTOR( m_Ragdolls, FIELD_EHANDLE ),
DEFINE_UTLVECTOR( m_ThrownKnives, FIELD_EHANDLE ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( cleanup_manager, CCleanupManager )

CCleanupManager* CCleanupManager::pManager;

static void cleanupMine( EHANDLE handle )
{
	auto bomb = static_cast<CBounceBomb*>(handle.Get());
	if ( bomb->GetMineState() == MINE_STATE_ARMED )
		bomb->SetMineState( MINE_STATE_TRIGGERED );
	else
		bomb->ExplodeThink();
}

static void cleanupNotSolid( EHANDLE handle )
{
	handle->SUB_StartFadeOut( 0, true, "cleanup" );
}

static void cleanupSolid( EHANDLE handle )
{
	handle->SUB_StartFadeOut( 0, false, "cleanup" );
}

CCleanupManager* CCleanupManager::GetManager()
{
	if ( pManager == NULL )
	{
		pManager = (CCleanupManager*)gEntList.FindEntityByClassname( NULL, "cleanup_manager" );
		if ( pManager == NULL )
			pManager = (CCleanupManager*)CBaseEntity::Create( "cleanup_manager", vec3_origin, vec3_angle );
	}
	return pManager;
}

void CCleanupManager::Add( Handles& handles, EHANDLE handle, const ConVar& var, cleanupFunc& func )
{
	if ( var.GetInt() < 0 || handle == NULL )
		return;

	handles.AddToTail( handle );
	while ( handles.Count() > var.GetInt() )
	{
		auto head = handles.Head();
		handles.Remove( 0 );
		if ( head != NULL )
			func( head );
		else
			DevMsg( "Manager using cvar %s had a null entry.", var.GetName() );
	}
}

bool CCleanupManager::Remove( Handles& handles, EHANDLE handle )
{
	return handles.FindAndRemove( handle );
}

void CCleanupManager::AddCombineMine( EHANDLE mine )
{
	Add( GetManager()->m_CombineMines, mine, g_max_combine_mines, cleanupMine );
}

void CCleanupManager::AddGib( EHANDLE gib )
{
	Add( GetManager()->m_Gibs, gib, g_max_gib_pieces, cleanupNotSolid );
}

void CCleanupManager::AddRagdoll( EHANDLE ragdoll )
{
	Add( GetManager()->m_Ragdolls, ragdoll, g_ragdoll_maxcount, cleanupNotSolid );
}

void CCleanupManager::AddThrownKnife( EHANDLE knife )
{
	Add( GetManager()->m_ThrownKnives, knife, g_max_thrown_knives, cleanupSolid );
}

bool CCleanupManager::RemoveCombineMine( EHANDLE mine )
{
	return Remove( GetManager()->m_CombineMines, mine );
}

bool CCleanupManager::RemoveGib( EHANDLE knife )
{
	return Remove( GetManager()->m_Gibs, knife );
}

bool CCleanupManager::RemoveRagdoll( EHANDLE mine )
{
	return Remove( GetManager()->m_Ragdolls, mine );
}

bool CCleanupManager::RemoveThrownKnife( EHANDLE knife )
{
	return Remove( GetManager()->m_ThrownKnives, knife );
}

void CCleanupManager::Shutdown()
{
	pManager = NULL;
}

#include "cbase.h"
#include "saverestore_utlvector.h"
#include "cleanup_manager.h"

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

LINK_ENTITY_TO_CLASS( ffr_cleanup_manager, CCleanupManager )

CCleanupManager* CCleanupManager::pManager;

CCleanupManager* CCleanupManager::GetManager()
{
	if ( pManager == NULL )
	{
		pManager = (CCleanupManager*)gEntList.FindEntityByClassname( NULL, "ffr_cleanup_manager" );
		if ( pManager == NULL )
			pManager = (CCleanupManager*)CBaseEntity::Create( "ffr_cleanup_manager", vec3_origin, vec3_angle );
	}
	return pManager;
}

void CCleanupManager::Add( Handles& handles, EHANDLE handle, const ConVar& var, bool bNotSolid )
{
	if ( var.GetInt() < 0 || handle == NULL )
		return;

	while ( handles.Count() >= var.GetInt() )
	{
		auto handle = handles.Head();
		handles.Remove( 0 );
		if ( handle != NULL )
			handle->SUB_StartFadeOut( 0, bNotSolid, "cleanup" );
	}
	handles.AddToTail( handle );
}

void CCleanupManager::AddCombineMine( EHANDLE mine )
{
	Add( GetManager()->m_CombineMines, mine, g_max_combine_mines, true );
}

void CCleanupManager::AddGib( EHANDLE gib )
{
	Add( GetManager()->m_Gibs, gib, g_max_gib_pieces, true );
}

void CCleanupManager::AddRagdoll( EHANDLE ragdoll )
{
	Add( GetManager()->m_Ragdolls, ragdoll, g_max_thrown_knives, true );
}

void CCleanupManager::AddThrownKnife( EHANDLE knife )
{
	Add( GetManager()->m_ThrownKnives, knife, g_max_thrown_knives, false );
}

bool CCleanupManager::RemoveCombineMine( EHANDLE mine )
{
	return GetManager()->m_CombineMines.FindAndRemove( mine );
}

bool CCleanupManager::RemoveThrownKnife( EHANDLE knife )
{
	return GetManager()->m_ThrownKnives.FindAndRemove( knife );
}

void CCleanupManager::Shutdown()
{
	pManager = NULL;
}

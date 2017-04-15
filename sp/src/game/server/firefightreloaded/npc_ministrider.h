//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Expose an IsAMiniStrider function
//
//=============================================================================//

#ifndef NPC_MINISTRIDER_H
#define NPC_MINISTRIDER_H

#if defined( _WIN32 )
#pragma once
#endif

class CBaseEntity;

/// true if given entity pointer is a hunter.
bool MiniStrider_IsMiniStrider(CBaseEntity *pEnt);

// call throughs for member functions

void MiniStrider_StriderBusterAttached( CBaseEntity *pMiniStrider, CBaseEntity *pAttached );
void MiniStrider_StriderBusterDetached( CBaseEntity *pMiniStrider, CBaseEntity *pAttached );
void MiniStrider_StriderBusterLaunched( CBaseEntity *pMiniStrider );

#endif

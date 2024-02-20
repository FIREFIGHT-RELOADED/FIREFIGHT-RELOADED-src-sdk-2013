//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Handling for the suit batteries.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hl2_player.h"
#include "basecombatweapon.h"
#include "gamerules.h"
#include "items.h"
#include "engine/IEngineSound.h"
#include "time.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar	sk_battery("sk_battery", "0");
extern ConVar sv_blueheartmode;

class CItemBattery : public CItem
{
public:
	DECLARE_CLASS( CItemBattery, CItem );
    
    CItemBattery();
    
	void Spawn( void );
	void Precache( void );
    bool CheckBlueheart(void);
	bool MyTouch( CBasePlayer *pPlayer );
    
private:
	bool m_bInBlueheartMode;
};

LINK_ENTITY_TO_CLASS(item_battery, CItemBattery);
PRECACHE_REGISTER(item_battery);
LINK_ENTITY_TO_CLASS(item_blueheart2, CItemBattery);
PRECACHE_REGISTER(item_blueheart2);

CItemBattery::CItemBattery()
{
    m_bInBlueheartMode = false;
}

//-----------------------------------------------------------------------------
// Purpose: to pay tribute to a friend
//-----------------------------------------------------------------------------
bool CItemBattery::CheckBlueheart(void)
{
	if (sv_blueheartmode.GetBool() || ClassMatches("item_blueheart2"))
	{
		m_bInBlueheartMode = true;
	}

	if (!m_bInBlueheartMode)
	{
		time_t ltime = time(0);
		const time_t* ptime = &ltime;
		struct tm* today = localtime(ptime);
		bool isCorrectTiming = false;
		if (today)
		{
			//HACK
			int iExactMonth = today->tm_mon + 1;

			DevMsg("CItemBattery::CheckBlueheart: Day is %i-%i. Checking if it's valid to turn into a blueheart\n", iExactMonth, today->tm_mday);

			if (iExactMonth == 4 && today->tm_mday == 14)
			{
				m_bInBlueheartMode = true;
				isCorrectTiming = true;
			}
			else if (iExactMonth == 11 && today->tm_mday == 29)
			{
				m_bInBlueheartMode = true;
				isCorrectTiming = true;
			}
		}

		if (isCorrectTiming == false)
		{
			DevMsg("CItemBattery::CheckBlueheart: Not the correct day, using randomization instead.\n");

			if (random->RandomInt(1, 30) == 30)
			{
				m_bInBlueheartMode = true;
			}
		}
	}

	if (m_bInBlueheartMode)
	{
		DevMsg("CItemBattery::CheckBlueheart: Blueheart mode on. <3\n");
	}

	return m_bInBlueheartMode;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItemBattery::Spawn( void )
{ 
    CheckBlueheart();
    Precache( );
    
    if (m_bInBlueheartMode)
	{
		SetModel("models/blueheart.mdl");
		m_nSkin = 1;
	}
	else
	{
		SetModel("models/items/battery.mdl");
	}
    
    BaseClass::Spawn( );
}

void CItemBattery::Precache( void )
{
    if (m_bInBlueheartMode)
	{
		PrecacheModel("models/blueheart.mdl");
	}
	else
	{
		PrecacheModel("models/items/battery.mdl");
	}
    
    PrecacheScriptSound( "ItemBattery.Touch" );
}

bool CItemBattery::MyTouch( CBasePlayer *pPlayer )
{
    CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player *>( pPlayer );
    return ( pHL2Player && pHL2Player->ApplyBattery() );
}


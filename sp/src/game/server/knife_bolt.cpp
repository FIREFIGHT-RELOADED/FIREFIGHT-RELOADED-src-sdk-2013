#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon_shared.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "IEffects.h"
#include "te_effect_dispatch.h"
#include "Sprite.h"
#include "SpriteTrail.h"
#include "beam_shared.h"
#include "rumble_shared.h"
#include "gamestats.h"
#include "decals.h"
#include "hl2_player.h"
#include "func_break.h"
#include "func_breakablesurf.h"

#ifdef PORTAL
#include "portal_util_shared.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_plr_dmg_knife_thrown;
extern ConVar sk_npc_dmg_knife_thrown;

void TE_StickyBolt( IRecipientFilter& filter, float delay, Vector vecDirection, const Vector *origin );

#define	BOLT_SKIN_NORMAL	0
#define BOLT_SKIN_GLOW		1

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CKnifeBolt : public CBaseCombatCharacter
{
	DECLARE_CLASS( CKnifeBolt, CBaseCombatCharacter );

public:
	CKnifeBolt() { };
	~CKnifeBolt();

	Class_T Classify( void ) { return CLASS_NONE; }

public:
	void Spawn( void );
	void Precache( void );
	void BubbleThink( void );
	void BoltTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CKnifeBolt *BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, CBasePlayer *pentOwner = NULL );

protected:

	bool	CreateSprites( void );
	void	DoneMoving( bool stuck );

	CHandle<CSprite>		m_pGlowSprite;
	//CHandle<CSpriteTrail>	m_pGlowTrail;

	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
};
LINK_ENTITY_TO_CLASS( knife_bolt, CKnifeBolt );

BEGIN_DATADESC( CKnifeBolt )
// Function Pointers
DEFINE_THINKFUNC( BubbleThink ),
DEFINE_ENTITYFUNC( BoltTouch ),

// These are recreated on reload, they don't need storage
DEFINE_FIELD( m_pGlowSprite, FIELD_EHANDLE ),
//DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CKnifeBolt, DT_KnifeBolt )
END_SEND_TABLE()

CKnifeBolt *CKnifeBolt::BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, CBasePlayer *pentOwner )
{
	// Create a new entity with CKnifeBolt private data
	CKnifeBolt *pBolt = (CKnifeBolt *)CreateEntityByName( "knife_bolt" );
	UTIL_SetOrigin( pBolt, vecOrigin );
	pBolt->SetAbsAngles( angAngles );
	pBolt->Spawn();
	pBolt->SetOwnerEntity( pentOwner );

	return pBolt;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CKnifeBolt::~CKnifeBolt( void )
{
	if ( m_pGlowSprite )
		UTIL_Remove( m_pGlowSprite );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CKnifeBolt::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, FSOLID_NOT_STANDABLE, false );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CKnifeBolt::PhysicsSolidMaskForEntity() const
{
	return MASK_PLAYERSOLID;

}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CKnifeBolt::CreateSprites( void )
{
	// Start up the eye glow
	m_pGlowSprite = CSprite::SpriteCreate( "sprites/light_glow02_noz.vmt", GetLocalOrigin(), false );

	if ( m_pGlowSprite != NULL )
	{
		m_pGlowSprite->FollowEntity( this );
		m_pGlowSprite->SetTransparency( kRenderGlow, 255, 255, 255, 128, kRenderFxNoDissipation );
		m_pGlowSprite->SetScale( 0.2f );
		m_pGlowSprite->TurnOff();
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKnifeBolt::Spawn( void )
{
	Precache();

	SetModel( "models/knife_proj.mdl" );

	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
	UTIL_SetSize( this, -Vector( 0.3f, 0.3f, 0.3f ), Vector( 0.3f, 0.3f, 0.3f ) );
	SetSolid( SOLID_BBOX );
	SetGravity( 0.05f );

	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch( &CKnifeBolt::BoltTouch );

	SetThink( &CKnifeBolt::BubbleThink );
	SetNextThink( gpGlobals->curtime + 0.1f );

	CreateSprites();

	// Make us glow until we've hit the wall
	m_nSkin = BOLT_SKIN_GLOW;
}


void CKnifeBolt::Precache( void )
{
	PrecacheModel( "models/knife_proj.mdl" );
	PrecacheModel( "sprites/light_glow02_noz.vmt" );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CKnifeBolt::BoltTouch( CBaseEntity *pOther )
{
	if ( pOther->IsSolidFlagSet( FSOLID_VOLUME_CONTENTS | FSOLID_TRIGGER ) )
	{
		// Some NPCs are triggers that can take damage (like antlion grubs). We should hit them.
		if ( (pOther->m_takedamage == DAMAGE_NO) || (pOther->m_takedamage == DAMAGE_EVENTS_ONLY) )
			return;
	}

	if ( pOther->m_takedamage != DAMAGE_NO )
	{
		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize( vecNormalizedVel );

		float curDamage = sk_plr_dmg_knife_thrown.GetFloat();
		const surfacedata_t *pdata = physprops->GetSurfaceData( tr.surface.surfaceProps );

		if ( GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC() )
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), curDamage, DMG_NEVERGIB );
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			DoneMoving( false );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );

			CBasePlayer *pPlayer = ToBasePlayer( GetOwnerEntity() );
			if ( pPlayer )
				gamestats->Event_WeaponHit( pPlayer, true, "weapon_knife", dmgInfo );
		}
		else if ( FClassnameIs( pOther, "func_breakable" ) || FClassnameIs( pOther, "func_breakable_surf" ) )
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), curDamage, DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );

			CBreakable* pOtherEntity = static_cast<CBreakable*>(pOther);
			if ( pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb) )
				return;
		}
		else
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), curDamage, DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );

			if ( pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS )
			{
				pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
				return;
			}
			else if ( FClassnameIs( pOther, "func_breakable" ) )
			{
				pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );

				CBreakable* pOtherEntity = static_cast<CBreakable*>(pOther);
				if ( pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb) )
					return;
			}
			else if ( FClassnameIs( pOther, "func_breakable_surf" ) )
			{
				pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );

				CBreakableSurface* pOtherEntity = static_cast<CBreakableSurface*>(pOther);
				if ( pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb) )
					return;
			}
			else if ( pdata->game.material != CHAR_TEX_GLASS )
				DoneMoving( false );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}

		ApplyMultiDamage();

		if ( !pOther->IsAlive() && pdata->game.material == CHAR_TEX_GLASS )
			return;

		SetAbsVelocity( Vector( 0, 0, 0 ) );

		// play body "thwack" sound
		EmitSound( "Weapon_Crossbow.BoltHitBody" );

		Vector vForward;

		AngleVectors( GetAbsAngles(), &vForward );
		VectorNormalize( vForward );

		UTIL_TraceLine( GetAbsOrigin(), GetAbsOrigin() + vForward * 128, MASK_BLOCKLOS, pOther, COLLISION_GROUP_NONE, &tr2 );

		if ( tr2.fraction != 1.0f )
		{
			//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
			//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

			if ( tr2.m_pEnt == NULL || (tr2.m_pEnt && tr2.m_pEnt->GetMoveType() == MOVETYPE_NONE) )
			{
				CEffectData	data;

				data.m_vOrigin = tr2.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = tr2.fraction != 1.0f;
			}
		}
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if ( pOther->GetMoveType() == MOVETYPE_NONE && !(tr.surface.flags & SURF_SKY) && !(tr.contents & CONTENTS_PLAYERCLIP) )
		{
			EmitSound( "Weapon_Crossbow.BoltHitWorld" );

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();
			float speed = VectorNormalize( vecDir );

			// See if we should reflect off this surface
			float hitDot = DotProduct( tr.plane.normal, -vecDir );

			if ( (hitDot < 0.5f) && (speed > 100) )
			{
				Vector vReflection = 2.0f * tr.plane.normal * hitDot + vecDir;

				QAngle reflectAngles;

				VectorAngles( vReflection, reflectAngles );

				SetLocalAngles( reflectAngles );

				SetAbsVelocity( vReflection * speed * 0.75f );

				// Start to sink faster
				SetGravity( 1.0f );
			}
			else
			{
				//FIXME: We actually want to stick (with hierarchy) to what we've hit
				SetMoveType( MOVETYPE_NONE );

				Vector vForward;

				AngleVectors( GetAbsAngles(), &vForward );
				VectorNormalize( vForward );

				CEffectData	data;

				data.m_vOrigin = tr.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = 0;

				UTIL_ImpactTrace( &tr, DMG_BULLET );

				// The shallower the angle, the less of chance of sticking.
				DoneMoving( random->RandomFloat( 0.5 ) <= hitDot );

				if ( m_pGlowSprite != NULL )
				{
					m_pGlowSprite->TurnOn();
					m_pGlowSprite->FadeAndDie( 3.0f );
				}
			}

			// Shoot some sparks
			if ( UTIL_PointContents( GetAbsOrigin() ) != CONTENTS_WATER )
			{
				g_pEffects->Sparks( GetAbsOrigin() );
			}
		}
		else if ( tr.surface.flags & SURF_SKY || tr.contents & CONTENTS_PLAYERCLIP )
			DoneMoving( false );
		else
		{
			UTIL_ImpactTrace( &tr, DMG_BULLET );
			DoneMoving( false );
		}
	}
}

void CKnifeBolt::DoneMoving( bool stuck )
{
	auto angle = GetAbsAngles();
	// The weapon model is reversed for some reason.
	angle[0] += 180;
	auto pWeap = (CBaseCombatWeapon*)CBaseEntity::CreateNoSpawn( "weapon_knife", GetAbsOrigin(), angle );
	pWeap->AddSpawnFlags( SF_NORESPAWN );
	DispatchSpawn( pWeap );

	auto phys = pWeap->VPhysicsGetObject();
	if ( stuck && phys != NULL )
	{
		phys->EnableMotion( false );
		//pWeap->SetCollisionGroup( COLLISION_GROUP_DEBRIS );
	}
	pWeap->SetAbsVelocity( Vector( 0, 0, 0 ) );
	pWeap->AddEffects( EF_ITEM_BLINK );

	Remove();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKnifeBolt::BubbleThink( void )
{
	QAngle angNewAngles;

	VectorAngles( GetAbsVelocity(), angNewAngles );
	SetAbsAngles( angNewAngles );

	SetNextThink( gpGlobals->curtime + 0.1f );

	// Make danger sounds out in front of me, to scare snipers back into their hole
	CSoundEnt::InsertSound( SOUND_DANGER_SNIPERONLY, GetAbsOrigin() + GetAbsVelocity() * 0.2, 120.0f, 0.5f, this, SOUNDENT_CHANNEL_REPEATED_DANGER );

	if ( GetWaterLevel() == 0 )
		return;

	UTIL_BubbleTrail( GetAbsOrigin() - GetAbsVelocity() * 0.1f, GetAbsOrigin(), 5 );
}

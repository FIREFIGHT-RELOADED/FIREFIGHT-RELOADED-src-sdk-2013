//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements the Sticky Bolt code. This constraints ragdolls to the world
//			after being hit by a crossbow bolt. If something here is acting funny
//			let me know - Adrian.
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
//#include "c_basetempentity.h"
#include "fx.h"
#include "decals.h"
#include "iefx.h"
#include "engine/IEngineSound.h"
#include "materialsystem/imaterialvar.h"
#include "IEffects.h"
#include "engine/IEngineTrace.h"
#include "vphysics/constraints.h"
#include "engine/ivmodelinfo.h"
#include "tempent.h"
#include "c_te_legacytempents.h"
#include "engine/ivdebugoverlay.h"
#include "c_te_effect_dispatch.h"
#include "stickybolt.h"
#include "firefightreloaded/c_weapon_knife.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IPhysicsSurfaceProps *physprops;
IPhysicsObject *GetWorldPhysObject( void );

extern ITempEnts* tempents;

class CRagdollBoltEnumerator : public IPartitionEnumerator
{
public:
	//Forced constructor   
	CRagdollBoltEnumerator( Ray_t& shot, Vector vOrigin )
	{
		m_ragdoll = nullptr;
		m_rayShot = shot;
		m_vWorld = vOrigin;
	}

	//Actual work code
	IterationRetval_t EnumElement( IHandleEntity *pHandleEntity )
	{
 		C_BaseEntity *pEnt = ClientEntityList().GetBaseEntityFromHandle( pHandleEntity->GetRefEHandle() );
		if ( pEnt == NULL )
			return ITERATION_CONTINUE;

		C_BaseAnimating *pModel = static_cast< C_BaseAnimating * >( pEnt );

		if ( pModel == NULL )
			return ITERATION_CONTINUE;

		trace_t tr;
		enginetrace->ClipRayToEntity( m_rayShot, MASK_SHOT, pModel, &tr );

		IPhysicsObject	*pPhysicsObject = NULL;
		
		//Find the real object we hit.
		if( tr.physicsbone >= 0 )
		{
			if ( pModel->m_pRagdoll )
			{
				CRagdoll *pCRagdoll = dynamic_cast < CRagdoll * > ( pModel->m_pRagdoll );

				if ( pCRagdoll )
				{
					ragdoll_t *pRagdollT = pCRagdoll->GetRagdoll();

					if ( tr.physicsbone < pRagdollT->listCount )
						pPhysicsObject = pRagdollT->list[tr.physicsbone].pObject;
				}
			}
		}

		if ( pPhysicsObject == NULL )
			return ITERATION_CONTINUE;

		if ( tr.fraction < 1.0 )
		{
			IPhysicsObject *pReference = GetWorldPhysObject();

			if ( pReference == NULL || pPhysicsObject == NULL )
				 return ITERATION_CONTINUE;

			m_ragdoll = pEnt;
			
			float flMass = pPhysicsObject->GetMass();
			pPhysicsObject->SetMass( flMass * 2 );

			constraint_ballsocketparams_t ballsocket;
			ballsocket.Defaults();
		
			pReference->WorldToLocal( &ballsocket.constraintPosition[0], m_vWorld );
			pPhysicsObject->WorldToLocal( &ballsocket.constraintPosition[1], tr.endpos );
	
			physenv->CreateBallsocketConstraint( pReference, pPhysicsObject, NULL, ballsocket );

			//Play a sound
			CPASAttenuationFilter filter( pEnt );

			EmitSound_t ep;
			ep.m_nChannel = CHAN_VOICE;
			ep.m_pSoundName =  "Weapon_Crossbow.BoltSkewer";
			ep.m_flVolume = 1.0f;
			ep.m_SoundLevel = SNDLVL_NORM;
			ep.m_pOrigin = &pEnt->GetAbsOrigin();

			C_BaseEntity::EmitSound( filter, SOUND_FROM_WORLD, ep );
	
			return ITERATION_STOP;
		}

		return ITERATION_CONTINUE;
	}

	C_BaseEntity* GetRagdoll() { return m_ragdoll; }

private:
	C_BaseEntity* m_ragdoll;
	Ray_t	m_rayShot;
	Vector  m_vWorld;
};

void CreateCrossbowBolt( const Vector &vecOrigin, const Vector &vecDirection )
{
	model_t* pModel = NULL;

	pModel = (model_t*)engine->LoadModel("models/crossbow_bolt.mdl");

	QAngle vAngles;

	VectorAngles( vecDirection, vAngles );

	tempents->SpawnTempModel(pModel, vecOrigin - vecDirection * 8, vAngles, Vector(0, 0, 0), 30.0f, FTENT_NONE);
}

void StickRagdollNow(const Vector &vecOrigin, const Vector &vecDirection, const int flags, C_BaseEntity* const sticker)
{
	if ( flags & SBFL_STICKRAGDOLL )
	{
		Ray_t	shotRay;
		trace_t tr;

		UTIL_TraceLine( vecOrigin, vecOrigin + vecDirection * 16, MASK_SOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &tr );

		if ( tr.surface.flags & SURF_SKY )
			return;

		Vector vecEnd = vecOrigin - vecDirection * 128;

		shotRay.Init( vecOrigin, vecEnd );

		CRagdollBoltEnumerator	ragdollEnum( shotRay, vecOrigin );
		partition->EnumerateElementsAlongRay( PARTITION_CLIENT_RESPONSIVE_EDICTS | PARTITION_ENGINE_NON_STATIC_EDICTS, shotRay, false, &ragdollEnum );

		auto knife = dynamic_cast<C_WeaponKnife*>(sticker);
		if ( knife != nullptr )
			knife->m_hStuckRagdoll = ragdollEnum.GetRagdoll();
	}
	
	if ( flags & SBFL_CROSSBOWBOLT )
		CreateCrossbowBolt( vecOrigin, vecDirection );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &data - 
//-----------------------------------------------------------------------------
void StickyBoltCallback( const CEffectData &data )
{
	StickRagdollNow( data.m_vOrigin, data.m_vNormal, data.m_fFlags, data.GetEntity() );
}

DECLARE_CLIENT_EFFECT( "BoltImpact", StickyBoltCallback );

void KnifeDislodgeCallback( const CEffectData &data )
{
	auto knife = dynamic_cast<C_WeaponKnife*>(data.GetEntity());
	if ( knife == nullptr )
		return;

	auto ragdoll = dynamic_cast<C_BaseAnimating*>(knife->m_hStuckRagdoll.Get());
	if ( ragdoll == nullptr || ragdoll->IsPlayer() )
		return;
	
	C_BaseAnimating *newRagdoll = ragdoll->CreateRagdollCopy();

	matrix3x4_t boneDelta0[MAXSTUDIOBONES];
	matrix3x4_t boneDelta1[MAXSTUDIOBONES];
	matrix3x4_t currentBones[MAXSTUDIOBONES];
	const float boneDt = 0.1f;
	ragdoll->GetRagdollInitBoneArrays( boneDelta0, boneDelta1, currentBones, boneDt );
	newRagdoll->InitAsClientRagdoll( boneDelta0, boneDelta1, currentBones, boneDt );
	knife->SetOwnerEntity( nullptr );
	ragdoll->Remove();
}

DECLARE_CLIENT_EFFECT( "KnifeDislodge", KnifeDislodgeCallback );
//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Moved all the duck functions out of gamemovement.cpp into this
//          file, to keep the size under control.
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "gamemovement.h"
#include "in_buttons.h"
#include <stdarg.h>
#include "movevars_shared.h"
#include "engine/IEngineTrace.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "decals.h"
#include "coordsize.h"
#include "rumble_shared.h"

#if defined(HL2_DLL) || defined(HL2_CLIENT_DLL)
#include "hl_movedata.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	NUM_CROUCH_HINTS	3

bool CGameMovement::CanUnduck()
{
	int i;
	trace_t trace;
	Vector newOrigin;
	VectorCopy( mv->GetAbsOrigin(), newOrigin );

	if (player->GetGroundEntity() != NULL)
	{
		for (i = 0; i < 3; i++)
		{
			newOrigin[i] += (VEC_DUCK_HULL_MIN_SCALED( player )[i] - VEC_HULL_MIN_SCALED( player )[i]);
		}
	}
	else
	{
		// If in air an letting go of crouch, make sure we can offset origin to make
		//  up for uncrouching
		Vector hullSizeNormal = VEC_HULL_MAX_SCALED( player ) - VEC_HULL_MIN_SCALED( player );
		Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED( player ) - VEC_DUCK_HULL_MIN_SCALED( player );
		Vector viewDelta = (hullSizeNormal - hullSizeCrouch);
		viewDelta.Negate();
		VectorAdd( newOrigin, viewDelta, newOrigin );
	}

	bool saveducked = player->m_Local.m_bDucked;
	player->m_Local.m_bDucked = false;
	TracePlayerBBox( mv->GetAbsOrigin(), newOrigin, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, trace );
	player->m_Local.m_bDucked = saveducked;
	if (trace.startsolid || (trace.fraction != 1.0f))
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Stop ducking
//-----------------------------------------------------------------------------
void CGameMovement::FinishUnDuck( void )
{
	int i;
	trace_t trace;
	Vector newOrigin;

	VectorCopy( mv->GetAbsOrigin(), newOrigin );

	if (player->GetGroundEntity() != NULL)
	{
		for (i = 0; i < 3; i++)
		{
			newOrigin[i] += (VEC_DUCK_HULL_MIN_SCALED( player )[i] - VEC_HULL_MIN_SCALED( player )[i]);
		}
	}
	else
	{
		// If in air an letting go of crouch, make sure we can offset origin to make
		//  up for uncrouching
		Vector hullSizeNormal = VEC_HULL_MAX_SCALED( player ) - VEC_HULL_MIN_SCALED( player );
		Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED( player ) - VEC_DUCK_HULL_MIN_SCALED( player );
		Vector viewDelta = (hullSizeNormal - hullSizeCrouch);
		viewDelta.Negate();
		VectorAdd( newOrigin, viewDelta, newOrigin );
	}

	player->m_Local.m_bDucked = false;
	player->RemoveFlag( FL_DUCKING );
	player->m_Local.m_bDucking = false;
	player->m_Local.m_bInDuckJump = false;
	player->SetViewOffset( GetPlayerViewOffset( false ) );
	player->m_Local.m_flDucktime = 0;

	// mobility
	if (player->m_bIsPowerSliding) {
		EndPowerSlide();
	}

	mv->SetAbsOrigin( newOrigin );

#ifdef CLIENT_DLL
#ifdef STAGING_ONLY
	if (debug_latch_reset_onduck.GetBool())
	{
		player->ResetLatched();
	}
#else
	player->ResetLatched();
#endif
#endif // CLIENT_DLL

	// Recategorize position since ducking can change origin
	CategorizePosition();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CGameMovement::UpdateDuckJumpEyeOffset( void )
{
	if (player->m_Local.m_flDuckJumpTime != 0.0f)
	{
		float flDuckMilliseconds = MAX( 0.0f, GAMEMOVEMENT_DUCK_TIME - (float)player->m_Local.m_flDuckJumpTime );
		float flDuckSeconds = flDuckMilliseconds / GAMEMOVEMENT_DUCK_TIME;
		if (flDuckSeconds > TIME_TO_UNDUCK)
		{
			player->m_Local.m_flDuckJumpTime = 0.0f;
			SetDuckedEyeOffset( 0.0f );
		}
		else
		{
			float flDuckFraction = SimpleSpline( 1.0f - (flDuckSeconds / TIME_TO_UNDUCK) );
			SetDuckedEyeOffset( flDuckFraction );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CGameMovement::FinishUnDuckJump( trace_t &trace )
{
	Vector vecNewOrigin;
	VectorCopy( mv->GetAbsOrigin(), vecNewOrigin );

	//  Up for uncrouching.
	Vector hullSizeNormal = VEC_HULL_MAX_SCALED( player ) - VEC_HULL_MIN_SCALED( player );
	Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED( player ) - VEC_DUCK_HULL_MIN_SCALED( player );
	Vector viewDelta = (hullSizeNormal - hullSizeCrouch);

	float flDeltaZ = viewDelta.z;
	viewDelta.z *= trace.fraction;
	flDeltaZ -= viewDelta.z;

	player->RemoveFlag( FL_DUCKING );
	player->m_Local.m_bDucked = false;
	player->m_Local.m_bDucking = false;
	player->m_Local.m_bInDuckJump = false;
	player->m_Local.m_flDucktime = 0.0f;
	player->m_Local.m_flDuckJumpTime = 0.0f;
	player->m_Local.m_flJumpTime = 0.0f;

	Vector vecViewOffset = GetPlayerViewOffset( false );
	vecViewOffset.z -= flDeltaZ;
	player->SetViewOffset( vecViewOffset );

	VectorSubtract( vecNewOrigin, viewDelta, vecNewOrigin );
	mv->SetAbsOrigin( vecNewOrigin );

	// Recategorize position since ducking can change origin
	CategorizePosition();
}

//-----------------------------------------------------------------------------
// Purpose: Finish ducking
//-----------------------------------------------------------------------------
void CGameMovement::FinishDuck( void )
{
	if (player->GetFlags() & FL_DUCKING)
		return;

	player->AddFlag( FL_DUCKING );
	player->m_Local.m_bDucked = true;
	player->m_Local.m_bDucking = false;

	player->SetViewOffset( GetPlayerViewOffset( true ) );

	// HACKHACK - Fudge for collision bug - no time to fix this properly
	if (player->GetGroundEntity() != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			Vector org = mv->GetAbsOrigin();
			org[i] -= (VEC_DUCK_HULL_MIN_SCALED( player )[i] - VEC_HULL_MIN_SCALED( player )[i]);
			mv->SetAbsOrigin( org );
		}
	}
	else
	{   // in the air
		Vector hullSizeNormal = VEC_HULL_MAX_SCALED( player ) - VEC_HULL_MIN_SCALED( player );
		Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED( player ) - VEC_DUCK_HULL_MIN_SCALED( player );
		Vector viewDelta = (hullSizeNormal - hullSizeCrouch);
		Vector out;
		VectorAdd( mv->GetAbsOrigin(), viewDelta, out );
		mv->SetAbsOrigin( out );

#ifdef CLIENT_DLL
#ifdef STAGING_ONLY
		if (debug_latch_reset_onduck.GetBool())
		{
			player->ResetLatched();
		}
#else
		player->ResetLatched();
#endif
#endif // CLIENT_DLL
	}

	// See if we are stuck?
	FixPlayerCrouchStuck( true );

	// Recategorize position since ducking can change origin
	CategorizePosition();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CGameMovement::StartUnDuckJump( void )
{
	player->AddFlag( FL_DUCKING );
	player->m_Local.m_bDucked = true;
	player->m_Local.m_bDucking = false;

	player->SetViewOffset( GetPlayerViewOffset( true ) );

	Vector hullSizeNormal = VEC_HULL_MAX_SCALED( player ) - VEC_HULL_MIN_SCALED( player );
	Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED( player ) - VEC_DUCK_HULL_MIN_SCALED( player );
	Vector viewDelta = (hullSizeNormal - hullSizeCrouch);
	Vector out;
	VectorAdd( mv->GetAbsOrigin(), viewDelta, out );
	mv->SetAbsOrigin( out );

	// See if we are stuck?
	FixPlayerCrouchStuck( true );

	// Recategorize position since ducking can change origin
	CategorizePosition();
}

//
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : duckFraction - 
//-----------------------------------------------------------------------------
void CGameMovement::SetDuckedEyeOffset( float duckFraction )
{
	Vector vDuckHullMin = GetPlayerMins( true );
	Vector vStandHullMin = GetPlayerMins( false );

	float fMore = (vDuckHullMin.z - vStandHullMin.z);

	Vector vecDuckViewOffset = GetPlayerViewOffset( true );
	Vector vecStandViewOffset = GetPlayerViewOffset( false );
	Vector temp = player->GetViewOffset();
	temp.z = ((vecDuckViewOffset.z - fMore) * duckFraction) +
		(vecStandViewOffset.z * (1 - duckFraction));
	player->SetViewOffset( temp );
}

//-----------------------------------------------------------------------------
// Purpose: Crop the speed of the player when ducking and on the ground.
//   Input: bInDuck - is the player already ducking
//          bInAir - is the player in air
//    NOTE: Only crop player speed once.
//-----------------------------------------------------------------------------
void CGameMovement::HandleDuckingSpeedCrop( void )
{
	if (!(m_iSpeedCropped & SPEED_CROPPED_DUCK) &&
		(player->GetFlags() & FL_DUCKING) &&
		(player->GetGroundEntity() != NULL))
	{
		float frac = 0.33333333f;
		mv->m_flForwardMove *= frac;
		mv->m_flSideMove *= frac;
		mv->m_flUpMove *= frac;
		m_iSpeedCropped |= SPEED_CROPPED_DUCK;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Check to see if we are in a situation where we can unduck jump.
//-----------------------------------------------------------------------------
bool CGameMovement::CanUnDuckJump( trace_t &trace )
{
	// Trace down to the stand position and see if we can stand.
	Vector vecEnd( mv->GetAbsOrigin() );
	vecEnd.z -= 36.0f;						// This will have to change if bounding hull change!
	TracePlayerBBox( mv->GetAbsOrigin(), vecEnd, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, trace );
	if (trace.fraction < 1.0f)
	{
		// Find the endpoint.
		vecEnd.z = mv->GetAbsOrigin().z + (-36.0f * trace.fraction);

		// Test a normal hull.
		trace_t traceUp;
		bool bWasDucked = player->m_Local.m_bDucked;
		player->m_Local.m_bDucked = false;
		TracePlayerBBox( vecEnd, vecEnd, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, traceUp );
		player->m_Local.m_bDucked = bWasDucked;
		if (!traceUp.startsolid)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: See if duck button is pressed and do the appropriate things
//-----------------------------------------------------------------------------
void CGameMovement::Duck( void )
{
	int buttonsChanged = (mv->m_nOldButtons ^ mv->m_nButtons);	// These buttons have changed this frame
	int buttonsPressed = buttonsChanged & mv->m_nButtons;			// The changed ones still down are "pressed"
	int buttonsReleased = buttonsChanged & mv->m_nOldButtons;		// The changed ones which were previously down are "released"



	// Check to see if we are in the air.
	bool bInAir = (player->GetGroundEntity() == NULL);
	bool bInDuck = (player->GetFlags() & FL_DUCKING) ? true : false;
	bool bDuckJump = (player->m_Local.m_flJumpTime > 0.0f);
	bool bDuckJumpTime = (player->m_Local.m_flDuckJumpTime > 0.0f);

	if (mv->m_nButtons & IN_DUCK)
	{
		mv->m_nOldButtons |= IN_DUCK;
	}
	else
	{
		mv->m_nOldButtons &= ~IN_DUCK;
	}

	/*if (buttonsReleased & IN_DUCK)
	{
		Msg( "DUCK BUTTON RELEASED\n" );
		Msg( "jumptime = %0.1f duckjumptime = %0.1f ducking = %s\n",
			player->m_Local.m_flJumpTime,
			player->m_Local.m_flDuckJumpTime,
			bInDuck ? "true" : "false" );
	}*/

	// Handle death.
	if (IsDead())
		return;

	// Slow down ducked players.
	if (!player->m_bIsPowerSliding)
	    HandleDuckingSpeedCrop();

	// If the player is holding down the duck button, the player is in duck transition, ducking, or duck-jumping.
	if ((mv->m_nButtons & IN_DUCK) || player->m_Local.m_bDucking || bInDuck || bDuckJump)
	{
		// DUCK
		if ((mv->m_nButtons & IN_DUCK) || bDuckJump)
		{
			// XBOX SERVER ONLY
#if !defined(CLIENT_DLL)
			if (IsX360() && buttonsPressed & IN_DUCK)
			{
				// Hinting logic
				if (player->GetToggledDuckState() && player->m_nNumCrouches < NUM_CROUCH_HINTS)
				{
					UTIL_HudHintText( player, "#Valve_Hint_Crouch" );
					player->m_nNumCrouches++;
				}
			}
#endif
			if ((buttonsPressed & IN_DUCK) && 
				    player->m_nWallRunState == WALLRUN_RUNNING)
			{
				Vector vecWallPush;
				VectorScale( player->m_vecWallNorm, 30.0f, vecWallPush );
				VectorAdd( mv->m_vecVelocity, vecWallPush, mv->m_vecVelocity );
				EndWallRun();
			}

			// Have the duck button pressed, but the player currently isn't in the duck position.
			if ((buttonsPressed & IN_DUCK) && !bInDuck && !bDuckJump && !bDuckJumpTime)
			{
				player->m_Local.m_flDucktime = GAMEMOVEMENT_DUCK_TIME;
				player->m_Local.m_bDucking = true;
				
				if (!bInAir)
				{
					CheckPowerSlide();
				}
			}

			// The player is in duck transition and not duck-jumping.
			if (player->m_Local.m_bDucking && !bDuckJump && !bDuckJumpTime )
			{
				float flDuckMilliseconds =
					MAX( 0.0f, GAMEMOVEMENT_DUCK_TIME - (float)player->m_Local.m_flDucktime );
				float flDuckSeconds = flDuckMilliseconds * 0.001f;

				// Finish in duck transition when transition time is over, in "duck", in air.
				if ((flDuckSeconds > TIME_TO_DUCK) || bInDuck || bInAir)
				{
					FinishDuck();
				}
				else
				{
					// Calc parametric time
					float flDuckFraction = SimpleSpline( flDuckSeconds / TIME_TO_DUCK );
					SetDuckedEyeOffset( flDuckFraction );
				}
			}

			if (bDuckJump)
			{
				// Make the bounding box small immediately.
				if (!bInDuck)
				{
					StartUnDuckJump();
				}
				else
				{
					// Check for a crouch override.
					if (!(mv->m_nButtons & IN_DUCK))
					{
						trace_t trace;
						if (CanUnDuckJump( trace ))
						{
							FinishUnDuckJump( trace );
							player->m_Local.m_flDuckJumpTime =
								(GAMEMOVEMENT_TIME_TO_UNDUCK * (1.0f - trace.fraction)) +
								GAMEMOVEMENT_TIME_TO_UNDUCK_INV;

						}
					}
				}
			}
		}
		// UNDUCK (or attempt to...)
		else
		{
			if (player->m_Local.m_bInDuckJump)
			{
				// Check for a crouch override.
				if (!(mv->m_nButtons & IN_DUCK))
				{
					trace_t trace;
					if (CanUnDuckJump( trace ))
					{
						FinishUnDuckJump( trace );

						if (trace.fraction < 1.0f)
						{
							player->m_Local.m_flDuckJumpTime = (GAMEMOVEMENT_TIME_TO_UNDUCK * (1.0f - trace.fraction)) + GAMEMOVEMENT_TIME_TO_UNDUCK_INV;
						}
					}
				}
				else
				{
					player->m_Local.m_bInDuckJump = false;
				}
			}

			if (bDuckJumpTime)
				return;

			// Try to unduck unless automovement is not allowed
			// NOTE: When not onground, you can always unduck
			if (player->m_Local.m_bAllowAutoMovement || bInAir || player->m_Local.m_bDucking)
			{

				// We released the duck button, we aren't in "duck" and we are not in the air - start unduck transition.
				if ((buttonsReleased & IN_DUCK))
				{
					if (bInDuck && !bDuckJump)
					{
						player->m_Local.m_flDucktime = GAMEMOVEMENT_DUCK_TIME;
						player->m_Local.m_bDucking = true;
					}
					else if (player->m_Local.m_bDucking && !player->m_Local.m_bDucked)
					{
						// Invert time if release before fully ducked!!!
						float unduckMilliseconds = 1000.0f * TIME_TO_UNDUCK;
						float duckMilliseconds = 1000.0f * TIME_TO_DUCK;
						float elapsedMilliseconds = GAMEMOVEMENT_DUCK_TIME - player->m_Local.m_flDucktime;

						float fracDucked = elapsedMilliseconds / duckMilliseconds;
						float remainingUnduckMilliseconds = fracDucked * unduckMilliseconds;

						player->m_Local.m_flDucktime = GAMEMOVEMENT_DUCK_TIME - unduckMilliseconds + remainingUnduckMilliseconds;
					}
				}


				// Check to see if we are capable of unducking.
				if (CanUnduck())
				{
					// or unducking
					if ((player->m_Local.m_bDucking || player->m_Local.m_bDucked))
					{
						float flDuckMilliseconds = MAX( 0.0f, GAMEMOVEMENT_DUCK_TIME - (float)player->m_Local.m_flDucktime );
						float flDuckSeconds = flDuckMilliseconds * 0.001f;

						// Finish ducking immediately if duck time is over or not on ground
						if (flDuckSeconds > TIME_TO_UNDUCK || (bInAir && !bDuckJump))
						{
							FinishUnDuck();
						}
						else
						{
							// Calc parametric time
							float flDuckFraction = SimpleSpline( 1.0f - (flDuckSeconds / TIME_TO_UNDUCK) );
							SetDuckedEyeOffset( flDuckFraction );
							player->m_Local.m_bDucking = true;
						}
					}
				}
				else
				{
					// Still under something where we can't unduck, so make sure we reset this timer so
					//  that we'll unduck once we exit the tunnel, etc.
					if (player->m_Local.m_flDucktime != GAMEMOVEMENT_DUCK_TIME)
					{
						SetDuckedEyeOffset( 1.0f );
						player->m_Local.m_flDucktime = GAMEMOVEMENT_DUCK_TIME;
						player->m_Local.m_bDucked = true;
						player->m_Local.m_bDucking = false;
						player->AddFlag( FL_DUCKING );
					}
				}
			}
		}
	}
	// HACK: (jimd 5/25/2006) we have a reoccuring bug (#50063 in Tracker) where the player's
	// view height gets left at the ducked height while the player is standing, but we haven't
	// been  able to repro it to find the cause.  It may be fixed now due to a change I'm
	// also making in UpdateDuckJumpEyeOffset but just in case, this code will sense the 
	// problem and restore the eye to the proper position.  It doesn't smooth the transition,
	// but it is preferable to leaving the player's view too low.
	//
	// If the player is still alive and not an observer, check to make sure that
	// his view height is at the standing height.
	
	else if (!IsDead() && !player->IsObserver() && !player->IsInAVehicle())
	{
		if ((player->m_Local.m_flDuckJumpTime == 0.0f) &&
			(fabs( player->GetViewOffset().z - GetPlayerViewOffset( false ).z ) > 0.1))
		{
			
			if (!player->GetGroundEntity())
			{
				player->m_Local.m_bInDuckJump = true;
				player->m_Local.m_flDuckJumpTime =
					GAMEMOVEMENT_TIME_TO_UNDUCK * 0.5f;
			}
			else {
				// set the eye height to the non-ducked height
				SetDuckedEyeOffset( 0.0f );
				//Msg( "Forced Eye Offset to 0.0\n" );
			}
		}
	}
	
}
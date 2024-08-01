//========= Copyright Rob Cruickshank, All rights reserved. ============//
//
// Purpose: Moved all the mobility functions out of gamemovement.cpp into this
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
#include "firefightreloaded/mapinfo.h"

#if defined(HL2_DLL) || defined(HL2_CLIENT_DLL)
#include "hl_movedata.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


/* HL2_NORM_SPEED + 5, must be going faster than this
* to powerslide */
#define GAMEMOVEMENT_POWERSLIDE_MIN_SPEED 195.0f

#define GAMEMOVEMENT_CORNER_ESC_SPEED 80.0f

// start easing out of the wallrun for last 500 ms
#define GAMEMOVEMENT_WALLRUN_OUT_TIME 500.0f

#define GAMEMOVEMENT_WALLRUN_MIN_Z -0.15f

// Don't wallrun or climb on props with a bounding sphere smaller than this
static ConVar sv_climb_props_size( "sv_climb_props_size", "50.0" );

// By default disable climbing/wallrunning on npcs.
static ConVar sv_climb_npcs( "sv_climb_npcs", "0" );

// By default disable climbing/wallrunning on props.
static ConVar sv_climb_props("sv_climb_props", "0");

float VectorYaw( Vector& v )
{
	QAngle ang;
	VectorAngles( v, ang );
	return AngleNormalizePositive( ang[YAW] );
}

//-----------------------------------------------------------------------------
// Purpose: Check if the player should powerslide
//          * moving faster than normal run speed
//          * have HEV suit
//          (only called if on the ground and ducking)
//-----------------------------------------------------------------------------
void CGameMovement::CheckPowerSlide( void )
{
	if (!sv_slide.GetBool())
		return;

	KeyValues* pInfo = CMapInfo::GetMapInfoData();
	if (!pInfo->GetBool("CanPowerslide", true))
		return;

	if (mv->m_nButtons & IN_GRAPPLE)
		return;

	// Only check horizontal speed, don't want to 
	// powerslide after a steep or vertical fall
	float speed = mv->m_vecVelocity.Length2D();

	// Dead 
	if (player->pl.deadflag)
		return;

	if (player->IsSuitEquipped() &&
		speed > GAMEMOVEMENT_POWERSLIDE_MIN_SPEED) {

		player->m_bIsPowerSliding = true;
		player->m_Local.m_flSlideTime = GetSlideTime();
		// Give speed boost
		float newspeed = speed + GetSlideSpeedBoost();
		float maxboostspeed = sv_maxspeed.GetFloat(); // don't boost beyond this speed if restrictions on

		// don't boost speed above max plus boost if we have agreed to abide by certain restrictions
		if ( certain_restrictions.GetBool() ) {

			if ( speed > maxboostspeed ) {
				newspeed = speed; // no boost
			}
			else if ( newspeed > maxboostspeed ) {
				newspeed = maxboostspeed; // only boost up to max
			}
		}
		
		mv->m_vecVelocity.z = 0.0f; // zero out z component of velocity
		VectorScale( mv->m_vecVelocity, newspeed / speed, mv->m_vecVelocity );

		player->EmitSound("HL2Player.kick_body");
		player->PlayPowerSlideSound( (Vector &)mv->GetAbsOrigin() );

		player->m_Local.m_vecPunchAngle.Set( PITCH, -2 ); // shake the view a bit

		// Workaround for bug - if they slide, then jump, then slide on landing
		// the view stays at standing height from the second time onwards.
		// Only happens when using toggle duck. For now, just override it
		if ((player->m_Local.m_flDuckJumpTime == 0.0f) &&
			(fabs( player->GetViewOffset().z - GetPlayerViewOffset( true ).z ) > 0.1))
		{
			// set the eye height to the non-ducked height
			SetDuckedEyeOffset( /*duckFraction=*/1.0f );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Ends powersliding
//          * sets vars
//          * stops sound
//-----------------------------------------------------------------------------
void CGameMovement::EndPowerSlide( void )
{
	player->m_bIsPowerSliding = false;
	player->m_Local.m_flSlideTime = 0.0f;
	player->StopPowerSlideSound();
}

//-----------------------------------------------------------------------------
// Purpose: Called instead of Friction() when powersliding. Ignore
//          surface friction and just steadily slow down to crawl
//          speed
//-----------------------------------------------------------------------------
void CGameMovement::PowerSlideFriction( void )
{

	if (!player->m_bIsPowerSliding)
		return;

	// Calculate speed
	float speed = VectorLength( mv->m_vecVelocity );

	// how much faster than crawl speed are we going
	float drop = speed - (0.33333333f * GAMEMOVEMENT_POWERSLIDE_MIN_SPEED);

	// If we drop below crawl speed or reach the end of slide time, 
	// powerslide is over (avoid chance of divide by zero in drop formula)
	if (drop <= 0.0f || player->m_Local.m_flSlideTime <= 0.0f)
	{
		EndPowerSlide();
		return;
	}

	// how much do we need to drop each frame to get to that speed at the end
	float dt = 1000.0f * gpGlobals->frametime;
	drop *= (dt / player->m_Local.m_flSlideTime);

	float newspeed = speed - drop;

	if (newspeed < 0)
		newspeed = 0;

	if (newspeed != speed)
	{
		// Determine proportion of old speed we are using.
		newspeed /= speed;
		// Adjust velocity according to proportion.
		VectorScale( mv->m_vecVelocity, newspeed, mv->m_vecVelocity );
	}

	mv->m_outWishVel -= (1.f - newspeed) * mv->m_vecVelocity;
}

//-----------------------------------------------------------------------------
// Purpose: Calculate the yaw angle between the player and the wall
//-----------------------------------------------------------------------------
float CGameMovement::GetWallRunYaw( void )
{
	QAngle angles;
	float player_yaw = AngleNormalizePositive( mv->m_vecAbsViewAngles[YAW] );
	VectorAngles( player->m_vecWallNorm, angles );
	float wall_yaw = AngleNormalizePositive( angles[YAW] );
	
	return player_yaw - wall_yaw;
}


//-----------------------------------------------------------------------------
// Purpose: Calculate the wallrun view roll angle based on the 
//          yaw angle between the player and the wall
//-----------------------------------------------------------------------------
float CGameMovement::GetWallRunRollAngle( void )
{
	return sv_wallrun_roll.GetFloat() * 
			   sin( DEG2RAD( GetWallRunYaw() ) );
}


//-----------------------------------------------------------------------------
// Purpose: Check whether we are about to start wallrunning in the next .5 sec
//          Trace out to where we think we'll be and see if we hit a wall
//-----------------------------------------------------------------------------
void CGameMovement::AnticipateWallRun( void )
{
	if (!sv_wallrun.GetBool())
		return;

	KeyValues* pInfo = CMapInfo::GetMapInfoData();
	if (!pInfo->GetBool("CanWallrun", true))
		return;

	// No idea how this can be called when wallrunning, but it is
	if (player->m_nWallRunState >= WALLRUN_RUNNING)
		return;

	// Dead 
	if (player->pl.deadflag)
		return;

	const float antime = 0.5;

	Vector move; // relative position .5sec in future
	Vector end;  // absolute position after moving 
	trace_t pm;

	move = mv->m_vecVelocity * antime;
	// throw in some gravity (d = .5 * g * t ^ 2)
	move.z -= (0.5f * sv_gravity.GetFloat() * antime * antime);

	end = mv->GetAbsOrigin() + move;

	TracePlayerBBox( mv->GetAbsOrigin(), end, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm );

	if (pm.fraction == 1.0) {
		return; // no wall 
	}
	else {
		if (pm.plane.normal[2] < WALLRUN_MAX_Z)
		{
			// Wall coming - start leaning
			player->m_nWallRunState = WALLRUN_LEAN_IN;
			player->m_vecWallNorm = pm.plane.normal;
			player->m_Local.m_vecTargetPunchAngle.Set( ROLL, 
					(1 - pm.fraction) * GetWallRunRollAngle() );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Check whether we should start wallrunning. Called when we hit 
//          a wall while airborn
//-----------------------------------------------------------------------------
void CGameMovement::CheckWallRun( Vector &vecWallNormal, trace_t &pm )
{
	if (!sv_wallrun.GetBool())
		return;

	KeyValues* pInfo = CMapInfo::GetMapInfoData();
	if (!pInfo->GetBool("CanWallrun", true))
		return;

	// Can't wallrun without the suit
	if (!player->IsSuitEquipped())
		return;

	if (sv_wallrun_requiredirectcontrol.GetBool())
	{
		bool movementkeys = ((mv->m_nButtons & IN_MOVELEFT) ||
			(mv->m_nButtons & IN_MOVERIGHT) ||
			(mv->m_nButtons & IN_GRAPPLE));

		// don't wallrun unless we are directly controlling it.
		if (!(movementkeys))
		{
			EndWallRun(); // seriously, don't wallrun if ducking
			return;
		}
	}

	// Don't attach to wall if ducking - super annoying
	if (mv->m_nButtons & IN_DUCK)
		return;

	// Don't wallrun if in water at all
	if (player->GetWaterLevel())
		return;

	// Dead 
	if (player->pl.deadflag)
		return;
	
	if (vecWallNormal.z < GAMEMOVEMENT_WALLRUN_MIN_Z)
	{
		// Can't wallrun if the wall leans over us
		return;
	}

	if (player->m_flNextWallRunTime > gpGlobals->curtime)
	{
        return;
	}

#ifndef CLIENT_DLL
	// These checks use ClassMatches, so they can only be done on the server

	CBaseEntity* pObject = pm.m_pEnt;

	// Don't climb npcs unless they want to
	if (!sv_climb_npcs.GetBool()) {

		if (pObject && pObject->ClassMatches( "npc*" )) {
			return;
		}
	}

	if (sv_climb_props.GetBool()) {
		// Don't climb/wallrun props smaller than specified min size
		if (pObject && pObject->ClassMatches("prop*")) {
			float objectHeight = 2 * pObject->BoundingRadius();
			if (objectHeight < sv_climb_props_size.GetFloat())
			{
				return;
			}
		}
	}
	else
	{
		if (pObject && pObject->ClassMatches("prop*")) {
			return;
		}
	}
#endif
	// Store the wall normal
	VectorCopy( vecWallNormal, player->m_vecWallNorm );

	// Make sure feet can touch wall, not just head
	CheckFeetCanReachWall();

	// Already wallrunning
	if ( player->m_nWallRunState >= WALLRUN_RUNNING )
	{
		// Msg( "Already wallrunning\n" );
		return;
	}
	// Determine movement angles
	Vector forward, right, up;
	AngleVectors( mv->m_vecViewAngles, &forward, &right, &up );

	if ( CheckForSteps( mv->GetAbsOrigin(), vecWallNormal * -1 ) ||
		 CheckForSteps( mv->GetAbsOrigin(), mv->m_vecVelocity ) ||
		 CheckForSteps( mv->GetAbsOrigin(), forward * player->GetStepSize() )
		)
	{
		//Msg( "Not starting a wallrun because there are steps ahead\n" );
		return;
	}

	//Msg( "Start Wallrun (%d) (X %0.00f Y %0.00f Z %0.00f)\n", 
	//	 player->m_nWallRunState, vecWallNormal.x, vecWallNormal.y, vecWallNormal.z );
	player->m_nWallRunState = WALLRUN_RUNNING;
	//Msg( " -> (%d)\n", player->m_nWallRunState );
	player->m_Local.m_flWallRunTime = GetWallRunTime();

	float newmaxspeed = 
		MAX( 
			(sv_wallrun_speed.GetFloat()) + sv_wallrun_boost.GetFloat() ,
			(mv->m_vecVelocity.Length2D())
		);

	player->SetMaxSpeed( newmaxspeed );

	// Redirect velocity along plane
	ClipVelocity( mv->m_vecVelocity, vecWallNormal, mv->m_vecVelocity, 1.0f );

	// give speed boost
	float speed = mv->m_vecVelocity.Length2D();
	if (speed > 0.0f)
	{
		float newspeed = speed + sv_wallrun_boost.GetFloat();
		mv->m_vecVelocity.z = 0.0f; // might be better to lerp down to zero instead of slamming
		VectorScale(mv->m_vecVelocity, newspeed / speed, mv->m_vecVelocity);
		player->PlayWallRunSound((Vector&)mv->GetAbsOrigin());
	}
}

// Handle wallrun movement
void CGameMovement::WallRunMove( void )
{
	if ( player->m_Local.m_flWallRunTime <= 0.0f )
	{
		// time's up
		//Msg( "*\nEndWallRun because times up\n*\n" );
		EndWallRun();
		return;
	}

	if (player->m_nWallRunState < WALLRUN_RUNNING)
		return;

	if (sv_wallrun_requiredirectcontrol.GetBool())
	{
		bool movementkeys = ((mv->m_nButtons & IN_MOVELEFT) ||
			(mv->m_nButtons & IN_MOVERIGHT) ||
			(mv->m_nButtons & IN_GRAPPLE));

		// don't wallrun unless we are directly controlling it.
		if (!(movementkeys))
		{
			EndWallRun(); // seriously, don't wallrun if ducking
			return;
		}
	}

	if (mv->m_nButtons & IN_DUCK)
	{
		//Msg( "\n*\nEndWallRun because ducked\n*\n" );
		EndWallRun(); // seriously, don't wallrun if ducking
		return;
	}

	// Dead 
	if (player->pl.deadflag)
		return;

	//bool msgs = false;
	//if (rand() % 20 == 0)
	//	msgs = true;

	Vector  oldvel = mv->m_vecVelocity;
	QAngle oldAngleV;
	VectorAngles( oldvel, oldAngleV );

	Vector wishvel;
	float fmove, smove;
	Vector wishdir;
	float wishspeed;
	Vector start, move, dest;
	Vector forward, right, up;
	trace_t trc;

	float wallrun_yaw = GetWallRunYaw();
	float max_climb;
	bool steps;

	float rollangle = GetWallRunRollAngle();
	// Decay the roll angle as we approach the end, as a cue that time's up
	if (player->m_Local.m_flWallRunTime < GAMEMOVEMENT_WALLRUN_OUT_TIME ) 
	{
		rollangle *= player->m_Local.m_flWallRunTime / GAMEMOVEMENT_WALLRUN_OUT_TIME;
	}
	player->m_Local.m_vecTargetPunchAngle.Set( ROLL, rollangle );

    // Determine movement angles
	AngleVectors( mv->m_vecViewAngles, &forward, &right, &up );  

	// Copy movement amounts
	fmove = mv->m_flForwardMove;
	smove = 0.0f; // can't strafe while wallrunning

	right = vec3_origin;

	VectorNormalize( forward );  // Normalize remainder of vectors.

	VectorScale( forward, fmove, wishvel );

	VectorCopy( wishvel, wishdir );   // Determine magnitude of speed of move
	wishspeed = VectorNormalize( wishdir );

	// These speed calculations are a bit of a mess. The intention was that you get a 
	// speed boost when you first start wallrunning, and then the speed eases down by the 
	// end if you stay on the wall. "Short wallruns connected by long leaps give you the most speed", as it were.


	// Set the new maxspeed = current speed + some fraction of boost speed that decays for
	// first half of wallrun
	float decel_time = sv_wallrun_time.GetFloat();
	float fraction = MAX(
		(player->m_Local.m_flWallRunTime / decel_time),
		0 );

	// If you stay on the wall for the full time limit you should end up at this speed
	float end_speed = sv_wallrun_speed.GetFloat(); // 300
	float start_speed = 
		MAX( mv->m_vecVelocity.Length2D() + sv_wallrun_boost.GetFloat(),
		     end_speed);
	
	float delta_speed = fabsf(start_speed - end_speed);
	float newmaxspeed = end_speed + (delta_speed * fraction);

	player->SetMaxSpeed( newmaxspeed );
	wishspeed = newmaxspeed;

	// Set pmove velocity
	mv->m_vecVelocity.z = 0.0f;

	player->m_surfaceFriction = 1.0f;

	float angle = DotProduct(forward, player->m_vecWallNorm);
	Vector direction = forward - angle * player->m_vecWallNorm;
	Accelerate(direction, sv_wallrun_speed.GetFloat(), sv_wallrun_accel.GetFloat());

	max_climb = fabsf(sinf(DEG2RAD(GetWallRunYaw()))) * (sv_wallrun_max_rise.GetFloat() + 5.0f) - 5.0f;

	if (player->m_nWallRunState == WALLRUN_STALL)
		max_climb = -5;

	if (mv->m_vecVelocity.Length2D() < 2.5f)
	{
		//Msg( "Stalled because not moving\n" );
		player->m_nWallRunState = WALLRUN_STALL;
	}

	// I repurposed the climbing out of water code, but didn't rename it. 
	// WaterJump() is also used for ledge-climbing/mantling. 
	// It might make sense to move this code to before all those speed calculations.
	if (player->m_flWaterJumpTime > 0)
	{
		WaterJump();
		return;
	}
	//=============================================================================
	// Add in any base velocity to the current velocity.
	// (We're still messing with velocity? God, your software is a mess...)
	//=============================================================================
	VectorAdd( mv->m_vecVelocity, player->GetBaseVelocity(), mv->m_vecVelocity );

	if (fabsf(wallrun_yaw) > 165.0f && fabsf(wallrun_yaw) < 195.0f)
	{
		CheckWallRunScramble( steps ); // Re-using water jump for scrambling/mantling
		
		// If we're facing straight into the wall and not scrambling - stall
		if (!(player->GetFlags() & FL_WATERJUMP))
		{

			//Msg( "Stalled because facing into wall and not scrambling\n" );

			if (steps)
			{
				// Don't wallrun if we hit stairs.
				// Don't wallrun towards stairs.
				// Don't wallrun if you can see stairs.
				// Don't wallrun if your dentist has ever seen stairs.
				// I hate stairs.
				VectorSubtract( mv->m_vecVelocity, player->GetBaseVelocity(), mv->m_vecVelocity );
				EndWallRun();
				return;
			}
			
			if (player->GetEscapeVel().Length() == 0.0)
			    player->m_nWallRunState = WALLRUN_STALL;

		}
	}
	if ( // facing out from wall 
		((fabsf(wallrun_yaw) < sv_wallrun_stick_angle.GetFloat() ||
			fabsf(wallrun_yaw) > 360.0f - sv_wallrun_stick_angle.GetFloat()) &&
			fmove > 0.0) ||
		// backing out from wall
		((fabsf(wallrun_yaw) > 180.0f - sv_wallrun_stick_angle.GetFloat() &&
			fabsf(wallrun_yaw) < 180.0f + sv_wallrun_stick_angle.GetFloat()) &&
			fmove < 0.0)
		)
	{   // Trying to move outward from wall

		// Check for another wall to transfer to
		Vector dest = mv->GetAbsOrigin() + wishdir * player->GetStepSize();
		trace_t pm;
		TracePlayerBBox(mv->GetAbsOrigin(), dest,
			PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
			pm );

		if (pm.fraction == 1.0)
		{   // open space, just fall off the wall
			EndWallRun();
			VectorSubtract( mv->m_vecVelocity, player->GetBaseVelocity(), mv->m_vecVelocity );
			return;
		}
		else if (pm.plane.normal.z > GAMEMOVEMENT_WALLRUN_MIN_Z &&
				pm.plane.normal.z < GAMEMOVEMENT_WALLRUN_MAX_Z)
		{
			player->m_vecWallNorm = pm.plane.normal;
		}
	}

	// Escape velocity is a way to automatically back out of a corner 
	// when wallrunning.
	if (player->GetEscapeVel().Length() != 0.0)
	{
		mv->m_vecVelocity = player->GetEscapeVel();
	}

	// Redirect velocity along plane
	ClipVelocity( 
		mv->m_vecVelocity, 
		player->m_vecWallNorm, 
		mv->m_vecVelocity, 
		1.0f );

	// Prevent getting whipped around a sharp corner
	QAngle newAngleV;
	VectorAngles( mv->m_vecVelocity, newAngleV );

	if ( player->GetEscapeVel().Length() == 0 &&
		fabs( AngleDiff( oldAngleV[YAW], newAngleV[YAW] ) ) > 
		  sv_wallrun_stick_angle.GetFloat() * 1.2 &&
		  mv->m_vecVelocity.Length2D() > 250 )
	{
		mv->m_vecVelocity = oldvel;
		EndWallRun();
	}

	if (player->m_nWallRunState < WALLRUN_SCRAMBLE)
	{
		mv->m_vecVelocity.z =
			MIN( mv->m_vecVelocity.z,
			max_climb );
	}

	// Do the basic movement along the wall

    mv->m_outWishVel += wishdir * wishspeed;


	int blocked = TryPlayerMove();

	if ( blocked & 1 )
	{
		//Msg( "EndWallRun because hit floor\n" );
		VectorSubtract( mv->m_vecVelocity, player->GetBaseVelocity(), mv->m_vecVelocity );
		EndWallRun();
		return;
	}

	// Check for stairs
	if (CheckForSteps( mv->GetAbsOrigin(), player->m_vecWallNorm * -1 ) ||
		CheckForSteps( mv->GetAbsOrigin(), mv->m_vecVelocity ) ||
		CheckForSteps( mv->GetAbsOrigin(), forward * player->GetStepSize() ))
	{
		//Msg( "These are stairs\n" );
		// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
		VectorSubtract( mv->m_vecVelocity, player->GetBaseVelocity(), mv->m_vecVelocity );
		EndWallRun();
		StepMove( dest, trc );
		return;
	}

	// Check if we are facing into an interior corner - this happens sometimes
	// with drainpipes, pillars etc. 
	WallRunEscapeCorner( forward );


	//===================================================
	// Check we are not too far from the wall or running
	// across a doorway etc.
	//===================================================

	CheckFeetCanReachWall();

	// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
	VectorSubtract( mv->m_vecVelocity, player->GetBaseVelocity(), mv->m_vecVelocity );
	
	// Turn out from the wall slightly if there's a bump coming up
	if (sv_wallrun_anticipation.GetInt() >= 2)
	    WallRunAnticipateBump();

}

// Handle end of wallrun - set vars, stop sound
void CGameMovement::EndWallRun( void )
{
	//Msg( "End Wallrun\n" );
	player->m_nWallRunState = WALLRUN_NOT;
	player->StopWallRunSound();

	SetGroundEntity( NULL );
	player->DeriveMaxSpeed();

	Vector vecWallPush;
	VectorScale(player->m_vecWallNorm, 16.0f, vecWallPush);
	mv->m_vecVelocity += vecWallPush;

	player->SetEscapeVel(vec3_origin);
	player->m_Local.m_vecTargetPunchAngle.Set(ROLL, 0);

	player->m_vecLastWallRunPos = mv->GetAbsOrigin();
	player->m_flCoyoteTime = gpGlobals->curtime + sv_coyote_time.GetFloat();
}


//-----------------------------------------------------------------------------
// Purpose: Try to steer around obstacles while wall running. 
// Basic guideline is if blocked turn away from wall, else try to turn more
// towards wall
//-----------------------------------------------------------------------------
void CGameMovement::WallRunAnticipateBump( void )
{
	Vector start, move, dest, temp, newheading, newnormal;
	trace_t pm;
	QAngle angles, bumpangles;
	//bool msgs = false;
	//if (rand() % 15 == 0)
	//	msgs = true;
	start = mv->GetAbsOrigin();

	float old_yaw, new_yaw, delta_yaw;
	VectorAngles( mv->m_vecVelocity, angles );
	old_yaw = AngleNormalizePositive( angles[YAW] );

	// how far we travel in the lookahead time
	move = mv->m_vecVelocity * sv_wallrun_lookahead.GetFloat();
	move.z = 0; // let's ignore height movement
	dest = start + move;

	// See how far we can go
	TracePlayerBBox(
		start, dest,
		PlayerSolidMask(),
		COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	temp = dest;

	Vector vecHeldObjOrigin;
	QAngle angHeldObjAngles;
	// Check if the thing in front is an object the player is holding
	CBaseEntity* held_object(NULL);

	// Check if the trace hits a held object
	if (pm.fraction < 1.0)
	{
		IPhysicsObject* pPhysObj = pm.m_pEnt->VPhysicsGetObject();
		if ( pPhysObj )
		{
			
			// No point trying to go around something that we are carrying
			if (pPhysObj->GetGameFlags() & FVPHYSICS_PLAYER_HELD)
			{
				held_object = pm.m_pEnt;
				vecHeldObjOrigin = held_object->GetAbsOrigin();
				// Send it to the edge of the universe
				held_object->SetAbsOrigin( Vector( -20000, -20000, -20000 ) );

				// Try the trace again
				TracePlayerBBox(
					start, dest,
					PlayerSolidMask(),
					COLLISION_GROUP_PLAYER_MOVEMENT,
					pm );
			}
		}
	}

	if ( pm.fraction == 1.0 )
	{
		// Made it all the way - could we turn towards the wall more?
		Vector wallwards = player->m_vecWallNorm * sv_wallrun_inness.GetFloat() * -1 * gpGlobals->frametime;
		dest += wallwards;

		// See how far we can go
		TracePlayerBBox(
			start, dest,
			PlayerSolidMask(),
			COLLISION_GROUP_PLAYER_MOVEMENT,
			pm );

		if ( pm.fraction == 1.0 )
		{
			// This means if we turn towards the wall, we can still move all the way without hitting anything
			// Check if this movement would take us past the edge of the wall
			VectorNormalize( wallwards );
			wallwards = wallwards * player->GetStepSize() + dest;
			TracePlayerBBox(
				dest, wallwards,
				PlayerSolidMask(),
				COLLISION_GROUP_PLAYER_MOVEMENT,
				pm );

			if (pm.fraction == 1.0)
			{
				// We just went around a corner and now we're out in empty space.
				// Guess whether the player wants to go around the corner or end the 
				// wallrun based on their yaw
				float player_wallrun_yaw = fabs(GetWallRunYaw());
				if (player_wallrun_yaw < sv_wallrun_corner_stick_angle.GetFloat() || 
					player_wallrun_yaw > 360 - sv_wallrun_corner_stick_angle.GetFloat())
				{
					EndWallRun();
					return;
				}
			}

			// there's still a wall within range, turn towards the wall more
			newheading = dest - start;
			VectorAngles( newheading, angles );
			new_yaw = AngleNormalizePositive( angles[YAW] );
			delta_yaw = new_yaw - old_yaw;
			//if (msgs) Msg( "Turning rail towards wall (%0.000f)\n", delta_yaw );
			QAngle turn( 0, delta_yaw, 0 );
			VectorRotate( player->m_vecWallNorm, turn, player->m_vecWallNorm );
			player->SetEscapeVel( vec3_origin );
			//Msg( "Setting escape yaw = 0 because turning towards wall\n" );
		}
		else
		{
			// see how close to the wall we could get
			start = temp;
			TracePlayerBBox(
				start, dest,
				PlayerSolidMask(),
				COLLISION_GROUP_PLAYER_MOVEMENT,
				pm );

			if (pm.fraction == 1.0)
			{
				// This suggests we are going around the outside of a rounded corner
				//Msg( "Not turning towards wall, but maybe going around a corner\n" );
			}
			else if (pm.fraction <= 0.0 + FLT_EPSILON)
			{
				//Msg( "It seems we are flush against and parallel to the wall\n" );
			}
			else {
				dest = pm.endpos;
				start = mv->GetAbsOrigin();
				TracePlayerBBox(
					start, dest,
					PlayerSolidMask(),
					COLLISION_GROUP_PLAYER_MOVEMENT,
					pm );

				if (pm.fraction == 1.0)
				{
					// okay, turn towards the wall this much
					newheading = dest - start;
					VectorAngles( newheading, angles );
					new_yaw = AngleNormalizePositive( angles[YAW] );
					delta_yaw = new_yaw - old_yaw;
					//if (msgs) Msg( "Turning rail towards wall less (%0.000f)\n", delta_yaw );
					QAngle turn( 0, delta_yaw, 0 );
					VectorRotate( player->m_vecWallNorm, turn, player->m_vecWallNorm );
					player->SetEscapeVel( vec3_origin );
					//Msg( "Clearing escape yaw because turned towards wall\n" );
				}
				else
				{
					//Msg( "No good - empty space around corner but can't go straight there from here...\n" );
				}
			}
		}
	}
	else if ( CheckForSteps( pm.endpos, move ) )
	{
		// steps coming - don't try to avoid them, just return.
		// If we moved a held object out of the way, put it back
		if (held_object)
		{
			held_object->SetAbsOrigin( vecHeldObjOrigin );
		}
		return;
	}
	else // blocked by something - turn away from wall
	{

		Vector block_norm = pm.plane.normal;

		dest += player->m_vecWallNorm * 
			    sv_wallrun_outness.GetFloat() *
				gpGlobals->frametime;

		newheading = dest - start;
		VectorAngles( newheading, angles );
		new_yaw = AngleNormalizePositive( angles[YAW] );
		delta_yaw = new_yaw - old_yaw;
		
        QAngle turn( 0, delta_yaw, 0 );

		VectorRotate( player->m_vecWallNorm, turn, player->m_vecWallNorm );
	}

	// Automatically aim their view along the wall if they aren't moving the mouse
	if (sv_wallrun_lookness.GetFloat() > 0 && 
		fabs( sin( DEG2RAD( GetWallRunYaw() ) ) ) > 0.2f && // not facing straight into or out from wall
		gpGlobals->curtime - player->m_flAutoViewTime > 0.300)  // haven't moved the mouse in more than 300 ms
	{
		QAngle look = player->EyeAngles();
		QAngle move;
		VectorAngles( mv->m_vecVelocity, move );

		float delta = AngleDiff( move[YAW], look[YAW] );

		if ( fabs(delta) < 75 )
		{

			look[YAW] += delta * gpGlobals->frametime;
#ifndef CLIENT_DLL
			player->SnapEyeAngles( look );
#endif
		}
	}
	if (held_object)
	{
		held_object->SetAbsOrigin( vecHeldObjOrigin );
	}
	return;
}

//-----------------------------------------------------------------------------
// Purpose: Check if the wall in front is something we can climb on top of
//-----------------------------------------------------------------------------
void CGameMovement::CheckWallRunScramble( bool& steps )
{
	if (!sv_wallrun.GetBool())
		return;

	KeyValues* pInfo = CMapInfo::GetMapInfoData();
	if (!pInfo->GetBool("CanWallrun", true))
		return;

	Vector	flatforward;
	Vector forward;
	Vector	flatvelocity;
	steps = false;
	AngleVectors( mv->m_vecViewAngles, &forward );  // Determine movement angles
	bool already_scrambling = false;
	// Already water jumping / scrambling.
	if (player->m_flWaterJumpTime)
	{
		already_scrambling = true;
	}
	// See if we are backing up
	flatvelocity[0] = mv->m_vecVelocity[0];
	flatvelocity[1] = mv->m_vecVelocity[1];
	flatvelocity[2] = 0;

	// see if near an edge
	flatforward[0] = forward[0];
	flatforward[1] = forward[1];
	flatforward[2] = 0;
	VectorNormalize( flatforward );

	Vector vecStart, vecUp;
 
	vecStart = mv->GetAbsOrigin();
	vecUp = vecStart;

	Vector vecEnd;
	VectorMA( vecStart, 24.0f, flatforward, vecEnd );

	trace_t tr;
	TracePlayerBBox( vecStart, vecEnd, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, tr );
	if (tr.fraction < 1.0)		// solid at waist
	{
#ifndef CLIENT_DLL
		if (!sv_climb_npcs.GetBool() &&
			tr.m_pEnt && tr.m_pEnt->ClassMatches( "npc*" ))
		{
			return;
		}

		if (sv_climb_props.GetBool()) {
			if (tr.m_pEnt && tr.m_pEnt->ClassMatches("prop*") &&
				tr.m_pEnt->BoundingRadius() * 2 < sv_climb_props_size.GetFloat())
			{
				return;
			}
		}
		else
		{
			if (tr.m_pEnt && tr.m_pEnt->ClassMatches("prop*"))
			{
				return;
			}
		}
#endif

		IPhysicsObject *pPhysObj = tr.m_pEnt->VPhysicsGetObject();
		if (pPhysObj)
		{
			if (pPhysObj->GetGameFlags() & FVPHYSICS_PLAYER_HELD)
				return;
		}

		// Make sure we have room to move up
		vecUp.z = mv->GetAbsOrigin().z +
			      player->GetViewOffset().z + 
				  sv_wallrun_scramble_z.GetFloat();

		TracePlayerBBox( vecStart, vecUp, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, tr );
		if (tr.endpos.z < vecStart.z + player->GetStepSize())
		{
			if (already_scrambling)
			{
				player->RemoveFlag( FL_WATERJUMP );
				player->m_flWaterJumpTime = 0.0f;
				//Msg( "Cancel scramble - hit head\n" );
			}
			return;
		}
		vecStart = tr.endpos;

		VectorMA( vecStart, 24.0f, flatforward, vecEnd );
		VectorMA( vec3_origin, -50.0f, tr.plane.normal, player->m_vecWaterJumpVel );

		TracePlayerBBox( vecStart, vecEnd, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, tr );
		if (tr.fraction == 1.0)		// open at eye level
		{
			// Now trace down to see if we would actually land on a standable surface.
			VectorCopy( vecEnd, vecStart );
			vecEnd.z -= 1024.0f;
			TracePlayerBBox( vecStart, vecEnd, PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, tr );
			if ( (tr.fraction < 1.0f) && (tr.plane.normal.z >= 0.7) )
			{
				float height_diff = tr.endpos.z - (mv->GetAbsOrigin().z + player->GetStepSize());
				if (height_diff > 0)
				{   // suitable for scrambling and we can't just step over it
					mv->m_vecVelocity[2] = 200.0f;			// Push up
					mv->m_nOldButtons |= IN_JUMP;		// Don't jump again until released
					player->AddFlag( FL_WATERJUMP );
					// 
					if (!already_scrambling)
					    player->m_flWaterJumpTime = 2000.0f;	// Do this for 2 seconds

					player->m_nWallRunState = WALLRUN_SCRAMBLE;

				}
				else {
					// can just step over it
					steps = true;
					return;
				}
			}
		}
		else if (already_scrambling)
		{
			// We were scrambling but now we are not facing the right way 
			// Stop scrambling
			player->RemoveFlag( FL_WATERJUMP );
			player->m_flWaterJumpTime = 0.0f;
			//Msg( "Cancel scramble - nowhere to go\n" );
		}
	}
	return;
}

//-----------------------------------------------------------------------------
// Purpose: Check if player's feet can reach the wall
//-----------------------------------------------------------------------------
void CGameMovement::CheckFeetCanReachWall( void )
{
	Vector start, end, move, actual_wall_norm;
	float minz = -60.0f + sv_wallrun_feet_z.GetFloat();
	start = mv->GetAbsOrigin();
	trace_t pm;
	// First compensate for any current distance from the wall
	// (could be moving out around a bump)
	bool steps;
	move = player->m_vecWallNorm * -2 * player->GetStepSize();
	end = start + move;
	TracePlayerBBox(
		start, end,
		PlayerSolidMask(),
		COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	if ( pm.fraction == 1.0 )
	{
		return;
	}
	start = pm.endpos;
	actual_wall_norm = pm.plane.normal;
	move = Vector( 0, 0, minz );
	end = start + move;
	
	float wallrun_yaw = GetWallRunYaw();
	if (fabsf( wallrun_yaw ) > 165.0f && fabsf( wallrun_yaw ) < 195.0f)
	{
		CheckWallRunScramble( steps );
		if (player->GetFlags() & FL_WATERJUMP) {
			return; // scrambling is allowed even if only upper body touching wall
		}
	}

	TracePlayerBBox(
		start, end,
		PlayerSolidMask(),
		COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	if ( pm.fraction > 0 )
	{
		// Now check if being lower allows us to move further wallwards
		start = pm.endpos;
		move = actual_wall_norm * -2 * player->GetStepSize();
		end = start + move;

		// TracePlayerBBox is basically a no-op, right? /s
		TracePlayerBBox(
			start, end,
			PlayerSolidMask(),
			COLLISION_GROUP_PLAYER_MOVEMENT,
			pm );

		if ( pm.fraction == 1.0 )
		{
			// We could move further wallwards if we were lower,
			// i.e. only our head/upper body is touching the wall. 
			EndWallRun();
			player->m_flNextWallRunTime = gpGlobals->curtime + 0.75;
		}
	}
}

bool    
CGameMovement::CheckForSteps( const Vector& startpos, const Vector& vel )
{
	// Check for steps - are we blocked in front but can move up and forwards?
	// Yes, that's really how it checks for stairs. 
	Vector stepstart = startpos;
	Vector stepmove = vel * gpGlobals->frametime;
	Vector stepdest = stepstart + stepmove;
	trace_t steptrace;

	TracePlayerBBox(
		stepstart, stepdest,
		PlayerSolidMask(),
		COLLISION_GROUP_PLAYER_MOVEMENT,
		steptrace );

	if (steptrace.fraction == 1.0 || fabs( steptrace.plane.normal.z ) > 0.1)
	{
		return false; // We have room to keep moving forwards
		              // Or the thing blocking us is not stairs 
		              // which we can tell from the angle
	}

	// Blocked in front - can we go up and then forwards?
	stepmove = Vector( 0, 0, player->GetStepSize() );
	stepdest = stepstart + stepmove;
	 steptrace;

	TracePlayerBBox(
		stepstart, stepdest,
		PlayerSolidMask(),
		COLLISION_GROUP_PLAYER_MOVEMENT,
		steptrace );
	if (steptrace.fraction == 1.0)
	{
		stepstart = stepdest;
		stepmove = vel.Normalized() * 6; // 6 inches to stand on
		stepdest += stepmove;
		TracePlayerBBox(
			stepstart, stepdest,
			PlayerSolidMask(),
			COLLISION_GROUP_PLAYER_MOVEMENT,
			steptrace );
		if (steptrace.fraction == 1.0)
		{
			// We can treat the obstacle as a step - don't turn
			return true;
		}
	}
	
	return false;
}

// This function tests whether you could move sideways then forwards from
// a position behind you, 
// to stop you getting stuck in a corner between a wall and a drainpipe or something.
bool 
CGameMovement::TryEscape( Vector& behind, float rotation, Vector move )
{
	VectorYawRotate( move, rotation, move );
	Vector posE = behind + move;
	trace_t pm;
	TracePlayerBBox(
		behind, posE,
		PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	if (pm.fraction == 1.0)
	{
		VectorYawRotate( move, rotation, move );
		Vector posF = posE + move;
		TracePlayerBBox(
			posE, posF,
			PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
			pm );
		
		return (pm.fraction == 1.0);
	}
	
	return false;

}

void CGameMovement::WallRunEscapeCorner( Vector& wishdir )
{
	// Try to escape a small corner 

	// Verify that we are in a corner - blocked in front and to the side

    // Try to move back, then sideways, then forward on one side, 
	// then the other if the first side fails.
	// If we can go back, sideways, forward, then this is a corner 
	// we can escape from, so set escape vel

	Vector start, forward, side, behind, move;
	const float small_dist = 3;
	const float stepsize = 12;
	float wallrun_yaw = GetWallRunYaw();
	if (wallrun_yaw < 0)
	{
		wallrun_yaw += 360;
	}
	const float rotation = (wallrun_yaw - 180 < 0) ? -90 : 90; // which side to we try to go around?

	//Msg( "Yaw %0.0f Rotation %0.0f\n", wallrun_yaw, rotation );
	trace_t pm;
	start = mv->GetAbsOrigin();

	move = small_dist * player->m_vecWallNorm * -1;
	forward = start + move;

	TracePlayerBBox(
		start, forward,
		PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	if (pm.fraction == 1.0)
		return; // not in a corner

	VectorYawRotate( move, rotation, move );
	side = start + move;

	TracePlayerBBox(
		start, side,
		PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	if (pm.fraction == 1.0)
	{
		// check the other side
		VectorYawRotate( move, 180, move );
		side = start + move;
		TracePlayerBBox(
			start, side,
			PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
			pm );

		if (pm.fraction == 1.0)
		    return; // not in a corner
	}

	// Try to move backwards from start to behind

	move = wishdir * -stepsize;
	behind = start + move;

	TracePlayerBBox(
		start, behind,
		PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT,
		pm );

	if (pm.fraction < 1.0)
		return; // can't move stepsize backwards - screwed.

	if ( TryEscape( behind, rotation, move ) )
	{
		Vector escape = forward - start;
		VectorYawRotate( escape, -rotation, escape );
		VectorScale( escape, GAMEMOVEMENT_CORNER_ESC_SPEED, escape );
		player->SetEscapeVel( escape );
	}
	else if ( TryEscape( behind, -rotation, move ) )
	{
		Vector escape = forward - start;
		VectorYawRotate( escape, rotation, escape );
		VectorScale( escape, GAMEMOVEMENT_CORNER_ESC_SPEED, escape );
		player->SetEscapeVel( escape );
	}
}
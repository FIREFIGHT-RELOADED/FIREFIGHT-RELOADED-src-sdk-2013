//========= Copyright Rob Cruickshank, All rights reserved. ============//
//
// Purpose: Shared definitions for client and server mobility code
//
//=============================================================================//

#ifndef PLAYER_MOBILITY_DEFS_H
#define PLAYER_MOBILITY_DEFS_H
#ifdef _WIN32
#pragma once
#endif

enum WallRunState
{
	WALLRUN_NOT = 0, // Not wallrunning
	WALLRUN_LEAN_IN, // About to start wall running, lean in
	WALLRUN_RUNNING, // Wallrunning
	WALLRUN_JUMPING, // Jumping off the wall
	WALLRUN_STALL,   // Wallrunning, but facing into the wall or 
	                 // otherwise not moving along it
	WALLRUN_SCRAMBLE // basically waterjumping - vertical velocity is allowed
};

/* HL2_NORM_SPEED + 5, must be going faster than this
* to powerslide */
#define GAMEMOVEMENT_POWERSLIDE_MIN_SPEED 195.0f

static const float GAMEMOVEMENT_WALLRUN_MAX_Z = 20.0f;
static const float GAMEMOVEMENT_WALLRUN_MIN_Z = -50.0f;

#endif // PLAYER_MOBILITY_DEFS_H
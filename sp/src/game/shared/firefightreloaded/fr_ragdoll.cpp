//Credit to Open Fortress for the gore code and idea!

#include "cbase.h"
#include "fr_ragdoll.h"

#if CLIENT_DLL
#include <c_basehlplayer.h>
#else

#endif // FR_CLIENT

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
#include <bone_setup.h>

#ifdef CLIENT_DLL

//-----------------------------------------------------------------------------
// Purpose: Gore!
//-----------------------------------------------------------------------------

// Scale head to nothing
static void ScaleGoreHead(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_Head1" };

		for (int i = 0; i < 4; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}

}

// Scale left arm to nothing
static void ScaleGoreLeftArm(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_UpperArm", "ValveBiped.Bip01_L_Hand", "ValveBiped.Bip01_L_Forearm",
									"ValveBiped.Bip01_L_Finger0", "ValveBiped.Bip01_L_Finger01", "ValveBiped.Bip01_L_Finger02",
									"ValveBiped.Bip01_L_Finger1", "ValveBiped.Bip01_L_Finger11", "ValveBiped.Bip01_L_Finger12",
									"ValveBiped.Bip01_L_Finger2", "ValveBiped.Bip01_L_Finger21", "ValveBiped.Bip01_L_Finger22",
									"ValveBiped.Bip01_L_Finger3", "ValveBiped.Bip01_L_Finger31", "ValveBiped.Bip01_L_Finger32",
									"ValveBiped.Bip01_L_Finger4", "ValveBiped.Bip01_L_Finger41", "ValveBiped.Bip01_L_Finger42",
		};

		for (int i = 0; i < 18; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale left hand to nothing
static void ScaleGoreLeftHand(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_Hand",
									"ValveBiped.Bip01_L_Finger0", "ValveBiped.Bip01_L_Finger01", "ValveBiped.Bip01_L_Finger02",
									"ValveBiped.Bip01_L_Finger1", "ValveBiped.Bip01_L_Finger11", "ValveBiped.Bip01_L_Finger12",
									"ValveBiped.Bip01_L_Finger2", "ValveBiped.Bip01_L_Finger21", "ValveBiped.Bip01_L_Finger22",
									"ValveBiped.Bip01_L_Finger3", "ValveBiped.Bip01_L_Finger31", "ValveBiped.Bip01_L_Finger32",
									"ValveBiped.Bip01_L_Finger4", "ValveBiped.Bip01_L_Finger41", "ValveBiped.Bip01_L_Finger42",
		};

		for (int i = 0; i < 16; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right arm to nothing
static void ScaleGoreRightArm(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_UpperArm", "ValveBiped.Bip01_R_Hand", "ValveBiped.Bip01_R_Forearm",
									"ValveBiped.Bip01_R_Finger0", "ValveBiped.Bip01_R_Finger01", "ValveBiped.Bip01_R_Finger02",
									"ValveBiped.Bip01_R_Finger1", "ValveBiped.Bip01_R_Finger11", "ValveBiped.Bip01_R_Finger12",
									"ValveBiped.Bip01_R_Finger2", "ValveBiped.Bip01_R_Finger21", "ValveBiped.Bip01_R_Finger22",
									"ValveBiped.Bip01_R_Finger3", "ValveBiped.Bip01_R_Finger31", "ValveBiped.Bip01_R_Finger32",
									"ValveBiped.Bip01_R_Finger4", "ValveBiped.Bip01_R_Finger41", "ValveBiped.Bip01_R_Finger42",
		};

		for (int i = 0; i < 18; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right hand to nothing
static void ScaleGoreRightHand(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_Hand",
									"ValveBiped.Bip01_R_Finger0", "ValveBiped.Bip01_R_Finger01", "ValveBiped.Bip01_R_Finger02",
									"ValveBiped.Bip01_R_Finger1", "ValveBiped.Bip01_R_Finger11", "ValveBiped.Bip01_R_Finger12",
									"ValveBiped.Bip01_R_Finger2", "ValveBiped.Bip01_R_Finger21", "ValveBiped.Bip01_R_Finger22",
									"ValveBiped.Bip01_R_Finger3", "ValveBiped.Bip01_R_Finger31", "ValveBiped.Bip01_R_Finger32",
									"ValveBiped.Bip01_R_Finger4", "ValveBiped.Bip01_R_Finger41", "ValveBiped.Bip01_R_Finger42",
		};

		for (int i = 0; i < 16; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale left knee to nothing
static void ScaleGoreLeftKnee(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_Knee", "ValveBiped.Bip01_L_Shin", "ValveBiped.Bip01_L_Ankle",
									"ValveBiped.Bip01_L_Calf", "ValveBiped.Bip01_L_Foot", "ValveBiped.Bip01_L_Toe0"
		};

		for (int i = 0; i < 6; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale left foot to nothing
static void ScaleGoreLeftFoot(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_Foot", "ValveBiped.Bip01_L_Toe0" };

		for (int i = 0; i < 2; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right knee to nothing
static void ScaleGoreRightKnee(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_Knee", "ValveBiped.Bip01_R_Shin", "ValveBiped.Bip01_R_Ankle",
									"ValveBiped.Bip01_R_Calf", "ValveBiped.Bip01_R_Foot", "ValveBiped.Bip01_R_Toe0"
		};

		for (int i = 0; i < 6; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right foot to nothing
static void ScaleGoreRightFoot(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_Foot", "ValveBiped.Bip01_R_Toe0" };

		for (int i = 0; i < 2; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleBy(0.001f, pAnimating->GetBoneForWrite(iBone));
		}
	}
}

//FRRagdoll_Player

IMPLEMENT_CLIENTCLASS_DT_NOBASE(C_FRRagdoll_Player, DT_FRRagdoll_Player, CFRRagdoll_Player)
RecvPropVector(RECVINFO(m_vecRagdollOrigin)),
RecvPropEHandle(RECVINFO(m_hEntity)),
RecvPropInt(RECVINFO(m_nModelIndex)),
RecvPropInt(RECVINFO(m_nForceBone)),
RecvPropVector(RECVINFO(m_vecForce)),
RecvPropVector(RECVINFO(m_vecRagdollVelocity)),
RecvPropInt(RECVINFO(m_iGoreHead)),
RecvPropInt(RECVINFO(m_iGoreLeftArm)),
RecvPropInt(RECVINFO(m_iGoreRightArm)),
RecvPropInt(RECVINFO(m_iGoreLeftLeg)),
RecvPropInt(RECVINFO(m_iGoreRightLeg))
END_RECV_TABLE()

C_FRRagdoll_Player::C_FRRagdoll_Player()
{
	m_iGoreHead = 0;
	m_iGoreLeftArm = 0;
	m_iGoreRightArm = 0;
	m_iGoreLeftLeg = 0;
	m_iGoreRightLeg = 0;
}

C_FRRagdoll_Player::~C_FRRagdoll_Player()
{
	PhysCleanupFrictionSounds(this);

	if (m_hEntity)
	{
		m_hEntity->CreateModelInstance();
	}
}

void C_FRRagdoll_Player::Interp_Copy(C_BaseAnimatingOverlay* pSourceEntity)
{
	if (!pSourceEntity)
		return;

	VarMapping_t* pSrc = pSourceEntity->GetVarMapping();
	VarMapping_t* pDest = GetVarMapping();

	// Find all the VarMapEntry_t's that represent the same variable.
	for (int i = 0; i < pDest->m_Entries.Count(); i++)
	{
		VarMapEntry_t* pDestEntry = &pDest->m_Entries[i];
		const char* pszName = pDestEntry->watcher->GetDebugName();
		for (int j = 0; j < pSrc->m_Entries.Count(); j++)
		{
			VarMapEntry_t* pSrcEntry = &pSrc->m_Entries[j];
			if (!Q_strcmp(pSrcEntry->watcher->GetDebugName(), pszName))
			{
				pDestEntry->watcher->Copy(pSrcEntry->watcher);
				break;
			}
		}
	}
}

void C_FRRagdoll_Player::ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName)
{
	IPhysicsObject* pPhysicsObject = VPhysicsGetObject();

	if (!pPhysicsObject)
		return;

	Vector dir = pTrace->endpos - pTrace->startpos;

	// m_iGore<limb> has a level, from 0 to 3
	// 1 is unused (reserved for normal TF bodygroups like pyro's head)
	// 2 means the lower limb is marked for removal, 3 means the upper limb is marked for removal, the head is an exception as it only has level 2
	// if our current level is at level 3, that means we can't dismember this limb anymore
	// if our current level is at level 2, that means we can dismember this limb once more up to level 3
	// if our current level is at level 0/1, that means we can dismember this limb up to level 2
	// Dismember<limb> function accepts true or false, true means this limb will be dismembered to level 3, false means dismembered to level 2

	if (m_bGoreEnabled)
	{
		switch (pTrace->hitgroup)
		{
		case HITGROUP_HEAD:
			if (m_iGoreHead == 3)
			{
				break;
			}
			else if (m_iGoreHead == 2)
			{
				break;
			}
			else
			{
				DismemberHead();
				break;
			}
		case HITGROUP_LEFTARM:
			if (m_iGoreLeftArm == 3)
			{
				break;
			}
			else if (m_iGoreLeftArm == 2)
			{
				DismemberLeftArm(true);
				break;
			}
			else
			{
				DismemberLeftArm(false);
				break;
			}
		case HITGROUP_RIGHTARM:
			if (m_iGoreRightArm == 3)
			{
				break;
			}
			else if (m_iGoreRightArm == 2)
			{
				DismemberRightArm(true);
				break;
			}
			else
			{
				DismemberRightArm(false);
				break;
			}
		case HITGROUP_LEFTLEG:
			if (m_iGoreLeftLeg == 3)
			{
				break;
			}
			else if (m_iGoreLeftLeg == 2)
			{
				DismemberLeftLeg(true);
				break;
			}
			else
			{
				DismemberLeftLeg(false);
				break;
			}
		case HITGROUP_RIGHTLEG:
			if (m_iGoreRightLeg == 3)
			{
				break;
			}
			else if (m_iGoreRightLeg == 2)
			{
				DismemberRightLeg(true);
				break;
			}
			else
			{
				DismemberRightLeg(false);
				break;
			}
		default:
			break;
		}
	}

	if (iDamageType == DMG_BLAST)
	{
		dir *= 4000;  // adjust impact strenght

		// apply force at object mass center
		pPhysicsObject->ApplyForceCenter(dir);

		if (m_bGoreEnabled)
			DismemberRandomLimbs();
	}
	else
	{
		Vector hitpos;

		VectorMA(pTrace->startpos, pTrace->fraction, dir, hitpos);
		VectorNormalize(dir);

		dir *= 4000;  // adjust impact strenght

		// apply force where we hit it
		pPhysicsObject->ApplyForceOffset(dir, hitpos);

		// Blood spray!
		//		FX_CS_BloodSpray( hitpos, dir, 10 );
	}

	m_pRagdoll->ResetRagdollSleepAfterTime();
}


void C_FRRagdoll_Player::CreateFRRagdoll_Player(void)
{
	// First, initialize all our data. If we have the player's entity on our client,
	// then we can make ourselves start out exactly where the player is.
	C_BaseHLPlayer* pPlayer = dynamic_cast<C_BaseHLPlayer*>(m_hEntity.Get());

	if (pPlayer && !pPlayer->IsDormant())
	{
		// move my current model instance to the ragdoll's so decals are preserved.
		pPlayer->SnatchModelInstance(this);

		VarMapping_t* varMap = GetVarMapping();

		// Copy all the interpolated vars from the player entity.
		// The entity uses the interpolated history to get bone velocity.
		bool bRemotePlayer = (pPlayer != C_BasePlayer::GetLocalPlayer());
		if (bRemotePlayer)
		{
			Interp_Copy(pPlayer);

			SetAbsAngles(pPlayer->GetRenderAngles());
			GetRotationInterpolator().Reset();

			m_flAnimTime = pPlayer->m_flAnimTime;
			SetSequence(pPlayer->GetSequence());
			m_flPlaybackRate = pPlayer->GetPlaybackRate();
		}
		else
		{
			// This is the local player, so set them in a default
			// pose and slam their velocity, angles and origin
			SetAbsOrigin(m_vecRagdollOrigin);

			SetAbsAngles(pPlayer->GetRenderAngles());

			SetAbsVelocity(m_vecRagdollVelocity);

			int iSeq = pPlayer->GetSequence();
			if (iSeq == -1)
			{
				Assert(false);	// missing walk_lower?
				iSeq = 0;
			}

			SetSequence(iSeq);	// walk_lower, basic pose
			SetCycle(0.0);

			Interp_Reset(varMap);
		}
	}
	else
	{
		// overwrite network origin so later interpolation will
		// use this position
		SetNetworkOrigin(m_vecRagdollOrigin);

		SetAbsOrigin(m_vecRagdollOrigin);
		SetAbsVelocity(m_vecRagdollVelocity);

		Interp_Reset(GetVarMapping());

	}

	SetModelIndex(m_nModelIndex);

	// Make us a ragdoll..
	m_nRenderFX = kRenderFxRagdoll;

	matrix3x4_t boneDelta0[MAXSTUDIOBONES];
	matrix3x4_t boneDelta1[MAXSTUDIOBONES];
	matrix3x4_t currentBones[MAXSTUDIOBONES];
	const float boneDt = 0.05f;

	if (pPlayer && !pPlayer->IsDormant())
	{
		pPlayer->GetRagdollInitBoneArrays(boneDelta0, boneDelta1, currentBones, boneDt);
	}
	else
	{
		GetRagdollInitBoneArrays(boneDelta0, boneDelta1, currentBones, boneDt);
	}

	InitAsClientRagdoll(boneDelta0, boneDelta1, currentBones, boneDt);

	int iBone = LookupBone("ValveBiped.Bip01_Neck1");

	if (iBone != -1)
	{
		m_bGoreEnabled = g_fr_headshotgore.GetBool();
	}
	else
	{
		m_bGoreEnabled = false;
	}

	if (m_bGoreEnabled)
		m_BoneAccessor.SetWritableBones(BONE_USED_BY_ANYTHING);

	SetNextClientThink(gpGlobals->curtime + 0.1f);
}


void C_FRRagdoll_Player::OnDataChanged(DataUpdateType_t type)
{
	BaseClass::OnDataChanged(type);

	if (type == DATA_UPDATE_CREATED)
	{
		CreateFRRagdoll_Player();
	}
}

IRagdoll* C_FRRagdoll_Player::GetIRagdoll() const
{
	return m_pRagdoll;
}

void C_FRRagdoll_Player::UpdateOnRemove(void)
{
	VPhysicsSetObject(NULL);

	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
// Purpose: clear out any face/eye values stored in the material system
//-----------------------------------------------------------------------------
void C_FRRagdoll_Player::SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights)
{
	BaseClass::SetupWeights(pBoneToWorld, nFlexWeightCount, pFlexWeights, pFlexDelayedWeights);

	static float destweight[128];
	static bool bIsInited = false;

	CStudioHdr* hdr = GetModelPtr();
	if (!hdr)
		return;

	int nFlexDescCount = hdr->numflexdesc();
	if (nFlexDescCount)
	{
		Assert(!pFlexDelayedWeights);
		memset(pFlexWeights, 0, nFlexWeightCount * sizeof(float));
	}

	if (m_iEyeAttachment > 0)
	{
		matrix3x4_t attToWorld;
		if (GetAttachment(m_iEyeAttachment, attToWorld))
		{
			Vector local, tmp;
			local.Init(1000.0f, 0.0f, 0.0f);
			VectorTransform(local, attToWorld, tmp);
			modelrender->SetViewTarget(GetModelPtr(), GetBody(), tmp);
		}
	}
}

//---------------------------------------------------------------------------- -
// Purpose: Scale the bones that need to be scaled for gore
//-----------------------------------------------------------------------------
void C_FRRagdoll_Player::BuildTransformations(CStudioHdr * pStudioHdr, Vector * pos, Quaternion q[], const matrix3x4_t & cameraTransform, int boneMask, CBoneBitList & boneComputed)
{
	BaseClass::BuildTransformations(pStudioHdr, pos, q, cameraTransform, boneMask, boneComputed);

	if (m_bGoreEnabled)
		ScaleGoreBones();
}

void C_FRRagdoll_Player::ScaleGoreBones()
{
	if (m_iGoreHead > 1)
		ScaleGoreHead(this);

	if (m_iGoreLeftArm == 2)
		ScaleGoreLeftHand(this);
	else if (m_iGoreLeftArm == 3)
		ScaleGoreLeftArm(this);

	if (m_iGoreRightArm == 2)
		ScaleGoreRightHand(this);
	else if (m_iGoreRightArm == 3)
		ScaleGoreRightArm(this);

	if (m_iGoreLeftLeg == 2)
		ScaleGoreLeftFoot(this);
	else if (m_iGoreLeftLeg == 3)
		ScaleGoreLeftKnee(this);

	if (m_iGoreRightLeg == 2)
		ScaleGoreRightFoot(this);
	else if (m_iGoreRightLeg == 3)
		ScaleGoreRightKnee(this);
}

void C_FRRagdoll_Player::DismemberHead()
{
	m_iGoreHead = 3;

	int iAttach = LookupBone("bip_neck");

	if (iAttach != -1)
	{
		ParticleProp()->Create("smod_blood_decap_r", PATTACH_BONE_FOLLOW, "ValveBiped.Bip01_Neck1");
		
		EmitSound("Gore.Headshot");

		m_iGoreDecalAmount += 15;
		m_iGoreDecalBone = iAttach;

	}
}

void C_FRRagdoll_Player::DismemberBase(char const* szBodyPart, bool bLevel, bool bBloodEffects, char const* szParticleBone)
{
	int iAttach = LookupBone(szParticleBone);

	if (iAttach != -1)
	{
		// I'm too lazy to make a new particle which is less bloody than blood_decap, but whatever, this works
		ParticleProp()->Create("smod_blood_gib_r", PATTACH_BONE_FOLLOW, szParticleBone);
	}

	if (iAttach != -1)
	{
		m_iGoreDecalAmount += 4;
		m_iGoreDecalBone = iAttach;

		EmitSound("Gore.Headshot");
	}
}

void C_FRRagdoll_Player::DismemberLeftArm(bool bLevel)
{
	DismemberBase("leftarm", bLevel, true, bLevel ? "ValveBiped.Bip01_L_UpperArm" : "ValveBiped.Bip01_L_Forearm");

	if (bLevel)
		m_iGoreLeftArm = 3;
	else
		m_iGoreLeftArm = 2;
}

void C_FRRagdoll_Player::DismemberRightArm(bool bLevel)
{
	DismemberBase("rightarm", bLevel, true, bLevel ? "ValveBiped.Bip01_R_UpperArm" : "ValveBiped.Bip01_R_Forearm");

	if (bLevel)
		m_iGoreRightArm = 3;
	else
		m_iGoreRightArm = 2;
}

void C_FRRagdoll_Player::DismemberLeftLeg(bool bLevel)
{
	DismemberBase("leftleg", bLevel, true, bLevel ? "ValveBiped.Bip01_L_Knee" : "ValveBiped.Bip01_L_Foot");

	if (bLevel)
		m_iGoreLeftLeg = 3;
	else
		m_iGoreLeftLeg = 2;
}

void C_FRRagdoll_Player::DismemberRightLeg(bool bLevel)
{
	DismemberBase("rightleg", bLevel, true, bLevel ? "ValveBiped.Bip01_R_Knee" : "ValveBiped.Bip01_R_Foot");

	if (bLevel)
		m_iGoreRightLeg = 3;
	else
		m_iGoreRightLeg = 2;
}

void C_FRRagdoll_Player::InitDismember()
{
	// head does not have two levels of dismemberment, only one
	if (m_iGoreHead > 1)
		DismemberHead();

	if (m_iGoreLeftArm == 3)
		DismemberLeftArm(true);
	else if (m_iGoreLeftArm == 2)
		DismemberLeftArm(false);

	if (m_iGoreRightArm == 3)
		DismemberRightArm(true);
	else if (m_iGoreRightArm == 2)
		DismemberRightArm(false);

	if (m_iGoreLeftLeg == 3)
		DismemberLeftLeg(true);
	else if (m_iGoreLeftLeg == 2)
		DismemberLeftLeg(false);

	if (m_iGoreRightLeg == 3)
		DismemberRightLeg(true);
	else if (m_iGoreRightLeg == 2)
		DismemberRightLeg(false);
}

void C_FRRagdoll_Player::DismemberRandomLimbs(void)
{
	int iGore = 0;

	// NOTE: head is not dismembered here intentionally

	if (m_iGoreLeftArm < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberLeftArm(false);
		else if (iGore == 3)
			DismemberLeftArm(true);
	}

	if (m_iGoreRightArm < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberRightArm(false);
		else if (iGore == 3)
			DismemberRightArm(true);
	}

	if (m_iGoreLeftLeg < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberLeftLeg(false);
		else if (iGore == 3)
			DismemberLeftLeg(true);
	}

	if (m_iGoreRightLeg < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberRightLeg(false);
		else if (iGore == 3)
			DismemberRightLeg(true);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_FRRagdoll_NPC : public C_BaseAnimating
{
public:
	DECLARE_CLASS(C_FRRagdoll_NPC, C_BaseAnimating);
	DECLARE_CLIENTCLASS();
	DECLARE_INTERPOLATION();

	C_FRRagdoll_NPC(void);

	virtual void PostDataUpdate(DataUpdateType_t updateType);

	virtual int InternalDrawModel(int flags);
	virtual CStudioHdr* OnNewModel(void);
	virtual unsigned char GetClientSideFade();
	virtual void	SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);

	void GetRenderBounds(Vector& theMins, Vector& theMaxs);
	virtual void AddEntity(void);
	virtual void OnDataChanged(DataUpdateType_t type);
	virtual void AccumulateLayers(IBoneSetup& boneSetup, Vector pos[], Quaternion q[], float currentTime);
	virtual void BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed);
	
	void ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName);

	// GORE
	void ScaleGoreBones(void);
	void InitDismember(void);

	void DismemberHead();
	void DismemberBase(char const* szBodyPart, bool bLevel, bool bBloodEffects, char const* szParticleBone);
	void DismemberLeftArm(bool bLevel);
	void DismemberRightArm(bool bLevel);
	void DismemberLeftLeg(bool bLevel);
	void DismemberRightLeg(bool bLevel);

	void DismemberRandomLimbs(void);
	
	IPhysicsObject* GetElement(int elementNum);
	virtual void UpdateOnRemove();
	virtual float LastBoneChangedTime();

	// Incoming from network
	Vector		m_ragPos[RAGDOLL_MAX_ELEMENTS];
	QAngle		m_ragAngles[RAGDOLL_MAX_ELEMENTS];

	CInterpolatedVarArray< Vector, RAGDOLL_MAX_ELEMENTS >	m_iv_ragPos;
	CInterpolatedVarArray< QAngle, RAGDOLL_MAX_ELEMENTS >	m_iv_ragAngles;

	int			m_elementCount;
	int			m_boneIndex[RAGDOLL_MAX_ELEMENTS];

	// gore stuff
	int m_iGoreHead;
	int m_iGoreLeftArm;
	int m_iGoreRightArm;
	int m_iGoreLeftLeg;
	int m_iGoreRightLeg;

	// checks if this model can utilise gore
	bool m_bGoreEnabled;

	// how many blood decals to spray out when we dismember a limb overtime
	int m_iGoreDecalAmount;
	// the index of the bone we should spray blood decals out from
	int m_iGoreDecalBone;
	// time when blood decal was sprayed so that blood decals sprays are delayed in bursts for ClientThink
	float m_fGoreDecalTime;

private:
	C_FRRagdoll_NPC(const C_FRRagdoll_NPC& src);

	typedef CHandle<C_BaseAnimating> CBaseAnimatingHandle;
	CNetworkVar(CBaseAnimatingHandle, m_hUnragdoll);
	CNetworkVar(float, m_flBlendWeight);
	float m_flBlendWeightCurrent;
	CNetworkVar(int, m_nOverlaySequence);
	float m_flLastBoneChangeTime;
};


EXTERN_RECV_TABLE(DT_RagdollFR);
IMPLEMENT_CLIENTCLASS_DT(C_FRRagdoll_NPC, DT_RagdollFR, CFRRagdoll_NPC)
RecvPropInt(RECVINFO(m_iGoreHead)),
RecvPropInt(RECVINFO(m_iGoreLeftArm)),
RecvPropInt(RECVINFO(m_iGoreRightArm)),
RecvPropInt(RECVINFO(m_iGoreLeftLeg)),
RecvPropInt(RECVINFO(m_iGoreRightLeg))
END_RECV_TABLE()

C_FRRagdoll_NPC::C_FRRagdoll_NPC(void) :
	m_iv_ragPos("C_FRRagdoll_NPC::m_iv_ragPos"),
	m_iv_ragAngles("C_FRRagdoll_NPC::m_iv_ragAngles")
{
	m_elementCount = 0;
	m_flLastBoneChangeTime = -FLT_MAX;

	AddVar(m_ragPos, &m_iv_ragPos, LATCH_SIMULATION_VAR);
	AddVar(m_ragAngles, &m_iv_ragAngles, LATCH_SIMULATION_VAR);

	m_flBlendWeight = 0.0f;
	m_flBlendWeightCurrent = 0.0f;
	m_nOverlaySequence = -1;
	m_flFadeScale = 1;

	m_iGoreHead = 0;
	m_iGoreLeftArm = 0;
	m_iGoreRightArm = 0;
	m_iGoreLeftLeg = 0;
	m_iGoreRightLeg = 0;
}

void C_FRRagdoll_NPC::PostDataUpdate(DataUpdateType_t updateType)
{
	BaseClass::PostDataUpdate(updateType);

	m_iv_ragPos.NoteChanged(gpGlobals->curtime, true);
	m_iv_ragAngles.NoteChanged(gpGlobals->curtime, true);
	// this is the local client time at which this update becomes stale
	m_flLastBoneChangeTime = gpGlobals->curtime + GetInterpolationAmount(m_iv_ragPos.GetType());
}

float C_FRRagdoll_NPC::LastBoneChangedTime()
{
	return m_flLastBoneChangeTime;
}

int C_FRRagdoll_NPC::InternalDrawModel(int flags)
{
	int ret = BaseClass::InternalDrawModel(flags);
	if (vcollide_wireframe.GetBool())
	{
		vcollide_t* pCollide = modelinfo->GetVCollide(GetModelIndex());
		IMaterial* pWireframe = materials->FindMaterial("shadertest/wireframevertexcolor", TEXTURE_GROUP_OTHER);

		matrix3x4_t matrix;
		for (int i = 0; i < m_elementCount; i++)
		{
			static color32 debugColor = { 0,255,255,0 };

			AngleMatrix(m_ragAngles[i], m_ragPos[i], matrix);
			engine->DebugDrawPhysCollide(pCollide->solids[i], pWireframe, matrix, debugColor);
		}
	}
	return ret;
}


CStudioHdr* C_FRRagdoll_NPC::OnNewModel(void)
{
	CStudioHdr* hdr = BaseClass::OnNewModel();

	if (!m_elementCount)
	{
		vcollide_t* pCollide = modelinfo->GetVCollide(GetModelIndex());
		if (!pCollide)
		{
			const char* pszName = modelinfo->GetModelName(modelinfo->GetModel(GetModelIndex()));
			Msg("*** ERROR: C_FRRagdoll_NPC::InitModel: %s missing vcollide data ***\n", (pszName) ? pszName : "<null>");
			m_elementCount = 0;
		}
		else
		{
			m_elementCount = RagdollExtractBoneIndices(m_boneIndex, hdr, pCollide);
		}
		m_iv_ragPos.SetMaxCount(m_elementCount);
		m_iv_ragAngles.SetMaxCount(m_elementCount);
	}

	return hdr;
}

//-----------------------------------------------------------------------------
// Purpose: clear out any face/eye values stored in the material system
//-----------------------------------------------------------------------------
void C_FRRagdoll_NPC::SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights)
{
	BaseClass::SetupWeights(pBoneToWorld, nFlexWeightCount, pFlexWeights, pFlexDelayedWeights);

	CStudioHdr* hdr = GetModelPtr();
	if (!hdr)
		return;

	int nFlexDescCount = hdr->numflexdesc();
	if (nFlexDescCount)
	{
		Assert(!pFlexDelayedWeights);
		memset(pFlexWeights, 0, nFlexWeightCount * sizeof(float));
	}

	if (m_iEyeAttachment > 0)
	{
		matrix3x4_t attToWorld;
		if (GetAttachment(m_iEyeAttachment, attToWorld))
		{
			Vector local, tmp;
			local.Init(1000.0f, 0.0f, 0.0f);
			VectorTransform(local, attToWorld, tmp);
			modelrender->SetViewTarget(GetModelPtr(), GetBody(), tmp);
		}
	}
}


void C_FRRagdoll_NPC::GetRenderBounds(Vector& theMins, Vector& theMaxs)
{
	if (!CollisionProp()->IsBoundsDefinedInEntitySpace())
	{
		IRotateAABB(EntityToWorldTransform(), CollisionProp()->OBBMins(), CollisionProp()->OBBMaxs(), theMins, theMaxs);
	}
	else
	{
		theMins = CollisionProp()->OBBMins();
		theMaxs = CollisionProp()->OBBMaxs();
	}
}

void C_FRRagdoll_NPC::AddEntity(void)
{
	BaseClass::AddEntity();

	// Move blend weight toward target over 0.2 seconds
	m_flBlendWeightCurrent = Approach(m_flBlendWeight, m_flBlendWeightCurrent, gpGlobals->frametime * 5.0f);
}

void C_FRRagdoll_NPC::OnDataChanged(DataUpdateType_t type)
{
	BaseClass::OnDataChanged(type);

	if (type == DATA_UPDATE_CREATED)
	{
		int iBone = LookupBone("ValveBiped.Bip01_Neck1");

		if (iBone != -1)
		{
			m_bGoreEnabled = g_fr_headshotgore.GetBool();
		}
		else
		{
			m_bGoreEnabled = false;
		}

		if (m_bGoreEnabled)
			m_BoneAccessor.SetWritableBones(BONE_USED_BY_ANYTHING);

		SetNextClientThink(gpGlobals->curtime + 0.1f);
	}
}

void C_FRRagdoll_NPC::ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName)
{
	IPhysicsObject* pPhysicsObject = VPhysicsGetObject();

	if (!pPhysicsObject)
		return;

	Vector dir = pTrace->endpos - pTrace->startpos;

	// m_iGore<limb> has a level, from 0 to 3
	// 1 is unused (reserved for normal TF bodygroups like pyro's head)
	// 2 means the lower limb is marked for removal, 3 means the upper limb is marked for removal, the head is an exception as it only has level 2
	// if our current level is at level 3, that means we can't dismember this limb anymore
	// if our current level is at level 2, that means we can dismember this limb once more up to level 3
	// if our current level is at level 0/1, that means we can dismember this limb up to level 2
	// Dismember<limb> function accepts true or false, true means this limb will be dismembered to level 3, false means dismembered to level 2

	if (m_bGoreEnabled)
	{
		switch (pTrace->hitgroup)
		{
		case HITGROUP_HEAD:
			if (m_iGoreHead == 3)
			{
				break;
			}
			else if (m_iGoreHead == 2)
			{
				break;
			}
			else
			{
				DismemberHead();
				break;
			}
		case HITGROUP_LEFTARM:
			if (m_iGoreLeftArm == 3)
			{
				break;
			}
			else if (m_iGoreLeftArm == 2)
			{
				DismemberLeftArm(true);
				break;
			}
			else
			{
				DismemberLeftArm(false);
				break;
			}
		case HITGROUP_RIGHTARM:
			if (m_iGoreRightArm == 3)
			{
				break;
			}
			else if (m_iGoreRightArm == 2)
			{
				DismemberRightArm(true);
				break;
			}
			else
			{
				DismemberRightArm(false);
				break;
			}
		case HITGROUP_LEFTLEG:
			if (m_iGoreLeftLeg == 3)
			{
				break;
			}
			else if (m_iGoreLeftLeg == 2)
			{
				DismemberLeftLeg(true);
				break;
			}
			else
			{
				DismemberLeftLeg(false);
				break;
			}
		case HITGROUP_RIGHTLEG:
			if (m_iGoreRightLeg == 3)
			{
				break;
			}
			else if (m_iGoreRightLeg == 2)
			{
				DismemberRightLeg(true);
				break;
			}
			else
			{
				DismemberRightLeg(false);
				break;
			}
		default:
			break;
		}
	}

	if (iDamageType == DMG_BLAST)
	{
		dir *= 4000;  // adjust impact strenght

		// apply force at object mass center
		pPhysicsObject->ApplyForceCenter(dir);

		if (m_bGoreEnabled)
			DismemberRandomLimbs();
	}
	else
	{
		Vector hitpos;

		VectorMA(pTrace->startpos, pTrace->fraction, dir, hitpos);
		VectorNormalize(dir);

		dir *= 4000;  // adjust impact strenght

		// apply force where we hit it
		pPhysicsObject->ApplyForceOffset(dir, hitpos);

		// Blood spray!
		//		FX_CS_BloodSpray( hitpos, dir, 10 );
	}

	m_pRagdoll->ResetRagdollSleepAfterTime();
}

void C_FRRagdoll_NPC::AccumulateLayers(IBoneSetup& boneSetup, Vector pos[], Quaternion q[], float currentTime)
{
	BaseClass::AccumulateLayers(boneSetup, pos, q, currentTime);

	if (m_nOverlaySequence >= 0 && m_nOverlaySequence < boneSetup.GetStudioHdr()->GetNumSeq())
	{
		boneSetup.AccumulatePose(pos, q, m_nOverlaySequence, GetCycle(), m_flBlendWeightCurrent, currentTime, m_pIk);
	}
}

void C_FRRagdoll_NPC::BuildTransformations(CStudioHdr* hdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed)
{
	if (!hdr)
		return;
	matrix3x4_t bonematrix;
	bool boneSimulated[MAXSTUDIOBONES];

	// no bones have been simulated
	memset(boneSimulated, 0, sizeof(boneSimulated));
	mstudiobone_t* pbones = hdr->pBone(0);

	mstudioseqdesc_t* pSeqDesc = NULL;
	if (m_nOverlaySequence >= 0 && m_nOverlaySequence < hdr->GetNumSeq())
	{
		pSeqDesc = &hdr->pSeqdesc(m_nOverlaySequence);
	}

	int i;
	for (i = 0; i < m_elementCount; i++)
	{
		int index = m_boneIndex[i];
		if (index >= 0)
		{
			if (hdr->boneFlags(index) & boneMask)
			{
				boneSimulated[index] = true;
				matrix3x4_t& matrix = GetBoneForWrite(index);

				if (m_flBlendWeightCurrent != 0.0f && pSeqDesc &&
					// FIXME: this bone access is illegal
					pSeqDesc->weight(index) != 0.0f)
				{
					// Use the animated bone position instead
					boneSimulated[index] = false;
				}
				else
				{
					AngleMatrix(m_ragAngles[i], m_ragPos[i], matrix);
				}

				//TEST - WE NOW KNOW THAT THIS CLIENTSIDE CLASS WORKS!
				//MatrixScaleByZero(matrix);
			}
		}
	}

	for (i = 0; i < hdr->numbones(); i++)
	{
		if (!(hdr->boneFlags(i) & boneMask))
			continue;

		// BUGBUG: Merge this code with the code in c_baseanimating somehow!!!
		// animate all non-simulated bones
		if (boneSimulated[i] ||
			CalcProceduralBone(hdr, i, m_BoneAccessor))
		{
			continue;
		}
		else
		{
			QuaternionMatrix(q[i], pos[i], bonematrix);

			if (pbones[i].parent == -1)
			{
				ConcatTransforms(cameraTransform, bonematrix, GetBoneForWrite(i));
			}
			else
			{
				ConcatTransforms(GetBone(pbones[i].parent), bonematrix, GetBoneForWrite(i));
			}
		}

		if (pbones[i].parent == -1)
		{
			// Apply client-side effects to the transformation matrix
		//	ApplyBoneMatrixTransform( GetBoneForWrite( i ) );
		}
	}

	if (m_bGoreEnabled)
		ScaleGoreBones();
}

void C_FRRagdoll_NPC::ScaleGoreBones()
{
	if (m_iGoreHead > 1)
		ScaleGoreHead(this);

	if (m_iGoreLeftArm == 2)
		ScaleGoreLeftHand(this);
	else if (m_iGoreLeftArm == 3)
		ScaleGoreLeftArm(this);

	if (m_iGoreRightArm == 2)
		ScaleGoreRightHand(this);
	else if (m_iGoreRightArm == 3)
		ScaleGoreRightArm(this);

	if (m_iGoreLeftLeg == 2)
		ScaleGoreLeftFoot(this);
	else if (m_iGoreLeftLeg == 3)
		ScaleGoreLeftKnee(this);

	if (m_iGoreRightLeg == 2)
		ScaleGoreRightFoot(this);
	else if (m_iGoreRightLeg == 3)
		ScaleGoreRightKnee(this);
}

void C_FRRagdoll_NPC::DismemberHead()
{
	m_iGoreHead = 3;

	int iAttach = LookupBone("bip_neck");

	if (iAttach != -1)
	{
		ParticleProp()->Create("smod_blood_decap_r", PATTACH_BONE_FOLLOW, "ValveBiped.Bip01_Neck1");

		EmitSound("Gore.Headshot");

		m_iGoreDecalAmount += 15;
		m_iGoreDecalBone = iAttach;

	}
}

void C_FRRagdoll_NPC::DismemberBase(char const* szBodyPart, bool bLevel, bool bBloodEffects, char const* szParticleBone)
{
	int iAttach = LookupBone(szParticleBone);

	if (iAttach != -1)
	{
		// I'm too lazy to make a new particle which is less bloody than blood_decap, but whatever, this works
		ParticleProp()->Create("smod_blood_gib_r", PATTACH_BONE_FOLLOW, szParticleBone);
	}

	if (iAttach != -1)
	{
		m_iGoreDecalAmount += 4;
		m_iGoreDecalBone = iAttach;

		EmitSound("Gore.Headshot");
	}
}

void C_FRRagdoll_NPC::DismemberLeftArm(bool bLevel)
{
	DismemberBase("leftarm", bLevel, true, bLevel ? "ValveBiped.Bip01_L_UpperArm" : "ValveBiped.Bip01_L_Forearm");

	if (bLevel)
		m_iGoreLeftArm = 3;
	else
		m_iGoreLeftArm = 2;
}

void C_FRRagdoll_NPC::DismemberRightArm(bool bLevel)
{
	DismemberBase("rightarm", bLevel, true, bLevel ? "ValveBiped.Bip01_R_UpperArm" : "ValveBiped.Bip01_R_Forearm");

	if (bLevel)
		m_iGoreRightArm = 3;
	else
		m_iGoreRightArm = 2;
}

void C_FRRagdoll_NPC::DismemberLeftLeg(bool bLevel)
{
	DismemberBase("leftleg", bLevel, true, bLevel ? "ValveBiped.Bip01_L_Knee" : "ValveBiped.Bip01_L_Foot");

	if (bLevel)
		m_iGoreLeftLeg = 3;
	else
		m_iGoreLeftLeg = 2;
}

void C_FRRagdoll_NPC::DismemberRightLeg(bool bLevel)
{
	DismemberBase("rightleg", bLevel, true, bLevel ? "ValveBiped.Bip01_R_Knee" : "ValveBiped.Bip01_R_Foot");

	if (bLevel)
		m_iGoreRightLeg = 3;
	else
		m_iGoreRightLeg = 2;
}

void C_FRRagdoll_NPC::InitDismember()
{
	// head does not have two levels of dismemberment, only one
	if (m_iGoreHead > 1)
		DismemberHead();

	if (m_iGoreLeftArm == 3)
		DismemberLeftArm(true);
	else if (m_iGoreLeftArm == 2)
		DismemberLeftArm(false);

	if (m_iGoreRightArm == 3)
		DismemberRightArm(true);
	else if (m_iGoreRightArm == 2)
		DismemberRightArm(false);

	if (m_iGoreLeftLeg == 3)
		DismemberLeftLeg(true);
	else if (m_iGoreLeftLeg == 2)
		DismemberLeftLeg(false);

	if (m_iGoreRightLeg == 3)
		DismemberRightLeg(true);
	else if (m_iGoreRightLeg == 2)
		DismemberRightLeg(false);
}

void C_FRRagdoll_NPC::DismemberRandomLimbs(void)
{
	int iGore = 0;

	// NOTE: head is not dismembered here intentionally

	if (m_iGoreLeftArm < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberLeftArm(false);
		else if (iGore == 3)
			DismemberLeftArm(true);
	}

	if (m_iGoreRightArm < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberRightArm(false);
		else if (iGore == 3)
			DismemberRightArm(true);
	}

	if (m_iGoreLeftLeg < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberLeftLeg(false);
		else if (iGore == 3)
			DismemberLeftLeg(true);
	}

	if (m_iGoreRightLeg < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 2)
			DismemberRightLeg(false);
		else if (iGore == 3)
			DismemberRightLeg(true);
	}
}

IPhysicsObject* C_FRRagdoll_NPC::GetElement(int elementNum)
{
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : 	virtual void
//-----------------------------------------------------------------------------
void C_FRRagdoll_NPC::UpdateOnRemove()
{
	C_BaseAnimating* anim = m_hUnragdoll.Get();
	if (NULL != anim &&
		anim->GetModel() &&
		(anim->GetModel() == GetModel()))
	{
		// Need to tell C_BaseAnimating to blend out of the ragdoll data that we received last
		C_BaseAnimating::AutoAllowBoneAccess boneaccess(true, false);
		anim->CreateUnragdollInfo(this);
	}

	// Do last to mimic destrictor order
	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
// Fade out
//-----------------------------------------------------------------------------
unsigned char C_FRRagdoll_NPC::GetClientSideFade()
{
	return UTIL_ComputeEntityFade(this, m_fadeMinDist, m_fadeMaxDist, m_flFadeScale);
}

static int GetHighestBit(int flags)
{
	for (int i = 31; i >= 0; --i)
	{
		if (flags & (1 << i))
			return (1 << i);
	}

	return 0;
}

#else

// -------------------------------------------------------------------------------- //
// Ragdoll entities.
// -------------------------------------------------------------------------------- //

LINK_ENTITY_TO_CLASS(fr_ragdoll_player, CFRRagdoll_Player);

IMPLEMENT_SERVERCLASS_ST_NOBASE(CFRRagdoll_Player, DT_FRRagdoll_Player)
SendPropVector(SENDINFO(m_vecRagdollOrigin), -1, SPROP_COORD),
SendPropEHandle(SENDINFO(m_hEntity)),
SendPropModelIndex(SENDINFO(m_nModelIndex)),
SendPropInt(SENDINFO(m_nForceBone), 8, 0),
SendPropVector(SENDINFO(m_vecForce), -1, SPROP_NOSCALE),
SendPropVector(SENDINFO(m_vecRagdollVelocity)),
SendPropInt(SENDINFO(m_iGoreHead), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreLeftArm), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreRightArm), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreLeftLeg), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreRightLeg), 2, SPROP_UNSIGNED),
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS(physics_prop_fr_ragdoll, CFRRagdoll_NPC);
LINK_ENTITY_TO_CLASS(prop_fr_ragdoll, CFRRagdoll_NPC);

EXTERN_SEND_TABLE(DT_RagdollFR)

IMPLEMENT_SERVERCLASS_ST(CFRRagdoll_NPC, DT_RagdollFR)
SendPropInt(SENDINFO(m_iGoreHead), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreLeftArm), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreRightArm), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreLeftLeg), 2, SPROP_UNSIGNED),
SendPropInt(SENDINFO(m_iGoreRightLeg), 2, SPROP_UNSIGNED),
END_SEND_TABLE()

CFRRagdoll_NPC::CFRRagdoll_NPC(void)
{
}

CFRRagdoll_NPC::~CFRRagdoll_NPC(void)
{
}

#endif // FR_CLIENT


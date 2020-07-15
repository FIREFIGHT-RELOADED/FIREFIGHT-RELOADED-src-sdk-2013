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
//FRRagdoll_Player

IMPLEMENT_CLIENTCLASS_DT_NOBASE(C_FRRagdoll_Player, DT_FRRagdoll_Player, CFRRagdoll_Player)
RecvPropVector(RECVINFO(m_vecRagdollOrigin)),
RecvPropEHandle(RECVINFO(m_hEntity)),
RecvPropInt(RECVINFO(m_nModelIndex)),
RecvPropInt(RECVINFO(m_nForceBone)),
RecvPropVector(RECVINFO(m_vecForce)),
RecvPropVector(RECVINFO(m_vecRagdollVelocity))
END_RECV_TABLE()

C_FRRagdoll_Player::C_FRRagdoll_Player()
{

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

	if (iDamageType == DMG_BLAST)
	{
		dir *= 4000;  // adjust impact strenght

		// apply force at object mass center
		pPhysicsObject->ApplyForceCenter(dir);
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

//TODO: make it use c_serverragdoll rather than copy pasting the entire class

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
	virtual void AccumulateLayers(IBoneSetup& boneSetup, Vector pos[], Quaternion q[], float currentTime);
	virtual void BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed);
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
SendPropVector(SENDINFO(m_vecRagdollVelocity))
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS(physics_prop_fr_ragdoll, CFRRagdoll_NPC);
LINK_ENTITY_TO_CLASS(prop_fr_ragdoll, CFRRagdoll_NPC);

EXTERN_SEND_TABLE(DT_RagdollFR)

IMPLEMENT_SERVERCLASS_ST(CFRRagdoll_NPC, DT_RagdollFR)
END_SEND_TABLE()

CFRRagdoll_NPC::CFRRagdoll_NPC(void)
{
}

CFRRagdoll_NPC::~CFRRagdoll_NPC(void)
{
}

void CFRRagdoll_NPC::Spawn(void)
{
	BaseClass::Precache();
	Msg("CFRRagdoll_NPC: I TALK!\n");
	BaseClass::Spawn();
}

void CFRRagdoll_NPC::TraceAttack(const CTakeDamageInfo& info, const Vector& dir, trace_t* ptr, CDmgAccumulator* pAccumulator)
{
	Msg("CFRRagdoll_NPC: OW.\n");
	BaseClass::TraceAttack(info, dir, ptr, pAccumulator);
}

#endif // FR_CLIENT


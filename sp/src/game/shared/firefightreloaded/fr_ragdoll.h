#ifdef CLIENT_DLL
#else
#include "basecombatcharacter.h"
#include "BaseAnimatingOverlay.h"
#include "physics_prop_ragdoll.h"
#endif

#ifdef CLIENT_DLL

class C_FRRagdoll_Player : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS(C_FRRagdoll_Player, C_BaseAnimatingOverlay);
	DECLARE_CLIENTCLASS();

	C_FRRagdoll_Player();
	~C_FRRagdoll_Player();

	virtual void OnDataChanged(DataUpdateType_t type);

	int GetPlayerEntIndex() const;
	IRagdoll* GetIRagdoll() const;

	void ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName);
	void UpdateOnRemove(void);
	virtual void SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);

	// c_baseanimating functions
	virtual void BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed);

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

	C_FRRagdoll_Player(const C_FRRagdoll_Player&) {}

	void Interp_Copy(C_BaseAnimatingOverlay* pDestinationEntity);
	void CreateFRRagdoll_Player(void);

private:

	EHANDLE	m_hEntity;
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);
};
#else

class CFRRagdoll_Player : public CBaseAnimatingOverlay
{
public:
	DECLARE_CLASS(CFRRagdoll_Player, CBaseAnimatingOverlay);
	DECLARE_SERVERCLASS();

	// Transmit ragdolls to everyone.
	virtual int UpdateTransmitState()
	{
		return SetTransmitState(FL_EDICT_ALWAYS);
	}

public:
	// In case the client has the player entity, we transmit the player index.
	// In case the client doesn't have it, we transmit the player's model index, origin, and angles
	// so they can create a ragdoll in the right place.
	CNetworkHandle(CBaseEntity, m_hEntity);	// networked entity handle 
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);

	CNetworkVar(unsigned short, m_iGoreHead);
	CNetworkVar(unsigned short, m_iGoreLeftArm);
	CNetworkVar(unsigned short, m_iGoreRightArm);
	CNetworkVar(unsigned short, m_iGoreLeftLeg);
	CNetworkVar(unsigned short, m_iGoreRightLeg);
};

class CFRRagdoll_NPC : public CRagdollProp
{
	DECLARE_CLASS(CFRRagdoll_NPC, CRagdollProp);
	DECLARE_SERVERCLASS();

public:
	CFRRagdoll_NPC(void);
	~CFRRagdoll_NPC(void);

	CNetworkVar(unsigned short, m_iGoreHead);
	CNetworkVar(unsigned short, m_iGoreLeftArm);
	CNetworkVar(unsigned short, m_iGoreRightArm);
	CNetworkVar(unsigned short, m_iGoreLeftLeg);
	CNetworkVar(unsigned short, m_iGoreRightLeg);
};
#endif // FR_CLIENT
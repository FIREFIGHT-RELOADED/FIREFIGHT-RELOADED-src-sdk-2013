#ifdef CLIENT_DLL
#else
#include "basecombatcharacter.h"
#include "BaseAnimatingOverlay.h"
#include "physics_prop_ragdoll.h"
#endif

#ifdef CLIENT_DLL

class C_FRRagdoll : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS(C_FRRagdoll, C_BaseAnimatingOverlay);
	DECLARE_CLIENTCLASS();

	C_FRRagdoll();
	~C_FRRagdoll();

	virtual void OnDataChanged(DataUpdateType_t type);

	IRagdoll* GetIRagdoll() const;

	void ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName);
	void UpdateOnRemove(void);
	virtual void SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);

	// c_baseanimating functions
	virtual void BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed);

private:

	C_FRRagdoll(const C_FRRagdoll&) {}

	void Interp_Copy(C_BaseAnimatingOverlay* pDestinationEntity);
	void CreateFRRagdoll(void);

private:

	EHANDLE	m_hEntity;
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);
};
#else

class CFRRagdoll : public CBaseAnimatingOverlay
{
public:
	DECLARE_CLASS(CFRRagdoll, CBaseAnimatingOverlay);
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
};
#endif // FR_CLIENT
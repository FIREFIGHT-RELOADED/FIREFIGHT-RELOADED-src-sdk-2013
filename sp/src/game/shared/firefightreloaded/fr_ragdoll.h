#ifdef CLIENT_DLL
#else
#include "basecombatcharacter.h"
#include "BaseAnimatingOverlay.h"
#include "physics_prop_ragdoll.h"
#endif

#ifdef CLIENT_DLL

class C_FRRagdoll_Base : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS(C_FRRagdoll_Base, C_BaseAnimatingOverlay);
	DECLARE_CLIENTCLASS();

	C_FRRagdoll_Base();
	~C_FRRagdoll_Base();

	virtual void OnDataChanged(DataUpdateType_t type);

	int GetPlayerEntIndex() const;
	IRagdoll* GetIRagdoll() const;

	void ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName);
	void UpdateOnRemove(void);
	virtual void SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);

	virtual void CreateFRRagdoll(void);

	void Interp_Copy(C_BaseAnimatingOverlay* pDestinationEntity);

private:

	C_FRRagdoll_Base(const C_FRRagdoll_Base&) {}

public:

	EHANDLE	m_hEntity;
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);
};

class C_FRRagdoll_Player : public C_FRRagdoll_Base
{
public:
	DECLARE_CLASS(C_FRRagdoll_Player, C_FRRagdoll_Base);
	DECLARE_CLIENTCLASS();

	C_FRRagdoll_Player();
	~C_FRRagdoll_Player();

	virtual void CreateFRRagdoll(void);

private:

	C_FRRagdoll_Player(const C_FRRagdoll_Player&) {}
};

#else

class CFRRagdoll_Base : public CBaseAnimatingOverlay
{
public:
	DECLARE_CLASS(CFRRagdoll_Base, CBaseAnimatingOverlay);
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

class CFRRagdoll_Player : public CFRRagdoll_Base
{
public:
	DECLARE_CLASS(CFRRagdoll_Player, CFRRagdoll_Base);
	DECLARE_SERVERCLASS();
};

void CreateFRRagdollEntity(CBaseAnimating* ent);
#endif // FR_CLIENT
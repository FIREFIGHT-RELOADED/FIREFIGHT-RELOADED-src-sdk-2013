#ifndef FRRENDERTARGETS_H_
#define FRRENDERTARGETS_H_
#ifdef _WIN32
#pragma once
#endif

#include "baseclientrendertargets.h" // Base class, with interfaces called by engine and inherited members to init common render   targets

// externs
class IMaterialSystem;
class IMaterialSystemHardwareConfig;

class CFRRenderTargets : public CBaseClientRenderTargets
{
	// no networked vars 
	DECLARE_CLASS_GAMEROOT(CFRRenderTargets, CBaseClientRenderTargets);
public:
	virtual void InitClientRenderTargets(IMaterialSystem* pMaterialSystem, IMaterialSystemHardwareConfig* pHardwareConfig);
	virtual void ShutdownClientRenderTargets();

	ITexture* CreateScopeTexture(IMaterialSystem* pMaterialSystem);

private:
	CTextureReference		m_ScopeTexture;
};

extern CFRRenderTargets* FRRenderTargets;

#endif //TNERENDERTARGETS_H_
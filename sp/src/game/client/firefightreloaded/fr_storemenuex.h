//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef FR_STOREMENU_H
#define FR_STOREMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>

#include <game/client/iviewport.h>

#include <vgui/KeyCode.h>
#include <utlvector.h>
#include <gameui/PanelListPanel.h>


//-----------------------------------------------------------------------------
// Purpose: Displays the team menu
//-----------------------------------------------------------------------------
class CFRStoreMenuEX : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CFRStoreMenuEX, vgui::Frame );

public:
	CFRStoreMenuEX(IViewPort *pViewPort);
	virtual ~CFRStoreMenuEX();

	virtual const char *GetName( void ) { return PANEL_BUY; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset() {};
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	void OnCommand(const char *command);

	void MoveToCenterOfScreen();

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }
	
protected:
	
	// VGUI2 overrides
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void OnKeyCodePressed(vgui::KeyCode code);

	// helper functions
	virtual void SetLabelText(const char *textEntryName, const char *text);

	IViewPort	*m_pViewPort;
	ButtonCode_t m_iJumpKey;
	ButtonCode_t m_iScoreBoardKey;

	CPanelListPanel* m_pItemList;
};

#endif // TEAMMENU_H

#include "cbase.h"
#include "LoadPanel.h"

// Constuctor: Initializes the Panel
CLoadPanel::CLoadPanel(vgui::VPANEL parent) : BaseClass(NULL, "LoadPanel")
{
	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(false);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(false);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	//Msg("[SRC] LoadPanel has been constructed\n");
}

void CLoadPanel::Activate()
{
	if (!m_hSingleplayerAdvancedDialog.Get())
	{
		m_hSingleplayerAdvancedDialog = new CSingleplayerAdvancedDialog(this);
	}
	m_hSingleplayerAdvancedDialog->Activate();
	this->DeletePanel();
	//Msg("[SRC] LoadPanel has been destroyed\n");
}
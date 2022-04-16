#include "cbase.h"
#include "fr_mainmenupanel.h"
#include "fr_mainmenu.h"
#include "gameui/SingleplayerAdvancedDialog.h"
#include "gameui/NewSingleplayerGameDialog.h"
#include "gameui/PlayerModelDialog.h"
#include "tier3/tier3.h"
#include "vgui/ILocalize.h"
#include "fmtstr.h"

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY_DEFAULT_TEXT(CHintLabel, CHintLabel);

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHintLabel::CHintLabel(Panel* parent, const char* name, const char* wszText) : Label(parent, name, wszText)
{
	ConstructorShared();
}

CHintLabel::CHintLabel(Panel* parent, const char* name, const wchar_t* wszText) : Label(parent, name, wszText)
{
	ConstructorShared();
}

void CHintLabel::ConstructorShared()
{
	m_szColor[0] = '\0';
	m_iTipCountAll = 0;
	m_iSplashCountAll = 0;
	m_iCurrentTip = 0;
	m_bInited = false;
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
void CHintLabel::InitTips()
{
	if (!m_bInited)
	{
		// count how many tips there are
		m_iTipCountAll = 0;
		wchar_t* wzTipCount = g_pVGuiLocalize->Find(CFmtStr("#FR_Tip_Count"));
		int iTipCount = wzTipCount ? _wtoi(wzTipCount) : 0;
		m_iTipCountAll += iTipCount;

		// count how many splashes there are
		m_iSplashCountAll = 0;
		wchar_t* wzSplashCount = g_pVGuiLocalize->Find(CFmtStr("#FR_Splash_Count"));
		int iSplashCount = wzSplashCount ? _wtoi(wzSplashCount) : 0;
		m_iSplashCountAll += iSplashCount;

		m_bInited = true;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Returns a random tip
//-----------------------------------------------------------------------------
void CHintLabel::GetRandomTip()
{
	InitTips();

	//we choose if we want splashes or tips.
	int randChoice = RandomInt(0, 1);
	int iTip = 0;

	switch (randChoice)
	{
	case 1:
		// pick a random spalsh
		iTip = RandomInt(1, m_iSplashCountAll);
		SetText(GetLocalizedString(iTip, CFmtStr("#FR_Splash_%i", iTip)));
		break;
	case 0:
	default:
		// pick a random tip
		iTip = RandomInt(1, m_iTipCountAll);
		SetText(GetLocalizedString(iTip, CFmtStr("#FR_Tip_%i", iTip)));
		break;
	}

	const char* szType = (randChoice == 1 ? "Splash" : "Tip");

	DevMsg("Grabbed %s: %i\n", szType, iTip);
}

//-----------------------------------------------------------------------------
// Purpose: Returns specified tip index from string
//-----------------------------------------------------------------------------
const wchar_t* CHintLabel::GetLocalizedString(int iTip, const char* token)
{
	static wchar_t wzTip[2048] = L"";

	// get the tip
	const wchar_t* wzFmt = g_pVGuiLocalize->Find(token);
	// replace any commands with their bound keys
	UTIL_ReplaceKeyBindings(wzFmt, 0, wzTip, sizeof(wzTip));

	return wzTip;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHintLabel::ApplySettings(KeyValues* inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	Q_strncpy(m_szColor, inResourceData->GetString("fgcolor", "Label.TextColor"), sizeof(m_szColor));

	InvalidateLayout(false, true); // force ApplySchemeSettings to run
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHintLabel::ApplySchemeSettings(IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	SetFgColor(pScheme->GetColor(m_szColor, Color(255, 255, 255, 255)));
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRMainMenuPanel::CFRMainMenuPanel(vgui::Panel* parent) : CFRMainMenuPanelBase(parent)
{
	SetParent(parent);
	SetScheme("ClientScheme");
	SetProportional(false);
	SetVisible(true);
	SetMainMenu(GetParent());

	int width, height;
	surface()->GetScreenSize(width, height);
	SetSize(width, height);
	SetPos(0, 0);
	LoadControlSettings("resource/UI/MainMenu.res");
	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	InGameLayout = false;
	m_flActionThink = -1;
	m_flAnimationThink = -1;
	m_bAnimationIn = true;
	m_pVersionLabel = dynamic_cast<CHintLabel *>(FindChildByName("VersionLabel"));
	m_pHintLabel = dynamic_cast<CHintLabel *>(FindChildByName("HintLabel"));
	m_pBackground = dynamic_cast<CFRImagePanel *>(FindChildByName("Background"));
	m_pDisconnectButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("DisconnectButton"));
	m_pResumeGameButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("ResumeGameButton"));
	m_pSaveGameButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("SaveGameButton"));
	m_pReloadMapButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("ReloadMapButton"));
	m_pLogo = dynamic_cast<CFRImagePanel *>(FindChildByName("Logo"));

	//Q_strncpy(m_pzVideoLink, GetRandomVideo(), sizeof(m_pzVideoLink));
	SetVersionLabel();

	DefaultLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFRMainMenuPanel::~CFRMainMenuPanel()
{

}

void CFRMainMenuPanel::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}

void CFRMainMenuPanel::PerformLayout()
{
	BaseClass::PerformLayout();
	SetHintLabel();
};

void CFRMainMenuPanel::OnCommand(const char* command)
{
	if (!Q_strcmp(command, "singleplayeroptions"))
	{
		CSingleplayerAdvancedDialog* pCSingleplayerAdvancedDialog = new CSingleplayerAdvancedDialog(this);
		pCSingleplayerAdvancedDialog->Activate();
	}
	else if (!Q_strcmp(command, "changesplash"))
	{
		SetHintLabel();
	}
	else if (!Q_strcmp(command, "createnewgame"))
	{
		CNewSingleplayerGameDialog* pCNewSingleplayerGameDialog = new CNewSingleplayerGameDialog(this);
		pCNewSingleplayerGameDialog->Activate();
	}
	else if (!Q_strcmp(command, "playermodeldialog"))
	{
		CPlayerModelDialog* pCPlayerModelDialog = new CPlayerModelDialog(this);
		pCPlayerModelDialog->Activate();
	}
	else
	{
		BaseClass::OnCommand(command);
	}
}


void CFRMainMenuPanel::OnTick()
{
	BaseClass::OnTick();
};

void CFRMainMenuPanel::OnThink()
{
	BaseClass::OnThink();

	if (!InGame() && InGameLayout)
	{
		DefaultLayout();
		InGameLayout = false;
	}
	else if (InGame() && !InGameLayout)
	{
		GameLayout();
		InGameLayout = true;
	}
};

void CFRMainMenuPanel::DefaultLayout()
{
	BaseClass::DefaultLayout();

	//we need to find better way to show/hide stuff
	if (m_pDisconnectButton)
	{
		if (m_pDisconnectButton->OnlyInGame())
		{
			m_pDisconnectButton->SetVisible(false);
		}
		else if (m_pDisconnectButton->OnlyAtMenu())
		{
			m_pDisconnectButton->SetVisible(true);
		}
	}
	if (m_pResumeGameButton)
	{
		if (m_pResumeGameButton->OnlyInGame())
		{
			m_pResumeGameButton->SetVisible(false);
		}
		else if (m_pResumeGameButton->OnlyAtMenu())
		{
			m_pResumeGameButton->SetVisible(true);
		}
	}
	if (m_pSaveGameButton)
	{
		if (m_pSaveGameButton->OnlyInGame())
		{
			m_pSaveGameButton->SetVisible(false);
		}
		else if (m_pSaveGameButton->OnlyAtMenu())
		{
			m_pSaveGameButton->SetVisible(true);
		}
	}
	if (m_pReloadMapButton)
	{
		if (m_pReloadMapButton->OnlyInGame())
		{
			m_pReloadMapButton->SetVisible(false);
		}
		else if (m_pReloadMapButton->OnlyAtMenu())
		{
			m_pReloadMapButton->SetVisible(true);
		}
	}
};

void CFRMainMenuPanel::GameLayout()
{
	BaseClass::GameLayout();

	if (m_pDisconnectButton)
	{
		if (m_pDisconnectButton->OnlyInGame())
		{
			m_pDisconnectButton->SetVisible(true);
		}
		else if (m_pDisconnectButton->OnlyAtMenu())
		{
			m_pDisconnectButton->SetVisible(false);
		}
	}
	if (m_pResumeGameButton)
	{
		if (m_pResumeGameButton->OnlyInGame())
		{
			m_pResumeGameButton->SetVisible(true);
		}
		else if (m_pResumeGameButton->OnlyAtMenu())
		{
			m_pResumeGameButton->SetVisible(false);
		}
	}
	if (m_pSaveGameButton)
	{
		if (m_pSaveGameButton->OnlyInGame())
		{
			m_pSaveGameButton->SetVisible(true);
		}
		else if (m_pSaveGameButton->OnlyAtMenu())
		{
			m_pSaveGameButton->SetVisible(false);
		}
	}
	if (m_pReloadMapButton)
	{
		if (m_pReloadMapButton->OnlyInGame())
		{
			m_pReloadMapButton->SetVisible(true);
		}
		else if (m_pReloadMapButton->OnlyAtMenu())
		{
			m_pReloadMapButton->SetVisible(false);
		}
	}
};

void CFRMainMenuPanel::SetVersionLabel()
{
	if (m_pVersionLabel)
	{
		char verString[30];
		if (g_pFullFileSystem->FileExists("version.txt"))
		{
			FileHandle_t fh = filesystem->Open("version.txt", "r", "MOD");
			int file_len = filesystem->Size(fh);
			char* GameInfo = new char[file_len + 1];

			filesystem->Read((void*)GameInfo, file_len, fh);
			GameInfo[file_len] = 0; // null terminator

			filesystem->Close(fh);

			Q_snprintf(verString, sizeof(verString), "%s", GameInfo + 8);

			delete[] GameInfo;
		}
		m_pVersionLabel->SetText(verString);
	}
};

void CFRMainMenuPanel::SetHintLabel()
{
	if (m_pHintLabel)
	{
		m_pHintLabel->GetRandomTip();
	}
};
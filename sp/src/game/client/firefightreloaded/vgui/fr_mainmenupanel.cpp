#include "cbase.h"
#include "fr_mainmenupanel.h"
#include "fr_mainmenu.h"
#include "fr_tips.h"
#include "gameui/SingleplayerAdvancedDialog.h"
#include "gameui/NewSingleplayerGameDialog.h"
#include "gameui/PlayerModelDialog.h"

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

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
	b_ShowVideo = false;
	m_flActionThink = -1;
	m_flAnimationThink = -1;
	m_bAnimationIn = true;
	m_pVersionLabel = dynamic_cast<CExLabel *>(FindChildByName("VersionLabel"));
	m_pHintLabel = dynamic_cast<CExLabel *>(FindChildByName("HintLabel"));
	m_pBackground = dynamic_cast<CFRImagePanel *>(FindChildByName("Background"));
	m_pDisconnectButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("DisconnectButton"));
	m_pResumeGameButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("ResumeGameButton"));
	m_pSaveGameButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("SaveGameButton"));
	m_pReloadMapButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("ReloadMapButton"));
	m_pVideo = dynamic_cast<CFRVideoPanel *>(FindChildByName("BackgroundVideo"));
	m_pLogo = dynamic_cast<CFRImagePanel *>(FindChildByName("Logo"));

	m_iVideoCount = CheckVideoCount();
	//GetRandomVideo(m_pzVideoLink, sizeof(m_pzVideoLink));

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

	if (m_pVideo && m_pVideo->IsVisible() && !InGameLayout && m_flActionThink < gpGlobals->curtime)
	{
		m_pVideo->Activate();
		m_pVideo->BeginPlayback(m_pzVideoLink);
		m_pVideo->MoveToFront();
		m_flActionThink = gpGlobals->curtime + m_pVideo->GetActiveVideoLength() - 0.21f;
		b_ShowVideo = false;
	}
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
	if (m_pVideo)
	{
		m_pVideo->SetVisible(true);
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
	if (m_pVideo)
	{
		m_pVideo->SetVisible(false);
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

			Q_snprintf(verString, sizeof(verString), "VERSION: %s", GameInfo + 8);

			delete[] GameInfo;
		}
		m_pVersionLabel->SetText(verString);
	}
};

void CFRMainMenuPanel::SetHintLabel()
{
	if (m_pHintLabel)
	{
		const wchar_t *wzTip = g_FRTips.GetRandomTip();
		m_pHintLabel->SetText(wzTip);
	}
};

size_t CFRMainMenuPanel::CheckVideoCount()
{
	size_t i = 0;

	for (; i <= MAX_VIDEO_COUNT; i++)
	{
		char szPath[MAX_PATH];
		GetVideoPath(szPath, sizeof(szPath), i + 1);
		if (!g_pFullFileSystem->FileExists(szPath))
			break;
	}

	return i;
}

void CFRMainMenuPanel::GetVideoPath(char *szPath, size_t szPathLen, size_t i)
{
	V_snprintf(szPath, szPathLen, "media/bg_0%zd.bik", i);
}

void CFRMainMenuPanel::GetRandomVideo(char *szPath, size_t szPathLen)
{
	GetVideoPath(szPath, szPathLen, random->RandomInt(1, m_iVideoCount + 1));
}

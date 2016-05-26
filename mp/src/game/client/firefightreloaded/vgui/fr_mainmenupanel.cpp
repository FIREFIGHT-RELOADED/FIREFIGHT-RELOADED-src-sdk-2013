#include "cbase.h"
#include "fr_mainmenupanel.h"
#include "fr_mainmenu.h"
#include "fr_tips.h"
#include "firefightreloaded/PlayerModelDialog.h"

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
	m_pPlayerListButton = dynamic_cast<CFRMainMenuButton *>(FindChildByName("PlayerListButton"));
	m_pVideo = dynamic_cast<CFRVideoPanel *>(FindChildByName("BackgroundVideo"));
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
	if (!Q_strcmp(command, "playermodeldialog"))
	{
		CPlayerModelDialog* pCPlayerModelDialog = new CPlayerModelDialog(this);
		pCPlayerModelDialog->Activate();
	}
	else if (!Q_strcmp(command, "changesplash"))
	{
		SetHintLabel();
	}
	else
	{
		BaseClass::OnCommand(command);
	}
}


void CFRMainMenuPanel::OnTick()
{
	BaseClass::OnTick();

	/*
	if (m_pVideo && m_pVideo->IsVisible() && !InGameLayout && m_flActionThink < gpGlobals->curtime)
	{
		m_pVideo->Activate();
		m_pVideo->BeginPlayback(m_pzVideoLink);
		m_pVideo->MoveToFront();
		m_flActionThink = gpGlobals->curtime + m_pVideo->GetActiveVideoLength() - 0.21f;
		b_ShowVideo = false;
	}
	*/
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
	if (m_pPlayerListButton)
	{
		if (m_pPlayerListButton->OnlyInGame())
		{
			m_pPlayerListButton->SetVisible(false);
		}
		else if (m_pPlayerListButton->OnlyAtMenu())
		{
			m_pPlayerListButton->SetVisible(true);
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
	if (m_pPlayerListButton)
	{
		if (m_pPlayerListButton->OnlyInGame())
		{
			m_pPlayerListButton->SetVisible(true);
		}
		else if (m_pPlayerListButton->OnlyAtMenu())
		{
			m_pPlayerListButton->SetVisible(false);
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

char* CFRMainMenuPanel::GetRandomVideo()
{
	char* pszBasePath = "media/bg_0";
	int iCount = 0;

	for (int i = 0; i < 9; i++)
	{
		char szPath[MAX_PATH];
		char szNumber[5];
		Q_snprintf(szNumber, sizeof(szNumber), "%d", iCount + 1);
		Q_strncpy(szPath, pszBasePath, sizeof(szPath));
		Q_strncat(szPath, szNumber, sizeof(szPath));
		Q_strncat(szPath, ".bik", sizeof(szPath));
		if (!g_pFullFileSystem->FileExists(szPath))
			break;
		iCount++;
	}

	int iRand = rand() % iCount + 1;
	char szPath[MAX_PATH];
	char szNumber[5];
	Q_snprintf(szNumber, sizeof(szNumber), "%d", iRand);
	Q_strncpy(szPath, pszBasePath, sizeof(szPath));
	Q_strncat(szPath, szNumber, sizeof(szPath));
	Q_strncat(szPath, ".bik", sizeof(szPath));
	char *szResult = szPath;
	return szResult;
}
#include "gamepadui_interface.h"
#include "gamepadui_image.h"
#include "gamepadui_util.h"
#include "gamepadui_frame.h"
#include "gamepadui_scroll.h"
#include "gamepadui_genericconfirmation.h"
#include "gamepadui_mainmenu.h"

#include "ienginevgui.h"
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "vgui/IVGui.h"

#include "vgui_controls/ComboBox.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/ScrollBar.h"

#include "KeyValues.h"
#include "filesystem.h"

#include "icommandline.h"

#include "tier0/memdbgon.h"

extern ConVar gamepadui_background_music_duck;

ConVar gamepadui_store_music("gamepadui_store_music", "1", FCVAR_ARCHIVE);

class GamepadUIStoreButton;

#define GAMEPADUI_MAP_SCHEME GAMEPADUI_RESOURCE_FOLDER "schemestorebutton.res"//"schemesavebutton.res"

class GamepadUIStore : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE(GamepadUIStore, GamepadUIFrame);

public:
    GamepadUIStore(vgui::Panel* pParent, const char* pPanelName);

    void UpdateGradients();

    void OnThink() OVERRIDE;
    void OnKeyCodeTyped(vgui::KeyCode code) OVERRIDE;
    void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;
    void OnCommand(char const* pCommand) OVERRIDE;
    void LoadItemFile(const char* kvName, const char* scriptPath);
    void CreateItemList();
    void UpdateFrameTitle();
    void OutOfBusiness();

    MESSAGE_FUNC_HANDLE(OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button);

    void LayoutStoreButtons();

    void OnMouseWheeled(int delta) OVERRIDE;

    bool StartBackgroundMusic(float flVolume);
    void ReleaseBackgroundMusic();

private:
    CUtlVector<KeyValues*> m_pItems;
    CUtlVector< GamepadUIStoreButton* > m_pChapterButtons;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY(float, m_ChapterFade, "Saves.Fade", "0", SchemeValueTypes::ProportionalFloat);
    GAMEPADUI_PANEL_PROPERTY(float, m_ChapterOffsetX, "Saves.OffsetX", "0", SchemeValueTypes::ProportionalFloat);
    GAMEPADUI_PANEL_PROPERTY(float, m_ChapterOffsetY, "Saves.OffsetY", "0", SchemeValueTypes::ProportionalFloat);
    GAMEPADUI_PANEL_PROPERTY(float, m_ChapterSpacing, "Saves.Spacing", "0", SchemeValueTypes::ProportionalFloat);

    bool m_bPlayingMusic;

    FMOD::Channel* m_pChannel;
    FMOD::Sound* m_pSound;
};

class GamepadUIStoreButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE(GamepadUIStoreButton, GamepadUIButton);

    GamepadUIStoreButton(vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char* pChapterImage, int pKashValue, int pItemLimit)
        : BaseClass(pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image(pChapterImage)
        , m_iKashValue(pKashValue)
        , m_iItemLimit(pItemLimit)
    {
        m_iItemPurchases = 0;
    }

    GamepadUIStoreButton(vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char* pChapterImage, int pKashValue, int pItemLimit)
        : BaseClass(pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image(pChapterImage)
        , m_iKashValue(pKashValue)
        , m_iItemLimit(pItemLimit)
    {
        m_iItemPurchases = 0;
    }

    ~GamepadUIStoreButton()
    {
    }

    void DoClick() OVERRIDE
    {
        BaseClass::DoClick();

        static ConVarRef player_cur_money("player_cur_money");
        int money = player_cur_money.GetInt();

        m_iItemPurchases++;

        if (money >= m_iKashValue)
        {
            if (m_iItemPurchases == 1)
            {
                if (m_iItemLimit == 1)
                {
                    SetButtonDescription(GamepadUIString("#FR_Store_GamepadUI_Purchased_LimitExceeded"));
                }
                else
                {
                    SetButtonDescription(GamepadUIString("#FR_Store_GamepadUI_Purchased"));
                }
            }
            else if (m_iItemPurchases < m_iItemLimit || m_iItemLimit <= 0)
            {
                char szPurchases[1024];
#ifdef _WIN32
                itoa(m_iItemPurchases, szPurchases, 10);
#else
                Q_snprintf( szPurchases, sizeof(szPurchases), "%d", m_iItemPurchases );
#endif

                wchar_t wzPurchases[1024];
                g_pVGuiLocalize->ConvertANSIToUnicode(szPurchases, wzPurchases, sizeof(wzPurchases));

                wchar_t string1[1024];
                g_pVGuiLocalize->ConstructString(string1, sizeof(string1), g_pVGuiLocalize->Find("#FR_Store_GamepadUI_Purchased_Multiple"), 1, wzPurchases);

                SetButtonDescription(GamepadUIString(string1));
            }
            else
            {
                SetButtonDescription(GamepadUIString("#FR_Store_GamepadUI_AlreadyPurchased"));
            }
        }
        else
        {
            SetButtonDescription(GamepadUIString("#FR_Store_GamepadUI_Denied"));
        }
    }

    void Paint() OVERRIDE
    {
        int x, y, w, t;
        GetBounds(x, y, w, t);

        PaintButton();

        vgui::surface()->DrawSetColor(Color(255, 255, 255, 255));
        vgui::surface()->DrawSetTexture(m_Image);
        int nImageSize = m_flHeight - m_flIconInset * 2;
        vgui::surface()->DrawTexturedRect(m_flIconInset, m_flIconInset, m_flIconInset + nImageSize, m_flIconInset + nImageSize);
        vgui::surface()->DrawSetTexture(0);

        vgui::surface()->DrawSetColor(m_colUnderlineColor);
        vgui::surface()->DrawFilledRect(0, GetDrawHeight() - m_flUnderlineHeight, m_flWidth * 100, GetDrawHeight());

        PaintText();
    }

private:
    GamepadUIImage m_Image;
    int m_iKashValue;
    int m_iItemPurchases;
    int m_iItemLimit;

    GAMEPADUI_PANEL_PROPERTY(Color, m_colUnderlineColor, "Button.Background.Underline", "255 0 0 255", SchemeValueTypes::Color);
    GAMEPADUI_PANEL_PROPERTY(float, m_flUnderlineHeight, "Button.Underline.Height", "1", SchemeValueTypes::ProportionalFloat);
    GAMEPADUI_PANEL_PROPERTY(float, m_flIconInset, "Button.Icon.Inset", "0", SchemeValueTypes::ProportionalFloat);
};

GamepadUIStore::GamepadUIStore( vgui::Panel *pParent, const char* PanelName ) : BaseClass( pParent, PanelName )
{
    vgui::HScheme hScheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( hScheme );

    UpdateFrameTitle();
    
    FooterButtonMask buttons = FooterButtons::Back | FooterButtons::Select;
    SetFooterButtons( buttons, FooterButtons::Select );

    Activate();
    
    FileFindHandle_t findHandle;
    
    const char* pFilename = g_pFullFileSystem->FindFirst("scripts/*.txt", &findHandle);
	while (pFilename)
	{
		const char* prefix = "shopcatalog_";
		if (Q_strncmp(pFilename, prefix, sizeof(prefix)) == 0)
		{
			char szScriptName[2048];
			Q_snprintf(szScriptName, sizeof(szScriptName), "scripts/%s", pFilename);
			LoadItemFile("ShopCatalog", szScriptName);
			DevMsg("'%s' added to shop item list!\n", szScriptName);
		}

		pFilename = g_pFullFileSystem->FindNext(findHandle);
	}

    CreateItemList();

    if ( m_pChapterButtons.Count() > 0 )
        m_pChapterButtons[0]->NavigateTo();

    for ( int i = 1; i < m_pChapterButtons.Count(); i++ )
    {
        m_pChapterButtons[i]->SetNavUp( m_pChapterButtons[i - 1] );
        m_pChapterButtons[i - 1]->SetNavDown( m_pChapterButtons[i] );
    }

    UpdateGradients();

    m_bPlayingMusic = false;
}

bool GamepadUIStore::StartBackgroundMusic(float flVolume)
{
    if (!gamepadui_store_music.GetBool())
        return false;

    const char* szSound = GetFMODManager()->GetFullPathToSound("music/fr_store_loop.wav");

    CGamepadUIFMODManager::CheckError(GetFMODManager()->GetSystem()->createSound(szSound, FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &m_pSound));
    eChannelGroupType channelgroupType = CHANNELGROUP_MUSIC;
    CGamepadUIFMODManager::CheckError(GetFMODManager()->GetSystem()->playSound(m_pSound, GetFMODManager()->GetChannelGroup(channelgroupType), true, &m_pChannel));

    if (m_pChannel)
    {
        CGamepadUIFMODManager::CheckError(m_pChannel->setPitch(1.0f));
        CGamepadUIFMODManager::CheckError(m_pChannel->setVolume(flVolume * gamepadui_background_music_duck.GetFloat()));

        m_pChannel->setPaused(false);
        GetFMODManager()->Unmute();
    }

    return (m_pChannel != NULL && m_pSound != NULL);
}

void GamepadUIStore::ReleaseBackgroundMusic()
{
    if (!gamepadui_store_music.GetBool())
        return;

    GetFMODManager()->Mute();

    if (m_pChannel != nullptr)
    {
        m_pChannel->stop();
        m_pChannel = nullptr;
    }

    if (m_pSound != nullptr)
    {
        m_pSound->release();
        m_pSound = nullptr;
    }
}

void GamepadUIStore::UpdateFrameTitle()
{
    static ConVarRef player_cur_money("player_cur_money");
    const char* money = player_cur_money.GetString();

    wchar_t wzmoney[1024];
    g_pVGuiLocalize->ConvertANSIToUnicode(money, wzmoney, sizeof(wzmoney));

    wchar_t string1[1024];
    g_pVGuiLocalize->ConstructString(string1, sizeof(string1), g_pVGuiLocalize->Find("#FR_Store_GamepadUI_Title_Expanded"), 1, wzmoney);

    GetFrameTitle() = GamepadUIString(string1);
}

void GamepadUIStore::LoadItemFile(const char* kvName, const char* scriptPath)
{
	KeyValues* pKV = new KeyValues(kvName);
	if (pKV->LoadFromFile(g_pFullFileSystem, scriptPath))
	{
		KeyValues* pNode = pKV->GetFirstSubKey();
		while (pNode)
		{
            m_pItems.AddToTail(pNode);
			pNode = pNode->GetNextKey();
		}
	}
}

void GamepadUIStore::CreateItemList()
{
    for (int i = 0; i < m_pItems.Count(); i++)
    {
        KeyValues* pNode = m_pItems[i];

        const char* itemName = pNode->GetString("name", "");
        int itemPrice = pNode->GetInt("price", 0);
        const char* itemCMD = pNode->GetString("command", "");
        int itemLimit = pNode->GetInt("limit", 0);

        char szNameString[2048];
        Q_snprintf(szNameString, sizeof(szNameString), "#GameUI_Store_Buy_%s", itemName);
        wchar_t* convertedText = g_pVGuiLocalize->Find(szNameString);
        if (!convertedText)
        {
            Q_snprintf(szNameString, sizeof(szNameString), "%s", itemName);
        }

        wchar_t text[32];
        wchar_t num[32];
        wchar_t* chapter = g_pVGuiLocalize->Find("#Valve_Hud_MONEY");
        char szPrice[1024];
#ifdef _WIN32
        itoa(itemPrice, szPrice, 10);
#else
        Q_snprintf( szPrice, sizeof(szPrice), "%d", itemPrice );
#endif

        g_pVGuiLocalize->ConvertANSIToUnicode(szPrice, num, sizeof(num));
        _snwprintf(text, ARRAYSIZE(text), L"%ls %ls", num, chapter ? chapter : L"KASH");

        GamepadUIString strItemPrice(text);

        char szCommand[2048];
        Q_snprintf(szCommand, sizeof(szCommand), "purchase_item %i \"%s\" %i", itemPrice, itemCMD, itemLimit);

        GamepadUIString strChapterName(szNameString);

        char chapterImage[64];
        Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/store/%s.vmt", itemName);
        if (!g_pFullFileSystem->FileExists(chapterImage))
        {
            Q_snprintf(chapterImage, sizeof(chapterImage), "vgui/hud/icon_locked.vmt");
        }

        GamepadUIStoreButton* pChapterButton = new GamepadUIStoreButton(
            this, this,
            GAMEPADUI_MAP_SCHEME, szCommand,
            strChapterName.String(), strItemPrice.String(), chapterImage, itemPrice, itemLimit);
        pChapterButton->SetEnabled(true);
        pChapterButton->SetPriority(i);
        pChapterButton->SetForwardToParent(true);

        m_pChapterButtons.AddToTail(pChapterButton);
    }
}

void GamepadUIStore::UpdateGradients()
{
    const float flTime = GamepadUI::GetInstance().GetTime();
    GamepadUI::GetInstance().GetGradientHelper()->ResetTargets( flTime );
    GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Up, { 1.0f, 1.0f }, flTime );
    GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Down, { 1.0f, 0.5f }, flTime );
}

void GamepadUIStore::OnThink()
{
    BaseClass::OnThink();
    UpdateFrameTitle();
    LayoutStoreButtons();

    if (!GamepadUI::GetInstance().IsInLevel())
    {
        OutOfBusiness();
    }
    else
    {
        if (!m_bPlayingMusic)
        {
            StartBackgroundMusic(1.0f);
            m_bPlayingMusic = true;
        }
    }
}

void GamepadUIStore::OnKeyCodeTyped(vgui::KeyCode code)
{
    ButtonCode_t buttonCode = GetBaseButtonCode(code);
    switch (buttonCode)
    {
#ifdef HL2_RETAIL
    case STEAMCONTROLLER_B:
#endif
    case KEY_XBUTTON_B:
    case KEY_ESCAPE:
    case KEY_LCONTROL:
    case KEY_RCONTROL:
        for (int i = 0; i < FooterButtons::MaxFooterButtons; i++)
        {
            if (FooterButtons::GetButtonByIdx(i) & FooterButtons::DeclineMask)
            {
                if (m_pFooterButtons[i])
                {
                    m_pFooterButtons[i]->ForceDepressed(true);
                    if (m_pFooterButtons[i]->IsDepressed())
                    {
                        m_pFooterButtons[i]->ForceDepressed(false);
                        m_pFooterButtons[i]->DoClick();
                    }
                }
            }
        }
        break;
    default:
        BaseClass::OnKeyCodeTyped(code);
        break;
    }
}

void GamepadUIStore::OutOfBusiness()
{
    Close();
    if (GamepadUI::GetInstance().IsInLevel())
    {
        ReleaseBackgroundMusic();
        GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted("gamemenucommand resumegame");
        // I tried it and didn't like it.
        // Oh well.
        //vgui::surface()->PlaySound( "UI/buttonclickrelease.wav" );
    }
}

void GamepadUIStore::ApplySchemeSettings(vgui::IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);
}

void GamepadUIStore::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
{
    GamepadUIButton* pButton = dynamic_cast<GamepadUIButton*>(vgui::ipanel()->GetPanel(button, GetModuleName()));
    if (pButton)
    {
        if (pButton->GetAlpha() != 255)
        {
            int nParentW, nParentH;
            GetParent()->GetSize(nParentW, nParentH);

            int nX, nY;
            pButton->GetPos(nX, nY);

            int nTargetY = pButton->GetPriority() * (pButton->m_flHeightAnimationValue[ButtonStates::Out] + m_ChapterSpacing);

            if (nY < nParentH / 2)
            {
                nTargetY += nParentH - m_ChapterOffsetY;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY -= m_ChapterSpacing;
            }
            else
            {
                nTargetY += pButton->m_flHeightAnimationValue[ButtonStates::Over];
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY += (pButton->m_flHeightAnimationValue[ButtonStates::Over] / 2) + m_ChapterSpacing;
            }


            m_ScrollState.SetScrollTarget(nTargetY - (nParentH - m_ChapterOffsetY), GamepadUI::GetInstance().GetTime());
        }
    }
}

void GamepadUIStore::LayoutStoreButtons()
{
    int nParentW, nParentH;
    GetParent()->GetSize(nParentW, nParentH);

    float scrollClamp = 0.0f;
    for (int i = 0; i < (int)m_pChapterButtons.Count(); i++)
    {
        int size = (m_pChapterButtons[i]->GetTall() + m_ChapterSpacing);

        if (i < ((int)m_pChapterButtons.Count()) - 3)
            scrollClamp += size;
    }

    m_ScrollState.UpdateScrollBounds(0.0f, scrollClamp);

    int previousSizes = 0;
    for (int i = 0; i < (int)m_pChapterButtons.Count(); i++)
    {
        int tall = m_pChapterButtons[i]->GetTall();
        int size = (tall + m_ChapterSpacing);

        int y = m_ChapterOffsetY + previousSizes - m_ScrollState.GetScrollProgress();
        int fade = 255;
        if (y < m_ChapterOffsetY)
            fade = (1.0f - clamp(-(y - m_ChapterOffsetY) / m_ChapterFade, 0.0f, 1.0f)) * 255.0f;
        if (y > nParentH - m_ChapterFade)
            fade = (1.0f - clamp((y - (nParentH - m_ChapterFade - size)) / m_ChapterFade, 0.0f, 1.0f)) * 255.0f;
        if (m_pChapterButtons[i]->HasFocus() && fade != 0)
            fade = 255;
        m_pChapterButtons[i]->SetAlpha(fade);
        m_pChapterButtons[i]->SetPos(m_ChapterOffsetX, y);
        m_pChapterButtons[i]->SetVisible(true);
        previousSizes += size;
    }

    m_ScrollState.UpdateScrolling(2.0f, GamepadUI::GetInstance().GetTime());
}

void GamepadUIStore::OnCommand( char const* pCommand )
{
    if ( !V_strcmp( pCommand, "action_back" ) )
    {
        OutOfBusiness();
    }
    else if (StringHasPrefixCaseSensitive(pCommand, "purchase_item "))
    {
        const char* pszparams = pCommand + 14;

        if (*pszparams)
        {
            char szPurchaseCommand[1024];

            // create the command to execute
            Q_snprintf(szPurchaseCommand, sizeof(szPurchaseCommand), "purchase %s\n", pszparams);

            // exec
            GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted(szPurchaseCommand);
        }
    }
    else
    {
        BaseClass::OnCommand( pCommand );
    }
}

void GamepadUIStore::OnMouseWheeled( int nDelta )
{
    m_ScrollState.OnMouseWheeled(nDelta * 200.0f, GamepadUI::GetInstance().GetTime());
}

CON_COMMAND( gamepadui_openstore, "" )
{
    if (GamepadUI::GetInstance().IsInLevel())
    {
        GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted("gameui_activate");
        new GamepadUIStore(GamepadUI::GetInstance().GetBasePanel(), "");
    }
}

#include "gamepadui_interface.h"
#include "gamepadui_image.h"
#include "gamepadui_util.h"
#include "gamepadui_frame.h"
#include "gamepadui_scroll.h"
#include "gamepadui_genericconfirmation.h"

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

class GamepadUIStoreButton;

#define GAMEPADUI_MAP_SCHEME GAMEPADUI_RESOURCE_FOLDER "schemechapterbutton.res"

class GamepadUIStore : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE( GamepadUIStore, GamepadUIFrame );

public:
    GamepadUIStore( vgui::Panel *pParent, const char* pPanelName );

    void UpdateGradients();

    void OnThink() OVERRIDE;
    void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;
    void OnCommand( char const* pCommand ) OVERRIDE;
    KeyValues* LoadItemFile(const char* kvName, const char* scriptPath);

    MESSAGE_FUNC_HANDLE( OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button );

    void LayoutMapButtons();

    void OnMouseWheeled( int delta ) OVERRIDE;

private:
    CUtlVector< GamepadUIStoreButton* > m_pChapterButtons;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY( float, m_ChapterOffsetX, "Chapters.OffsetX", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_ChapterOffsetY, "Chapters.OffsetY", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_ChapterSpacing, "Chapters.Spacing", "0", SchemeValueTypes::ProportionalFloat );

    bool m_bCommentaryMode = false;
};

class GamepadUIStoreButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE( GamepadUIStoreButton, GamepadUIButton );

    GamepadUIStoreButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription )
        , m_Image( pChapterImage )
    {
    }

    GamepadUIStoreButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription )
        , m_Image( pChapterImage )
    {
    }

    ~GamepadUIStoreButton()
    {
        if ( s_pLastNewGameButton == this )
            s_pLastNewGameButton = NULL;
    }

    ButtonState GetCurrentButtonState() OVERRIDE
    {
        if ( s_pLastNewGameButton == this )
            return ButtonState::Over;
        return BaseClass::GetCurrentButtonState();
    }

    void Paint() OVERRIDE
    {
        int x, y, w, t;
        GetBounds( x, y, w, t );

        PaintButton();

        vgui::surface()->DrawSetColor( Color( 255, 255, 255, 255 ) );
        vgui::surface()->DrawSetTexture( m_Image );
        int imgH = ( w * 9 ) / 16;
        //vgui::surface()->DrawTexturedRect( 0, 0, w, );
        float offset = m_flTextOffsetYAnimationValue[ButtonStates::Out] - m_flTextOffsetY;
        vgui::surface()->DrawTexturedSubRect( 0, 0, w, imgH - offset, 0.0f, 0.0f, 1.0f, ( imgH - offset ) / imgH );
        vgui::surface()->DrawSetTexture( 0 );
        if ( !IsEnabled() )
        {
            vgui::surface()->DrawSetColor( Color( 255, 255, 255, 16 ) );
            vgui::surface()->DrawFilledRect( 0, 0, w, imgH - offset );
        }

        PaintText();
    }

    void ApplySchemeSettings(vgui::IScheme* pScheme)
    {
        BaseClass::ApplySchemeSettings(pScheme);

        if (GamepadUI::GetInstance().GetScreenRatio() != 1.0f)
        {
            float flScreenRatio = GamepadUI::GetInstance().GetScreenRatio();

            m_flHeight *= flScreenRatio;
            for (int i = 0; i < ButtonStates::Count; i++)
                m_flHeightAnimationValue[i] *= flScreenRatio;

            // Also change the text offset
            m_flTextOffsetY *= flScreenRatio;
            for (int i = 0; i < ButtonStates::Count; i++)
                m_flTextOffsetYAnimationValue[i] *= flScreenRatio;

            SetSize(m_flWidth, m_flHeight + m_flExtraHeight);
            DoAnimations(true);
        }
    }

    void NavigateTo() OVERRIDE
    {
        BaseClass::NavigateTo();
        s_pLastNewGameButton = this;
    }

    static GamepadUIStoreButton* GetLastNewGameButton() { return s_pLastNewGameButton; }

private:
    GamepadUIImage m_Image;

    static GamepadUIStoreButton *s_pLastNewGameButton;
};

GamepadUIStoreButton* GamepadUIStoreButton::s_pLastNewGameButton = NULL;

GamepadUIStore::GamepadUIStore( vgui::Panel *pParent, const char* PanelName ) : BaseClass( pParent, PanelName )
{
    vgui::HScheme hScheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( hScheme );

    GetFrameTitle() = GamepadUIString( "#FR_Store_Title" );
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

    if ( m_pChapterButtons.Count() > 0 )
        m_pChapterButtons[0]->NavigateTo();

    for ( int i = 1; i < m_pChapterButtons.Count(); i++ )
    {
        m_pChapterButtons[i]->SetNavLeft( m_pChapterButtons[i - 1] );
        m_pChapterButtons[i - 1]->SetNavRight( m_pChapterButtons[i] );
    }

    UpdateGradients();
}

KeyValues* GamepadUIStore::LoadItemFile(const char* kvName, const char* scriptPath)
{
    int mapIndex = 0;
	KeyValues* pKV = new KeyValues(kvName);
	if (pKV->LoadFromFile(g_pFullFileSystem, scriptPath))
	{
		KeyValues* pNode = pKV->GetFirstSubKey();
		while (pNode)
		{
			const char* itemName = pNode->GetString("name", "");
            const char* itemPrice = pNode->GetString("price", 0);
			const char* itemCMD = pNode->GetString("command", "");

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
            g_pVGuiLocalize->ConvertANSIToUnicode(itemPrice, num, sizeof(num));
            _snwprintf(text, ARRAYSIZE(text), L"%ls %ls", num, chapter ? chapter : L"KASH");

            GamepadUIString strItemPrice(text);
            
            char szCommand[2048];
            Q_snprintf(szCommand, sizeof(szCommand), "purchase_item %s \"%s\"", itemPrice, itemCMD);
            
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
            strChapterName.String(), strItemPrice.String(), chapterImage);
            pChapterButton->SetEnabled(true);
            pChapterButton->SetPriority(mapIndex);
            pChapterButton->SetForwardToParent(true);

            m_pChapterButtons.AddToTail(pChapterButton);
            mapIndex++;

			pNode = pNode->GetNextKey();
		}
	}

	return pKV;
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

    LayoutMapButtons();
}

void GamepadUIStore::ApplySchemeSettings(vgui::IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);

    if (GamepadUI::GetInstance().GetScreenRatio() != 1.0f)
    {
        float flScreenRatio = GamepadUI::GetInstance().GetScreenRatio();
        m_ChapterOffsetX *= (flScreenRatio * flScreenRatio);
    }
}

void GamepadUIStore::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
{
    GamepadUIButton *pButton = dynamic_cast< GamepadUIButton * >( vgui::ipanel()->GetPanel( button, GetModuleName() ) );
    if ( pButton )
    {
        int nParentW, nParentH;
	    GetParent()->GetSize( nParentW, nParentH );

        int nX, nY;
        pButton->GetPos( nX, nY );
        if ( nX + pButton->m_flWidth > nParentW || nX < 0 )
        {
            int nTargetX = pButton->GetPriority() * (pButton->m_flWidth + m_ChapterSpacing);

            if ( nX < nParentW / 2 )
            {
                nTargetX += nParentW - m_ChapterOffsetX;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetX -= m_ChapterSpacing;
            }
            else
            {
                nTargetX += pButton->m_flWidth;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetX += (pButton->m_flWidth / 2) + m_ChapterSpacing;
            }


            m_ScrollState.SetScrollTarget( nTargetX - ( nParentW - m_ChapterOffsetX ), GamepadUI::GetInstance().GetTime() );
        }
    }
}

void GamepadUIStore::LayoutMapButtons()
{
    int nParentW, nParentH;
	GetParent()->GetSize( nParentW, nParentH );

    float flScrollClamp = 0.0f;
    for ( int i = 0; i < m_pChapterButtons.Count(); i++ )
    {
        int nSize = ( m_pChapterButtons[0]->GetWide() + m_ChapterSpacing );

        if ( i < m_pChapterButtons.Count() - 2 )
            flScrollClamp += nSize;
    }

    m_ScrollState.UpdateScrollBounds( 0.0f, flScrollClamp );

    for ( int i = 0; i < m_pChapterButtons.Count(); i++ )
    {
        int size = ( m_pChapterButtons[0]->GetWide() + m_ChapterSpacing );

        m_pChapterButtons[i]->SetPos( m_ChapterOffsetX + i * size - m_ScrollState.GetScrollProgress(), m_ChapterOffsetY );
        m_pChapterButtons[i]->SetVisible( true );
    }

    m_ScrollState.UpdateScrolling( 2.0f, GamepadUI::GetInstance().GetTime() );
}

void GamepadUIStore::OnCommand( char const* pCommand )
{
    if ( !V_strcmp( pCommand, "action_back" ) )
    {
        Close();
        if (GamepadUI::GetInstance().IsInLevel())
        {
            GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted("gamemenucommand resumegame");
            // I tried it and didn't like it.
            // Oh well.
            //vgui::surface()->PlaySound( "UI/buttonclickrelease.wav" );
        }
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
            Close();
            if (GamepadUI::GetInstance().IsInLevel())
            {
                GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted("gamemenucommand resumegame");
                // I tried it and didn't like it.
                // Oh well.
                //vgui::surface()->PlaySound( "UI/buttonclickrelease.wav" );
            }
        }
    }
    else
    {
        BaseClass::OnCommand( pCommand );
    }
}

void GamepadUIStore::OnMouseWheeled( int nDelta )
{
    m_ScrollState.OnMouseWheeled( nDelta * m_ChapterSpacing * 20.0f, GamepadUI::GetInstance().GetTime() );
}

CON_COMMAND( gamepadui_openstore, "" )
{
    if (GamepadUI::GetInstance().IsInLevel())
    {
        GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted("gameui_activate");
        new GamepadUIStore(GamepadUI::GetInstance().GetBasePanel(), "");
    }
}

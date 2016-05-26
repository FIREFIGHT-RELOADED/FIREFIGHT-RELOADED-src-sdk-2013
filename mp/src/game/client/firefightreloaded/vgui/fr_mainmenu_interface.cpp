#include "cbase.h"
#include "fr_mainmenu_interface.h"
#include "fr_mainmenu.h"

// Class
class CMainMenu : public IMainMenu
{
private:
	CFRMainMenu *MainMenu;

public:
	void UI_Interface(void)
	{
		MainMenu = NULL;
	}

	void Create(vgui::VPANEL parent)
	{
		// Create immediately
		MainMenu = new CFRMainMenu(parent);
	}

	vgui::VPANEL GetPanel(void)
	{
		if (!MainMenu)
			return NULL;
		return MainMenu->GetVPanel();
	}

	void Destroy(void)
	{
		if (MainMenu)
		{
			MainMenu->SetParent((vgui::Panel *)NULL);
			delete MainMenu;
		}
	}

};

static CMainMenu g_MainMenu;
IMainMenu *MainMenu = (IMainMenu *)&g_MainMenu;
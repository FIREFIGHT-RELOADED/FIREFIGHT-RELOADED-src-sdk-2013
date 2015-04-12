#include "cbase.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include "gameui\SingleplayerAdvancedDialog.h"

class CLoadPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CLoadPanel, vgui::Frame);

	CLoadPanel(vgui::VPANEL parent); 	// Constructor
	~CLoadPanel(){};				// Destructor

public:

	vgui::DHANDLE<CSingleplayerAdvancedDialog> m_hSingleplayerAdvancedDialog;
	virtual void Activate();

};
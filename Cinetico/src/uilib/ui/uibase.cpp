
//UIBase
//File: uibase.cpp
//Last edit: 27/09/2013 02:15 (UTC3)
//Author: CGR

#include "uibase.h"

namespace uilib {

	FontDesc g_pUIdefFontDesc;

	bool uibase::g_uiinit(false);


	void uibase::UIInit() //static
	{
		if (g_uiinit) return;
		OSDBase::UIInit();
		uibase::g_uiinit = true;
		g_pUIdefFontDesc = FontDesc("Arial", 10, 0);
	}


	int uibase::UIProcess() //static 
	{
		return OSDBase::UIProcess();
	}

	void uibase::UIQuit() //static
	{
		OSDBase::UIQuit();
	}

}
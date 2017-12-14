
#ifndef __CINETICO_GENERAL_SETTINGS_CONTROLLER_H__
#define __CINETICO_GENERAL_SETTINGS_CONTROLLER_H__

#include "Controller.h"
#include "components/cPageTitle.h"
#include "components/cSeparator.h"
#include "components/cTextBox.h"
#include "components/cComboBox.h"

namespace cinetico {
	using namespace uilib;

	class GeneralSettingsController : public Controller
	{
		std::vector<Adapter> m_adapters;
		std::vector<DisplayMode> m_displayModes;

	public:
		VerticalLayout layout;
		cPageTitle title;
		HorizontalLayout layoutActionButtons;
		Button buttonSave;
		Button buttonBack;

		cSeparator sepCore;
		VerticalLayout layoutCore;
		cTextBox tbDistThreshold;
		cTextBox tbMinHoldtime;

		cSeparator sepGraphics;
		VerticalLayout layoutGraphics;
		cComboBox cbAdapter;
		cComboBox cbResolution;
		CheckBox chkFullscreen;
		CheckBox chkAntialiasing;

		friend void GeneralSettingsController_buttonSave_onClick(Button &button);
		friend void GeneralSettingsController_buttonBack_onClick(Button &button);

	public:
		GeneralSettingsController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		void onViewEnter(ViewParams params);
		void onViewUpdate();
		void onViewQuit();
	};

}

#endif
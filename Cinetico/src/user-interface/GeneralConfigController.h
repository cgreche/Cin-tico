
#ifndef __CINETICO_GENERAL_CONFIG_CONTROLLER_H__
#define __CINETICO_GENERAL_CONFIG_CONTROLLER_H__

#include "Controller.h"
#include "components/cPageTitle.h"
#include "components/cSeparator.h"
#include "components/cTextBox.h"
#include "components/cComboBox.h"

namespace cinetico {
	using namespace uilib;

	class GeneralConfigController : public Controller
	{
		std::vector<Adapter> m_adapters;
		std::vector<DisplayMode> m_displayModes;
	public:
		VerticalLayout layout;
			cPageTitle title;
			HorizontalLayout layoutActionButtons;
				Button buttonSave;
				Button buttonBack;

			cSeparator sepSensor;
			VerticalLayout layoutSensor;
				cTextBox tbPosThreshold;

			cSeparator sepGraphics;
			VerticalLayout layoutGraphics;
				cComboBox cbAdapter;
				cComboBox cbResolution;
				CheckBox chkFullscreen;
				CheckBox chkAntialiasing;

		UserProfileDAO *m_userProfileDAO;

		friend void GeneralConfigController_buttonSave_onClick(Button &button);
		friend void GeneralConfigController_buttonBack_onClick(Button &button);

	public:
		GeneralConfigController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		void onViewEnter(ViewParams params);
		void onViewUpdate();
		void onViewQuit();
	};

}

#endif
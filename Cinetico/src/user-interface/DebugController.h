
#ifndef __CINETICO_DEBUG_CONTROLLER_H__
#define __CINETICO_DEBUG_CONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"
#include "components/cPageTitle.h"
#include "components/cTextBox.h"
#include "components/cButton.h"
#include <map>

namespace cinetico {

	using namespace uilib;

	class DebugController : public Controller
	{
	public:
		VerticalLayout layout;
			Label labelQuaternion;
			HorizontalLayout layoutQValue;
				cTextBox tbQW;
				cTextBox tbQX;
				cTextBox tbQY;
				cTextBox tbQZ;
				cButton buttonToEuler;

			Label labelEuler;
			HorizontalLayout layoutEValue;
				cTextBox tbEX;
				cTextBox tbEY;
				cTextBox tbEZ;
				cButton buttonToQuaternion;

		friend void buttonToEuler_onAction(cButton& button);
		friend void buttonToQuaternion_onAction(cButton& button);

	public:
		DebugController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		void onViewEnter(ViewParams params);
		void onViewUpdate();
		void onViewTick();
		void onViewQuit();
	};

}

#endif
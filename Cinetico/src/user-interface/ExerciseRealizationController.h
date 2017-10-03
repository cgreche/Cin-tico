
#ifndef __CINETICO_EXERCISEREALIZATIONCONTROLLER_H__
#define __CINETICO_EXERCISEREALIZATIONCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {
	using namespace uilib;

	class ExerciseRealizationController : public Controller
	{
	public:
		VerticalLayout layout;

	public:
		ExerciseRealizationController();

		Layout *viewDefinition();
		void onViewEnter(ViewParams *params);
		void onViewTick();
		void onViewQuit();
	};

}

#endif
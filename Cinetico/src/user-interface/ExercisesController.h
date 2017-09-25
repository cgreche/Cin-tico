
#ifndef __CINETICO_EXERCISESCONTROLLER_H__
#define __CINETICO_EXERCISESCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	class ExercisesController : public Controller
	{
	public:
		VerticalLayout layout;

	public:
		ExercisesController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
	};

}

#endif
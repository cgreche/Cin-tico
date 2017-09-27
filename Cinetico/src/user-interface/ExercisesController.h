
#ifndef __CINETICO_EXERCISESCONTROLLER_H__
#define __CINETICO_EXERCISESCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	class ExercisesController : public Controller
	{
	public:
		VerticalLayout layout;
			VerticalLayout layoutActions;
				Label labelViewTitle;
				Label labelViewDescr;
				HorizontalLayout layouActionbuttons;
					Button buttonCreateExercise;
					Button buttonRecordExercise;
					Button buttonDoExercise;

			VerticalLayout layoutContent;
				ListView listExercises;
			
	public:
		ExercisesController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
	};

}

#endif
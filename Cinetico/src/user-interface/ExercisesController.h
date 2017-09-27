
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
				HorizontalLayout layoutActionButtons;
					Button buttonCreateExercise;
					Button buttonRecordExercise;
					Button buttonDoExercise;

			VerticalLayout layoutContent;

			VerticalLayout layoutContentList;
				ListView listExercises;

			VerticalLayout layoutContentEditData;
				EditBox editExerciseName;
					CheckBox checkHead;
					CheckBox checkLeftHand;
					CheckBox checkRightHand;
					CheckBox checkHip;
					CheckBox checkLeftFoot;
					CheckBox checkRightFoot;
			
	public:
		ExercisesController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
	};

}

#endif
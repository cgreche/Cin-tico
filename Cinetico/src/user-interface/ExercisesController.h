
#ifndef __CINETICO_EXERCISESCONTROLLER_H__
#define __CINETICO_EXERCISESCONTROLLER_H__

#include "Controller.h"
#include "entity/core/exercise.h"
#include "uilib/ui/uibase.h"

namespace cinetico {
	using namespace cinetico_core;

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
				GridView gridExercises;

			VerticalLayout layoutContentEditData;
				VerticalLayout layoutExerciseName;
					Label labelExerciseName;
					EditBox editExerciseName;
					CheckBox checkPublic;

				Label labelChecks;
				HorizontalLayout layoutChecks1;
						CheckBox checkHead;
						CheckBox checkLeftHand;
						CheckBox checkRightHand;
				HorizontalLayout layoutChecks2;
						CheckBox checkHip;
						CheckBox checkLeftFoot;
						CheckBox checkRightFoot;

				HorizontalLayout layoutEditActions;
					Button buttonCreateEdit;
					Button buttonCancel;

		int m_editMode;
		int m_currentSelection;


		void setEditionMode(int mode);
		void updateExerciseList();

	public:
		ExercisesController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
	};

}

#endif
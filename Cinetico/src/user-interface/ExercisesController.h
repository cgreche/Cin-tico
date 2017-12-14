
#ifndef __CINETICO_EXERCISES_CONTROLLER_H__
#define __CINETICO_EXERCISES_CONTROLLER_H__

#include "Controller.h"
#include "core/entity/exercise.h"
#include "uilib/ui/uibase.h"
#include "components/cPageTitle.h"
#include "components/cSeparator.h"
#include <list>

namespace cinetico {
	using namespace cinetico_core;
	using namespace uilib;

	class ExercisesController : public Controller
	{
	public:
		VerticalLayout layout;
			cPageTitle title;
			HorizontalLayout layoutActionButtons;
				Button buttonCreateExercise;
				Button buttonManageActions;
				Button buttonDoExercise;

			VerticalLayout layoutContent;

			HorizontalLayout layoutContentList;
				GridView gridExercises;
				VerticalLayout layoutExerciseListActions;
					Button buttonEdit;
					Button buttonDelete;

			VerticalLayout layoutContentEditData;
				VerticalLayout layoutExerciseData;
					VerticalLayout layoutExerciseName;
						Label labelExerciseName;
						HorizontalLayout layoutExerciseNameData;
							EditBox editExerciseName;
							CheckBox checkPublic;

					cSeparator separatorChecks;
					std::vector<HorizontalLayout> layoutCheckBodyPointList;
					std::vector<CheckBox> checkBodyPointList;

				HorizontalLayout layoutEditActions;
					Button buttonCreateEdit;
					Button buttonCancel;


	private:
		int m_editMode;
		int m_currentSelection;

		Exercise *m_currentExercise;

	public:
		ExercisesController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		void onViewEnter(ViewParams params);
		void onViewUpdate();
		void onViewTick();
		void onViewQuit();

		void setEditionMode(int mode);
		void onExerciseSelect(int row);
		void updateExerciseList();

		void saveCurrentExercise();
		void deleteSelectedExercise();
		void doSelectedExercise();

		friend void buttonManageActions_onClick(Button &button);
	};

}

#endif
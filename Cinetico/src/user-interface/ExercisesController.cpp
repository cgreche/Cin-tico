
#include "ExercisesController.h"

namespace cinetico {

	ExercisesController::ExercisesController() {

		labelViewTitle.setText("Exerc�cios");
		labelViewDescr.setText("Crie, altere, grave ou realize um exerc�cio.");

		buttonCreateExercise.setText("Criar Exerc�cio");
		buttonRecordExercise.setText("Gravar Exerc�cio");
		buttonRecordExercise.setEnabled(false);
		buttonDoExercise.setText("Realizar Exerc�cio");

		layoutActionButtons.append(buttonCreateExercise);
		layoutActionButtons.append(buttonRecordExercise);
		layoutActionButtons.append(buttonDoExercise);

		layoutActions.append(labelViewTitle);
		layoutActions.append(labelViewDescr);
		layoutActions.append(layoutActionButtons);

		layoutContent.append(listExercises);
		layout.append(layoutActions);
		layout.append(layoutContent);
	}

	Layout *ExercisesController::viewDefinition() {
		return &layout;
	}

	void ExercisesController::onViewEnter() {

	}

	void ExercisesController::onViewQuit() {

	}

}
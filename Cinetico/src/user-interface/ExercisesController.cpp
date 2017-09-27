
#include "ExercisesController.h"

namespace cinetico {

	ExercisesController::ExercisesController() {

		labelViewTitle.setText("Exercícios");
		labelViewDescr.setText("Crie, altere, grave ou realize um exercício.");

		buttonCreateExercise.setText("Criar Exercício");
		buttonRecordExercise.setText("Gravar Exercício");
		buttonRecordExercise.setEnabled(false);
		buttonDoExercise.setText("Realizar Exercício");

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
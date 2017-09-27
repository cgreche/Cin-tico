
#include "ExerciseManagementController.h"

namespace cinetico {

	ExerciseManagementController::ExerciseManagementController() {
		labelViewTitle.setText("Gerenciamento de Exercício");
		labelViewDescr.setText("Crie ou altere um exercício.");

		buttonSaveExercise.setText("Salvar");
		buttonCancelExercise.setText("Cancelar");
	}

	Layout *ExerciseManagementController::viewDefinition() {
		return &layout; 
	}

	void ExerciseManagementController::onViewEnter() {
	}

	void ExerciseManagementController::onViewQuit() {

	}

}
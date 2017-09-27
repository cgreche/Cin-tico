
#include "ExerciseManagementController.h"

namespace cinetico {

	ExerciseManagementController::ExerciseManagementController() {
		labelViewTitle.setText("Gerenciamento de Exerc�cio");
		labelViewDescr.setText("Crie ou altere um exerc�cio.");

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
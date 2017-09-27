
#include "ExerciseRealizationController.h"
#include "cinetico.h"

namespace cinetico {

	extern Cinetico g_cinetico;

	ExerciseRealizationController::ExerciseRealizationController() {

	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter() {
		g_cinetico.enter3DWorld();
	}

	void ExerciseRealizationController::onViewQuit() {

	}

}
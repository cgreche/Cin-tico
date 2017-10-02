
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

	void ExerciseRealizationController::onViewTick() {
		g_cinetico.cinetico3D()->update();
		g_cinetico.cinetico3D()->render();
	}

	void ExerciseRealizationController::onViewQuit() {

	}

}